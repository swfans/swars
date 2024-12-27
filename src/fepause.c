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
static struct ScreenBox pause_main_box;
static TbPixel pause_colr1, pause_colr2;
static struct ScreenBox samplevol_slider_box;
static struct ScreenBox samplevol_arrow_l_box;
static struct ScreenBox samplevol_arrow_r_box;
static struct ScreenBox midivol_slider_box;
static struct ScreenBox midivol_arrow_l_box;
static struct ScreenBox midivol_arrow_r_box;
static struct ScreenBox cdvolume_slider_box;
static struct ScreenBox cdvolume_arrow_l_box;
static struct ScreenBox cdvolume_arrow_r_box;

ubyte update_button(short scr_x, short scr_y, char *text, TbPixel colr1, TbPixel colr2, ubyte enabled)
{
#if 0
    ubyte ret;
    asm volatile (
      "push %6\n"
      "push %5\n"
      "call ASM_update_button\n"
        : "=r" (ret) : "a" (scr_x), "d" (scr_y), "b" (text), "c" (colr1), "g" (colr2), "g" (enabled));
    return ret;
#endif
    struct ScreenButton btn1;
    short line_h;
    short text_w;
    short margin;

    if (lbDisplay.GraphicsScreenHeight < 400) {
        line_h = font_height('A');
        text_w = my_string_width(text);
        margin = 1;
    } else {
        scr_x *= 2;
        scr_y *= 2;
        line_h = 2 * font_height('A');
        text_w = 2 * my_string_width(text);
        margin = 2;
    }

    btn1.Width = text_w + 4 * margin;
    btn1.Height = line_h + 4 * margin;
    btn1.X = scr_x - (btn1.Width >> 1);
    btn1.Y = scr_y;

    lbDisplay.DrawFlags = 0x0004;
    LbDrawBox(btn1.X - margin, btn1.Y - margin, btn1.Width + 2 * margin, btn1.Height + 2 * margin, colr1);

    lbDisplay.DrawFlags = 0x0010;
    lbDisplay.DrawColour = colour_lookup[1];

    if (enabled || mouse_down_over_box(&btn1))
    {
      short text_x, text_y;

      LbDrawBox(btn1.X, btn1.Y, btn1.Width, btn1.Height, lbDisplay.DrawColour);

      lbDisplay.DrawFlags |= 0x0040;
      if (lbDisplay.GraphicsScreenHeight < 400) {
          text_x = btn1.X + 2 * margin;
          text_y = btn1.Y + 2 * margin;
      } else {
          text_x = btn1.X + (text_w >> 2) + 2 * margin;
          text_y = btn1.Y + (line_h >> 1) + margin;
      }
      my_draw_text(text_x, text_y, text, 0);

      lbDisplay.DrawFlags &= ~0x0040;
      if (lbDisplay.LeftButton)
      {
        if (mouse_down_over_box(&btn1))
            lbDisplay.LeftButton = 0;

        if (mouse_down_over_box(&btn1))
            return 1;
      }
    }
    else
    {
      short text_x, text_y;

      if (ingame.PanelPermutation == 2 || ingame.PanelPermutation == -3)
      {
          lbDisplay.DrawFlags |= 0x0040;
          lbDisplay.DrawColour = colr2;
      }
      LbDrawBox(btn1.X, btn1.Y, btn1.Width, btn1.Height, colr2);

      if (lbDisplay.GraphicsScreenHeight < 400) {
          text_x = btn1.X + 2 * margin;
          text_y = btn1.Y + 2 * margin;
      } else {
          text_x = btn1.X + (text_w >> 2) + 2 * margin;
          text_y = btn1.Y + (line_h >> 1) + margin;
      }
      my_draw_text(text_x, text_y, text, 0);
      lbDisplay.DrawFlags &= ~0x0040;
    }
    return 0;
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
              colour_lookup[ColLU_WHITE]);
        else
            LbSpriteDrawOneColour(box->X, box->Y - stp, &pop1_sprites[100],
              colour_lookup[ColLU_WHITE]);
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
              &pop1_sprites[101], colour_lookup[ColLU_WHITE]);
        else
            LbSpriteDrawOneColour(box->X - stp, box->Y - stp,
              &pop1_sprites[101], colour_lookup[ColLU_WHITE]);
    }
    else
    {
        if (lbDisplay.GraphicsScreenHeight < 400)
            LbSpriteDraw((box->X >> 1) - stp, (box->Y >> 1) - stp, &pop1_sprites[101]);
        else
            LbSpriteDraw(box->X - stp, box->Y - stp, &pop1_sprites[101]);
    }
}

