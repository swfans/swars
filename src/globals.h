/******************************************************************************/
// Free implementation of Bullfrog's Dungeon Keeper strategy game.
/******************************************************************************/
/** @file globals.h
 *     Bullfrog engine global compile config file.
 * @par Purpose:
 *     Header file for global definitions.
 * @par Comment:
 *     Defines basic includes and definitions, used in whole program.
 * @author   Tomasz Lis
 * @date     08 Aug 2008 - 03 Jan 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_GLOBALS_H
#define BFLIB_GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>

#if defined(unix) && !defined(GO32)
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>
#if !defined(stricmp)
#define stricmp strcasecmp
#endif
#if !defined(strnicmp)
#define strnicmp strncasecmp
#endif

#elif defined(MSDOS)
#include <dos.h>
#include <process.h>
#endif

#ifdef __cplusplus
#include <algorithm>
using std::min;
using std::max;
extern "C" {
#endif

// Basic Definitions

#if defined(unix) && !defined (GO32)
#define SEPARATOR "/"
#else
#define SEPARATOR "\\"
#endif

#ifndef __cplusplus
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#endif

// Return values for verification functions
#define VERIF_ERROR   0
#define VERIF_OK      1
#define VERIF_WARN    2

// Return values for all other functions
#define ERR_NONE           0
// Note: error codes -1..-79 are reserved standard C library errors with sign reverted.
//    these are defined in errno.h
#define ERR_BASE_RNC      -90

#pragma pack(1)

/** Screen coordinate in scale of the game (resolution independent). */
typedef int ScreenCoord;
/** Screen coordinate in scale of the real screen. */
typedef int RealScreenCoord;
/** Player identification number, or owner of in-game thing/room/slab. */
typedef signed char PlayerNumber;
/** Type which stores thing class. */
typedef unsigned char ThingClass;
/** Type which stores thing model. */
typedef unsigned char ThingModel;
/** Type which stores thing index. */
typedef unsigned short ThingIndex;
typedef long HitPoints;
/** Level number within a campaign. */
typedef long LevelNumber;
/** Game turn number, used for in-game time computations. */
typedef unsigned long GameTurn;
/** Game turns difference, used for in-game time computations. */
typedef long GameTurnDelta;
/** Identifier of a national text string. */
typedef int TextStringId;
/** Map coordinate in full resolution. Position within subtile is scaled 0..255. */
typedef long MapCoord;
/** Distance between map coordinates in full resolution. */
typedef long MapCoordDelta;
/** Map subtile coordinate. Every slab consists of 3x3 subtiles. */
typedef long MapSubtlCoord;
/** Distance between map subtiles. */
typedef long MapSubtlDelta;
/** Map slab coordinate. Slab is a cubic part of map with specific content. */
/** Movement speed on objects in the game. */
typedef short MoveSpeed;

#pragma pack()

#ifdef __cplusplus
}
#endif
#endif // BFLIB_GLOBALS_H
