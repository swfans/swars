/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file guiboxes.h
 *     Header file for guiboxes.c.
 * @par Purpose:
 *     GUI boxes for menu system.
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
#ifndef GUIBOXES_H
#define GUIBOXES_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

enum GUIBoxState {
  /** The box is disabled and does react to input. Should draw
   *  as mostly transparent, greyed out or darkened.
   */
  GBxSta_DISABLED = 0,
  /** The box is in normal, non-highlited state.
   */
  GBxSta_NORMAL,
  /** Highlight level 1 - toggled on, but without mouse over.
   */
  GBxSta_HLIGHT1,
  /** Highlight level 2 - mouse is over the box.
   */
  GBxSta_HLIGHT2,
  /** The box is pushed - either by holding a key or mouse button.
   */
  GBxSta_PUSHED,
};

enum GUIBoxFlags {
  GBxFlg_NONE = 0x0000,
  GBxFlg_Unkn0001 = 0x0001,
  GBxFlg_Unkn0002 = 0x0002,
  GBxFlg_BkCopied = 0x0004,		/**< Background with static part of the component was stored for reuse */
  GBxFlg_NoBkCopy = 0x0008,		/**< Do not store background static part */
  GBxFlg_Unkn0010 = 0x0010,
  GBxFlg_Unkn0020 = 0x0020,
  GBxFlg_Unkn0040 = 0x0040,
  GBxFlg_Unkn0080 = 0x0080,
  GBxFlg_RadioBtn = 0x0100,		/**< Seem to be used for more than marking one of exclusive radio buttons */
  GBxFlg_IsMouseOver = 0x0200,
  GBxFlg_IsPushed = 0x0400,
  GBxFlg_IsRPushed = 0x0800,
  GBxFlg_TextCopied = 0x1000,	/**< Text on the box became static and was stored for reuse */
  GBxFlg_TextRight = 0x2000,	/**< Align the text within the box to the right */
  GBxFlg_TextCenter = 0x4000,	/**< Center the text within the box */
  GBxFlg_BkgndDrawn = 0x8000,	/**< Whether static background of the box has been already drawn. */
};

/** Point on the screen surface. */
struct ScreenPoint {
	short X;
	short Y;
};

/** Rectangle on the screen surface. */
struct ScreenRect {
	short X;
	short Y;
    ushort Width;
    ushort Height;
};

/** Base to which every Screen Box/Button/Text can be casted.
 */
struct ScreenBoxBase {
    short X;
    short Y;
    ushort Width;
    ushort Height;
};

struct ScreenButton;

struct ScreenButton {
    short X;
    short Y;
    ushort Width;
    ushort Height;
    const char *Text;
    struct TbSprite *Font;
    ubyte (*DrawFn)(struct ScreenButton *btn);
    ubyte (*DrawTextFn)(struct ScreenButton *btn);
    ubyte (*CallBackFn)(ubyte click);
    ubyte *Radio;
    ushort TextTopLine;
    short TextFadePos;
    ushort Flags;
    ubyte DrawSpeed;
    ubyte Timer;
    ubyte TextSpeed;
    ubyte Border;
    ubyte Colour;
    ubyte BGColour;
    ubyte AccelKey;
    ubyte RadioValue;
};

struct ScreenBox;

struct ScreenBox
{
    short X;
    short Y;
    ushort Width;
    ushort Height;
    ubyte DrawSpeed;
    ubyte Timer;
    ushort Flags;
    ubyte (*DrawFn)(struct ScreenBox *box);
    ubyte (*SpecialDrawFn)(struct ScreenBox *box);
    ushort Timer2;
    ubyte Colour;
    ubyte BGColour;
};

struct ScreenInfoBox;

struct ScreenInfoBox { // sizeof=43
    short X;
    short Y;
    ushort Width;
    ushort Height;
    char *Text1;
    char *Text2;
	struct TbSprite *Font1;
	struct TbSprite *Font2;
	ubyte (*DrawFn)(struct ScreenInfoBox *box);
	ubyte (*DrawTextFn)(struct ScreenInfoBox *box);
    short TextFadePos1;
    short TextFadePos2;
    ushort Flags;
    ubyte DrawSpeed;
    ubyte Timer;
    ubyte TextSpeed;
    ubyte Colour;
    ubyte BGColour;
};

struct ScreenTextBox;

struct ScreenTextBox {
  short X;
  short Y;
  ushort Width;
  ushort Height;
  ubyte DrawSpeed;
  ubyte Timer;
  ubyte TextSpeed;
  ubyte LineSpacing;
  short ScrollBarPos;
  ushort ScrollBarSize;
  short ScrollWindowHeight;
  short ScrollWindowOffset;
  short GrabPos;
  ushort Lines;
  const char *Text;
  struct TbSprite *Font;
  ubyte (*DrawFn)(struct ScreenTextBox *box);
  ubyte (*DrawTextFn)(struct ScreenTextBox *box);
  struct ScreenButton *Buttons[2];
  struct ScreenInfoBox *Infos[2];
  ushort field_38;
  short TextFadePos;
  ushort Flags;
  ubyte Colour1;
  ubyte BGColour;
  ubyte LineHeight;
  ubyte field_41;
  ubyte field_42;
  ubyte field_43;
};

