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
extern ubyte *pointer_data_end;

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

extern struct TbSprite *unk3_sprites;
extern struct TbSprite *unk3_sprites_end;
extern ubyte *unk3_sprites_data;

/** Panel sprites, but loaded for front-end menu purposes.
 * The main purpose is to get small weapon sprites.
 */
extern struct TbSprite *fepanel_sprites;
extern struct TbSprite *fepanel_sprites_end;
extern ubyte *fepanel_sprites_data;

/** In-game panel sprites.
 */
extern struct TbSprite *pop1_sprites;
extern struct TbSprite *pop1_sprites_end;
extern ubyte *pop1_data;
extern ubyte *pop1_data_end;
extern short pop1_sprites_scale;

extern struct TbSprite *m_sprites;
extern struct TbSprite *m_sprites_end;
extern ubyte *m_spr_data;
extern ubyte *m_spr_data_end;

extern ubyte *dword_1C6DE4;
extern ubyte *dword_1C6DE8;

/******************************************************************************/

/** Loads mouse pointer sprites for given style and detail level.
 * Requires the sprite buffers to be already allocated.
 */
TbResult load_sprites_mouse_pointers_up_to(
  const char *dir, short styleno, short max_detail);
/** Sets up previously loaded mouse sprites.
 */
void setup_mouse_pointers(void);
/** Frees up any resources and states associated to the mouse sprites.
 * Does not free the preallocated memory buffers.
 */
void reset_mouse_pointers(void);

TbResult load_sprites_icons(ubyte **pp_buf, const char *dir);
void setup_sprites_icons(void);
void reset_sprites_icons(void);

TbResult load_sprites_wicons(ubyte **pp_buf, const char *dir);
void setup_sprites_wicons(void);
void reset_sprites_wicons(void);

TbResult load_sprites_fepanel(ubyte **pp_buf, const char *dir);
void setup_sprites_fepanel(void);
void reset_sprites_fepanel(void);

TbResult load_sprites_fe_mouse_pointers(ubyte **pp_buf,
  const char *dir, ushort styleno, ushort detail);
void setup_sprites_fe_mouse_pointers(void);
void reset_sprites_fe_mouse_pointers(void);

void setup_sprites_small_font(void);
void reset_sprites_small_font(void);

TbResult load_sprites_small_med_font(ubyte **pp_buf, const char *dir);
void setup_sprites_small_med_font(void);
void reset_sprites_small_med_font(void);

TbResult load_sprites_med_font(ubyte **pp_buf, const char *dir);
void setup_sprites_med_font(void);
void reset_sprites_med_font(void);

TbResult load_sprites_med2_font(ubyte **pp_buf, const char *dir);
void setup_sprites_med2_font(void);
void reset_sprites_med2_font(void);

TbResult load_sprites_big_font(ubyte **pp_buf, const char *dir);
void setup_sprites_big_font(void);
void reset_sprites_big_font(void);

TbResult load_sprites_small2_font(ubyte **pp_buf, const char *dir);
void setup_sprites_small2_font(void);
void reset_sprites_small2_font(void);

/** Loads and sets up multicolor sprites for currently set TrenchcoatPreference.
 */
TbResult load_multicolor_sprites(const char *dir);

/** Sets up initially loaded multicolor sprites.
 * Use load_multicolor_sprites() for further reloads.
 */
void setup_multicolor_sprites(void);
void reset_multicolor_sprites(void);
void debug_multicolor_sprite(int idx);

/** Loads panel sprites for given style and detail level.
 * Requires the sprite buffers to be already allocated.
 */
TbResult load_pop_sprites_up_to(const char *dir, const char *name,
  short styleno, short max_detail);
/** Sets up previously loaded panel sprites.
 */
void setup_pop_sprites(void);
/** Frees up any resources and states associated to the panel sprites.
 * Does not free the preallocated memory buffers.
 */
void reset_pop_sprites(void);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
