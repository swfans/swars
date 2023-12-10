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
#pragma pack(1)

struct BfSoundBankHead {
  long TabPos;
  long DatPos;
  long TabSize;
  long DatSize;
};

#pragma pack()


extern TbBool SoundInstalled;
extern TbBool DisableLoadSounds;

extern TbBool SoundAble;
extern ushort SoundType;

extern TbBool sample_queue_handle_initiated;
extern TbBool sample_queue_handle_stopped;
extern SNDSAMPLE *sample_queue_handle;

extern char full_sound_data_path[224];
extern ubyte CurrentSoundBank; // = -1;

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

    sample_queue_handle_stopped = 1;
    AIL_register_EOS_callback(sample_queue_handle, 0);
    AIL_end_sample(sample_queue_handle);

    for (i = 0; i < 8; i++)
        AIL_set_sample_user_data(sample_queue_handle, i, 0);
}

void format_sounds(void)
{
    asm volatile ("call ASM_format_sounds\n"
        :  : );
}

ubyte load_sound_bank(TbFileHandle fh, ubyte bank_tpno)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_load_sound_bank\n"
        : "=r" (ret) : "a" (fh),  "d" (bank_tpno));
    return ret;
#else
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
#endif
}

int LoadSounds(ubyte bank_no)
{
    TbFileHandle fh;
    long len, banks_offs;
    ushort tpno;
    ushort banks_per_tpno[9];

    sprintf(SoundProgressMessage, "BF42 - load sound bank %d\n", bank_no);
    SoundProgressLog(SoundProgressMessage);

    if (!SoundInstalled || !SoundAble || (CurrentSoundBank == bank_no) || DisableLoadSounds)
    {
        if (!SoundInstalled)
            sprintf(SoundProgressMessage, "BF47 - load sound bank - failed - Sound not installed\n");
        else if (!SoundAble)
            sprintf(SoundProgressMessage, "BF47 - load sound bank - failed - SoundAble = 0\n");
        else if (DisableLoadSounds)
            sprintf(SoundProgressMessage, "BF47 - load sound bank - failed - LoadSounds disabled\n");
        else
            sprintf(SoundProgressMessage, "BF47 - load sound bank - failed - already loaded\n");

        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    StopAllSamples();
    fh = LbFileOpen(full_sound_data_path, Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE) {
        sprintf(SoundProgressMessage, "BF46 - load sound bank - failed - no sound.dat\n");
        SoundProgressLog(SoundProgressMessage);
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
        sprintf(SoundProgressMessage, "BF43 - load sound bank - failed - bad sound type\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    if (bank_no + 1 > banks_per_tpno[tpno]) {
        LbFileClose(fh);
        sprintf(SoundProgressMessage, "BF43 - load sound bank - failed - bank not found\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }
    LbFileSeek(fh, 144 * bank_no, 1u);

    if (!load_sound_bank(fh, tpno)) {
        LbFileClose(fh);
        sprintf(SoundProgressMessage, "BF44 - load sound bank - failed - cannot allocate\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }
    CurrentSoundBank = bank_no;
    LbFileClose(fh);
    sprintf(SoundProgressMessage, "BF45 - load sound bank - passed\n");
    SoundProgressLog(SoundProgressMessage);
    return 0;
}

/******************************************************************************/