struct ScreenShape { // sizeof=42
    /** Positions of points making the shape, X coord. */
    ushort PtX[9];
    /** Positions of points making the shape, Y coord. */
    ushort PtY[9];
    ubyte field_24;
    ubyte field_25;
    ushort Flags;
    ubyte Colour;
    ubyte BGColour;
};

#pragma pack()
/******************************************************************************/

/** Returns if given position is inside given box with size (excl. borders).
 */
TbBool in_box(short x, short y, short box_x, short box_y, short box_w, short box_h);

/** Returns if given position is inside given box with coords (excl. borders).
 */
TbBool in_box_coords(short x, short y, short box_x1, short box_y1, short box_x2, short box_y2);


/** Returns if given position is over given box with size (incl. borders).
 */
TbBool over_box(short x, short y, short box_x, short box_y, short box_w, short box_h);

/** Returns if given position is over given box with coords (incl. borders).
 */
TbBool over_box_coords(short x, short y, short box_x1, short box_y1, short box_x2, short box_y2);

/** Returns if given two boxes have a common part (incl. borders).
 */
TbBool boxes_intersect(short box1_x, short box1_y, short box1_w, short box1_h,
  short box2_x, short box2_y, short box2_w, short box2_h);

/** Returns if given two boxes have a common part (incl. borders).
 */
#define screen_boxes_intersect(box1, box2) base_boxes_intersect((struct ScreenBoxBase *)box1, (struct ScreenBoxBase *)box2)
TbBool base_boxes_intersect(struct ScreenBoxBase *p_box1, struct ScreenBoxBase *p_box2);

/** Returns if current mouse move position is over given box with coords (incl. borders).
 */
TbBool mouse_move_over_box_coords(short box_x1, short box_y1, short box_x2, short box_y2);

/** Returns if mouse position when button was pressed down is over given box with coords (incl. borders).
 */
TbBool mouse_down_over_box_coords(short box_x1, short box_y1, short box_x2, short box_y2);

// TODO switch order of arguments and to above function, remove when this is no longer used
TbBool mouse_move_over_rect(short box_x1, short box_x2, short box_y1, short box_y2);

TbBool mouse_move_over_rect_adv(short x1, short y1, short width, short height, TbBool a5);

short mouse_move_position_horizonal_over_bar_coords(short x, short w);
short mouse_down_position_horizonal_over_bar_coords(short x, short w);

/** Returns if given position is over given box (incl. borders).
 */
#define is_over_box(x, y, p_box) is_over_box_base(x, y, (struct ScreenBoxBase *)p_box)
TbBool is_over_box_base(short x, short y, struct ScreenBoxBase *p_box);

/** Returns if given position is over given slant (leaning right) box (incl. borders).
 */
#define is_over_slant_box(x, y, p_box) is_over_slant_box_base(x, y, (struct ScreenBoxBase *)p_box)
TbBool is_over_slant_box_base(short x, short y, struct ScreenBoxBase *p_box);

/** Returns if current mouse move position is over given box (incl. borders).
 */
#define mouse_move_over_box(p_box) mouse_move_over_box_base((struct ScreenBoxBase *)p_box)
TbBool mouse_move_over_box_base(struct ScreenBoxBase *p_box);

/** Returns if mouse position when button was pressed down is over given box (incl. borders).
 */
#define mouse_down_over_box(p_box) mouse_down_over_box_base((struct ScreenBoxBase *)p_box)
TbBool mouse_down_over_box_base(struct ScreenBoxBase *p_box);

/** Returns if current mouse move position is over given slant (leaning right) box (incl. borders).
 */
#define mouse_move_over_slant_box(p_box) mouse_move_over_slant_box_base((struct ScreenBoxBase *)p_box)
TbBool mouse_move_over_slant_box_base(struct ScreenBoxBase *p_box);

/** Returns X coord of current mouse move position relative to given box.
 */
#define mouse_move_position_horizonal_over_box(p_box) mouse_move_position_horizonal_over_box_base((struct ScreenBoxBase *)p_box)
short mouse_move_position_horizonal_over_box_base(struct ScreenBoxBase *p_box);

/** Returns Y coord of current mouse move position relative to given box.
 */
#define mouse_move_y_coord_over_box(p_box) mouse_move_y_coord_over_box_base((struct ScreenBoxBase *)p_box)
short mouse_move_y_coord_over_box_base(struct ScreenBoxBase *p_box);

short mouse_move_position_vertical_scrollbar_over_text_box(struct ScreenTextBox *p_box);

/** Draws slant box / kicked box / parallellogram skewed 45 deg.
 * This function is like a extension to bflibrary API, but it's defined on application side.
 */
TbResult ApDrawSlantBox(short x, short y, ushort w, ushort h, TbPixel col);

void init_screen_box(struct ScreenBox *p_box, ushort x, ushort y,
  ushort width, ushort height, int drawspeed);
void init_screen_text_box(struct ScreenTextBox *p_box, ushort x, ushort y,
  ushort width, ushort height, int drawspeed,
  struct TbSprite *p_font, ushort textspeed);
void init_screen_button(struct ScreenButton *p_box, ushort x, ushort y,
  const char *text, int drawspeed,
  struct TbSprite *font, int textspeed, int flags);
void init_screen_info_box(struct ScreenInfoBox *p_box, ushort x, ushort y,
  ushort width, char *text1, char *text2, int drawspeed,
  struct TbSprite *p_font1, struct TbSprite *p_font2, int textspeed);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
