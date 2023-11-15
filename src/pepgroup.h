/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file pepgroup.h
 *     Header file for pepgroup.c.
 * @par Purpose:
 *     People groups support.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2023 - 11 Jul 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef PEPGROUP_H
#define PEPGROUP_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

#define PEOPLE_GROUPS_COUNT 32

/** Data presenting group attitude toward other groups.
 */
struct WarFlag { // sizeof=40
 /** Bitfields of groups whose members are attacked  on sight.
  */
  u32 KillOnSight;
 /** Bitfields of groups whose members are attacked if wielding visible weapons.
  */
  u32 KillIfWeaponOut;
 /** Bitfields of groups whose members are attacked if have any weapons, even concealed.
  */
  u32 KillIfArmed;
 /** Bitfields of groups whose members are not attacked.
  */
  u32 Truce;
  u8 Guardians[8];
  i32 ugfld_24;
  i32 ugfld_28;
  i32 ugfld_32;
  i32 ugfld_36;
};

struct GroupAction {
  u32 FiredOn;
  u32 Hit;
  i16 Dead;
  i16 Alive;
  i16 Persuaded;
  i16 Storming;
};

#pragma pack()
/******************************************************************************/
extern struct WarFlag war_flags[PEOPLE_GROUPS_COUNT];
extern struct GroupAction group_actions[PEOPLE_GROUPS_COUNT];

/** Finds Group ID for which there are no things created.
 */
i16 find_unused_group_id(TbBool largest);

/** Count how many people of given kind are in given group.
 *
 * This can be called during level load, as it does not use per-type linked lists.
 */
u16 count_people_in_group(u16 group, i16 subtype);

/** Copy all properties of one group into another group.
 */
void thing_group_copy(i16 pv_group, i16 nx_group, u8 allow_kill);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
