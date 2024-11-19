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
  ulong KillOnSight;
 /** Bitfields of groups whose members are attacked if wielding visible weapons.
  */
  ulong KillIfWeaponOut;
 /** Bitfields of groups whose members are attacked if have any weapons, even concealed.
  */
  ulong KillIfArmed;
 /** Bitfields of groups whose members are not attacked.
  */
  ulong Truce;
  ubyte Guardians[8];
  long ugfld_24;
  long ugfld_28;
  long ugfld_32;
  long ugfld_36;
};

struct GroupAction {
  ulong FiredOn;
  ulong Hit;
  short Dead;
  short Alive;
  short Persuaded;
  ushort Storming;
};

#pragma pack()
/******************************************************************************/
extern struct WarFlag war_flags[PEOPLE_GROUPS_COUNT];
extern struct GroupAction group_actions[PEOPLE_GROUPS_COUNT+1];

/** Finds Group ID for which there are no things created.
 */
short find_unused_group_id(TbBool largest);

/** Count how many people of given kind are in given group.
 *
 * This can be called during level load, as it does not use per-type linked lists.
 */
ushort count_people_in_group(ushort group, short subtype);

/** Copy all properties of one group into another group.
 */
void thing_group_copy(short pv_group, short nx_group, ubyte allow_kill);

/** Transfer some people of given subtype from one group to the other.
 * Skips `stay_limit` of people, then transfers the next `tran_limit`.
 */
int thing_group_transfer_people(short pv_group, short nx_group, short subtype,
  int stay_limit, int tran_limit);

void thing_groups_clear_all_actions(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
