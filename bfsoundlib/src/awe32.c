/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file awe32.c
 *     SoundBlaster AWE32 specific routines.
 * @par Purpose:
 *     Functions which are specific to SoundBlaster AWE32 and not part of AIL.
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
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "awe32.h"
#include "dpmi.h"
#include "snderr.h"
#include "bffile.h"
/******************************************************************************/
TbBool UseCurrentAwe32Soundfont = false;
TbBool Awe32SoundfontLoaded = false;

char CurrentAwe32SoundfontPrefix[12] = "Bullfrog";
TbFileHandle sbkHandle = INVALID_FILE;

uint8_t *awe_buffer = NULL;
uint16_t awe_buffer_sel = 0;
uint8_t *awe_preset = NULL;
uint16_t awe_preset_sel = 0;

extern TbBool MusicAble;
extern TbBool MusicInstalled;
extern char MusicType[6];
extern AIL_INI MusicInstallChoice;
extern MDI_DRIVER *MusicDriver;
extern char SoundDataPath[144];

/******************************************************************************/

void FreeAwe32Soundfont(void)
{
    if (UseCurrentAwe32Soundfont) {
        return;
    }
    if (!MusicInstalled || !MusicAble) {
        return;
    }
    if (strcasecmp(MusicInstallChoice.driver_name, "sbawe32.mdi") != 0) {
        return;
    }
    if (strcasecmp(MusicType, "w") != 0) {
        return;
    }

    AWEFreeMem(MusicDriver, 1);
    FreeDOSmem(awe_buffer, awe_buffer_sel);
    FreeDOSmem(awe_preset, awe_preset_sel);
    Awe32SoundfontLoaded = 0;
}

