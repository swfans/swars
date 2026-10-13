/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file huddrwlstx.h
 *     Header file for huddrwlstm.c.
 * @par Purpose:
 *     Drawlist execution for the HUD over 3D engine.
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
#ifndef HUDDRWLSTX_H
#define HUDDRWLSTX_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct TbSprite;

struct DIScrPoint {
    short X;
    short Y;
};

struct DIScrRect {
    short X;
    short Y;
    short Width;
    short Height;
};

struct DIMapPoint {
    short X;
    short Y;
    short Z;
};

struct DIHudLine {
    struct DIScrPoint Beg;
    struct DIScrPoint End;
    short Timer;
    ushort DrwFlags;
    ubyte Thick;
    ubyte Bright;
    ubyte Col;
};

struct DIHudBox {
    struct DIScrRect Rect;
    short Timer;
    ushort DrwFlags;
    ubyte Bright;
    ubyte Col;
};

struct DIHudTexturedBox {
    struct DIScrRect Rect;
    ubyte VecMode;
    ubyte Bright;
    ubyte TMapNo;
    ushort NumUa;
    ushort NumVb;
};

struct DIHudFrame {
    struct DIScrPoint Pos;
    ushort Frame;
    ushort DrwFlags;
    short Scale;
    ubyte Bright;
    ubyte Col;
};

struct DIHudSprite {
    struct DIScrRect Rect;
    struct TbSprite *pSpr;
    short Timer;
    ushort DrwFlags;
    ubyte Bright;
    ubyte Col;
};

struct DIHudClippedText {
    struct DIScrRect Rect;
    struct TbSprite *pFont;
    const char *Text;
    struct DIScrPoint Shift;
    ubyte Scale;
    ubyte Bright;
    ubyte Col;
    ubyte Shade;
};

struct DIHudWrappedText {
    struct DIScrRect Rect;
    struct TbSprite *pFont;
    const char *Text;
    short Timer;
    ushort DrwFlags;
    ubyte Scale;
    ubyte Bright;
    ubyte Col;
    ubyte Shade;
};

struct DIHudMapCoordLine {
    struct DIMapPoint PtBeg;
    struct DIMapPoint PtEnd;
    ushort DrwFlags;
    ubyte Thick;
    ubyte Col;
};

struct DIHudMapCoordAnnotLine {
    struct DIMapPoint PtBeg;
    struct DIMapPoint PtEnd;
    intptr_t AnnotVal;
    ushort DrwFlags;
    ubyte Thick;
    ubyte Col1;
    ubyte Col2;
};

struct DIHudMap2ScrCoordLine {
    struct DIScrPoint Beg;
    struct DIMapPoint PtEnd;
    ushort DrwFlags;
    ubyte Thick;
    ubyte Col;
};

struct DIHudMapCoordRect {
    struct DIMapPoint Pt;
    short Width;
    short Height;
    short LnLen;
    ushort DrwFlags;
    ubyte Thick;
    ubyte Bright;
    ubyte Col;
};

struct DIHudMapCoordCircle {
    struct DIMapPoint Pt;
    short Radius;
    ushort DrwFlags;
    ubyte Thick;
    ubyte Bright;
    ubyte Col;
};

struct DIHudMapCoordFrame {
    struct DIMapPoint Pt;
    ushort Frame;
    ushort DrwFlags;
    ubyte Scale;
    ubyte Bright;
    ubyte Col;
};

struct DIHudMapCoordSprList {
    struct DIMapPoint Pt;
    struct TbSprite * const *pSprLst;
    ushort DrwFlags;
    ushort Radius;
    ubyte Scale;
    ubyte Bright;
    ubyte Col;
};

struct DrawItemHud {
	union {
        struct DIHudLine Line;
        struct DIHudBox Box;
        struct DIHudTexturedBox TxtrdBox;
        struct DIHudFrame Frame;
        struct DIHudSprite Sprite;
		struct DIHudClippedText ClpText;
		struct DIHudWrappedText WrpText;
        struct DIHudMapCoordLine MapCorLine;
        struct DIHudMap2ScrCoordLine M2SCorLine;
        struct DIHudMapCoordAnnotLine MapCorAntLine;
        struct DIHudMapCoordRect MapCorRect;
        struct DIHudMapCoordCircle MapCorCircl;
        struct DIHudMapCoordFrame MapCorFrame;
        struct DIHudMapCoordSprList MapCorSprLst;
	} U;
	ubyte Type;
	ushort Flags;
};

#pragma pack()
/******************************************************************************/

void hud_draw_line(struct DIHudLine *p_diLine);

void hud_draw_box(struct DIHudBox *p_diBox);
void hud_draw_slant_box(struct DIHudBox *p_diBox);
void hud_draw_vslant_box(struct DIHudBox *p_diBox);

void hud_draw_low_trans_grey_box(struct DIHudBox *p_diBox);
void hud_draw_low_trans_grey_slant_box(struct DIHudBox *p_diBox);
void hud_draw_low_trans_grey_vslant_box(struct DIHudBox *p_diBox);
void hud_draw_textured_flow_slant_box(struct DIHudTexturedBox *p_diBox);

void hud_draw_frame_alpha(struct DIHudFrame *p_diFrame);
void hud_draw_sprite(struct DIHudSprite *p_diSprite);
void hud_draw_sprite_scaled(struct DIHudSprite *p_diSprite);

void hud_draw_clipped_text(struct DIHudClippedText *p_diClpText);
void hud_draw_colour_wave_wrapped_text(struct DIHudWrappedText *p_diWrpText);
void hud_draw_shad_cl_flash_wrapped_text(struct DIHudWrappedText *p_diWrpText);

int hud_width_shad_cl_flash_wrapped_text(short px, short py,
  struct TbSprite *p_font, const char *text, ushort drwflags, short units_per_px);

void hud_draw_mapcoord_line(struct DIHudMapCoordLine *p_diMapCorLine);
void hud_draw_mapcoord_line_annotated_num(struct DIHudMapCoordAnnotLine *p_diMapCorAntLine);
void hud_draw_mapcoord_to_scrcoord_line(struct DIHudMap2ScrCoordLine *p_diM2SCorLine);
void hud_draw_mapcoord_corner_rect(struct DIHudMapCoordRect *p_diMapCorRect);
void hud_draw_mapcoord_flat_circle(struct DIHudMapCoordCircle *p_diMapCorCircl);
void hud_draw_mapcoord_frame(struct DIHudMapCoordFrame *p_diMapCorFrame);
void hud_draw_mapcoord_sprites_in_quarters(struct DIHudMapCoordSprList *p_diMapCorSprLst);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
