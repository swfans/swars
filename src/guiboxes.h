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

#pragma pack()
/******************************************************************************/

/** Returns if given position is inside given box with size (excl. borders).
 */
TbBool in_box(short x, short y, short box_x, short box_y, short box_w, short box_h);

/** Returns if given position is inside given box with coords (excl. borders).
 */
TbBool in_box_coords(short x, short y, short box_x1, short box_y1, short box_x2, short box_y2);

/** Returns if given position is over given box with coords (incl. borders).
 */
TbBool over_box_coords(short x, short y, short box_x1, short box_y1, short box_x2, short box_y2);

/** Returns if current mouse move position is over given box with coords (incl. borders).
 */
TbBool mouse_move_over_box_coords(short box_x1, short box_y1, short box_x2, short box_y2);

// TODO switch order of arguments and to above function, remove when this is no longer used
TbBool mouse_move_over_rect(short box_x1, short box_x2, short box_y1, short box_y2);

TbBool mouse_move_over_rect_adv(short x1, short y1, short width, short height, TbBool a5);

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

/** Returns if current mouse move position is over given slant (leaning right) box (incl. borders).
 */
#define mouse_move_over_slant_box(box) mouse_move_over_slant_box_base((struct ScreenBoxBase *)box)
TbBool mouse_move_over_slant_box_base(struct ScreenBoxBase *box);

/** Returns X coord of current mouse move position relative to given box.
 */
#define mouse_move_x_coord_over_box(box) mouse_move_x_coord_over_box_base((struct ScreenBoxBase *)box)
short mouse_move_x_coord_over_box_base(struct ScreenBoxBase *box);

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
