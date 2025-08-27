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

enum BATStates {
  BATSt_Reset = 0,
  BATSt_LevelIntro,
  BATSt_Gameplay,
  BATSt_BallLost,
  BATSt_GameOver,
  BATSt_GameWon,
};

#pragma pack()

extern int BAT_data_1e26e8;
extern int BAT_game_won_timer;
extern int BAT_level_intro_timer;
extern int BAT_ball_lost_timer;
extern int BAT_game_over_timer;
extern int BAT_state;
extern int BAT_levelno;
extern int BAT_num_lives;
extern int BAT_score;
extern int BAT_blocks_remain;
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

    if (BAT_level_intro_timer < 60)
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
          BAT_blocks_remain = 0;
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

    if (BAT_level_intro_timer > 60)
    {
        fade_lv = 32 - 2 * (90 - BAT_level_intro_timer);
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
    BAT_state = BATSt_LevelIntro;
    BAT_levelno = 1;
    BAT_score = 0;
    BAT_num_lives = 2;
    BAT_level_intro_timer = 90;
}

void BAT_unknitm2(void)
{
    struct BATItem *brick_a;
    struct BATItem *brick_r;
    struct BATItem *brick_b;
    ushort rnd;

    brick_a = BAT_ptr_1e2ca0;
    if (brick_a != NULL)
    {
        brick_b = brick_a->UnkDw8;
        BAT_ptr_1e2ca0 = brick_b;
        if (brick_b != NULL)
          brick_b->UnkDw9 = &BAT_ptr_1e2ca0;

        brick_b = BAT_data_1e2ca4;
        if (brick_b) {
          brick_a->UnkDw8 = BAT_data_1e2ca4;
          brick_b->UnkDw9 = &brick_a->UnkDw8;
        } else {
          brick_a->UnkDw8 = 0;
        }
        brick_a->UnkDw9 = &BAT_data_1e2ca4;
        BAT_data_1e2ca4 = brick_a;
        brick_r = brick_a;
    }
    else
    {
        brick_r = 0;
    }
    if ( brick_r )
    {
        brick_r->UnkDw0 = 0x3000;
        brick_r->UnkDw1 = 0x3100;
        rnd = rand();
        brick_r->UnkDw7 = 0;
        brick_r->UnkDw6 = ((rnd & 7) << 8) - 768;
    }
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
    case BATSt_Reset:
        if (BAT_unknsub_27())
        {
            BAT_start_new_game();
            BAT_level_clear();
        }
        return;

    case BATSt_LevelIntro:
        BAT_screen_clear();
        BAT_level_intro_timer--;
        BAT_input_paddle();

        BAT_ball_colour_fade();
        breakout_play_sub1();
        BAT_draw_score_and_level();
        BAT_draw_remainig_lives(90, 2);
        BAT_unknsub_21();
        breakout_play_sub2();
        BAT_draw_level_intro_text();

        if (BAT_level_intro_timer == 60)
        {
            breakout_func_ddae0(BAT_levelno);
            BAT_link_blocks();
            BAT_unknitm2();
        }
        if (BAT_level_intro_timer == 0)
        {
            BAT_state = BATSt_Gameplay;
        }
        break;

    case BATSt_Gameplay:
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
            BAT_state = BATSt_BallLost;
            BAT_ball_lost_timer = 80;
            play_sample_using_heap(0, 73, 127, 64, 100, 0, 3u);
        }
        BAT_input_level();
        if (BAT_blocks_remain == 0)
        {
            if (BAT_levelno == 10)
            {
              BAT_state = BATSt_GameWon;
              BAT_game_won_timer = 150;
              rand();
              BAT_data_1e26e8 = 0;
            }
            else
            {
              BAT_state = BATSt_LevelIntro;
              BAT_level_intro_timer = 90;
            }
            BAT_level_clear();
            BAT_levelno++;
            play_sample_using_heap(0, 70, 127, 64, 100, 0, 3u);
        }
        break;

    case BATSt_BallLost:
      BAT_screen_clear();
      BAT_ball_lost_timer--;
      BAT_input_paddle();

      breakout_play_sub1();
      BAT_draw_score_and_level();
      BAT_draw_remainig_lives(90, 2);
      BAT_unknsub_21();
      breakout_play_sub2();
      BAT_draw_lost_life_message();

      if (BAT_ball_lost_timer == 50)
      {
          if (BAT_num_lives) {
            BAT_num_lives--;
          } else {
            BAT_state = BATSt_GameOver;
            BAT_game_over_timer = 100;
          }
      }
      if (BAT_ball_lost_timer == 40)
      {
          BAT_unknitm2();
      }
      if (BAT_ball_lost_timer == 0)
      {
          BAT_state = BATSt_Gameplay;
      }
      break;

    case BATSt_GameOver:
      BAT_screen_clear();
      BAT_game_over_timer--;
      BAT_input_paddle();

      breakout_play_sub1();
      BAT_draw_score_and_level();
      BAT_draw_remainig_lives(90, 2);
      breakout_play_sub2();
      BAT_unknsub_21();
      BAT_draw_game_over_message();

      if (BAT_game_over_timer == 0)
      {
          BAT_level_clear();
          BAT_start_new_game();
      }
      break;

    case BATSt_GameWon:
      BAT_screen_clear();
      BAT_game_won_timer--;

      BAT_draw_win_message();
      player_agents_add_random_epidermises(&players[local_player_no]);

      if (BAT_game_won_timer == 0)
      {
          BAT_level_clear();
          BAT_link_blocks();
          BAT_start_new_game();
          ingame.UserFlags |= 0x01;
      }
      break;

    default:
      break;
    }
    if (!BAT_unknsub_27())
    {
        BAT_state = BATSt_Reset;
        BAT_level_clear();
        BAT_screen_clear();
        BAT_link_blocks();
        BAT_link_unkstr();
    }
#endif
}

/******************************************************************************/
