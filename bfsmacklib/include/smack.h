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

struct SmackSndTrk { // sizeof=112
    uint32_t field_0[5];
    uint32_t field_14;
    uint32_t field_18[7];
    SmackSndTrk *next;
    struct SNDSAMPLE *smp;
    uint32_t field_3C;
    uint32_t field_40;
    uint32_t field_44;
    uint32_t field_48;
    uint32_t field_4C;
    uint32_t field_50;
    void *field_54;
    uint32_t field_58;
    uint32_t field_5C[3];
    uint32_t field_68;
    uint32_t field_6C;
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
