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
struct SNDSAMPLE;
typedef struct SmackSndTrk SmackSndTrk;
typedef struct Smack Smack;

struct SmackSndTrk { // sizeof=?
    uint32_t field_0[13];
    SmackSndTrk *next;
    struct SNDSAMPLE *smp;
    uint8_t field_3C[68];
};

struct Smack { // sizeof=1196
    uint8_t field_0[1024];
    SmackSndTrk *Unkn404PerTrack[7];
    uint8_t field_420[144];
};

/******************************************************************************/

#if defined(WIN32) && 0 // Disabled - `mkwrappers` expects cdecl on C side
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
