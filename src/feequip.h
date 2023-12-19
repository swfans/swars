/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file feequip.h
 *     Header file for feequip.c.
 * @par Purpose:
 *     Front-end desktop and menu system, equipment screen.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef FEEQUIP_H
#define FEEQUIP_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/
// TODO for this variable the first weapon is 0 instead of 1
// to be fixed to define variables in the same way everywhere
extern sbyte selected_weapon;

void show_equipment_screen(void);

void update_equip_cost_text(void);
void update_cybmod_cost_text(void);

void switch_shared_equip_screen_buttons_to_equip(void);
void switch_equip_offer_to_buy(void);
void init_weapon_anim(ubyte weapon);

void init_equip_screen_boxes(void);
void reset_equip_screen_boxes_flags(void);
void set_flag01_equip_screen_boxes(void);

void init_equip_screen_shapes(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
