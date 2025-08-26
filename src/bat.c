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

/** Minigame screen line width.
 *
 * To allow the output buffer to be used as texture, it needs to have
 * size which is compatible with textures. The actual size which we will
 * utilize can be smaller.
 */
#define BAT_SCREEN_LINE_WIDTH 256

struct BreakoutLevel {
    char *level;
    ubyte field_4[120];
};

struct BATItem;

struct BATItem {
    int UnkDw0;
    int UnkDw1;
    int UnkDw2[4];
    int UnkDw6;
    int UnkDw7;
    struct BATItem *UnkDw8;
    struct BATItem **UnkDw9;
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
extern void *BAT_btarr_1e27a0[320];
extern struct BATItem *BAT_ptr_1e2ca0;
extern struct BATItem *BAT_data_1e2ca4;
extern int BAT_data_1e2ca8;
extern ubyte BAT_btarr_1e2cbc[16];
extern void *BAT_dwarr_1e2ccc[92];
extern void *BAT_ptr_1e2e3c;
extern int BAT_data_1e2e40;

extern struct BreakoutLevel BAT_levels[];

static void BAT_screen_clear(int width, int height)
{
    ubyte *o;
    int y;

    o = BAT_screen;
    for (y = 0; y < height; y++)
    {
        memset(o, 0, width);
        o += BAT_SCREEN_LINE_WIDTH;
    }
}

static void BAT_draw_remainig_lives(int pos_x, int pos_y)
{
    ubyte *o;
    int y;

    o = BAT_screen + pos_y * BAT_SCREEN_LINE_WIDTH + pos_x;
    for (y = 0; y < BAT_data_1e2704; y++, o -= 5)
    {
          TbPixel px;

          px = BAT_byte_1e279c;
          o[1] = px;
          o[2] = px;
          o[3] = px;
          o[4] = px;
    }
}

void BAT_draw_char(int x, int y, char chr, ubyte col)
{
    asm volatile (
      "call ASM_BAT_draw_char\n"
        : : "a" (x), "d" (y), "b" (chr), "c" (col));
}

void BAT_input_paddle(void)
{
    int ms_x;

    if ( lbDisplay.ScreenMode == 1 )
        ms_x = 2 * lbDisplay.MMouseX;
    else
        ms_x = lbDisplay.MMouseX;
    BAT_paddle_x = ((ms_x - 320) >> 2) + 48;

    if (BAT_data_1e2798 < BAT_data_1e2798 + 1)
        BAT_paddle_x = BAT_data_1e2798 + 1;
    if (BAT_data_1e2798 + BAT_paddle_x > 95)
        BAT_paddle_x = 95 - BAT_data_1e2798;
}

int BAT_unknsub_27(void)
{
    int ret;
    asm volatile (
      "call ASM_BAT_unknsub_27\n"
        : "=r" (ret) : );
    return ret;
}

static void BAT_print(short pos_x, short pos_y, const char *str, ubyte coll)
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
#if 0
    asm volatile ("call ASM_BAT_play\n"
        :  :  : "eax" );
#else
  int v3;
  int v7;
  int v22;
  struct BATItem *v25;
  struct BATItem *v26;
  struct BATItem *v27;
  struct BATItem *v28;
  char v29;
  int v42;
  int n;
  int v44;
  char *v58;
  struct BATItem *v61;
  struct BATItem *v62;
  struct BATItem *v63;
  struct BATItem *v64;
  char v65;
  int v75;
  ubyte *v76;
  char v77;
  int v78;
  char *v79;
  int v82;
  int kk;
  int v84;
  char *v87;
  char *v90;
  char *v93;
  char *v96;
  ushort plagent;
  struct Thing *v100;
  char v101;
  int v105;
  int v106;
  struct BATItem *v107;
  int v109;
  int i1;
  int v111;
  int v114;
  struct BATItem *v115;
  int v116;
  int v118;
  char *v119;
  int v120;
  char *v121;
  char locstr[64];

