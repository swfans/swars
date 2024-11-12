/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file fepause.c
 *     Front-end desktop and menu system, in-game pause screen.
 * @par Purpose:
 *     Implement functions for pause screen displayed in-game.
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
#include "fepause.h"

#include "bfbox.h"
#include "bfline.h"
#include "bfscreen.h"
#include "bfsprite.h"
#include "bftext.h"
#include "bftringl.h"
#include "bfkeybd.h"
#include "bfaudio.h"
#include "bfmusic.h"
#include "bfscd.h"
#include "ssampply.h"
#include "bflib_joyst.h"
#include "campaign.h"
#include "display.h"
#include "game_data.h"
#include "game_sprts.h"
#include "game.h"
#include "guiboxes.h"
#include "guitext.h"
#include "keyboard.h"
#include "mouse.h"
#include "mydraw.h"
#include "sound.h"
#include "swlog.h"
/******************************************************************************/

ubyte sub_71694(int a1, int a2, char *text, int a4, ubyte a5, ubyte a6)
{
    ubyte ret;
    asm volatile (
      "push %6\n"
      "push %5\n"
      "call ASM_sub_71694\n"
        : "=r" (ret) : "a" (a1), "d" (a2), "b" (text), "c" (a4), "g" (a5), "g" (a6));
    return ret;
}

TbBool input_kicked_left_arrow(struct ScreenBox *box, short *target)
{
    if (mouse_move_over_slant_box(box))
    {
        if (lbDisplay.MLeftButton)
        {
            lbDisplay.LeftButton = 0;
            (*target)--;
            if ((*target) < 0)
                (*target) = 0;
            return true;
        }
    }
    return false;
}

TbBool input_kicked_right_arrow(struct ScreenBox *box, short *target)
{
    if (mouse_move_over_slant_box(box))
    {
        if (lbDisplay.MLeftButton)
        {
            lbDisplay.LeftButton = 0;
            (*target)++;
            if ((*target) > 322)
                (*target) = 322;
            return true;
        }
    }
    return false;
}

TbBool input_slant_box(struct ScreenBox *box, short *target)
{
    int ms_x, ms_y;

    if (lbDisplay.MLeftButton)
    {
        ms_x = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
        ms_y = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
        if ((ms_y >= box->Y + 2) && (ms_y <= box->Y + box->Height - 4))
        {
            int dx, dy;
            dx = ms_x - box->X - 2;
            dy = ms_y - box->Y - 2;
            if (dx + dy >= 0 && dx + dy <= box->Width - 4 - 2)
            {
                (*target) = 322 * (dx + dy) / (box->Width - 4 - 2);
                return true;
            }
        }
    }
    return false;
}

void draw_box_cutedge(struct ScreenBox *box, TbPixel colr1)
{
    short cut, stp;
    if (lbDisplay.GraphicsScreenHeight < 400) {
        stp = 1;
        cut = 25;
    } else {
        stp = 2;
        cut = 50;
    }

    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    LbDrawBox(box->X + 0, box->Y + 0, box->Width - cut, box->Height - cut, colr1);
    LbDrawBox(box->X + box->Width - cut, box->Y +  cut, cut, box->Height - 2*cut, colr1);
    LbDrawBox(box->X + cut, box->Y + box->Height - cut, box->Width - cut, cut, colr1);
    LbDrawTriangle(box->X + box->Width - cut, box->Y + 0,
      box->X + box->Width, box->Y + cut,
      box->X + box->Width - cut, box->Y + cut, colr1);
    LbDrawTriangle(box->X + stp, box->Y + box->Height - cut,
      box->X + cut, box->Y + box->Height - cut,
      box->X + cut, box->Y + box->Height - stp, colr1);

    lbDisplay.DrawFlags = 0;
    LbDrawLine(box->X + 0, box->Y + 0, box->X + box->Width - cut, box->Y + 0, colr1);
    LbDrawLine(box->X + 0, box->Y + 0, box->X + 0, box->Y + box->Height - cut - stp, colr1);
    LbDrawLine(box->X + box->Width, box->Y + cut, box->X + box->Width, box->Y + box->Height - stp, colr1);
    LbDrawLine(box->X + cut, box->Y + box->Height - stp, box->X + box->Width, box->Y + box->Height - stp, colr1);
    LbDrawLine(box->X + box->Width - cut, box->Y + 0, box->X + box->Width, box->Y + cut, colr1);
    LbDrawLine(box->X + stp, box->Y + box->Height - cut, box->X + cut, box->Height + cut + stp, colr1);
}

