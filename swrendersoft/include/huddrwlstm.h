/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file huddrwlstm.h
 *     Header file for huddrwlstm.c.
 * @par Purpose:
 *     Making drawlists for the HUD over 3D engine.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 12 May 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef HUDDRWLSTM_H
#define HUDDRWLSTM_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

 struct TbSprite;

#pragma pack()
/******************************************************************************/

/**  Enlist drawing a line between two points on screen.
 *
 * @param beg_x First point screen coordinate.
 * @param beg_y First point screen coordinate.
 * @param end_x Second point screen coordinate.
 * @param end_y Second point screen coordinate.
 * @param drwflags DrawFlags to be set before drawing.
 * @param thickness Thickness of the line drawn, in pixels.
 * @param bright Brightness of the drawn colour.
 * @param colour Base colour of the drawn pixels.
 */
TbBool enlist_hud_draw_line(short beg_x, short beg_y, short end_x, short end_y,
  ushort drwflags, ubyte thickness, short brig, TbPixel colour);

TbBool enlist_hud_draw_mapcoord_line_annotated_num(short cor1_x,
  short cor1_y_m8, short cor1_z, short cor2_x, short cor2_y_m8,
  short cor2_z, int val, ushort drwflags, ubyte thickness,
  TbPixel lncol, TbPixel txcol);

TbBool enlist_hud_draw_box(short px, short py, short width, short height,
  ushort drwflags, short brig, TbPixel colour);

TbBool enlist_hud_draw_slant_box(short px, short py, short width, short height,
  ushort drwflags, short brig, TbPixel colour);

TbBool enlist_hud_draw_vslant_box(short px, short py, short width, short height,
  ushort drwflags, short brig, TbPixel colour);

TbBool enlist_hud_draw_low_trans_grey_box(short px, short py,
  short width, short height, ushort drwflags, short brig, TbPixel colour);

TbBool enlist_hud_draw_low_trans_grey_slant_box(short px, short py,
  short width, short height, ushort drwflags, short brig, TbPixel colour);

TbBool enlist_hud_draw_low_trans_grey_vslant_box(short px, short py,
  short width, short height, ushort drwflags, short brig, TbPixel colour);

TbBool enlist_hud_draw_textured_flow_slant_box(short px, short py,
  short width, short height, ubyte vecmode,
  ubyte brig, ubyte tmapno, ushort num_ua, ushort num_vb);

/** Enlist drawing given frame, standing at given position and with brightness.
 *
 * The function accepts precise scale parameter, with 8 bits used
 * for fractional part, unlike most other functions.
 *
 * @param px On-screen Coordinate of the standing position.
 * @param py On-screen Coordinate of the standing position.
 * @param frm Index of the frame to be drawn.
 * @param drwflags DrawFlags to be set before drawing.
 * @param scale Scale at which the frame is drawn, 100% is 256.
 * @param bright Brightness of the drawn sprite.
 */
TbBool enlist_hud_draw_frame_alpha(short px, short py, ushort frm,
  ushort drwflags, short scale, short brig);

TbBool enlist_hud_draw_sprite(short px, short py, struct TbSprite *p_spr,
  ushort drwflags, short brig);

TbBool enlist_hud_draw_sprite_scaled(short px, short py,
  struct TbSprite *p_spr, short dest_width, short dest_height,
  ushort drwflags, short brig);

TbBool enlist_hud_draw_clipped_text(short px, short py,
  short width, short height, short shift_x, short shift_y,
  struct TbSprite *p_font, const char *text,
  short units_per_px, short brig, TbPixel colour);

/** Check height of line-wrapped text with shadow colour flash effect.
 */
int get_width_shad_cl_flash_wrapped_text(short px, short py,
  struct TbSprite *p_font, const char *text, short units_per_px);

/** Enlist drawing line-wrapped text with shadow colour flash effect.
 */
TbBool enlist_hud_draw_shad_cl_flash_wrapped_text(short px, short py,
  short width, short height, struct TbSprite *p_font, const char *text,
  short units_per_px, short timer, TbPixel colour, TbPixel shcolour);

/** Enlist drawing line-wrapped text with colour brightness wave effect.
 */
TbBool enlist_hud_draw_colour_wave_wrapped_text(short px, short py,
  short width, short height, struct TbSprite *p_font, const char *text,
  short units_per_px, TbPixel colour, TbPixel shcolour);

/** Enlist drawing line between the provided map coordinates.
 */
TbBool enlist_hud_draw_mapcoord_line(
  short cor1_x, short cor1_y_m8, short cor1_z,
  short cor2_x, short cor2_y_m8, short cor2_z,
  ushort drwflags, ubyte thickness, TbPixel colour);

/** Enlist drawing line between the map coords, only if visible in whole.
 */
TbBool enlist_hud_draw_mapcoord_line_only_whole(
  short cor1_x, short cor1_y_m8, short cor1_z,
  short cor2_x, short cor2_y_m8, short cor2_z,
  ushort drwflags, ubyte thickness, TbPixel colour);

/** Enlist drawing a flat box facing the camera, centered on given map coords.
 *
 * Accepts the radius in map coord units, but draws the box facing the camera.
 */
TbBool enlist_hud_draw_mapcoord_corner_rect(short cor_x, short cor_y_m8,
  short cor_z, short width, short height, short lnlen, ubyte thickness,
  ushort drwflags, TbPixel colour);

/** Enlist drawing a flat circle on XZ plane, centered on given map coords.
 *
 * Accepts the radius in map coord units, and draws the circle in the
 * perspective of the map.
 */
TbBool enlist_hud_draw_mapcoord_flat_circle(short cor_x,
  short cor_y_m8, short cor_z, short radius,
  ushort drwflags, ubyte thickness, TbPixel colour);

/** Enlist drawing a flat box on XZ plane, centered on given map coords.
 *
 * Accepts the radius in map coord units, and draws the box in the
 * perspective of the map.
 */
TbBool enlist_hud_draw_mapcoord_flat_box(short cor_x,
  short cor_y_m8, short cor_z, short width, short height,
  ushort drwflags, ubyte thickness, TbPixel colour);

/** Enlist drawing line between map coordinates and screen coordinates.
 */
TbBool enlist_hud_draw_mapcoord_to_scrcoord_line(short scr1_x,
  short scr1_y, short cor2_x, short cor2_y_m8, short cor2_z,
  ushort drwflags, ubyte thickness, TbPixel colour);

/** Enlist drawing a frame on the provided map coordinates.
 */
TbBool enlist_hud_draw_mapcoord_frame_one_colour(short cor_x, short cor_y,
  short cor_z, ushort frm, ushort drwflags, TbPixel colour);

TbBool enlist_hud_draw_mapcoord_sprites_in_quarters_unscaled(short cor_x,
  short cor_y_m8, short cor_z, struct TbSprite * const p_sprlst[],
  ushort drwflags, short radius);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
