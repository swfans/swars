/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file lvfiles.h
 *     Header file for lvfiles.c.
 * @par Purpose:
 *     Routines for level and map files handling.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef LVFILES_H
#define LVFILES_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct Objective;

struct QuickLoad {
    ushort *Numb;
    void **Ptr;
    ushort Size;
    ushort Extra;
};

struct LevelMisc { // sizeof=22
    short X;
    short Y;
    short Z;
    short Group;
    short Weapon;
    ubyte Flag;
    ubyte Type;
    ubyte field_C[9];
    ubyte field_15;
};

#pragma pack()
/******************************************************************************/
extern struct LevelMisc *game_level_miscs;
extern TbBool level_deep_fix;
extern struct Objective *game_used_lvl_objectives;
extern ushort next_used_lvl_objective;


TbResult load_mad_pc(ushort mapno);

void load_level_pc(short level, short missi, ubyte reload);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