void LoadAwe32Soundfont(const char *str)
{
    char locstr[FILENAME_MAX];
    long fsize, card_ram;
    SF_INFO *info;
    long *lmap;
    int i;

    if (UseCurrentAwe32Soundfont) {
        Awe32SoundfontLoaded = 1;
        return;
    }
    if (!MusicInstalled || !MusicAble) {
        return;
    }
    if (strcasecmp(MusicInstallChoice.driver_name, "sbawe32.mdi") != 0) {
        sprintf(SoundProgressMessage, "Soundfont - incompatible MIDI driver\n");
        SoundProgressLog(SoundProgressMessage);
        return;
    }
    if (strcasecmp(MusicType, "w") != 0) {
        sprintf(SoundProgressMessage, "Soundfont - no wave sound bank capability\n");
        SoundProgressLog(SoundProgressMessage);
        return;
    }

    if (Awe32SoundfontLoaded == 1) {
        FreeAwe32Soundfont();
        Awe32SoundfontLoaded = 0;
    }

    strncpy(CurrentAwe32SoundfontPrefix, str, sizeof(CurrentAwe32SoundfontPrefix));
    sprintf(locstr, "%s/%s.sbk", SoundDataPath, CurrentAwe32SoundfontPrefix);
    sprintf(SoundProgressMessage, "Soundfont - loading \"%s\"\n", locstr);
    SoundProgressLog(SoundProgressMessage);

    sbkHandle = LbFileOpen(locstr, Lb_FILE_MODE_READ_ONLY);
    if (sbkHandle == INVALID_FILE) {
        sprintf(SoundProgressMessage, "Soundfont - cannot open sound bank file\n");
        SoundProgressLog(SoundProgressMessage);
        return;
    }

    fsize = LbFileLengthHandle(sbkHandle);

    awe_buffer = AllocDOSmem(&awe_buffer_sel, 512);
    if ((awe_buffer_sel == 0) && (awe_buffer == 0)) {
        sprintf(SoundProgressMessage, "Soundfont - cannot alloc buffer\n");
        SoundProgressLog(SoundProgressMessage);
        LbFileClose(sbkHandle);
        return;
    }

    /* Get total amount of RAM on the card */
    card_ram = AWEGetTotalRAM(MusicDriver);
    if (card_ram == -1) {
        sprintf(SoundProgressMessage, "Soundfont - card refuses to tell its RAM\n");
        SoundProgressLog(SoundProgressMessage);
        LbFileClose(sbkHandle);
        return;
    }

    /* Define storage for each sample bank */
    lmap = (long *)awe_buffer;
    lmap[0] = 0;      /* General MIDI bank (no RAM) */
    lmap[1] = fsize;  /* SBK file */

    if (AWEDefMemMap(MusicDriver, 2, lmap, awe_buffer_sel) == 0) {
        sprintf(SoundProgressMessage, "Soundfont - setting sound banks map failed\n");
        SoundProgressLog(SoundProgressMessage);
        LbFileClose(sbkHandle);
        return;
    }

    /* Get Sound Font loading information */
    if (LbFileRead(sbkHandle, awe_buffer, 0x200) != 0x200) {
        sprintf(SoundProgressMessage, "Soundfont - sound bank read info failed\n");
        SoundProgressLog(SoundProgressMessage);
        LbFileClose(sbkHandle);
        return;
    }

    info = AWEGetSFInfo(MusicDriver, 1, awe_buffer, awe_buffer_sel);
    if (info == NULL) {
        sprintf(SoundProgressMessage, "Soundfont - get font info failed\n");
        SoundProgressLog(SoundProgressMessage);
        LbFileClose(sbkHandle);
        return;
    }

    /* Stream PCM data to on-board RAM */
    LbFileSeek(sbkHandle, info->sample_seek, Lb_FILE_SEEK_BEGINNING);
    for (i = 0; i < info->no_sample_packets; i++)
    {
        sprintf(SoundProgressMessage, "Soundfont - Loading samples - %d packets\n", i + 1);
        SoundProgressLog(SoundProgressMessage);
        if (LbFileRead(sbkHandle, awe_buffer, 0x200) <= 0)
        {
            sprintf(SoundProgressMessage, "Soundfont - sound bank read samples failed\n");
            SoundProgressLog(SoundProgressMessage);
            LbFileClose(sbkHandle);
            return;
        }
        if (AWEStreamSample(MusicDriver, 1, awe_buffer, awe_buffer_sel) == 0) {
            sprintf(SoundProgressMessage, "Soundfont - load samples to card failed\n");
            SoundProgressLog(SoundProgressMessage);
            LbFileClose(sbkHandle);
            return;
        }
    }

    /* Setup preset data */
    sprintf(SoundProgressMessage, "Soundfont - Loading presets - %ld bytes\n", info->preset_read_size);
    SoundProgressLog(SoundProgressMessage);

    awe_preset = AllocDOSmem(&awe_preset_sel, info->preset_read_size);
    if ((awe_preset_sel == 0) && (awe_preset == 0)) {
        sprintf(SoundProgressMessage, "Soundfont - cannot alloc preset buffer\n");
        SoundProgressLog(SoundProgressMessage);
        LbFileClose(sbkHandle);
        return;
    }

    LbFileSeek(sbkHandle, info->preset_seek, Lb_FILE_SEEK_BEGINNING);
    {
        if (LbFileRead(sbkHandle, awe_preset, info->preset_read_size) != info->preset_read_size)
        {
            sprintf(SoundProgressMessage, "Soundfont - sound bank read presets failed\n");
            SoundProgressLog(SoundProgressMessage);
            LbFileClose(sbkHandle);
            return;
        }
        if (AWELoadPreset(MusicDriver, 1, awe_preset, awe_preset_sel) != 0) {
            sprintf(SoundProgressMessage, "Soundfont - load presets to card failed\n");
            SoundProgressLog(SoundProgressMessage);
            LbFileClose(sbkHandle);
            return;
        }
    }

    Awe32SoundfontLoaded = 1;
    LbFileClose(sbkHandle);
}

/******************************************************************************/
