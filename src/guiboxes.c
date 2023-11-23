/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file guiboxes.c
 *     GUI boxes for menu system.
 * @par Purpose:
 *     Implement functions for handling various boxes for game GUI.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "guiboxes.h"

#include "swlog.h"
/******************************************************************************/

void init_screen_box(struct ScreenBox *box, ushort x, ushort y, ushort width, ushort height, int drawspeed)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_init_screen_box\n"
        : : "a" (box), "d" (x), "b" (y), "c" (width), "g" (height), "g" (drawspeed));
}

void init_screen_text_box(struct ScreenTextBox *box, ushort x, ushort y, ushort width, ushort height, int drawspeed, struct TbSprite *font, int textspeed)
{
    asm volatile (
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_init_screen_text_box\n"
        : : "a" (box), "d" (x), "b" (y), "c" (width), "g" (height), "g" (drawspeed), "g" (font), "g" (textspeed));
}

void init_screen_button(struct ScreenButton *box, ushort x, ushort y, const char *text, int drawspeed, struct TbSprite *font, int textspeed, int flags)
{
    asm volatile (
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_init_screen_button\n"
        : : "a" (box), "d" (x), "b" (y), "c" (text), "g" (drawspeed), "g" (font), "g" (textspeed), "g" (flags));
}

void init_screen_info_box(struct ScreenInfoBox *box, ushort x, ushort y, ushort width, const char *text1, const char *text2, int drawspeed, struct TbSprite *font1, struct TbSprite *font2, int textspeed)
{
    asm volatile (
      "push %9\n"
      "push %8\n"
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_init_screen_info_box\n"
        : : "a" (box), "d" (x), "b" (y), "c" (width), "g" (text1), "g" (text2), "g" (drawspeed), "g" (font1), "g" (font2), "g" (textspeed));
}

/******************************************************************************/
