/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ssamplst.h
 *     Header file for ssamplst.c.
 * @par Purpose:
 *     Sound Sample List support routines.
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
#ifndef BFSOUNDLIB_SSAMPLST_H_
#define BFSOUNDLIB_SSAMPLST_H_

#include <stdint.h>
#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct BfSoundBankHead {
  long TabPos;
  long DatPos;
  long TabSize;
  long DatSize;
};

struct BfSfxInfo { // sizeof=32
  char FileName[12];
  long sffld_12;
  short sffld_16;
  ubyte *DataBeg;
  long sffld_22;
  ubyte *DataEnd;
  short sffld_30;
};

#pragma pack()
/******************************************************************************/

void StopSampleQueueList(void);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // BFSOUNDLIB_SSAMPLST_H_