void draw_parallelogram_45degi(short x, short y, short w, short h, TbPixel colr2)
{
    short cx, i;

    cx = x;
    for (i = 0; i < h; i += 2)
    {
        if (lbDisplay.GraphicsScreenHeight < 400)
            LbDrawLine((cx) >> 1, (y+i) >> 1, (cx+w) >> 1, (y+i) >> 1, colr2);
        else
            LbDrawLine((cx), (y+i), (cx+w), (y+i), colr2);
        cx -= 2;
    }
}

void draw_slant_box(struct ScreenBox *box, TbPixel colr2)
{
    draw_parallelogram_45degi(box->X, box->Y, box->Width, box->Height, colr2);
    if (lbDisplay.GraphicsScreenHeight >= 400)
        draw_parallelogram_45degi(box->X - 1, box->Y + 1, box->Width, box->Height, colr2);
}

void draw_kicked_left_arrow(struct ScreenBox *box, TbPixel colr2)
{
    short stp;

    if (lbDisplay.GraphicsScreenHeight < 400)
        stp = 1;
    else
        stp = 2;

    if (mouse_move_over_slant_box(box))
    {
        if (lbDisplay.GraphicsScreenHeight < 400)
            LbSpriteDrawOneColour(box->X >> 1, (box->Y >> 1) - stp, &pop1_sprites[100],
              colour_lookup[1]);
        else
            LbSpriteDrawOneColour(box->X, box->Y - stp, &pop1_sprites[100],
              colour_lookup[1]);
    }
    else
    {
        if (lbDisplay.GraphicsScreenHeight < 400)
            LbSpriteDraw(box->X >> 1, (box->Y >> 1) - stp, &pop1_sprites[100]);
        else
            LbSpriteDraw(box->X, box->Y - stp, &pop1_sprites[100]);
    }
}

void draw_kicked_right_arrow(struct ScreenBox *box, TbPixel colr2)
{
    short stp;

    if (lbDisplay.GraphicsScreenHeight < 400)
        stp = 1;
    else
        stp = 2;

    if (mouse_move_over_slant_box(box))
    {
        if (lbDisplay.GraphicsScreenHeight < 400)
            LbSpriteDrawOneColour((box->X >> 1) - stp, (box->Y >> 1) - stp,
              &pop1_sprites[101], colour_lookup[1]);
        else
            LbSpriteDrawOneColour(box->X - stp, box->Y - stp,
              &pop1_sprites[101], colour_lookup[1]);
    }
    else
    {
        if (lbDisplay.GraphicsScreenHeight < 400)
            LbSpriteDraw((box->X >> 1) - stp, (box->Y >> 1) - stp, &pop1_sprites[101]);
        else
            LbSpriteDraw(box->X - stp, box->Y - stp, &pop1_sprites[101]);
    }
}

