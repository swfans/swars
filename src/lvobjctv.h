/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file lvobjctv.h
 *     Header file for lvobjctv.c.
 * @par Purpose:
 *     Routines for level objectives handling.
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
#ifndef LVOBJCTV_H
#define LVOBJCTV_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

/* deprecated - when callbacks are defined within objectv_defs[],
 * there will be no need to reference specific objective.
 */
enum GameObjectiveTypes {
    GAME_OBJ_NONE = 0,
    GAME_OBJ_P_DEAD,
    GAME_OBJ_ALL_G_DEAD,
    GAME_OBJ_MEM_G_DEAD,
    GAME_OBJ_P_NEAR,
    GAME_OBJ_MEM_G_NEAR,
    GAME_OBJ_P_ARRIVES,
    GAME_OBJ_MEM_G_ARRIVES,
    GAME_OBJ_ALL_G_ARRIVES,
    GAME_OBJ_PERSUADE_P,
    GAME_OBJ_PERSUADE_MEM_G,
    GAME_OBJ_PERSUADE_ALL_G,
    GAME_OBJ_TIME,
    GAME_OBJ_GET_ITEM,
    GAME_OBJ_USE_ITEM,
    GAME_OBJ_FUNDS,
    GAME_OBJ_DESTROY_OBJECT,
    GAME_OBJ_PKILL_P,
    GAME_OBJ_PKILL_G,
    GAME_OBJ_PKILL_ALL_G,
    GAME_OBJ_USE_PANET,
    GAME_OBJ_UNUSED_21,
    GAME_OBJ_PROTECT_G,
    GAME_OBJ_P_PERS_G,
    GAME_OBJ_ALL_G_USE_V,
    GAME_OBJ_UNUSED_25,
    GAME_OBJ_UNUSED_26,
};

struct Objective { // sizeof=32
    ushort Next;
    ubyte Map;
    ubyte Level;
    ushort Status;
    ushort Type;
    ushort Flags;
    short Thing;
    short X;
    short Y;
    short Z;
    short Radius;
    ubyte Pri;
    ubyte Arg2;
    ushort StringIndex;
    ushort UniqueID;
    ubyte ObjText;
    ubyte field_1B[4];
    ubyte field_1F;
};

#pragma pack()
/******************************************************************************/
extern struct Objective *game_used_objectives;
extern ushort next_used_objective; // = 1;
extern struct Objective *game_objectives;
extern ushort next_objective;

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
