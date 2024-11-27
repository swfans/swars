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
  GBxFlg_Unkn0004 = 0x0004,
  GBxFlg_Unkn0008 = 0x0008,
  GBxFlg_Unkn0010 = 0x0010,
  GBxFlg_Unkn0020 = 0x0020,
  GBxFlg_Unkn0040 = 0x0040,
  GBxFlg_Unkn0080 = 0x0080,
  GBxFlg_RadioBtn = 0x0100,
  GBxFlg_IsMouseOver = 0x0200,
  GBxFlg_IsPushed = 0x0400,
  GBxFlg_IsRPushed = 0x0800,
  GBxFlg_Unkn1000 = 0x1000,
  GBxFlg_TextRight = 0x2000,	/**< Align the text within the box to the right */
  GBxFlg_TextCenter = 0x4000,	/**< Center the text within the box */
  GBxFlg_BkgndDrawn = 0x8000,	/**< Whether static background of the box has been already drawn. */
};

struct ScreenPoint {
	short X;
	short Y;
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
    ubyte (*DrawTextFn)();
    ubyte (*CallBackFn)(ubyte click);
    ubyte *Radio;
    ushort TextTopLine;
    ushort TextFadePos;
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
    char field_16;
    char field_17;
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
	ubyte (*DrawFn)();
	ubyte (*DrawTextFn)();
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
  ushort GrabPos;
  ushort Lines;
  const char *Text;
  struct TbSprite *Font;
  ubyte (*DrawFn)(struct ScreenTextBox *box);
  ubyte (*DrawTextFn)(struct ScreenTextBox *box);
  struct ScreenButton *Buttons[2];
  short Infos[2];
  //struct ScreenInfoBox *Infos[2];
  ushort TextTopLine;
  ushort field_36;
  ushort field_38;
  short TextFadePos;
  ushort Flags;
  ushort field_3E;
  ubyte BGColour;
  ubyte LineHeight;
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
#define is_over_box(x, y, box) is_over_box_base(x, y, (struct ScreenBoxBase *)box)
TbBool is_over_box_base(short x, short y, struct ScreenBoxBase *box);

/** Returns if given position is over given slant (leaning right) box (incl. borders).
 */
#define is_over_slant_box(x, y, box) is_over_slant_box_base(x, y, (struct ScreenBoxBase *)box)
TbBool is_over_slant_box_base(short x, short y, struct ScreenBoxBase *box);

/** Returns if current mouse move position is over given box (incl. borders).
 */
#define mouse_move_over_box(box) mouse_move_over_box_base((struct ScreenBoxBase *)box)
TbBool mouse_move_over_box_base(struct ScreenBoxBase *box);

/** Returns if mouse position when button was pressed down is over given box (incl. borders).
 */
#define mouse_down_over_box(box) mouse_move_over_box_base((struct ScreenBoxBase *)box)
TbBool mouse_down_over_box_base(struct ScreenBoxBase *box);

/** Returns if current mouse move position is over given slant (leaning right) box (incl. borders).
 */
#define mouse_move_over_slant_box(box) mouse_move_over_slant_box_base((struct ScreenBoxBase *)box)
TbBool mouse_move_over_slant_box_base(struct ScreenBoxBase *box);

/** Returns X coord of current mouse move position relative to given box.
 */
#define mouse_move_position_horizonal_over_box(box) mouse_move_position_horizonal_over_box_base((struct ScreenBoxBase *)box)
short mouse_move_position_horizonal_over_box_base(struct ScreenBoxBase *box);

/** Returns Y coord of current mouse move position relative to given box.
 */
#define mouse_move_y_coord_over_box(box) mouse_move_y_coord_over_box_base((struct ScreenBoxBase *)box)
short mouse_move_y_coord_over_box_base(struct ScreenBoxBase *box);

void init_screen_box(struct ScreenBox *box, ushort x, ushort y,
  ushort width, ushort height, int drawspeed);
void init_screen_text_box(struct ScreenTextBox *box, ushort x, ushort y,
  ushort width, ushort height, int drawspeed,
  struct TbSprite *font, int textspeed);
void init_screen_button(struct ScreenButton *box, ushort x, ushort y,
  const char *text, int drawspeed,
  struct TbSprite *font, int textspeed, int flags);
void init_screen_info_box(struct ScreenInfoBox *box, ushort x, ushort y,
  ushort width, const char *text1, const char *text2, int drawspeed,
  struct TbSprite *font1, struct TbSprite *font2, int textspeed);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
