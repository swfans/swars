/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file bat.c
 *     Routines implementing Breakout minigame.
 * @par Purpose:
 *     Implement functions for handling breakout-like minigame.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 22 Sep 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bat.h"

#include <string.h>
#include "bfgentab.h"
#include "bfkeybd.h"
#include "bfmemut.h"
#include "bfscreen.h"
#include "bfutility.h"

#include "display.h"
#include "game.h"
#include "player.h"
#include "sound.h"
#include "swlog.h"
#include "thing.h"
/******************************************************************************/

#pragma pack(1)

struct BreakoutLevel {
    char *level;
    ubyte field_4[120];
};

#pragma pack()

extern int BAT_data_1e26e8;
extern int BAT_data_1e26ec;
extern int BAT_data_1e26f0;
extern int BAT_data_1e26f4;
extern int BAT_data_1e26f8;
extern int BAT_state;
extern int BAT_data_1e2700;
extern int BAT_data_1e2704;
extern int BAT_data_1e2708;
extern int BAT_data_1e270c;
extern ubyte *BAT_screen;
extern ubyte BAT_data_1e271c[120];
extern int BAT_paddle_x;
extern int BAT_data_1e2798;
extern ubyte BAT_byte_1e279c;
extern char BAT_btarr_1e27a0[32];
extern void *BAT_dwarr_1e27c0[312];
extern void *BAT_ptr_1e2ca0;
extern int BAT_data_1e2ca4;
extern int BAT_data_1e2ca8;
extern void *BAT_dwarr_1e2ccc[92];
extern int BAT_data_1e2e40;

extern struct BreakoutLevel BAT_levels[];

void BAT_draw_char(int x, int y, char chr, ubyte col)
{
    asm volatile (
      "call ASM_BAT_draw_char\n"
        : : "a" (x), "d" (y), "b" (chr), "c" (col));
}

int BAT_unknsub_27(void)
{
    int ret;
    asm volatile (
      "call ASM_BAT_unknsub_27\n"
        : "=r" (ret) : );
    return ret;
}

void BAT_print(short pos_x, short pos_y, const char *str, ubyte coll)
{
    char chr;
    ubyte colour;

    colour = colour_lookup[coll];
    chr = *str;
    while (chr != '\0')
    {
        if (chr != ' ')
            BAT_draw_char(pos_x, pos_y, chr, colour);
        chr = *++str;
        pos_x += 4;
    }
}

void BAT_unknsub_21(void)
{
    asm volatile ("call ASM_BAT_unknsub_21\n"
        :  :  : "eax" );
}

void BAT_unknsub_22(void)
{
    asm volatile ("call ASM_BAT_unknsub_22\n"
        :  :  : "eax" );
}

void breakout_play_sub1(void)
{
    asm volatile ("call ASM_breakout_play_sub1\n"
        :  :  : "eax" );
}

void breakout_play_sub2(void)
{
    asm volatile ("call ASM_breakout_play_sub2\n"
        :  :  : "eax" );
}

void breakout_func_ddae0(int a1)
{
    asm volatile ("call ASM_breakout_func_ddae0\n"
        :  :  : "eax" );
}

void BAT_play(void)
{
    asm volatile ("call ASM_BAT_play\n"
        :  :  : "eax" );
}

/******************************************************************************/
