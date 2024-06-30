/******************************************************************************/
// Bullfrog Engine Smacker Playback Library - for use to remake classic games
// like Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file smack.h
 *     Header file for Smacker API.
 * @par Purpose:
 *     Interface of the smacker library.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2011 - 13 Jun 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFSMACKLIB_SMACK_H_
#define BFSMACKLIB_SMACK_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

typedef struct {
    uint8_t Field_0[1196];
} Smack;

/******************************************************************************/

#if defined(WIN32)
#  define RADAPI __stdcall
#else
#  define RADAPI
#endif

/******************************************************************************/

#ifdef __cplusplus
};
#endif

#endif // BFSMACKLIB_SMACK_H_
/******************************************************************************/
