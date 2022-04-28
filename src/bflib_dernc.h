/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_dernc.h
 *     Header file for bflib_dernc.c.
 * @par Purpose:
 *     Rob Northern Computing compression support.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     10 Feb 2008 - 30 Dec 2008
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_DERNC_H
#define BFLIB_DERNC_H

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
long LbFileLengthRnc(const char *fname);
long LbFileLoadAt(const char *fname, void *buffer);
long LbFileSaveAt(const char *fname, const void *buffer,unsigned long len);
/******************************************************************************/

#ifdef __cplusplus
}
#endif
#endif
