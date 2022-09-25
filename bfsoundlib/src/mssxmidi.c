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
#include "aildebug.h"
/******************************************************************************/
extern char GTL_prefix[128];

void AIL2OAL_API_set_GTL_filename_prefix(char const *prefix)
{
    int i;

    strcpy(GTL_prefix,prefix);

    // Truncate prefix string at final '.' character, if it exists and
    // if it occurs after the last directory separator
    for (i=strlen(GTL_prefix)-1; i > 0; i--)
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

MDI_DRIVER *AIL2OAL_API_install_MDI_driver_file(char *filename, SNDCARD_IO_PARMS *iop)
{
    MDI_DRIVER *mdidrv;
    asm volatile (
      "push %2\n"
      "push %1\n"
      "call ASM_AIL_API_install_MDI_driver_file\n"
        : "=r" (mdidrv) : "g" (filename), "g" (iop));
    return mdidrv;
}

void AIL2OAL_API_uninstall_MDI_driver(MDI_DRIVER *mdidrv)
{
    asm volatile (
      "push %0\n"
      "call ASM_AIL_API_uninstall_MDI_driver\n"
        :  : "g" (mdidrv));
}

/******************************************************************************/
