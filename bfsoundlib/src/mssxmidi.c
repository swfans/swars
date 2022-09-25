/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mssxmidi.c
 *     OpenAL based reimplementation of MSS API.
 * @par Purpose:
 *     SS functions from MSS API.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "mssxmidi.h"
#include "ail.h"
#include "aildebug.h"
#include "dllload.h"
#include "memfile.h"
#include "msssys.h"
/******************************************************************************/
extern char GTL_prefix[128];
extern char SoundDriverPath[144];
extern char AIL_error[256];
extern SNDCARD_IO_PARMS AIL_last_IO_attempt;
extern int32_t AIL_preference[AIL_N_PREFS];

extern uint32_t MDI_entry;
extern SNDSEQUENCE *MDI_S;
extern int32_t MDI_i,MDI_j,MDI_n,MDI_sequence_done;
extern int32_t MDI_q,MDI_t;
extern uint32_t MDI_channel,MDI_status,MDI_type,MDI_len;
extern const uint8_t *MDI_ptr;
extern const uint8_t *MDI_event;

extern int32_t MDI_locked;


void AILXMIDI_end(void);

/** Lock function, doubling as start of locked code.
 */
void AILXMIDI_start(void)
{
    if (MDI_locked)
        return;
    AIL_VMM_lock_range(AILXMIDI_start, AILXMIDI_end);

    AIL_vmm_lock(GTL_prefix, sizeof(GTL_prefix));
    AIL_vmm_lock(&MDI_entry, sizeof(MDI_entry));
    AIL_vmm_lock(&MDI_S, sizeof(MDI_S));
    AIL_vmm_lock(&MDI_i, sizeof(MDI_i));
    AIL_vmm_lock(&MDI_j, sizeof(MDI_j));
    AIL_vmm_lock(&MDI_n, sizeof(MDI_n));
    AIL_vmm_lock(&MDI_sequence_done, sizeof(MDI_sequence_done));
    AIL_vmm_lock(&MDI_q, sizeof(MDI_q));
    AIL_vmm_lock(&MDI_t, sizeof(MDI_t));
    AIL_vmm_lock(&MDI_channel, sizeof(MDI_channel));
    AIL_vmm_lock(&MDI_status, sizeof(MDI_status));
    AIL_vmm_lock(&MDI_type, sizeof(MDI_type));
    AIL_vmm_lock(&MDI_len, sizeof(MDI_len));
    AIL_vmm_lock(&MDI_ptr, sizeof(MDI_ptr));
    AIL_vmm_lock(&MDI_event, sizeof(MDI_event));

    MDI_locked = 1;
}

void AIL2OAL_API_set_GTL_filename_prefix(char const *prefix)
{
    int i;

    strcpy(GTL_prefix, prefix);

    // Truncate prefix string at final '.' character, if it exists and
    // if it occurs after the last directory separator
    for (i = strlen(GTL_prefix)-1; i > 0; i--)
    {
        if ((GTL_prefix[i] == '\\') || (GTL_prefix[i] == '/'))
        {
            if (i == (int)strlen(GTL_prefix)-1) {
                GTL_prefix[i] = '\0';
            }
            break;
        }

        if (GTL_prefix[i] == '.') {
            GTL_prefix[i] = '\0';
            break;
        }
    }
}

int32_t AIL2OAL_API_MDI_driver_type(MDI_DRIVER *mdidrv)
{
    const char *name;

    // If driver name corresponds to Tandy or IBM speaker driver, return
    // speaker type
    //
    // Name field was added in VDI 1.12 spec, so don't check earlier drivers
    if (mdidrv->drvr->VHDR->driver_version >= 0x112)
    {
        name = mdidrv->drvr->VHDR->dev_name;

        if (strcasecmp(name, "Tandy 3-voice music") == 0)
            return MDIDRVRTYPE_SPKR;
        if (strcasecmp(name, "IBM internal speaker music") == 0)
            return MDIDRVRTYPE_SPKR;
    }

    // If no GTL suffix, assume it's a hardwired General MIDI device
    name = mdidrv->DDT->GTL_suffix;
    if ((name == NULL) || (name[0] == '\0'))
        return MDIDRVRTYPE_GM;

     // If GTL suffix = '.AD', it's an OPL-2
     //
     // Note: Creative AWE32 driver incorrectly declares '.AD' GTL prefix,
     // so provide workaround here - if driver bigger than 20K, it's not one
     // of our FM drivers!
    if (strcasecmp(name, ".AD") == 0)
    {
        if (mdidrv->drvr->size > 20480)
            return MDIDRVRTYPE_GM;
        return MDIDRVRTYPE_FM_2;
    }

    // If GTL suffix = '.OPL', it's an OPL-3
    if (strcasecmp(name, ".OPL") == 0)
        return MDIDRVRTYPE_FM_4;

    // Otherwise, it's a currently-undefined GTL type - assume it's a GM device
    return MDIDRVRTYPE_GM;
}

/** Call device I/O verification function using current detection policy.
 */
