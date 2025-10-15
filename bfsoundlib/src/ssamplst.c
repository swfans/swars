/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ssamplst.c
 *     Sound Sample List support routines.
 * @par Purpose:
 *     Functions for handling a list/queue of sound samples.
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
#include <assert.h>
#include <string.h>

#include "ssamplst.h"
#include "aildebug.h"
#include "mssal.h"
#include "snderr.h"
#include "ssampply.h"
#include "bfsound.h"
#include "bfsvaribl.h"
#include "bffile.h"
#include "bfendian.h"
#include "rnc_1fm.h"
/******************************************************************************/

TbBool sample_queue_handle_initiated = false;
TbBool sample_queue_handle_stopped = 1;
SNDSAMPLE *sample_queue_handle = NULL;

sbyte CurrentSoundBank = -1;
long current_sample_queue_count = 0;

extern TbBool SoundInstalled;
extern TbBool DisableLoadSounds;

extern TbBool SoundAble;
extern ushort SoundType;
extern TbBool SoundActive;

extern ushort NumberOfSamples;
extern char full_sound_data_path[224];
extern DIG_DRIVER *SoundDriver;

extern void *SfxData;
extern void *Sfx;
extern void *EndSfxs;

extern long largest_dat_size;
extern long largest_tab_size;

/******************************************************************************/

void StopSampleQueueList(void)
{
    int i;

    if (!sample_queue_handle_initiated)
        return;

    sample_queue_handle_stopped = true;
    AIL_register_EOS_callback(sample_queue_handle, 0);
    AIL_end_sample(sample_queue_handle);

    for (i = 0; i < 8; i++)
        AIL_set_sample_user_data(sample_queue_handle, i, 0);
}

void cb_sample_queue_callback(SNDSAMPLE *s)
{
    struct BfSfxInfo *sfi;
    short sfxid;

    if (sample_queue_handle_stopped || current_sample_queue_count >= 8)
        return;

    sfxid = AIL_sample_user_data(sample_queue_handle, current_sample_queue_count);

    if (sfxid == 0)
        return;

    AIL_init_sample(sample_queue_handle);
    AIL_register_EOS_callback(sample_queue_handle, cb_sample_queue_callback);
    sfi = &((struct BfSfxInfo *)Sfx)[sfxid];
    AIL_set_sample_file(sample_queue_handle, sfi->DataBeg, 1);
    current_sample_queue_count++;
    AIL_start_sample(sample_queue_handle);
}

void PlaySampleList(int sfx1, int sfx2, int sfx3, int sfx4, int sfx5, int sfx6, int sfx7, int sfx8)
{
    if (!SoundInstalled || !SoundAble || !SoundActive)
        return;

    if ( !sample_queue_handle_initiated )
    {
        sample_queue_handle = AIL_allocate_sample_handle(SoundDriver);
        sample_queue_handle_initiated = 1;
    }
    sample_queue_handle_stopped = true;
    AIL_end_sample(sample_queue_handle);
    AIL_set_sample_user_data(sample_queue_handle, 0, sfx1);
    AIL_set_sample_user_data(sample_queue_handle, 1, sfx2);
    AIL_set_sample_user_data(sample_queue_handle, 2, sfx3);
    AIL_set_sample_user_data(sample_queue_handle, 3, sfx4);
    AIL_set_sample_user_data(sample_queue_handle, 4, sfx5);
    AIL_set_sample_user_data(sample_queue_handle, 5, sfx6);
    AIL_set_sample_user_data(sample_queue_handle, 6, sfx7);
    AIL_set_sample_user_data(sample_queue_handle, 7, sfx8);
    current_sample_queue_count = 0;
    sample_queue_handle_stopped = false;
    cb_sample_queue_callback(sample_queue_handle);
}

