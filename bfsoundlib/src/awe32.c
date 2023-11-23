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
#include "bffile.h"
/******************************************************************************/
extern TbBool MusicAble;
extern TbBool MusicInstalled;
extern char MusicType[6];
extern AIL_INI MusicInstallChoice;
extern MDI_DRIVER *MusicDriver;
extern char SoundDataPath[144];

extern char CurrentAwe32SoundfontPrefix[12]; // = "Bullfrog";
extern TbFileHandle sbkHandle; // = INVALID_FILE;

extern uint8_t *awe_buffer;
extern uint16_t awe_buffer_seg;
extern uint8_t *awe_preset;
extern uint16_t awe_preset_seg;
/******************************************************************************/

void FreeAwe32Soundfont(void)
{
    if (UseCurrentAwe32Soundfont) {
        return;
    }
    if (!MusicInstalled || !MusicAble)
        return;
    if (strcasecmp(MusicInstallChoice.driver_name, "SBAWE32.MDI") != 0)
        return;
    if (strcasecmp(MusicType, "w") != 0)
        return;

    AWEFreeMem(MusicDriver, 1);
    FreeDOSmem(awe_buffer, awe_buffer_seg);
    FreeDOSmem(awe_preset, awe_preset_seg);
    Awe32SoundfontLoaded = 0;
}

void LoadAwe32Soundfont(const char *str)
{
    char locstr[FILENAME_MAX];
    long fsize;

    if (UseCurrentAwe32Soundfont) {
        Awe32SoundfontLoaded = 1;
        return;
    }
    if (!MusicInstalled || !MusicAble)
        return;
    if (strcasecmp(MusicInstallChoice.driver_name, "SBAWE32.MDI") != 0)
        return;
    if (strcasecmp(MusicType, "w") != 0)
        return;

    if (Awe32SoundfontLoaded == 1) {
        FreeAwe32Soundfont();
        Awe32SoundfontLoaded = 0;
    }

    strncpy(CurrentAwe32SoundfontPrefix, str, sizeof(CurrentAwe32SoundfontPrefix));
    sprintf(locstr, "%s/%s.sbk", SoundDataPath, CurrentAwe32SoundfontPrefix);
    sbkHandle = LbFileOpen(locstr, Lb_FILE_MODE_READ_ONLY);
    if (sbkHandle == INVALID_FILE) {
        return;
    }

    fsize = LbFileLengthHandle(sbkHandle);

    (void)fsize; // disable unused ver warning

#if 0 // Awe32 sound bank not implemented
    alloc = AllocDOSmem(512);
    awe_buffer_seg = alloc.seg;
    awe_buffer = alloc.offs;
    if ((awe_buffer_seg == 0) && (awe_buffer == 0)) {
        LbFileClose(sbkHandle);
        return;
    }

    if (AWEGetTotalRAM(MusicDriver) == -1) {
        LbFileClose(sbkHandle);
        return;
    }

    if (AWEDefMemMap(MusicDriver, 2, awe_buffer) == 0) {
        LbFileClose(sbkHandle);
        return;
    }

    if (LbFileRead(sbkHandle, awe_buffer, 0x200) != 0x200) {
        LbFileClose(sbkHandle);
        return;
    }

    if (AWEGetSFInfo(MusicDriver, 1, awe_buffer) == 0) {
        LbFileClose(sbkHandle);
        return;
    }

    while (LbFileRead(sbkHandle, awe_buffer, 0x200) > 0)
    {
        if (AWEStreamSample(MusicDriver, 1, awe_buffer) == 0) {
            LbFileClose(sbkHandle);
            return;
        }
    }

    if (LbFileRead(sbkHandle, awe_preset, preset_len) != preset_len) {
        LbFileClose(sbkHandle);
        return;
    }

    if (AWELoadPreset(MusicDriver, 1, awe_preset) != 0) {
        LbFileClose(sbkHandle);
        return;
    }
#endif

    Awe32SoundfontLoaded = 1;
    LbFileClose(sbkHandle);
}

/******************************************************************************/
