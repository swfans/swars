/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file feoptions.c
 *     Front-end desktop and menu system, options screen.
 * @par Purpose:
 *     Implement functions for options screen in front-end desktop.
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
#include "feoptions.h"

#include "guiboxes.h"
#include "display.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/
extern char options_title_text[];
extern struct ScreenButton options_audio_buttons[7];
extern struct ScreenButton options_gfx_buttons[16];

ubyte ac_change_panel_permutation(ubyte click);
ubyte ac_change_trenchcoat_preference(ubyte click);

void init_options_screen_boxes(void)
{
    int i;
    int val;

    init_screen_box(&unkn33_box, 213u, 285u, 420u, 62, 6);
    init_screen_button(&options_audio_buttons[0], 393u, 289u, gui_strings[531], 6,
        med2_font, 1, 0);
    init_screen_button(&options_audio_buttons[1], 458u, 289u, gui_strings[532],
        6, med2_font, 1, 0);
    init_screen_button(&options_audio_buttons[2], 523u, 289u, gui_strings[533],
        6, med2_font, 1, 0);
    init_screen_button(&options_audio_buttons[3], 458u, 307u, gui_strings[531],
        6, med2_font, 1, 0);
    init_screen_button(&options_audio_buttons[4], 523u, 307u, gui_strings[532],
        6, med2_font, 1, 0);
    init_screen_button(&options_audio_buttons[5], 458u, 325u, gui_strings[478],
        6, med2_font, 1, 0);
    init_screen_button(&options_audio_buttons[6], 523u, 325u, gui_strings[479],
        6, med2_font, 1, 0);

    val = 2;
    for (i = 0; i < 3; i++)
    {
        options_audio_buttons[i].Radio = &ingame.CDTrack;
        options_audio_buttons[i].RadioValue = val++;
        options_audio_buttons[i].Flags |= 0x0100;
    }

    val = 1;
    for (i = 3; i < 5; i++)
    {
        options_audio_buttons[i].Radio = &ingame.DangerTrack;
        options_audio_buttons[i].RadioValue = val++;
        options_audio_buttons[i].Flags |= 0x0100;
    }

    val = 0;
    for (i = 5; i < 7; i++)
    {
        options_audio_buttons[i].Radio = &ingame.UseMultiMedia;
        options_audio_buttons[i].RadioValue = val++;
        options_audio_buttons[i].Flags |= 0x0100;
    }

    init_screen_button(&options_gfx_buttons[0], 456u, 94u, gui_strings[465], 6,
        med2_font, 1, 0);
    init_screen_button(&options_gfx_buttons[1], 544u, 94u, gui_strings[466], 6,
        med2_font, 1, 0);
    init_screen_button(&options_gfx_buttons[2], 456u, 112u, gui_strings[473], 6,
        med2_font, 1, 0);
    init_screen_button(&options_gfx_buttons[3], 544u, 112u, gui_strings[474], 6,
        med2_font, 1, 0);
    init_screen_button(&options_gfx_buttons[4], 456u, 130u, gui_strings[475], 6,
        med2_font, 1, 0);
    init_screen_button(&options_gfx_buttons[5], 544u, 130u, gui_strings[477], 6,
        med2_font, 1, 0);
    init_screen_button(&options_gfx_buttons[6], 456u, 148u, gui_strings[478], 6,
        med2_font, 1, 0);
    init_screen_button(&options_gfx_buttons[7], 544u, 148u, gui_strings[479], 6,
        med2_font, 1, 0);
    init_screen_button(&options_gfx_buttons[8], 456u, 166u, gui_strings[478], 6,
        med2_font, 1, 0);
    init_screen_button(&options_gfx_buttons[9], 544u, 166u, gui_strings[479], 6,
        med2_font, 1, 0);
    init_screen_button(&options_gfx_buttons[10], 456u, 184u, gui_strings[478],
        6, med2_font, 1, 0);
    init_screen_button(&options_gfx_buttons[11], 544u, 184u, gui_strings[479],
        6, med2_font, 1, 0);
    init_screen_button(&options_gfx_buttons[12], 456u, 202u, gui_strings[478],
        6, med2_font, 1, 0);
    init_screen_button(&options_gfx_buttons[13], 544u, 202u, gui_strings[479],
        6, med2_font, 1, 0);

    val = abs(ingame.PanelPermutation);
    init_screen_button(&options_gfx_buttons[14], 320u, 274u,
        gui_strings[579 + val], 6, med2_font, 1, 0);

    val = ingame.TrenchcoatPreference;
    init_screen_button(&options_gfx_buttons[15], 320u, 310u,
        gui_strings[583 + val], 6, med2_font, 1, 0);

    options_gfx_buttons[14].CallBackFn = ac_change_panel_permutation;
    options_gfx_buttons[15].CallBackFn = ac_change_trenchcoat_preference;
    options_gfx_buttons[14].Width += 60;
    options_gfx_buttons[15].Width = options_gfx_buttons[14].Width;

    val = 0;
    for (i = 0; i < 2; i++)
    {
        options_gfx_buttons[i].Radio = &game_projector_speed;
        options_gfx_buttons[i].RadioValue = val;
        options_gfx_buttons[i].Flags |= 0x0100;
        val++;
    }

    val = 0;
    for (i = 2; i < 4; i++)
    {
        options_gfx_buttons[i].Radio = &game_high_resolution;
        options_gfx_buttons[i].RadioValue = val;
        options_gfx_buttons[i].Flags |= 0x0100;
        val++;
    }

    val = 0;
    for (i = 4; i < 6; i++)
    {
        options_gfx_buttons[i].Radio = &ingame.DetailLevel;
        options_gfx_buttons[i].RadioValue = val;
        options_gfx_buttons[i].Flags |= 0x0100;
        val++;
    }

    val = 0;
    for (i = 6; i < 8; i++)
    {
        options_gfx_buttons[i].Radio = &game_perspective;
        options_gfx_buttons[i].RadioValue = val;
        options_gfx_buttons[i].Flags |= 0x0100;
        val += 5;
    }

    val = 0;
    for (i = 8; i < 10; i++)
    {
        options_gfx_buttons[i].Radio = &unkn_gfx_option_2;
        options_gfx_buttons[i].RadioValue = val;
        options_gfx_buttons[i].Flags |= 0x0100;
        ingame.Flags |= GamF_Unkn0002;
        val++;
    }

    val = 0;
    for (i = 10; i < 12; i++)
    {
        options_gfx_buttons[i].Radio = &unkn_option_3;
        options_gfx_buttons[i].RadioValue = val;
        options_gfx_buttons[i].Flags |= 0x0100;
        ingame.Flags |= GamF_Unkn0001;
        val++;
    }

    val = 0;
    for (i = 12; i < 14; i++)
    {
        options_gfx_buttons[i].Radio = &unkn_option_4;
        options_gfx_buttons[i].RadioValue = val;
        options_gfx_buttons[i].Flags |= 0x0100;
        ingame.Flags &= ~GamF_Unkn0400;
        val++;
    }
}

void reset_options_screen_boxes_flags(void)
{
    int i;

    for (i = 0; i < 7; i++) {
        options_audio_buttons[i].Flags = 0x0101;
    }
    for (i = 0; i < 14; i++) {
      options_gfx_buttons[i].Flags = 0x0101;
    }
    for (; i < 16; i++) {
      options_gfx_buttons[i].Flags = 0x0001;
    }
}

void update_options_screen_state(void)
{
    const char *text;
    int i;

    i = ingame.PanelPermutation;
    if (i < 0)
        text = gui_strings[579 + abs(i)];
    else
        text = gui_strings[580 + i];
    options_gfx_buttons[14].Text = text;

    i = ingame.TrenchcoatPreference;
    options_gfx_buttons[15].Text = gui_strings[583 + i];
}

/******************************************************************************/