void init_slider_with_arrows_centered(struct ScreenBox *slider_box, struct ScreenBox *arrow_l_box,
  struct ScreenBox *arrow_r_box, const struct ScreenBox *parent_box, int shift_y, int slider_w, int arrow_w, int h)
{
    slider_box->Width = slider_w;
    slider_box->Height = h;
    slider_box->X = parent_box->X + (parent_box->Width - slider_box->Width) / 2 + 4;
    slider_box->Y = parent_box->Y + shift_y;

    arrow_l_box->Width = arrow_w;
    arrow_l_box->Height = h;
    arrow_l_box->X = slider_box->X - 28;
    arrow_l_box->Y = slider_box->Y;

    arrow_r_box->Width = arrow_w;
    arrow_r_box->Height = h;
    arrow_r_box->X = slider_box->X + slider_box->Width - 10;
    arrow_r_box->Y = slider_box->Y;
}

void init_pause_screen_boxes(void)
{
    int slider_w, arrow_w, slider_h, margin_h;

    pause_main_box.Width = 466;
    pause_main_box.Height = 244;
    pause_main_box.X = 86;
    pause_main_box.Y = 54;
    slider_h = 18;

    slider_w = 466 * 131 / 256;
    arrow_w = slider_w * 9 / 256;
    margin_h = 26;

    init_slider_with_arrows_centered(&samplevol_slider_box, &samplevol_arrow_l_box,
      &samplevol_arrow_r_box, &pause_main_box, 68, slider_w, arrow_w, slider_h);

    init_slider_with_arrows_centered(&midivol_slider_box, &midivol_arrow_l_box,
      &midivol_arrow_r_box, &pause_main_box, 68 + 1 * (slider_h + margin_h), slider_w, arrow_w, slider_h);

    init_slider_with_arrows_centered(&cdvolume_slider_box, &cdvolume_arrow_l_box,
      &cdvolume_arrow_r_box, &pause_main_box, 68 + 2 * (slider_h + margin_h), slider_w, arrow_w, slider_h);

    // Currenlty not all components should be scaled to low res, hence changing the parent box here
    if (lbDisplay.GraphicsScreenHeight < 400) {
        pause_main_box.Width = 233;
        pause_main_box.Height = 122;
        pause_main_box.X = 43;
        pause_main_box.Y = 27;
    }
}

void start_pause_screen(void)
{
    if ((ingame.PanelPermutation != 2) && (ingame.PanelPermutation != -3)) {
        pause_colr1 = 20;
        pause_colr2 = 15;
    } else {
        pause_colr1 = 40;
        pause_colr2 = 35;
    }

    snd_unkn1_volume_all_samples();
    update_danger_music(2);

    do_change_mouse(8);

    pause_main_box.Flags &= ~GBxFlg_BkgndDrawn;

}

/** Wait for the pause toggle key to be released.
 */
void wait_for_keypress_end(ushort game_key, TbBool impatient)
{
    TbBool ended;

    if (impatient) {
        lbKeyOn[kbkeys[game_key]] = 0;
    }

    ended = false;
    while (!ended)
    {
        joy_func_065(&joy);

        if (!lbKeyOn[kbkeys[game_key]]
          && (jskeys[game_key] == 0 || jskeys[game_key] != joy.Buttons[0]))
            ended = true;

        PlayCDTrack(ingame.CDTrack);

        swap_wscreen();
        game_update();
    }
}

