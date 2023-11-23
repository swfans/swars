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
