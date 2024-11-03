/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file game_sprts.h
 *     Header file for game_sprts.c.
 * @par Purpose:
 *     Lists of sprites used by the game.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     10 Feb 2024 - 02 May 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef GAME_SPRTS_H
#define GAME_SPRTS_H

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

#include "bftypes.h"
#include "game_bstype.h"

/******************************************************************************/

extern struct TbSprite *pointer_sprites;
extern struct TbSprite *pointer_sprites_end;
extern ubyte *pointer_data;

extern struct TbSprite *small_font;
extern struct TbSprite *small_font_end;
extern ubyte *small_font_data;

extern struct TbSprite *small2_font;
extern struct TbSprite *small2_font_end;
extern ubyte *small2_font_data;

extern struct TbSprite *small_med_font;
extern struct TbSprite *small_med_font_end;
extern ubyte *small_med_font_data;

extern struct TbSprite *med_font;
extern struct TbSprite *med_font_end;
extern ubyte *med_font_data;

extern struct TbSprite *med2_font;
extern struct TbSprite *med2_font_end;
extern ubyte *med2_font_data;

extern struct TbSprite *big_font;
extern struct TbSprite *big_font_end;
extern ubyte *big_font_data;

extern struct TbSprite *sprites_Icons0_0;
extern struct TbSprite *sprites_Icons0_0_end;
extern ubyte *sprites_Icons0_0_data;

extern struct TbSprite *unk1_sprites;
extern struct TbSprite *unk1_sprites_end;
extern ubyte *unk1_sprites_data;

extern struct TbSprite *unk3_sprites_end;
extern ubyte *unk3_sprites_data;

extern struct TbSprite *unk2_sprites;
extern struct TbSprite *unk2_sprites_end;
extern ubyte *unk2_sprites_data;

extern struct TbSprite *pop1_sprites;
extern struct TbSprite *pop1_sprites_end;
extern ubyte *pop1_data;

extern struct TbSprite *m_sprites;
extern struct TbSprite *m_sprites_end;
extern ubyte *m_spr_data;
extern ubyte *m_spr_data_end;

/******************************************************************************/

void setup_mouse_pointers(void);
void reset_mouse_pointers(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