void draw_pause_screen_static(struct ScreenBox *box)
{
    int w;
    const char *s;

    draw_box_cutedge(box, pause_colr1);

    lbFontPtr = small_font;
    my_set_text_window(0, 0, lbDisplay.PhysicalScreenWidth,
      lbDisplay.PhysicalScreenHeight);
    if ((ingame.PanelPermutation == 2) || (ingame.PanelPermutation == -3))
    {
        lbDisplay.DrawFlags |= Lb_TEXT_ONE_COLOR;
        lbDisplay.DrawColour = pause_colr2;
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
}

void draw_pause_volume_bar(struct ScreenBox *p_box1, struct ScreenBox *p_box2, struct ScreenBox *p_box3, short *p_target)
{
    // Draw the main slider box
    draw_slant_box(p_box1, pause_colr2);
    // Draw the side arrows
    draw_kicked_left_arrow(p_box2, pause_colr2);
    draw_kicked_right_arrow(p_box3, pause_colr2);

    if (*p_target) // Draw slider box filling
    {
        struct ScreenBox box4;
        box4.X = p_box1->X + 2;
        box4.Y = p_box1->Y + 2;
        box4.Width = (p_box1->Width - 6) * (*p_target) / 322;
        box4.Height = p_box1->Height - 4;
        draw_slant_box(&box4, colour_lookup[ColLU_WHITE]);
    }
}

ubyte show_pause_screen(struct ScreenBox *box)
{
    if ((box->Flags & GBxFlg_BkgndDrawn) == 0)
    {
        draw_pause_screen_static(box);
        box->Flags |= GBxFlg_BkgndDrawn;
    }

    draw_pause_volume_bar(&samplevol_slider_box, &samplevol_arrow_l_box, &samplevol_arrow_r_box, &startscr_samplevol);
    draw_pause_volume_bar(&midivol_slider_box, &midivol_arrow_l_box, &midivol_arrow_r_box, &startscr_midivol);
    draw_pause_volume_bar(&cdvolume_slider_box, &cdvolume_arrow_l_box, &cdvolume_arrow_r_box, &startscr_cdvolume);

    return 0;
}

TbBool pause_screen_handle(void)
{
    short *affected;
    TbBool resume_game;

    init_pause_screen_boxes();
    start_pause_screen();

    // Wait for the pause key to be released
    wait_for_keypress_end(GKey_PAUSE, true);

    show_pause_screen(&pause_main_box);

    swap_wscreen();
    if (!ingame.fld_unk7DA)
        SetMusicVolume(100, 0x7F);

    resume_game = false;
    while (!resume_game)
    {
        short *target;

        joy_func_065(&joy);
        affected = NULL;

        {
        target = &startscr_samplevol;
        if (input_kicked_left_arrow(&samplevol_arrow_l_box, target))
            affected = target;
        if (input_kicked_right_arrow(&samplevol_arrow_r_box, target))
            affected = target;
        if (input_slant_box(&samplevol_slider_box, target))
            affected = target;
        }

        show_pause_screen(&pause_main_box);

        {
        target = &startscr_midivol;

        if (input_kicked_left_arrow(&midivol_arrow_l_box, target))
            affected = target;
        if (input_kicked_right_arrow(&midivol_arrow_r_box, target))
            affected = target;
        if (input_slant_box(&midivol_slider_box, target))
            affected = target;
        }

        {
        target = &startscr_cdvolume;

        if (input_kicked_left_arrow(&cdvolume_arrow_l_box, target))
            affected = target;
        if (input_kicked_right_arrow(&cdvolume_arrow_r_box, target))
            affected = target;
        if (input_slant_box(&cdvolume_slider_box, target))
            affected = target;
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

        if (update_button(180, 120, gui_strings[477], pause_colr1, pause_colr2, ingame.DetailLevel == 1))
        {
            ingame.DetailLevel = 1;
            bang_set_detail(0);
        }
        if (update_button(220, 120, gui_strings[475], pause_colr1, pause_colr2, ingame.DetailLevel == 0))
        {
            ingame.DetailLevel = 0;
            bang_set_detail(1);
        }

        if (update_button(140, 134, gui_strings[455], pause_colr1, pause_colr2, 0))
            resume_game = true;

        if (update_button(197, 134, gui_strings[445], pause_colr1, pause_colr2, 0))
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
    wait_for_keypress_end(GKey_PAUSE, false);

    lbDisplay.RightButton = 0;
    lbDisplay.LeftButton = 0;
    if (!ingame.fld_unk7DA)
        SetMusicVolume(100, 0);
    return 0;
}

/******************************************************************************/
