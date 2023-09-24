/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ssamplst.c
 *     Music Sequence List support routines.
 * @par Purpose:
 *     Functions for handling a list/queue of music sequences.
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

#include "bfmusic.h"
#include "bffile.h"
#include "bfendian.h"
#include "rnc_1fm.h"
#include "aildebug.h"
#include "mssal.h"
#include "snderr.h"
/******************************************************************************/
#pragma pack(1)

struct MusicBankHead {
  long info_offset;
  long data_offset;
  long info_size;
  long data_size;
};

#pragma pack()

extern TbBool MusicInstalled;
extern TbBool MusicAble;
extern TbBool MusicActive;
extern char MusicType[6];

extern ushort NumberOfSongs;

extern TbBool DisableLoadMusic;
extern char full_music_data_path[144];
extern void *BfMusicData;
extern struct BfMusicInfo *BfMusic;
extern struct BfMusicInfo *BfEndMusic;

/******************************************************************************/

void format_music(void)
{
    short nsongs;
    ubyte *dt;
    struct BfMusicInfo *musinf;
    struct BfMusicInfo *musinfend;
    ulong offs;

    nsongs = NumberOfSongs;
    musinfend = BfEndMusic;
    dt = BfMusicData;
    if ((BfMusic != NULL) && (BfMusicData != NULL))
    {
        nsongs = 0;
        for (musinf = &BfMusic[1]; musinf < musinfend; musinf++)
        {
            offs = (ulong)musinf->DataBeg;
            musinf->DataBeg = &dt[offs];
            offs = (ulong)musinf->DataEnd;
            musinf->DataEnd = &dt[offs];
            nsongs++;
        }
    }
    BfMusicData = dt;
    BfEndMusic = musinfend;
    NumberOfSongs = nsongs;
}

void init_music_bank_songs(void)
{
    // No action needed, song will be initiated by StartMusic()
}

ubyte load_music_bank(TbFileHandle fh, ubyte bankId)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_load_music_bank\n"
        : "=r" (ret) : "a" (fh),  "d" (bankId));
    return ret;
#endif
    ubyte *m;
    ubyte *dt;
    struct MusicBankHead mbhead[4];

    LbFilePosition(fh);
    LbFileRead(fh, &mbhead, sizeof(mbhead));
    if (mbhead[bankId].data_offset == -1)
        return 0;

    dt = BfMusicData;
    if (dt == NULL)
        return 0;
    m = (ubyte *)BfMusic;
    if (m == NULL)
        return 0;
    // Read songs data
    BfEndMusic = (struct BfMusicInfo *)(m + mbhead[bankId].info_size);
    LbFileSeek(fh, mbhead[bankId].data_offset, Lb_FILE_SEEK_BEGINNING);
    LbFileRead(fh, dt, 8);
    if (blong(dt+0) == RNC_SIGNATURE)
    {
        long flength;
        flength = blong(dt+4);
        LbFileRead(fh, dt + 8, flength - 8);
        UnpackM1(dt, flength);
    }
    else
    {
        LbFileRead(fh, dt + 8, mbhead[bankId].data_size - 8);
    }
    // Read songs info
    LbFileSeek(fh, mbhead[bankId].info_offset, Lb_FILE_SEEK_BEGINNING);
    LbFileRead(fh, m, 8);
    if (blong(m+0) == RNC_SIGNATURE)
    {
        long flength;
        flength = blong(m+4);
        LbFileRead(fh, m + 8, flength - 8);
        UnpackM1(m, flength);
    }
    else
    {
        LbFileRead(fh, m + 8, mbhead[bankId].info_size - 8);
    }

    format_music();
    init_music_bank_songs();

    sprintf(SoundProgressMessage, "BF113 - loaded music bank %d, %d songs\n",
            (int)bankId, (int)NumberOfSongs);
    SoundProgressLog(SoundProgressMessage);

    MusicAble = true;
    return 1;
}

int LoadMusic(ushort bankNo)
{
#if 0
    int ret;
    asm volatile ("call ASM_LoadMusic\n"
        : "=r" (ret) : "a" (a1));
    return ret;
#endif
    TbFileHandle fh;
    long fsize;
    ulong nbanks_offs;
    ushort nbanks[4];
    ubyte bankId;

    sprintf(SoundProgressMessage, "BF48 - load music bank %d\n", bankNo);
    SoundProgressLog(SoundProgressMessage);

    if (!MusicInstalled) {
        sprintf(SoundProgressMessage, "BF53 - load music bank - failed - music not installed\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }
    if (!MusicAble) {
        sprintf(SoundProgressMessage, "BF53 - load music bank - failed - MusicAble = 0\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }
    if (DisableLoadMusic) {
        sprintf(SoundProgressMessage, "BF53 - load music bank - failed - LoadMusic Disabled = 0\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    StopMusic();
    fh = LbFileOpen(full_music_data_path, Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE) {
        sprintf(SoundProgressMessage, "BF52 - load music bank - failed - no music.dat\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    LbFileSeek(fh, 0, Lb_FILE_SEEK_END);
    fsize = LbFilePosition(fh);

    // The bank file ends with offset to array of banks
    LbFileSeek(fh, fsize - 4, Lb_FILE_SEEK_BEGINNING);
    LbFileRead(fh, &nbanks_offs, 4);
    LbFileSeek(fh, nbanks_offs, Lb_FILE_SEEK_BEGINNING);
    // Array of banks has 4 16-bit entries
    LbFileRead(fh, nbanks, 8);

    switch (MusicType[0])
    {
    case 'G':
    case 'g':
    default:
        bankId = 0;
        break;
    case 'R':
    case 'r':
        bankId = 1;
        break;
    case 'F':
    case 'f':
        bankId = 2;
        break;
    case 'W':
    case 'w':
        bankId = 3;
        break;
    }

    if (bankNo + 1 > nbanks[bankId]) {
        LbFileClose(fh);
        sprintf(SoundProgressMessage, "BF49 - load music bank - failed - bank %d not found\n", (int)bankId);
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    // Each bank has a 64-byte header, place the file at the one we want
    LbFileSeek(fh, bankNo * 64, Lb_FILE_SEEK_CURRENT);
    // Load the music tracks and info on each track
    if (!load_music_bank(fh, bankId)) {
        LbFileClose(fh);
        sprintf(SoundProgressMessage, "BF50 - load music bank - failed - cannot allocate\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    LbFileClose(fh);
    sprintf(SoundProgressMessage, "BF51 - load music bank - passed\n");
    SoundProgressLog(SoundProgressMessage);
    return 0;
}

/******************************************************************************/