TbBool pause_screen_handle(void)
{
    int w;
    const char *s;
    TbBool resume_game;
    TbPixel colr1, colr2;
    struct ScreenBox main_box;
    short *target;
    short *affected;

    if (lbDisplay.GraphicsScreenHeight < 400) {
        main_box.X = 43;
        main_box.Width = 233;
        main_box.Y = 27;
        main_box.Height = 122;
    } else {
        main_box.X = 86;
        main_box.Width = 466;
        main_box.Y = 54;
        main_box.Height = 244;
    }

    if ((ingame.PanelPermutation != 2) && (ingame.PanelPermutation != -3))
        colr1 = 20;
    else
        colr1 = 40;
    snd_unkn1_volume_all_samples();
    update_danger_music(2);

    // Wait for the pause key to be released
    lbKeyOn[kbkeys[GKey_PAUSE]] = 0;
    while ((jskeys[GKey_PAUSE] != 0) &&
      (jskeys[GKey_PAUSE] != joy.Buttons[0]))
    {
        joy_func_065(&joy);

        swap_wscreen();
        game_update();
    }

    do_change_mouse(8);

    draw_box_cutedge(&main_box, colr1);

    if ((ingame.PanelPermutation != 2) && (ingame.PanelPermutation != -3))
        colr2 = 15;
    else
        colr2 = 35;
    lbFontPtr = small_font;
    my_set_text_window(0, 0, lbDisplay.PhysicalScreenWidth,
      lbDisplay.PhysicalScreenHeight);
    if ((ingame.PanelPermutation == 2) || (ingame.PanelPermutation == -3))
    {
        lbDisplay.DrawFlags |= Lb_TEXT_ONE_COLOR;
        lbDisplay.DrawColour = colr2;
    }
    if (lbDisplay.GraphicsScreenHeight < 400)
    {
        s = mission_name;
        w = my_string_width(s);
        my_draw_text(161 - (w >> 1), 44, s, 0);
        s = gui_strings[419];
        w = my_string_width(s);
        my_draw_text(161 - (w >> 1), 54, s, 0);
        s = gui_strings[420];
        w = my_string_width(s);
        my_draw_text(161 - (w >> 1), 76, s, 0);
        s = gui_strings[516];
        w = my_string_width(s);
        my_draw_text(161 - (w >> 1), 98, s, 0);
    }
    else
    {
        s = mission_name;
        w = my_string_width(s);
        my_draw_text(322 - (w >> 1), 88, s, 0);
        s = gui_strings[419];
        w = my_string_width(s);
        my_draw_text(322 - (w >> 1), 108, s, 0);
        s = gui_strings[420];
        w = my_string_width(s);
        my_draw_text(322 - (w >> 1), 152, s, 0);
        s = gui_strings[516];
        w = my_string_width(s);
        my_draw_text(322 - (w >> 1), 196, s, 0);
    }
    if (ingame.PanelPermutation == 2 || ingame.PanelPermutation == -3)
        lbDisplay.DrawFlags &= ~Lb_TEXT_ONE_COLOR;

    if (language_3str[0] == 'e')
    {
        if (lbDisplay.GraphicsScreenHeight < 400)
        {
            LbSpriteDraw(89, 31, &pop1_sprites[102]);
            LbSpriteDraw(83, 120, &pop1_sprites[103]);
        }
        else
        {
            LbSpriteDraw(178, 62, &pop1_sprites[102]);
            w = pop1_sprites[102].SWidth;
            LbSpriteDraw(178 + w, 62, &pop1_sprites[104]);
            LbSpriteDraw(166, 240, &pop1_sprites[103]);
        }
    }
    else
    {
        if (lbDisplay.GraphicsScreenHeight < 400)
        {
            s = gui_strings[606];
            w = my_string_width(s);
            my_draw_text(161 - (w >> 1), 31, s, 0);
            s = gui_strings[470];
            w = my_string_width(s);
            my_draw_text(103 - (w >> 1), 123, s, 0);
        }
        else
        {
            s = gui_strings[606];
            w = my_string_width(s);
            my_draw_text(322 - (w >> 1), 62, s, 0);
            s = gui_strings[470];
            w = my_string_width(s);
            my_draw_text(206 - (w >> 1), 246, s, 0);
        }
    }
    swap_wscreen();
    if (!ingame.fld_unk7DA)
        SetMusicVolume(100, 0x7F);

    resume_game = false;
    while (!resume_game)
    {
        joy_func_065(&joy);
        affected = NULL;

        {
        target = &startscr_samplevol;
        struct ScreenBox box1;
        box1.X = 204;
        box1.Y = 122;
        box1.Width = 238;
        box1.Height = 18;

        struct ScreenBox box2;
        box2.X = 176;
        box2.Y = 122;
        box2.Width = 8;
        box2.Height = 18;

        struct ScreenBox box3;
        box3.X = 432;
        box3.Y = 122;
        box3.Width = 8;
        box3.Height = 18;

        // Draw the main slider box
        draw_slant_box(&box1, colr2);
        // Draw the side arrows
        draw_kicked_left_arrow(&box2, colr2);
        draw_kicked_right_arrow(&box3, colr2);

        if (input_kicked_left_arrow(&box2, target))
            affected = target;
        if (input_kicked_right_arrow(&box3, target))
            affected = target;
        if (input_slant_box(&box1, target))
            affected = target;

        if (*target) // Draw slider box filling
        {
            struct ScreenBox box4;
            box4.X = box1.X + 2;
            box4.Y = box1.Y + 2;
            box4.Width = (box1.Width - 6) * (*target) / 322;
            box4.Height = box1.Height - 4;
            draw_slant_box(&box4, colour_lookup[1]);
        }
        }

        {
        target = &startscr_midivol;
        struct ScreenBox box1;
        box1.X = 204;
        box1.Y = 166;
        box1.Width = 238;
        box1.Height = 18;

        struct ScreenBox box2;
        box2.X = 176;
        box2.Y = 166;
        box2.Width = 8;
        box2.Height = 18;

        struct ScreenBox box3;
        box3.X = 432;
        box3.Y = 166;
        box3.Width = 8;
        box3.Height = 18;

        draw_slant_box(&box1, colr2);
        draw_kicked_left_arrow(&box2, colr2);
        draw_kicked_right_arrow(&box3, colr2);

        if (input_kicked_left_arrow(&box2, target))
            affected = target;
        if (input_kicked_right_arrow(&box3, target))
            affected = target;
        if (input_slant_box(&box1, target))
            affected = target;

        if (*target)
        {
            struct ScreenBox box4;
            box4.X = box1.X + 2;
            box4.Y = box1.Y + 2;
            box4.Width = (box1.Width - 6) * (*target) / 322;
            box4.Height = box1.Height - 4;
            draw_slant_box(&box4, colour_lookup[1]);
        }
        }

        {
        target = &startscr_cdvolume;
        struct ScreenBox box1;
        box1.X = 204;
        box1.Y = 210;
        box1.Width = 238;
        box1.Height = 18;

        struct ScreenBox box2;
        box2.X = 176;
        box2.Y = 210;
        box2.Width = 8;
        box2.Height = 18;

        struct ScreenBox box3;
        box3.X = 432;
        box3.Y = 210;
        box3.Width = 8;
        box3.Height = 18;

        draw_slant_box(&box1, colr2);
        draw_kicked_left_arrow(&box2, colr2);
        draw_kicked_right_arrow(&box3, colr2);

        if (input_kicked_left_arrow(&box2, target))
            affected = target;
        if (input_kicked_right_arrow(&box3, target))
            affected = target;
        if (input_slant_box(&box1, target))
            affected = target;

        if (*target)
        {
            struct ScreenBox box4;
            box4.X = box1.X + 2;
            box4.Y = box1.Y + 2;
            box4.Width = (box1.Width - 6) * (*target) / 322;
            box4.Height = box1.Height - 4;
            draw_slant_box(&box4, colour_lookup[1]);
        }
        }

        if (affected == &startscr_samplevol)
        {
            SetSoundMasterVolume(127 * (*affected) / 322);
            if (!IsSamplePlaying(0, 80, 0))
                play_sample_using_heap(0, 80, 127, 64, 100, 0, 1u);
        }
        else if (affected == &startscr_midivol)
        {
            SetMusicMasterVolume(127 * (*affected) / 322);
        }
        else if (affected == &startscr_cdvolume)
        {
            SetCDVolume(70 * (127 * (*affected) / 322) / 100);
        }

        if (sub_71694(180, 120, gui_strings[477], colr1, colr2, ingame.DetailLevel == 1))
        {
            ingame.DetailLevel = 1;
            bang_set_detail(0);
        }
        if (sub_71694(220, 120, gui_strings[475], colr1, colr2, ingame.DetailLevel == 0))
        {
            ingame.DetailLevel = 0;
            bang_set_detail(1);
        }

        if (sub_71694(140, 134, gui_strings[455], colr1, colr2, 0))
            resume_game = true;
        if (sub_71694(197, 134, gui_strings[445], colr1, colr2, 0))
        {
            swap_wscreen();
            SetMusicVolume(100, 0);
            StopAllSamples();
            StopCD();
            return 1;
        }

        if (lbKeyOn[kbkeys[GKey_PAUSE]]
          || (jskeys[GKey_PAUSE] && jskeys[GKey_PAUSE] == joy.Buttons[0]))
            resume_game = true;

        lbDisplay.DrawFlags = 0;
        process_sound_heap();
        PlayCDTrack(ingame.CDTrack);

        swap_wscreen();
        game_update();
    }

    // Wait for the pause key to be released
    resume_game = false;
    while (!resume_game)
    {
        joy_func_065(&joy);

        if (!(lbKeyOn[kbkeys[GKey_PAUSE]]
          || (jskeys[GKey_PAUSE] && jskeys[GKey_PAUSE] == joy.Buttons[0])))
            resume_game = true;

        PlayCDTrack(ingame.CDTrack);

        swap_wscreen();
        game_update();
    }
    lbDisplay.RightButton = 0;
    lbDisplay.LeftButton = 0;
    if (!ingame.fld_unk7DA)
        SetMusicVolume(100, 0);
    return 0;
}

/******************************************************************************/
