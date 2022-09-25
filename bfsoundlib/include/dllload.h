/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file dllload.h
 *     Header file for dllload.c.
 * @par Purpose:
 *     File load functions from MSS API.
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
#ifndef AIL2OAL_DLLLOAD_H_
#define AIL2OAL_DLLLOAD_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
/** Return size of file, or -1 if error.
 */
int32_t FILE_size(char const *fname);

/** Read file into memory at *dest, allocating memory first if dest=NULL.
 */
void *FILE_read(const char *fname, void *dest);
/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_DLLLOAD_H_