void format_sounds(void)
{
    short n;
    struct BfSfxInfo *sfiend;
    ubyte *dt;
    struct BfSfxInfo *sfi;

    n = NumberOfSamples;
    sfiend = (struct BfSfxInfo *)EndSfxs;
    sfi = (struct BfSfxInfo *)Sfx;
    dt = SfxData;
    if ((Sfx != NULL) && (dt != NULL))
    {
        n = 0;
        for (sfi++; sfi < sfiend; sfi++)
        {
            ulong offs;
            offs = (ulong)sfi->DataBeg;
            sfi->DataBeg = &dt[offs];
            n++;
        }
    }
    SfxData = dt;
    EndSfxs = sfiend;
    NumberOfSamples = n;
}

ubyte load_sound_bank(TbFileHandle fh, ubyte bank_tpno)
{
    struct BfSoundBankHead head[9];

    LbFileRead(fh, head, 9 * sizeof(struct BfSoundBankHead));
    if (head[bank_tpno].DatPos == -1)
        return 0;
    SoundAble = false;
    if (SfxData == NULL || Sfx == NULL)
        return 0;
    memset(SfxData, 0, largest_dat_size);
    memset(Sfx, 0, largest_tab_size);
    EndSfxs = Sfx + head[bank_tpno].TabSize;

    LbFileSeek(fh, head[bank_tpno].DatPos, 0);
    LbFileRead(fh, SfxData, 8);
    if (blong(SfxData+0) == RNC_SIGNATURE)
    {
        long fsize = blong(SfxData+4);
        LbFileRead(fh, SfxData + 8, fsize - 8);
        UnpackM1(SfxData, fsize);
    }
    else
    {
        LbFileRead(fh, SfxData + 8, head[bank_tpno].DatSize - 8);
    }

    LbFileSeek(fh, head[bank_tpno].TabPos, 0);
    LbFileRead(fh, Sfx, 8);
    if (blong(Sfx+0) == RNC_SIGNATURE)
    {
        long fsize = blong(Sfx+4);
        LbFileRead(fh, Sfx + 8, fsize - 8);
        UnpackM1(Sfx, fsize);
    }
    else
    {
        LbFileRead(fh, Sfx + 8, head[bank_tpno].TabSize - 8);
    }

    format_sounds();

    SoundAble = 1;
    return 1;
}

int LoadSounds(ubyte bank_no)
{
    TbFileHandle fh;
    long len, banks_offs;
    ushort tpno;
    ushort banks_per_tpno[9];

    SNDLOGSYNC("Sound bank", "start load bank %d", bank_no);

    if (!SoundInstalled || !SoundAble || (CurrentSoundBank == bank_no) || DisableLoadSounds)
    {
        if (!SoundInstalled) {
            SNDLOGSYNC("Sound bank", "sound not installed");
        } else if (!SoundAble) {
            SNDLOGSYNC("Sound bank", "not SoundAble");
        } else if (DisableLoadSounds) {
            SNDLOGSYNC("Sound bank", "LoadSounds disabled");
        } else {
            SNDLOGSYNC("Sound bank", "already loaded");
        }
        return 1;
    }

    StopAllSamples();
    fh = LbFileOpen(full_sound_data_path, Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE) {
        SNDLOGFAIL("Sound bank", "no 'sound.dat'");
        return 1;
    }

    len = LbFileLength(full_sound_data_path);
    LbFileSeek(fh, len - 4, 0);
    LbFileRead(fh, &banks_offs, 4);
    LbFileSeek(fh, banks_offs, 0);
    LbFileRead(fh, banks_per_tpno, sizeof(banks_per_tpno));

    tpno = GetSoundTpNo(SoundType);
    if (tpno >= 255) {
        LbFileClose(fh);
        SNDLOGFAIL("Sound bank", "bad sound type");
        return 1;
    }

    if (bank_no + 1 > banks_per_tpno[tpno]) {
        LbFileClose(fh);
        SNDLOGFAIL("Sound bank", "bank not found");
        return 1;
    }
    LbFileSeek(fh, 144 * bank_no, 1u);

    if (!load_sound_bank(fh, tpno)) {
        LbFileClose(fh);
        SNDLOGFAIL("Sound bank", "cannot allocate");
        return 1;
    }
    CurrentSoundBank = bank_no;
    LbFileClose(fh);
    SNDLOGSYNC("Sound bank", "load bank passed");
    return 0;
}

/******************************************************************************/
