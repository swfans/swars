/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfdata.h
 *     Header file for gdata.cpp.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_GDATA_H_
#define BFLIBRARY_GDATA_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

struct TbLoadFiles { // sizeof=44
    CBYTE FName[28]; // offset=0
    void **Start; // offset=28
    void **SEnd; // offset=32
    ULONG SLength; // offset=36
    UWORD Flags; // offset=40
    UWORD Spare; // offset=42
};

typedef struct TbLoadFiles TbLoadFiles;

int LbDataLoadSetModifyFilenameFunction();
int LbDataLoad();
int LbDataLoadAll();
int LbDataFree();
int LbDataFreeAll();

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_GDATA_H_
/******************************************************************************/
