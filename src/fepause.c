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
#include "bfkeybd.h"
#include "bfaudio.h"
#include "bfmusic.h"
#include "bfscd.h"
#include "ssampply.h"
#include "bflib_vidraw.h"
#include "bflib_joyst.h"
#include "campaign.h"
#include "display.h"
#include "game_data.h"
#include "game.h"
#include "guiboxes.h"
#include "guitext.h"
#include "keyboard.h"
#include "mouse.h"
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

TbBool pause_screen_handle(void)
{
#if 0
    TbBool ret;
    asm volatile ("call ASM_pause_screen_handle\n"
        : "=r" (ret) : );
    return ret;
#endif
    int w;
    const char *s;
    int x1, x2, y1;
    int ms_x, ms_y, i;
    TbBool is_unkn1;
    TbPixel colr1, colr2;

    if ((ingame.PanelPermutation != 2) && (ingame.PanelPermutation != -3))
        colr1 = 20;
    else
        colr1 = 40;
    snd_unkn1_volume_all_samples();
    person_func_unknown_310(2u);
    lbKeyOn[kbkeys[GKey_PAUSE]] = 0;
    while ((jskeys[GKey_PAUSE] != 0) &&
      (jskeys[GKey_PAUSE] != joy.Buttons[0]))
    {
        joy_func_065(&joy);
    }
    do_change_mouse(8);

    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    if (lbDisplay.ScreenMode == 1)
        LbDrawBox(43, 27, 208, 97, colr1);
    else
        LbDrawBox(86, 54, 416, 194, colr1);
    if (lbDisplay.ScreenMode == 1)
        LbDrawBox(251, 52, 25, 72, colr1);
    else
        LbDrawBox(502, 104, 50, 144, colr1);
    if (lbDisplay.ScreenMode == 1)
        LbDrawBox(68, 124, 208, 25, colr1);
    else
        LbDrawBox(136, 248, 416, 50, colr1);
    if (lbDisplay.ScreenMode == 1)
    {
        LbDrawTriangle(251, 27, 276, 52, 251, 52, colr1);
        LbDrawTriangle(44, 124, 68, 124, 68, 148, colr1);
    }
    else
    {
        LbDrawTriangle(502, 54, 552, 104, 502, 104, colr1);
        LbDrawTriangle(88, 248, 136, 248, 136, 296, colr1);
    }

    lbDisplay.DrawFlags = 0;
    if (lbDisplay.ScreenMode == 1)
        LbDrawLine(43, 27, 251, 27, colr1);
    else
        LbDrawLine(86, 54, 502, 54, colr1);
    if (lbDisplay.ScreenMode == 1)
        LbDrawLine(43, 27, 43, 123, colr1);
    else
        LbDrawLine(86, 54, 86, 246, colr1);
    if (lbDisplay.ScreenMode == 1)
        LbDrawLine(276, 52, 276, 148, colr1);
    else
        LbDrawLine(552, 104, 552, 296, colr1);
    if (lbDisplay.ScreenMode == 1)
        LbDrawLine(68, 148, 276, 148, colr1);
    else
        LbDrawLine(136, 296, 552, 296, colr1);
    if (lbDisplay.ScreenMode == 1)
    {
        LbDrawLine(251, 27, 276, 52, colr1);
        LbDrawLine(44, 124, 68, 148, colr1);
    }
    else
    {
        LbDrawLine(502, 54, 552, 104, colr1);
        LbDrawLine(88, 248, 136, 296, colr1);
    }

    if ((ingame.PanelPermutation != 2) && (ingame.PanelPermutation != -3))
        colr2 = 15;
    else
        colr2 = 35;
    lbFontPtr = small_font;
    my_set_text_window(0, 0, lbDisplay.PhysicalScreenWidth,
      lbDisplay.PhysicalScreenHeight);
    if ((ingame.PanelPermutation == 2) || (ingame.PanelPermutation == -3))
    {
        lbDisplay.DrawFlags |= 0x0040;
        lbDisplay.DrawColour = colr2;
    }
    if (lbDisplay.ScreenMode == 1)
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
        lbDisplay.DrawFlags &= ~0x0040;

    if (language_3str[0] == 'e')
    {
        if (lbDisplay.ScreenMode == 1)
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
        if (lbDisplay.ScreenMode == 1)
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

    is_unkn1 = 0;
    while (!lbKeyOn[kbkeys[GKey_PAUSE]]
        && (!jskeys[GKey_PAUSE] || jskeys[GKey_PAUSE] != joy.Buttons[0])
        && !is_unkn1)
    {
        game_update();
        joy_func_065(&joy);
        PlayCDTrack(ingame.CDTrack);

        x2 = 442;
        y1 = 122;
        x1 = 204;
        while (y1 < 140)
        {
            if (lbDisplay.ScreenMode == 1)
                LbDrawLine(x1 >> 1, y1 >> 1, x2 >> 1, y1 >> 1, colr2);
            else
                LbDrawLine(x1, y1, x2, y1, colr2);
            y1 += 2;
            x2 -= 2;
            x1 -= 2;
        }

        x2 = 442;
        y1 = 166;
        x1 = 204;
        while (y1 < 184)
        {
            if (lbDisplay.ScreenMode == 1)
                LbDrawLine(x1 >> 1, y1 >> 1, x2 >> 1, y1 >> 1, colr2);
            else
                LbDrawLine(x1, y1, x2, y1, colr2);
            y1 += 2;
            x2 -= 2;
            x1 -= 2;
        }

        x2 = 442;
        y1 = 210;
        x1 = 204;
        while (y1 < 228)
        {
            if (lbDisplay.ScreenMode == 1)
                LbDrawLine(x1 >> 1, y1 >> 1, x2 >> 1, y1 >> 1, colr2);
            else
                LbDrawLine(x1, y1, x2, y1, colr2);
            y1 += 2;
            x2 -= 2;
            x1 -= 2;
        }

        if (lbDisplay.ScreenMode != 1)
        {
            y1 = 123;
            x2 = 441;
            x1 = 203;
            while (y1 < 141)
            {
                if (lbDisplay.ScreenMode == 1)
                    LbDrawLine(x1 >> 1, y1 >> 1, x2 >> 1, y1 >> 1, colr2);
                else
                    LbDrawLine(x1, y1, x2, y1, colr2);
                y1 += 2;
                x2 -= 2;
                x1 -= 2;
            }

            x2 = 441;
            y1 = 167;
            x1 = 203;
            while (y1 < 185)
            {
                if (lbDisplay.ScreenMode == 1)
                    LbDrawLine(x1 >> 1, y1 >> 1, x2 >> 1, y1 >> 1, colr2);
                else
                    LbDrawLine(x1, y1, x2, y1, colr2);
                y1 += 2;
                x2 -= 2;
                x1 -= 2;
            }

            y1 = 211;
            x2 = 441;
            x1 = 203;
            while (y1 < 229)
            {
                if (lbDisplay.ScreenMode == 1)
                    LbDrawLine(x1 >> 1, y1 >> 1, x2 >> 1, y1 >> 1, colr2);
                else
                    LbDrawLine(x1, y1, x2, y1, colr2);
                y1 += 2;
                x2 -= 2;
                x1 -= 2;
            }
        }

        if (lbDisplay.ScreenMode == 1)
            ms_x = 2 * lbDisplay.MMouseX;
        else
            ms_x = lbDisplay.MMouseX;
        if (lbDisplay.ScreenMode == 1)
            ms_y = 2 * lbDisplay.MMouseY;
        else
            ms_y = lbDisplay.MMouseY;
        if ((ms_x >> 1 >= 88) && (ms_x >> 1 <= 92) && (ms_y >> 1 >= 62) && (ms_y >> 1 <= 70))
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDrawOneColour(88, 60, &pop1_sprites[100],
                  colour_lookup[1]);
            }
            else
            {
                LbSpriteDrawOneColour(176, 120, &pop1_sprites[100],
                  colour_lookup[1]);
            }
            if (lbDisplay.MLeftButton)
            {
                lbDisplay.LeftButton = 0;
                --startscr_samplevol;
                if (startscr_samplevol < 0)
                    startscr_samplevol = 0;
                SetSoundMasterVolume(127 * startscr_samplevol / 322);
                if (!IsSamplePlaying(0, 80, 0))
                    play_sample_using_heap(0, 80, 127, 64, 100, 0, 1u);
            }
        }
        else
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDraw(88, 60, &pop1_sprites[100]);
            }
            else
            {
                LbSpriteDraw(176, 120, &pop1_sprites[100]);
            }
        }

        if (lbDisplay.ScreenMode == 1)
            ms_x = 2 * lbDisplay.MMouseX;
        else
            ms_x = lbDisplay.MMouseX;
        if (lbDisplay.ScreenMode == 1)
            ms_y = 2 * lbDisplay.MMouseY;
        else
            ms_y = lbDisplay.MMouseY;
        if ((ms_x >> 1 >= 88) && (ms_x >> 1 <= 92) && (ms_y >> 1 >= 84) && (ms_y >> 1 <= 92))
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDrawOneColour(88, 82,
                  &pop1_sprites[100], colour_lookup[1]);
            }
            else
            {
                LbSpriteDrawOneColour(176, 164,
                  &pop1_sprites[100], colour_lookup[1]);
            }
            if (lbDisplay.MLeftButton)
            {
                lbDisplay.LeftButton = 0;
                --startscr_midivol;
                if (startscr_midivol < 0)
                    startscr_midivol = 0;
                SetMusicMasterVolume(127 * startscr_midivol / 322);
            }
        }
        else
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDraw(88, 82, &pop1_sprites[100]);
            }
            else
            {
                LbSpriteDraw(176, 164, &pop1_sprites[100]);
            }
        }

        if (lbDisplay.ScreenMode == 1)
            ms_x = 2 * lbDisplay.MMouseX;
        else
            ms_x = lbDisplay.MMouseX;
        if (lbDisplay.ScreenMode == 1)
            ms_y = 2 * lbDisplay.MMouseY;
        else
            ms_y = lbDisplay.MMouseY;
        if ((ms_x >> 1 >= 88) && (ms_x >> 1 <= 92) && (ms_y >> 1 >= 106) && (ms_y >> 1 <= 114))
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDrawOneColour(88, 104,
                  &pop1_sprites[100], colour_lookup[1]);
            }
            else
            {
                LbSpriteDrawOneColour(176, 208,
                  &pop1_sprites[100], colour_lookup[1]);
            }
            if (lbDisplay.MLeftButton)
            {
                lbDisplay.LeftButton = 0;
                --startscr_cdvolume;
                if (startscr_cdvolume < 0)
                    startscr_cdvolume = 0;
                SetCDVolume(70 * (127 * startscr_cdvolume / 322) / 100);
            }
        }
        else
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDraw(88, 104, &pop1_sprites[100]);
            }
            else
            {
                LbSpriteDraw(176, 208, &pop1_sprites[100]);
            }
        }

        if (lbDisplay.ScreenMode == 1)
            ms_x = 2 * lbDisplay.MMouseX;
        else
            ms_x = lbDisplay.MMouseX;
        if (lbDisplay.ScreenMode == 1)
            ms_y = 2 * lbDisplay.MMouseY;
        else
            ms_y = lbDisplay.MMouseY;
        if ((ms_x >> 1 >= 222) && (ms_x >> 1 <= 226) && (ms_y >> 1 >= 60) && (ms_y >> 1 <= 68))
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDrawOneColour(215, 60,
                  &pop1_sprites[101], colour_lookup[1]);
            }
            else
            {
                LbSpriteDrawOneColour(430, 120,
                  &pop1_sprites[101], colour_lookup[1]);
            }
            if (lbDisplay.MLeftButton)
            {
                lbDisplay.LeftButton = 0;
                ++startscr_samplevol;
                if (startscr_samplevol > 322)
                    startscr_samplevol = 322;
                SetSoundMasterVolume(127 * startscr_samplevol / 322);
                if (!IsSamplePlaying(0, 80, 0))
                    play_sample_using_heap(0, 80, 127, 64, 100, 0, 1u);
            }
        }
        else
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDraw(215, 60, &pop1_sprites[101]);
            }
            else
            {
                LbSpriteDraw(430, 120, &pop1_sprites[101]);
            }
        }

        if (lbDisplay.ScreenMode == 1)
            ms_x = 2 * lbDisplay.MMouseX;
        else
            ms_x = lbDisplay.MMouseX;
        if (lbDisplay.ScreenMode == 1)
            ms_y = 2 * lbDisplay.MMouseY;
        else
            ms_y = lbDisplay.MMouseY;
        if ((ms_x >> 1 >= 222) && (ms_x >> 1 <= 226) && (ms_y >> 1 >= 82) && (ms_y >> 1 <= 90))
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDrawOneColour(215, 82,
                  &pop1_sprites[101], colour_lookup[1]);
            }
            else
            {
                LbSpriteDrawOneColour(430, 164,
                  &pop1_sprites[101], colour_lookup[1]);
            }
            if (lbDisplay.MLeftButton)
            {
                lbDisplay.LeftButton = 0;
                ++startscr_midivol;
                if (startscr_midivol > 322)
                    startscr_midivol = 322;
                SetMusicMasterVolume(127 * startscr_midivol / 322);
            }
        }
        else
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDraw(215, 82, &pop1_sprites[101]);
            }
            else
            {
                LbSpriteDraw(430, 164, &pop1_sprites[101]);
            }
        }

        if (lbDisplay.ScreenMode == 1)
            ms_x = 2 * lbDisplay.MMouseX;
        else
            ms_x = lbDisplay.MMouseX;
        if (lbDisplay.ScreenMode == 1)
            ms_y = 2 * lbDisplay.MMouseY;
        else
            ms_y = lbDisplay.MMouseY;
        if ((ms_x >> 1 >= 222) && (ms_x >> 1 <= 226) && (ms_y >> 1 >= 104) && (ms_y >> 1 <= 112))
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDrawOneColour(215, 104, &pop1_sprites[101],
                  colour_lookup[1]);
            }
            else
            {
                LbSpriteDrawOneColour(430, 208, &pop1_sprites[101],
                  colour_lookup[1]);
            }
            if (lbDisplay.MLeftButton)
            {
                lbDisplay.LeftButton = 0;
                ++startscr_cdvolume;
                if (startscr_cdvolume > 322)
                    startscr_cdvolume = 322;
                SetCDVolume(70 * (127 * startscr_cdvolume / 322) / 100);
            }
        }
        else
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDraw(215, 104, &pop1_sprites[101]);
            }
            else
            {
                LbSpriteDraw(430, 208, &pop1_sprites[101]);
            }
        }

        if (lbDisplay.MLeftButton)
        {
            ms_x = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
            ms_y = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
            if ((ms_y >> 1 >= 62) && (ms_y >> 1 <= 68))
            {
                int ms_delta;
                ms_delta = (ms_y >> 1) + (ms_x >> 1) - 62;
                if (ms_delta >= 103 && ms_delta <= 219)
                {
                    startscr_samplevol = 322 * (ms_delta - 103) / 116;
                    SetSoundMasterVolume(127 * startscr_samplevol / 322);
                    if (!IsSamplePlaying(0, 80, 0))
                        play_sample_using_heap(0, 80, 127, 64, 100, 0, 1u);
                }
            }
        }

        if (lbDisplay.MLeftButton)
        {
            ms_x = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
            ms_y = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
            if ((ms_y >> 1 >= 84) && (ms_y >> 1 <= 90))
            {
                int ms_delta;
                ms_delta = (ms_y >> 1) + (ms_x >> 1) - 84;
                if (ms_delta >= 103 && ms_delta <= 219)
                {
                    startscr_midivol = 322 * (ms_delta - 103) / 116;
                    SetMusicMasterVolume(127 * startscr_midivol / 322);
                }
            }
        }

        if (lbDisplay.MLeftButton)
        {
            ms_x = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
            ms_y = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
            if ((ms_y >> 1 >= 106) && (ms_y >> 1 <= 112))
            {
                int ms_delta;
                ms_delta = (ms_y >> 1) + (ms_x >> 1) - 106;
                if (ms_delta >= 103 && ms_delta <= 219)
                {
                    startscr_cdvolume = 322 * (ms_delta - 103) / 116;
                    SetCDVolume(70 * (127 * startscr_cdvolume / 322) / 100);
                }
            }
        }

        if (startscr_samplevol)
        {
            int val7;
            x1 = 206;
            y1 = 124;
            for (i = 0; i < 7; i++)
            {
                val7 = 103 - i;
                if (lbDisplay.ScreenMode == 1)
                    LbDrawLine(x1 >> 1, y1 >> 1,
                        (2 * (116 * startscr_samplevol / 322 + val7))
                            >> 1, y1 >> 1, colour_lookup[1]);
                else
                    LbDrawLine(x1, y1,
                        2 * (116 * startscr_samplevol / 322 + val7),
                        y1, colour_lookup[1]);
                x1 -= 2;
                y1 += 2;
            }
            if (lbDisplay.ScreenMode != 1)
            {
                x1 = 205;
                y1 = 125;
                for (i = 0; i < 7; i++)
                {
                    val7 = 103 - i;
                    if (lbDisplay.ScreenMode == 1)
                        LbDrawLine(x1 >> 1, y1 >> 1,
                            (2 * (116 * startscr_samplevol / 322 + val7)
                                - 1) >> 1, y1 >> 1, colour_lookup[1]);
                    else
                        LbDrawLine(x1, y1,
                            2 * (val7 + 116 * startscr_samplevol / 322)
                                - 1, y1, colour_lookup[1]);
                    x1 -= 2;
                    y1 += 2;
                }
            }
        }

        if (startscr_midivol)
        {
            int val7;
            x1 = 206;
            y1 = 168;
            for (i = 0; i < 7; i++)
            {
                val7 = 103 - i;
                if (lbDisplay.ScreenMode == 1)
                    LbDrawLine(x1 >> 1, y1 >> 1,
                        (2 * (116 * startscr_midivol / 322 + val7))
                            >> 1, y1 >> 1, colour_lookup[1]);
                else
                    LbDrawLine(x1, y1,
                        2 * (116 * startscr_midivol / 322 + val7),
                        y1, colour_lookup[1]);
                x1 -= 2;
                y1 += 2;
            }
            if (lbDisplay.ScreenMode != 1)
            {
                x1 = 205;
                y1 = 169;
                for (i = 0; i < 7; i++)
                {
                    val7 = 103 - i;
                    if (lbDisplay.ScreenMode == 1)
                        LbDrawLine(x1 >> 1, y1 >> 1,
                            (2
                                * (116 * startscr_midivol / 322
                                    + val7) - 1) >> 1, y1 >> 1,
                            colour_lookup[1]);
                    else
                        LbDrawLine(x1, y1,
                            2
                                * (116 * startscr_midivol / 322
                                    + val7) - 1, y1, colour_lookup[1]);
                    x1 -= 2;
                    y1 += 2;
                }
            }
        }

        if (startscr_cdvolume)
        {
            int val7;
            x1 = 206;
            y1 = 212;
            for (i = 0; i < 7; i++)
            {
                val7 = 103 - i;
                if (lbDisplay.ScreenMode == 1)
                    LbDrawLine(x1 >> 1, y1 >> 1,
                        (2 * (116 * startscr_cdvolume / 322 + val7))
                            >> 1, y1 >> 1, colour_lookup[1]);
                else
                    LbDrawLine(x1, y1,
                        2 * (116 * startscr_cdvolume / 322 + val7),
                        y1, colour_lookup[1]);
                x1 -= 2;
                y1 += 2;
            }
            if (lbDisplay.ScreenMode != 1)
            {
                x1 = 205;
                y1 = 213;
                for (i = 0; i < 7; i++)
                {
                    val7 = 103 - i;
                    if (lbDisplay.ScreenMode == 1)
                        LbDrawLine(x1 >> 1, y1 >> 1,
                            (2 * (val7 + 116 * startscr_cdvolume / 322)
                                - 1) >> 1, y1 >> 1, colour_lookup[1]);
                    else
                        LbDrawLine(x1, y1,
                            2 * (val7 + 116 * startscr_cdvolume / 322)
                                - 1, y1, colour_lookup[1]);
                    x1 -= 2;
                    y1 += 2;
                }
            }
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
            is_unkn1 = 1;
        if (sub_71694(197, 134, gui_strings[445], colr1, colr2, 0))
        {
            swap_wscreen();
            SetMusicVolume(100, 0);
            StopAllSamples();
            StopCD();
            return 1;
        }
        lbDisplay.DrawFlags = 0;
        process_sound_heap();
        swap_wscreen();
    }

    while ((lbKeyOn[kbkeys[GKey_PAUSE]])
        || (jskeys[GKey_PAUSE] && jskeys[GKey_PAUSE] == joy.Buttons[0]))
    {
        game_update();
        joy_func_065(&joy);
        PlayCDTrack(ingame.CDTrack);
        swap_wscreen();
    }
    lbDisplay.RightButton = 0;
    lbDisplay.LeftButton = 0;
    if (!ingame.fld_unk7DA)
        SetMusicVolume(100, 0);
    return 0;
}

/******************************************************************************/
