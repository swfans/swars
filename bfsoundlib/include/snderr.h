/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file snderr.h
 *     Header file for snderr.c.
 * @par Purpose:
 *     Unknown.
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
#ifndef BFSOUNDLIB_SNDERROR_H_
#define BFSOUNDLIB_SNDERROR_H_

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

extern char SoundProgressMessage[256];
extern long DebugAudio;

void SoundProgressLog(const char *msg);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // BFSOUNDLIB_SNDERROR_H_
