/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file streamfx.c
 *     Reimplementation of Bullfrog Sound Library.
 * @par Purpose:
 *     Streamed digital sound playback implementation.
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

#include "streamfx.h"
#include "bfmemory.h"
#include "bffile.h"
#include "aildebug.h"
#include "memfile.h"
#include "mssal.h"
#include "snderr.h"
/******************************************************************************/

TbBool ssnd_active = true;
TbFileHandle adpcm_handle = INVALID_FILE;
long adpcm_file_open = 0;
SNDSAMPLE *sample_handle = NULL;
uint8_t *ssnd_buffer[2];
uint8_t *adpcm_source_buffer = NULL;
int16_t *mixer_buffer = NULL;
TbFileHandle sample_file = INVALID_FILE;
TbBool mixed_file_open = false;

TbBool StreamedSoundAble = false;

extern TbBool SoundInstalled;
extern TbBool SoundAble;
extern DIG_DRIVER *SoundDriver;
/******************************************************************************/

void close_adpcm_file(void)
{
    if (adpcm_handle == INVALID_FILE)
        return;
    LbFileClose(adpcm_handle);
    adpcm_handle = INVALID_FILE;
}

TbBool allocate_buffers(void)
{
    mixer_buffer = LbMemoryAlloc(0x8000u);
    if (mixer_buffer == NULL)
        return false;
    adpcm_source_buffer = LbMemoryAlloc(0x800u);
    if (adpcm_source_buffer == NULL)
        return false;
    ssnd_buffer[0] = AIL_MEM_alloc_lock(0x4000);
    if (ssnd_buffer[0] == NULL)
        return false;
    ssnd_buffer[1] = AIL_MEM_alloc_lock(0x4000);
    if (ssnd_buffer[1] == NULL)
        return false;
    return true;
}

void free_buffers(void)
{
    if (ssnd_buffer[0] != NULL) {
        AIL_MEM_free_lock(ssnd_buffer[0], 0x4000);
        ssnd_buffer[0] = NULL;
    }
    if (ssnd_buffer[1] != NULL) {
        AIL_MEM_free_lock(ssnd_buffer[1], 0x4000);
        ssnd_buffer[1] = NULL;
    }
    if (adpcm_source_buffer != NULL) {
        LbMemoryFree(adpcm_source_buffer);
        adpcm_source_buffer = NULL;
    }
    if (mixer_buffer != NULL) {
        LbMemoryFree(mixer_buffer);
        mixer_buffer = NULL;
    }
}

void InitStreamedSound(void)
{
    if (!SoundInstalled || !SoundAble || StreamedSoundAble)
        return;

#if 0
    flushall(); // Not a standard C func; also why?
    setbuf(stdout, NULL); // No reason for that
#endif

    if (!allocate_buffers())
    {
        free_buffers();
        SNDLOGFAIL("Streamed sound", "cannot allocate buffers");
        return;
    }

    sample_handle = AIL_allocate_sample_handle(SoundDriver);
    if (!sample_handle)
    {
        free_buffers();
        SNDLOGFAIL("Streamed sound", "cannot allocate handle");
        return;
    }
    StreamedSoundAble = true;
}

void SwitchOffStreamedSound(void)
{
    if (!StreamedSoundAble || !ssnd_active)
        return;
    AIL_end_sample(sample_handle);
    memset(ssnd_buffer[0], 0, 0x4000u);
    memset(ssnd_buffer[1], 0, 0x4000u);
    if (sample_file != INVALID_FILE) {
        LbFileClose(sample_file);
        sample_file = INVALID_FILE;
    }
    if (adpcm_file_open) {
        close_adpcm_file();
        adpcm_file_open = 0;
    }
    if (mixed_file_open)
        mixed_file_open = false;
    ssnd_active = false;
}

void FreeStreamedSound(void)
{
    if (sample_file != INVALID_FILE) {
        LbFileClose(sample_file);
        sample_file = INVALID_FILE;
    }
    SwitchOffStreamedSound();
    free_buffers();
    StreamedSoundAble = false;
}

/******************************************************************************/
