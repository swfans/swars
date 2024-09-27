/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file purpldrw.c
 *     Adding items to drawlist for purple projector screens.
 * @par Purpose:
 *     Implements functions for filling items within drawlist.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2024 - 28 Sep 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "purpldrw.h"

#include "purpldrwlst.h"
#include "guiboxes.h"
#include "swlog.h"
/******************************************************************************/

void draw_line_purple_list(int x1, int y1, int x2, int y2, int colour)
{
    asm volatile (
      "push %4\n"
      "call ASM_draw_line_purple_list\n"
        : : "a" (x1), "d" (y1), "b" (x2), "c" (y2), "g" (colour));
}

void draw_box_purple_list(int x, int y, ulong width, ulong height, int colour)
{
    asm volatile (
      "push %4\n"
      "call ASM_draw_box_purple_list\n"
        : : "a" (x), "d" (y), "b" (width), "c" (height), "g" (colour));
}

void draw_text_purple_list2(int x, int y, const char *text, ushort line)
{
    asm volatile (
      "call ASM_draw_text_purple_list2\n"
        : : "a" (x), "d" (y), "b" (text), "c" (line));
}

void draw_sprite_purple_list(int x, int y, struct TbSprite *sprite)
{
    asm volatile (
      "call ASM_draw_sprite_purple_list\n"
        : : "a" (x), "d" (y), "b" (sprite));
}

void draw_trig_purple_list(long x2, long y2, long x3, long y3)
{
    asm volatile (
      "call ASM_draw_trig_purple_list\n"
        : : "a" (x2), "d" (y2), "b" (x3), "c" (y3));
}

void copy_box_purple_list(long x, long y, ulong width, ulong height)
{
    asm volatile (
      "call ASM_copy_box_purple_list\n"
        : : "a" (x), "d" (y), "b" (width), "c" (height));
}

void draw_hotspot_purple_list(int x, int y)
{
    asm volatile (
      "call ASM_draw_hotspot_purple_list\n"
        : : "a" (x), "d" (y));
}

ubyte flashy_draw_purple_shape(struct ScreenShape *shape)
{
    ubyte ret;
    asm volatile ("call ASM_flashy_draw_purple_shape\n"
        : "=r" (ret) : "a" (shape));
    return ret;
}

ubyte flashy_draw_purple_button(struct ScreenButton *button)
{
    ubyte ret;
    asm volatile ("call ASM_flashy_draw_purple_button\n"
        : "=r" (ret) : "a" (button));
    return ret;
}

void draw_triangle_purple_list(int x1, int y1, int x2, int y2, int x3, int y3, TbPixel colour)
{
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_draw_triangle_purple_list\n"
        : : "a" (x1), "d" (y1), "b" (x2), "c" (y2), "g" (x3), "g" (y3), "g" (colour));
}

void draw_flic_purple_list(void (*fn)())
{
    asm volatile ("call ASM_draw_flic_purple_list\n"
        : : "a" (fn));
}

void draw_noise_box_purple_list(int x, int y, ulong width, ulong height)
{
    draw_box_purple_list(x, y, width, height, 0);
    purple_draw_list[purple_draw_index - 1].Type = PuDT_NOISEBOX;
}

/******************************************************************************/
