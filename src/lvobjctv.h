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
#include "bffile.h"

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
    GAME_OBJ_PKILL_MEM_G,
    GAME_OBJ_PKILL_ALL_G,
    GAME_OBJ_USE_PANET,
    GAME_OBJ_UNUSED_21,
    GAME_OBJ_PROTECT_G,
    GAME_OBJ_P_PERS_G,
    GAME_OBJ_ALL_G_USE_V,
    GAME_OBJ_MEM_G_USE_V,
    GAME_OBJ_V_ARRIVES,
    GAME_OBJ_DESTROY_V,
    GAME_OBJ_ITEM_ARRIVES,
    GAME_OBJ_TYPES_COUNT,
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

struct BriefObjective { // sizeof = 20
  ushort CreditCost;
  ushort TextOffset;
  ubyte brobjfld_2;
  ushort brobjfld_3;
  ubyte TextLines;
  ubyte X[5];
  ubyte Z[5];
  ubyte brobjfld_12;
  ubyte brobjfld_13;
};

#pragma pack()
/******************************************************************************/
extern struct Objective *game_used_objectives;
extern ushort next_used_objective; // = 1;
extern struct Objective *game_objectives;
extern ushort next_objective;

extern struct BriefObjective brief_objectives[10];
extern ubyte brief_objectives_count;

int add_used_objective(long mapno, long levelno);

TbBool objective_target_is_to_be_acquired(struct Objective *p_objectv);
TbBool objective_target_is_ally(struct Objective *p_objectv);

TbBool objective_target_is_group(struct Objective *p_objectv);
TbBool objective_target_is_group_to_area(struct Objective *p_objectv);
TbBool objective_target_is_group_to_thing(struct Objective *p_objectv);
TbBool objective_target_is_group_to_vehicle(struct Objective *p_objectv);
TbBool objective_target_is_person(struct Objective *p_objectv);
TbBool objective_target_is_person_to_area(struct Objective *p_objectv);
TbBool objective_target_is_person_to_thing(struct Objective *p_objectv);
TbBool objective_target_is_vehicle(struct Objective *p_objectv);
TbBool objective_target_is_vehicle_to_area(struct Objective *p_objectv);
TbBool objective_target_is_item(struct Objective *p_objectv);
TbBool objective_target_is_item_to_area(struct Objective *p_objectv);
TbBool objective_target_is_object(struct Objective *p_objectv);
TbBool objective_target_is_any_thing(struct Objective *p_objectv);

/** Fixes parameters within objectives.
 *
 * @return Gives 2 if a parameter was updated, 1 if no update was neccessary, 0 if update failed.
 */
ubyte fix_single_objective(struct Objective *p_objectv, ushort objectv, const char *srctext);

void save_objective_chain_conf(TbFileHandle fh, ushort objectv_head, char *buf, ulong buflen);
int parse_next_used_objective(const char *buf, long buflen, long pri, long mapno, long levelno);

void load_brief_objectives(ubyte mapno, ubyte level);

void snprint_objective(char *buf, ulong buflen, struct Objective *p_objectv, ushort objectv);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
