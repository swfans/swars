/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfdata.h
 *     Header file for gdata.c.
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
#ifndef BFLIBRARY_BFDATA_H_
#define BFLIBRARY_BFDATA_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

typedef struct TbLoadFiles TbLoadFiles;

typedef char *(*ModifyDataLoadFnameFunc)(TbLoadFiles *);

struct TbLoadFiles { // sizeof=44
    char FName[28]; // offset=0
    void **Start; // offset=28
    void **SEnd; // offset=32
    ulong SLength; // offset=36
    ushort Flags; // offset=40
    ushort Spare; // offset=42
};

#pragma pack()

char *defaultModifyDataLoadFilename(TbLoadFiles *ldfiles);
ModifyDataLoadFnameFunc LbDataLoadSetModifyFilenameFunction(ModifyDataLoadFnameFunc newfunc);


short LbDataFree(TbLoadFiles *load_file);
short LbDataFreeAll(TbLoadFiles load_files[]);

short LbDataLoad(TbLoadFiles *load_file);

/**
 * Loads a list of files. Allocates memory and loads new data.
 *
 * @return Returns amount of entries failed, or 0 on success.
 */
short LbDataLoadAll(TbLoadFiles load_files[]);

int LbDataFindNameIndex(TbLoadFiles load_files[], char *fname);
int LbDataFindStartIndex(TbLoadFiles load_files[], unsigned char **start);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFDATA_H_
/******************************************************************************/
