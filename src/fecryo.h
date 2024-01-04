/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file fecryo.h
 *     Header file for fecryo.c.
 * @par Purpose:
 *     Front-end desktop and menu system, cryogenic chamber screen.
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
#ifndef FECRYO_H
#define FECRYO_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/
extern sbyte selected_mod;

void show_cryo_chamber_screen(void);
void update_flic_mods(ubyte *mods);

void init_cryo_screen_boxes(void);
void reset_cryo_screen_boxes_flags(void);
void set_flag01_cryo_screen_boxes(void);

void switch_shared_equip_screen_buttons_to_cybmod(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