  switch (BAT_state)
  {
    case 0:
      if (BAT_unknsub_27())
      {
        int v0, i, v2;

        v0 = 12;
        BAT_state = 1;
        BAT_data_1e26f0 = 90;
        for ( i = 0; i < 10; ++i )
        {
          v2 = v0;
          for (v3 = 12 * i; v3 != v0; v3++)
            BAT_data_1e271c[v3] = 0x80;
          v0 += 12;
        }
        BAT_data_1e2708 = v2 ^ v3;
        BAT_data_1e2700 = 1;
        BAT_data_1e2704 = 2;
      }
      return;

    case 1:
      BAT_screen_clear(96, 64);
      BAT_data_1e26f0--;
      BAT_input_paddle();

      if (BAT_data_1e26f0 > 60)
      {
          v7 = 32 - 2 * (90 - BAT_data_1e26f0);
          if (v7 < 0)
            v7 = 0;
          BAT_data_1e2ca8 = pixmap.fade_table[256 * v7 + colour_lookup[1]];
          BAT_unknsub_22();
          BAT_unknsub_22();
          BAT_unknsub_22();
          BAT_unknsub_22();
      }
      else
      {
        BAT_data_1e2ca8 = colour_lookup[1];
      }
      breakout_play_sub1();
      sprintf(locstr, "%d", BAT_data_1e2708);
      BAT_print(1, 1, locstr, 1);
      sprintf(locstr, "%d", BAT_data_1e2700);
      BAT_print(48 - 2 * strlen(locstr), 1, locstr, 1);

      BAT_draw_remainig_lives(90, 2);
      BAT_unknsub_21();
      breakout_play_sub2();

      if (BAT_data_1e26f0 < 60)
          sprintf(locstr, "%s", BAT_levels[BAT_data_1e2700 - 1].level);
      else
          sprintf(locstr, "ENTERING LEVEL : %d", BAT_data_1e2700);
      BAT_print(48 - 2 * strlen(locstr) + 1, 52, locstr, 8);

      if (BAT_data_1e26f0 == 60)
      {
        breakout_func_ddae0(BAT_data_1e2700);
        BAT_ptr_1e2ca0 = (struct BATItem *)BAT_btarr_1e27a0;
        BAT_btarr_1e27a0[9] = &BAT_ptr_1e2ca0;

        for (v22 = 0; v22 < 31; v22++)
        {
          BAT_btarr_1e27a0[10 * v22 + 8] = &BAT_btarr_1e27a0[10 * v22 + 10];
          BAT_btarr_1e27a0[10 * v22 + 10 + 9] = &BAT_btarr_1e27a0[10 * v22 + 8];
        }
        BAT_data_1e2ca4 = 0;
        BAT_btarr_1e27a0[10 * v22 + 8] = 0;

        v25 = BAT_ptr_1e2ca0;
        if ( v25 )
        {
          v27 = v25->UnkDw8;
          BAT_ptr_1e2ca0 = v27;
          if ( v27 )
            v27->UnkDw9 = &BAT_ptr_1e2ca0;

          v28 = BAT_data_1e2ca4;
          if (v28)
          {
            v25->UnkDw8 = BAT_data_1e2ca4;
            v28->UnkDw9 = &v25->UnkDw8;
            BAT_data_1e2ca4 = v25;
            v25->UnkDw9 = &BAT_data_1e2ca4;
          }
          else
          {
            v25->UnkDw9 = &BAT_data_1e2ca4;
            v25->UnkDw8 = 0;
            BAT_data_1e2ca4 = v25;
          }
          v26 = v25;
        }
        else
        {
          v26 = 0;
        }
        if ( v26 )
        {
          v26->UnkDw0 = 0x3000;
          v26->UnkDw1 = 0x3100;
          v29 = rand();
          v26->UnkDw7 = 0;
          v26->UnkDw6 = ((v29 & 7) << 8) - 768;
        }
      }
      if (BAT_data_1e26f0 == 0)
        BAT_state = 2;
      goto LABEL_190;
    case 2:
      BAT_screen_clear(96, 64);
      BAT_input_paddle();

      BAT_unknsub_22();
      BAT_unknsub_22();
      BAT_unknsub_22();
      BAT_unknsub_22();
      breakout_play_sub1();

      sprintf(locstr, "%d", BAT_data_1e2708);
      BAT_print(1, 1, locstr, 1);
      sprintf(locstr, "%d", BAT_data_1e2700);
      BAT_print(48 - 2 * strlen(locstr), 1, locstr, 1);
      BAT_draw_remainig_lives(90, 2);
      BAT_unknsub_21();
      breakout_play_sub2();
      if (!BAT_data_1e2ca4)
      {
        BAT_state = 3;
        BAT_data_1e26f4 = 80;
        play_sample_using_heap(0, 73, 127, 64, 100, 0, 3u);
      }
      if (lbKeyOn[KC_N] && lbShift == KMod_SHIFT)
      {
          BAT_data_1e270c = 0;
          BAT_data_1e2704++;
      }
      if (!BAT_data_1e270c)
      {
        if (BAT_data_1e2700 == 10)
        {
          BAT_state = 5;
          BAT_data_1e26ec = 150;
          rand();
          BAT_data_1e26e8 = 0;
        }
        else
        {
          BAT_state = 1;
          BAT_data_1e26f0 = 90;
        }
        v42 = 12;
        for (n = 0; n < 10; n++)
        {
          v44 = 12 * n;
          do
            BAT_data_1e271c[v44++] = 0x80;
          while ( v44 != v42 );
          v42 += 12;
        }
        ++BAT_data_1e2700;
        play_sample_using_heap(0, 70, 127, 64, 100, 0, 3u);
      }
      goto LABEL_190;
    case 3:
      BAT_screen_clear(96, 64);
      --BAT_data_1e26f4;
      BAT_input_paddle();

      breakout_play_sub1();

      sprintf(locstr, "%d", BAT_data_1e2708);
      BAT_print(1, 1, locstr, 1);
      sprintf(locstr, "%d", BAT_data_1e2700);
      BAT_print(48 - 2 * strlen(locstr), 1, locstr, 1);

      BAT_draw_remainig_lives(90, 2);
      BAT_unknsub_21();
      breakout_play_sub2();
      v58 = "BAD LUCK!";
      BAT_print(30, 52, v58, 8);

      if ( BAT_data_1e26f4 == 50 )
      {
        if (BAT_data_1e2704)
        {
          --BAT_data_1e2704;
        }
        else
        {
          BAT_state = 4;
          BAT_data_1e26f8 = 100;
        }
      }
      if ( BAT_data_1e26f4 == 40 )
      {
        v61 = BAT_ptr_1e2ca0;
        if ( v61 )
        {
          v63 = v61->UnkDw8;
          BAT_ptr_1e2ca0 = v63;
          if ( v63 )
            v63->UnkDw9 = &BAT_ptr_1e2ca0;
          v64 = BAT_data_1e2ca4;
          if ( v64 )
          {
            v61->UnkDw8 = BAT_data_1e2ca4;
            v64->UnkDw9 = &v61->UnkDw8;
            BAT_data_1e2ca4 = v61;
            v61->UnkDw9 = &BAT_data_1e2ca4;
          }
          else
          {
            v61->UnkDw9 = &BAT_data_1e2ca4;
            v61->UnkDw8 = 0;
            BAT_data_1e2ca4 = v61;
          }
          v62 = v61;
        }
        else
        {
          v62 = 0;
        }
        v62->UnkDw0 = 0x3000;
        v62->UnkDw1 = 0x3100;
        v65 = rand();
        v62->UnkDw7 = 0;
        v62->UnkDw6 = ((v65 & 7) << 8) - 768;
      }
      if (!BAT_data_1e26f4)
        BAT_state = 2;
      goto LABEL_190;
    case 4:
      BAT_screen_clear(96, 64);
      BAT_input_paddle();

      breakout_play_sub1();
      sprintf(locstr, "%d", BAT_data_1e2708);
      BAT_print(1, 1, locstr, 1);
      sprintf(locstr, "%d", BAT_data_1e2700);
      BAT_print(48 - 2 * strlen(locstr), 1, locstr, 1);

      v75 = 0;
      v76 = BAT_screen + 602;
      if ( BAT_data_1e2704 > 0 )
      {
        do
        {
          v76 -= 5;
          v77 = BAT_byte_1e279c;
          v76[6] = BAT_byte_1e279c;
          v76[7] = v77;
          v78 = BAT_data_1e2704;
          v76[8] = v77;
          ++v75;
          v76[9] = v77;
        }
        while ( v75 < v78 );
      }
      breakout_play_sub2();
      BAT_unknsub_21();
      v79 = "GAME OVER";
      BAT_print(30, 52, v79, 8);

      if (!BAT_data_1e26f8)
      {
        v82 = 12;
        for ( kk = 0; kk < 10; ++kk )
        {
          v84 = 12 * kk;
          do
            BAT_data_1e271c[v84++] = 0x80;
          while ( v84 != v82 );
          v82 += 12;
        }
        BAT_state = 1;
        BAT_data_1e26f0 = 90;
        BAT_data_1e2700 = 1;
        BAT_data_1e2708 = 0;
        BAT_data_1e2704 = 2;
      }
      goto LABEL_190;
    case 5:
      BAT_screen_clear(96, 64);

      v87 = "CONGRATULATIONS";
      BAT_print(18, 10, v87, 5);
      v90 = "BONUS GAME COMPLETE";
      BAT_print(10, 16, v90, 5);
      v93 = "BONUS ITEM";
      BAT_print(28, 30, v93, 1);
      v96 = "AWARDED";
      BAT_print(34, 38, v96, 1);

      for (plagent = 0; plagent < playable_agents; plagent++)
      {
        v100 = players[local_player_no].MyAgent[plagent];
        if ( !(v100->U.UPerson.UMod.Mods >> 12) )
        {
          v101 = LbRandomAnyShort();
          v100->U.UPerson.UMod.Mods &= 0x0FFF;
          v100->U.UPerson.UMod.Mods |= (((v101 & 3) + 1) & 0xF) << 12;
        }
      }
      if (!BAT_data_1e26ec)
      {
        int v102, nn, v104;

        v102 = 12;
        for ( nn = 0; nn < 10; ++nn )
        {
          for (v104 = 12 * nn; v104 < v102; v104++)
            BAT_data_1e271c[v104] = 0x80;
          v102 += 12;
        }
        v106 = 0;
        BAT_ptr_1e2ca0 = (struct BATItem *)&BAT_btarr_1e27a0[0];
        v107 = (struct BATItem *)&BAT_btarr_1e27a0[10];
        BAT_btarr_1e27a0[9] = &BAT_ptr_1e2ca0;
        for (v105 = 0; v105 < 31; v105++)
        {
          BAT_btarr_1e27a0[10 * v105 + 8] = v107;
          BAT_btarr_1e27a0[10 * v105 + 10 + 9] = &BAT_btarr_1e27a0[10 * v105 + 8];
          v107 += 1;
        }

        BAT_data_1e2ca4 = 0;
        BAT_state = 1;
        BAT_data_1e2700 = 1;
        BAT_data_1e2708 = 0;
        BAT_data_1e2704 = 2;
        ingame.UserFlags |= 0x01;
        BAT_btarr_1e27a0[10 * v105 + 8] = 0;
        BAT_data_1e26f0 = 90;
      }
      goto LABEL_190;
    default:
LABEL_190:
      if (!BAT_unknsub_27())
      {
        v109 = 12;
        BAT_state = 0;
        for ( i1 = 0; i1 < 10; ++i1 )
        {
          v111 = 12 * i1;
          do
            BAT_data_1e271c[v111++] = 0x80;
          while ( v111 != v109 );
          v109 += 12;
        }
        BAT_screen_clear(96, 64);

        BAT_ptr_1e2ca0 = (struct BATItem *)BAT_btarr_1e27a0;
        BAT_btarr_1e27a0[9] = &BAT_ptr_1e2ca0;
        v115 = (struct BATItem *)&BAT_btarr_1e27a0[10];
        v116 = 0;
        for (v114 = 0; v114 < 31; v114++)
        {
          BAT_btarr_1e27a0[v116+ 8] = v115;
          BAT_btarr_1e27a0[10 * v114 + 10 + 9] = &BAT_btarr_1e27a0[v116 + 8];
          v116 += 10;
          v115 += 1;
        }
        v118 = 0;
        BAT_data_1e2ca4 = 0;
        BAT_ptr_1e2e3c = BAT_btarr_1e2cbc;
        BAT_dwarr_1e2ccc[1] = &BAT_ptr_1e2e3c;
        BAT_btarr_1e27a0[v116 + 8] = 0;
        v119 = &BAT_btarr_1e2cbc[24]; // access beyond size?
        v120 = 0;
        do
        {
          ++v118;
          BAT_dwarr_1e2ccc[v120] = v119;
          v121 = &BAT_btarr_1e2cbc[v120 * 4 + 16];
          v120 += 6;
          v119 += 24;
          BAT_dwarr_1e2ccc[6 * v118 + 1] = v121;
        }
        while ( v118 < 15 );
        BAT_data_1e2e40 = 0;
        BAT_dwarr_1e2ccc[v120] = 0;
      }
      return;
  }
#endif
}

/******************************************************************************/