int32_t XMI_attempt_MDI_detection(MDI_DRIVER *mdidrv, SNDCARD_IO_PARMS *iop)
{
    SNDCARD_IO_PARMS *f;
    SNDCARD_IO_PARMS try;
    uint32_t i;

    // Set up working I/O params structure
    try = *iop;

    // If any needed parameters are not specified, use parameter
    // values from first factory-default IO structure
    if (mdidrv->drvr->VHDR->num_IO_configurations > 0)
    {
        f = (SNDCARD_IO_PARMS *) (mdidrv->drvr->VHDR->common_IO_configurations);

        if (try.IO < 1)
            try.IO = f->IO;

        if (try.IRQ < 1)
            try.IRQ = f->IRQ;

        if (try.DMA_8_bit < 1)
            try.DMA_8_bit = f->DMA_8_bit;

        if (try.DMA_16_bit < 1)
            try.DMA_16_bit = f->DMA_16_bit;

        for (i=0; i < 4; i++)
        {
            if (try.IO_reserved[i] < 1)
                try.IO_reserved[i] = f->IO_reserved[i];
        }
    }

     // Copy IO parameter block to driver
     mdidrv->drvr->VHDR->IO = try;

     // Call detection function
     return AIL_call_driver(mdidrv->drvr, DRV_VERIFY_IO, NULL, NULL);
}

/** Install and initialize XMIDI audio driver.
*/
MDI_DRIVER *XMI_construct_MDI_driver(AIL_DRIVER *drvr, SNDCARD_IO_PARMS *iop)
{
    MDI_DRIVER *mdidrv;
    asm volatile (
      "push %2\n"
      "push %1\n"
      "call ASM_XMI_construct_MDI_driver\n"
        : "=r" (mdidrv) : "g" (drvr), "g" (iop));
    return mdidrv;
}

int32_t AIL2OAL_API_install_MDI_INI(MDI_DRIVER **mdidrv)
{
    AIL_INI ini;

    // Attempt to read MDI_INI file
    if (!AIL_read_INI(&ini, "MDI.INI")) {
        AIL_set_error("Unable to open file MDI.INI.");
        return AIL_NO_INI_FILE;
    }

    *mdidrv = AIL_install_MDI_driver_file(ini.driver_name, &ini.IO);
    if (*mdidrv == NULL) {
        return AIL_INIT_FAILURE;
    }
    return AIL_INIT_SUCCESS;
}

SNDSEQUENCE *AIL2OAL_API_allocate_sequence_handle(MDI_DRIVER *mdidrv)
{
    SNDSEQUENCE *seq;
    asm volatile (
      "push %1\n"
      "call ASM_AIL_API_allocate_sequence_handle\n"
        : "=r" (seq) : "g" (mdidrv));
    return seq;
}

MDI_DRIVER *AIL2OAL_API_install_MDI_driver_file(char *fname, SNDCARD_IO_PARMS *iop)
{
    MDI_DRIVER *mdidrv;
#if 0
    asm volatile (
      "push %2\n"
      "push %1\n"
      "call ASM_AIL_API_install_MDI_driver_file\n"
        : "=r" (mdidrv) : "g" (fname), "g" (iop));
#else
    char locstr[156];
    int32_t flen;
    void *drvbuf;
    AIL_DRIVER *drvr;

    sprintf(locstr, "%s/%s", SoundDriverPath, fname);
    drvbuf = FILE_read(locstr, NULL);
    if (drvbuf == NULL) {
        strcpy(AIL_error, "Driver file not found\n");
        return NULL;
    }
    flen = FILE_size(locstr);
    drvr = AIL_install_driver(drvbuf, flen);
    MEM_free(drvbuf);
    if (drvr == NULL) {
        return NULL;
    }
    mdidrv = XMI_construct_MDI_driver(drvr, iop);
    if (mdidrv == NULL)
        AIL_uninstall_driver(drvr);
#endif
    return mdidrv;
}

void AIL2OAL_API_uninstall_MDI_driver(MDI_DRIVER *mdidrv)
{
#if 0
    asm volatile (
      "push %0\n"
      "call ASM_AIL_API_uninstall_MDI_driver\n"
        :  : "g" (mdidrv));
#endif
   AIL_uninstall_driver(mdidrv->drvr);
}

/** Unlock function, doubling as end of locked code.
 */
void AILXMIDI_end(void)
{
    if (!MDI_locked)
        return;
    AIL_VMM_unlock_range(AILXMIDI_start, AILXMIDI_end);

    AIL_vmm_unlock(GTL_prefix, sizeof(GTL_prefix));
    AIL_vmm_unlock(&MDI_entry, sizeof(MDI_entry));
    AIL_vmm_unlock(&MDI_S, sizeof(MDI_S));
    AIL_vmm_unlock(&MDI_i, sizeof(MDI_i));
    AIL_vmm_unlock(&MDI_j, sizeof(MDI_j));
    AIL_vmm_unlock(&MDI_n, sizeof(MDI_n));
    AIL_vmm_unlock(&MDI_sequence_done, sizeof(MDI_sequence_done));
    AIL_vmm_unlock(&MDI_q, sizeof(MDI_q));
    AIL_vmm_unlock(&MDI_t, sizeof(MDI_t));
    AIL_vmm_unlock(&MDI_channel, sizeof(MDI_channel));
    AIL_vmm_unlock(&MDI_status, sizeof(MDI_status));
    AIL_vmm_unlock(&MDI_type, sizeof(MDI_type));
    AIL_vmm_unlock(&MDI_len, sizeof(MDI_len));
    AIL_vmm_unlock(&MDI_ptr, sizeof(MDI_ptr));
    AIL_vmm_unlock(&MDI_event, sizeof(MDI_event));

    MDI_locked = 0;
}

/******************************************************************************/
