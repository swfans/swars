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

#include "display.h"
#include "game.h"
#include "player.h"
#include "sound.h"
#include "swlog.h"
/******************************************************************************/

#pragma pack(1)

/** Minigame screen line width.
 *
 * To allow the output buffer to be used as texture, it needs to have
 * size which is compatible with textures. The actual size which we will
 * utilize can be smaller.
 */
#define BAT_SCREEN_LINE_WIDTH 256

#define BAT_SCREEN_WIDTH 96
#define BAT_SCREEN_HEIGHT 64

#define BAT_BRICK_COLUMNS 12
#define BAT_BRICK_ROWS 10

struct BreakoutLevel {
    char *lv_name;
    ubyte field_4[BAT_BRICK_COLUMNS * BAT_BRICK_ROWS];
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

struct BATUnkn2;

struct BATUnkn2 {
    int UnkDw0;
    int UnkDw1;
    int UnkDw2[2];
    struct BATUnkn2 *UnkDw4;
    struct BATUnkn2 **UnkDw5;
};

#pragma pack()

extern int BAT_data_1e26e8;
extern int BAT_data_1e26ec;
extern int BAT_data_1e26f0;
extern int BAT_data_1e26f4;
extern int BAT_data_1e26f8;
extern int BAT_state;
extern int BAT_levelno;
extern int BAT_num_lives;
extern int BAT_score;
extern int BAT_data_1e270c;
extern ubyte *BAT_screen;
extern ubyte BAT_data_1e271c[BAT_BRICK_COLUMNS * BAT_BRICK_ROWS];
extern int BAT_paddle_x;
extern int BAT_data_1e2798;
extern TbPixel BAT_lives_colour;
extern struct BATItem BAT_btarr_1e27a0[32];
extern struct BATItem *BAT_ptr_1e2ca0;
extern struct BATItem *BAT_data_1e2ca4;
extern TbPixel BAT_ball_colour;
extern struct BATUnkn2 BAT_btarr_1e2cbc[16];
extern struct BATUnkn2 *BAT_ptr_1e2e3c;
extern int BAT_data_1e2e40;

extern struct BreakoutLevel BAT_levels[];

static void BAT_screen_clear_rect(int beg_x, int beg_y, int end_x, int end_y)
{
    ubyte *o;
    int y;

    o = BAT_screen + beg_y * BAT_SCREEN_LINE_WIDTH;
    for (y = beg_y; y < end_y; y++)
    {
        memset(o + beg_x, 0, end_x - beg_x);
        o += BAT_SCREEN_LINE_WIDTH;
    }
}

static void BAT_screen_clear(void)
{
    BAT_screen_clear_rect(0, 0, BAT_SCREEN_WIDTH, BAT_SCREEN_HEIGHT);
}

static void BAT_draw_remainig_lives(int pos_x, int pos_y)
{
    ubyte *o;
    int y;

    o = BAT_screen + pos_y * BAT_SCREEN_LINE_WIDTH + pos_x;
    for (y = 0; y < BAT_num_lives; y++, o -= 5)
    {
        TbPixel px;

        px = BAT_lives_colour;
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

static void BAT_draw_score_and_level(void)
{
    char locstr[32];

    sprintf(locstr, "%d", BAT_score);
    BAT_print(1, 1, locstr, ColLU_WHITE);
    sprintf(locstr, "%d", BAT_levelno);
    BAT_print(BAT_SCREEN_WIDTH / 2 - 2 * strlen(locstr), 1, locstr, ColLU_WHITE);
}

static void BAT_draw_win_message(void)
{
    const char *str;

    str = "CONGRATULATIONS";
    BAT_print(18, 10, str, ColLU_YELLOW);
    str = "BONUS GAME COMPLETE";
    BAT_print(10, 16, str, ColLU_YELLOW);
    str = "BONUS ITEM";
    BAT_print(28, 30, str, ColLU_WHITE);
    str = "AWARDED";
    BAT_print(34, 38, str, ColLU_WHITE);
}

static void BAT_draw_lost_life_message(void)
{
    const char *str;

    str = "BAD LUCK!";
    BAT_print(30, 52, str, 8);
}

static void BAT_draw_game_over_message(void)
{
    const char *str;

    str = "GAME OVER";
    BAT_print(30, 52, str, 8);
}

static void BAT_draw_level_intro_text(void)
{
    char locstr[64];

    if (BAT_data_1e26f0 < 60)
          sprintf(locstr, "%s", BAT_levels[BAT_levelno - 1].lv_name);
    else
          sprintf(locstr, "ENTERING LEVEL : %d", BAT_levelno);
    BAT_print(BAT_SCREEN_WIDTH / 2 - 2 * strlen(locstr) + 1, 52, locstr, 8);
}

static void BAT_input_paddle(void)
{
    int ms_x, scr_x;

    ms_x = lbDisplay.MMouseX;
    scr_x = lbDisplay.GraphicsScreenWidth;
    BAT_paddle_x = BAT_SCREEN_WIDTH / 2 + ((ms_x - scr_x/2) * BAT_SCREEN_WIDTH) / (scr_x / 2);

    if (BAT_paddle_x < BAT_data_1e2798 + 1)
        BAT_paddle_x = BAT_data_1e2798 + 1;
    if (BAT_paddle_x + BAT_data_1e2798 > 95)
        BAT_paddle_x = 95 - BAT_data_1e2798;
}

/** Inputs active only during a level play.
 */
static void BAT_input_level(void)
{
      if (lbKeyOn[KC_N] && lbShift == KMod_SHIFT)
      {
          BAT_data_1e270c = 0;
          BAT_num_lives++;
      }
}

static void BAT_level_set_rect(int beg_c, int beg_r, int end_c, int end_r, ubyte val)
{
    int c, r;
    for (r = beg_r; r < end_r; r++)
    {
        for (c = beg_c; c < end_c; c++)
            BAT_data_1e271c[BAT_BRICK_COLUMNS * r + c] = val;
    }
}

static void BAT_level_clear(void)
{
    BAT_level_set_rect(0, 0, BAT_BRICK_COLUMNS, BAT_BRICK_ROWS, 0x80);
}

void BAT_unknsub_22(void)
{
    asm volatile ("call ASM_BAT_unknsub_22\n"
        :  :  : "eax" );
}

static void BAT_ball_colour_fade(void)
{
    int fade_lv;

    if (BAT_data_1e26f0 > 60)
    {
        fade_lv = 32 - 2 * (90 - BAT_data_1e26f0);
        if (fade_lv < 0)
            fade_lv = 0;
        BAT_ball_colour = pixmap.fade_table[256 * fade_lv + colour_lookup[1]];
        BAT_unknsub_22();
        BAT_unknsub_22();
        BAT_unknsub_22();
        BAT_unknsub_22();
    }
    else
    {
        BAT_ball_colour = colour_lookup[1];
    }
}

void BAT_link_blocks(void)
{
    int i;

    BAT_ptr_1e2ca0 = &BAT_btarr_1e27a0[0];
    BAT_btarr_1e27a0[0].UnkDw9 = &BAT_ptr_1e2ca0;
    for (i = 0; i < 31; i++)
    {
        BAT_btarr_1e27a0[i].UnkDw8 = &BAT_btarr_1e27a0[i + 1];
        BAT_btarr_1e27a0[i + 1].UnkDw9 = &BAT_btarr_1e27a0[i].UnkDw8;
    }
    BAT_btarr_1e27a0[i].UnkDw8 = 0;
    BAT_data_1e2ca4 = 0;
}

void BAT_link_unkstr(void)
{
    int i;

    BAT_ptr_1e2e3c = &BAT_btarr_1e2cbc[0];
    BAT_btarr_1e2cbc[0].UnkDw5 = &BAT_ptr_1e2e3c;
    for (i = 0; i < 15; i++)
    {
        BAT_btarr_1e2cbc[i].UnkDw4 = &BAT_btarr_1e2cbc[i + 1];
        BAT_btarr_1e2cbc[i + 1].UnkDw5 = &BAT_btarr_1e2cbc[i].UnkDw4;
    }
    BAT_btarr_1e2cbc[i].UnkDw4 = 0;
    BAT_data_1e2e40 = 0;
}

void BAT_start_new_game(void)
{
    BAT_state = 1;
    BAT_levelno = 1;
    BAT_score = 0;
    BAT_num_lives = 2;
    BAT_data_1e26f0 = 90;
}

int BAT_unknsub_27(void)
{
    int ret;
    asm volatile (
      "call ASM_BAT_unknsub_27\n"
        : "=r" (ret) : );
    return ret;
}

void BAT_unknsub_21(void)
{
    asm volatile ("call ASM_BAT_unknsub_21\n"
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
  switch (BAT_state)
  {
    case 0:
      if (BAT_unknsub_27())
      {
        BAT_start_new_game();
        BAT_level_clear();
      }
      return;

    case 1:
      BAT_screen_clear();
      BAT_data_1e26f0--;
      BAT_input_paddle();
      BAT_ball_colour_fade();
      breakout_play_sub1();
      BAT_draw_score_and_level();
      BAT_draw_remainig_lives(90, 2);
      BAT_unknsub_21();
      breakout_play_sub2();
      BAT_draw_level_intro_text();

      if (BAT_data_1e26f0 == 60)
      {
        struct BATItem *v25;
        struct BATItem *v26;
        struct BATItem *v27;
        struct BATItem *v28;
        char v29;

        breakout_func_ddae0(BAT_levelno);
        BAT_link_blocks();

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
      BAT_screen_clear();
      BAT_input_paddle();

      BAT_unknsub_22();
      BAT_unknsub_22();
      BAT_unknsub_22();
      BAT_unknsub_22();
      breakout_play_sub1();
      BAT_draw_score_and_level();
      BAT_draw_remainig_lives(90, 2);
      BAT_unknsub_21();
      breakout_play_sub2();
      if (!BAT_data_1e2ca4)
      {
        BAT_state = 3;
        BAT_data_1e26f4 = 80;
        play_sample_using_heap(0, 73, 127, 64, 100, 0, 3u);
      }
      BAT_input_level();
      if (!BAT_data_1e270c)
      {
        if (BAT_levelno == 10)
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
        BAT_level_clear();
        BAT_levelno++;
        play_sample_using_heap(0, 70, 127, 64, 100, 0, 3u);
      }
      goto LABEL_190;
    case 3:
      BAT_screen_clear();
      BAT_data_1e26f4--;
      BAT_input_paddle();

      breakout_play_sub1();
      BAT_draw_score_and_level();
      BAT_draw_remainig_lives(90, 2);
      BAT_unknsub_21();
      breakout_play_sub2();
      BAT_draw_lost_life_message();

      if (BAT_data_1e26f4 == 50)
      {
        if (BAT_num_lives)
        {
          --BAT_num_lives;
        }
        else
        {
          BAT_state = 4;
          BAT_data_1e26f8 = 100;
        }
      }
      if (BAT_data_1e26f4 == 40)
      {
        struct BATItem *v61;
        struct BATItem *v62;
        struct BATItem *v63;
        struct BATItem *v64;
        char v65;

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
      BAT_screen_clear();
      BAT_input_paddle();

      breakout_play_sub1();
      BAT_draw_score_and_level();
      BAT_draw_remainig_lives(90, 2);
      breakout_play_sub2();
      BAT_unknsub_21();
      BAT_draw_game_over_message();

      if (!BAT_data_1e26f8)
      {
        BAT_level_clear();
        BAT_start_new_game();
      }
      goto LABEL_190;
    case 5:
      BAT_screen_clear();
      BAT_draw_win_message();
      player_agents_add_random_epidermises(&players[local_player_no]);
      if (!BAT_data_1e26ec)
      {
        BAT_level_clear();
        BAT_link_blocks();
        BAT_start_new_game();
        ingame.UserFlags |= 0x01;
      }
      goto LABEL_190;
    default:
LABEL_190:
      if (!BAT_unknsub_27())
      {
        BAT_state = 0;
        BAT_level_clear();
        BAT_screen_clear();
        BAT_link_blocks();
        BAT_link_unkstr();
      }
      return;
  }
#endif
}

/******************************************************************************/
