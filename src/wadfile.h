/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file wadfile.h
 *     Header file for wadfile.c.
 * @par Purpose:
 *     Support for WAD files, used as an aggregate for a set of small files.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 19 Aug 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef WADFILE_H
#define WADFILE_H

#include "bftypes.h"
#include "bffile.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct WADIndexEntry {
    char Filename[12];
    ulong Offset;
    ulong Length;
};

#pragma pack()
/******************************************************************************/

TbFileHandle open_file_from_wad(const char *filename, const char *wadfile);
int load_file_wad(const char *filename, const char *wadfile, void *outbuf);

int load_file_alltext(const char *filename, void *outbuf);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
