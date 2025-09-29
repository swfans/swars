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

enum ModDrawStateFlags {
    ModDSt_ModAnimIn	= 0x01,	/**< cybernetic mod insertion animation plays */
    ModDSt_ModAnimOut	= 0x02,	/**< cybernetic mod removal animation plays */
    ModDSt_Unkn04	= 0x04,
    ModDSt_Unkn08	= 0x08,
    ModDSt_Unkn10	= 0x10,
    ModDSt_Unkn20	= 0x20,
    ModDSt_Unkn40	= 0x40,
    ModDSt_Unkn80	= 0x80,
};

enum ModDrawPart {
    ModDPt_CHEST	= 0,
    ModDPt_BRAIN	= 1,
    ModDPt_ARMS		= 2,
    ModDPt_LEGS		= 3,
    ModDPt_BREATH	= 4,
    ModDPt_BKGND	= 5,
};

enum ModDrawStage {
    ModDSt_BRT	= 0, /**< breathing animation */
    ModDSt_OUT	= 1, /**< drawing out a mod */
    ModDSt_IN	= 2, /**< drawing in a mod */
    ModDSt_END	= 3, /**< not animating */
};

#pragma pack()
/******************************************************************************/
extern sbyte selected_mod;

ubyte show_cryo_chamber_screen(void);
void update_flic_mods(ubyte *mods);
void cryo_update_for_selected_cybmod(void);

void init_cryo_screen_boxes(void);

/** Reset players part of cryo screen state.
 *
 * Resets all parts of cryo screen which are set by the player
 * or related to players game progress.
 */
void reset_cryo_screen_player_state(void);

void reset_cryo_screen_boxes_flags(void);
void set_flag01_cryo_screen_boxes(void);

/** Returns size of the cyborg parts buffer.
 *
 * The buffer must be large enough to hold either cyborg background,
 * or all cyborg parts.
 */
uint cryo_cyborg_part_buf_max_size(void);

void set_mod_draw_states_flag08(void);
void reset_mod_draw_states_flag08(void);

void switch_shared_equip_screen_buttons_to_cybmod(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
