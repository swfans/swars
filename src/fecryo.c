/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file fecryo.c
 *     Front-end desktop and menu system, cryogenic chamber screen.
 * @par Purpose:
 *     Implement functions for cryogenic chamber screen in front-end desktop.
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
#include "fecryo.h"

#include "guiboxes.h"
#include "display.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenTextBox cryo_agent_list_box;
extern struct ScreenButton cryo_offer_cancel_button;

ubyte ac_do_cryo_offer_cancel(ubyte click);
ubyte ac_show_cryo_agent_list(struct ScreenTextBox *box);

ubyte do_cryo_offer_cancel(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_cryo_offer_cancel\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte show_cryo_agent_list(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_cryo_agent_list\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

void init_cryo_screen_boxes(void)
{
    init_screen_text_box(&cryo_agent_list_box, 7u, 122u, 196u, 303, 6,
        small_med_font, 1);
    init_screen_button(&cryo_offer_cancel_button, 628u, 404u,
      gui_strings[437], 6, med2_font, 1, 128);
    cryo_agent_list_box.BGColour = 25;
    cryo_agent_list_box.DrawTextFn = ac_show_cryo_agent_list;
    cryo_offer_cancel_button.CallBackFn = ac_do_cryo_offer_cancel;
    cryo_agent_list_box.ScrollWindowOffset += 27;
    cryo_agent_list_box.Flags |= 0x0300;
    cryo_agent_list_box.ScrollWindowHeight -= 27;
}

void set_flag01_cryo_screen_boxes(void)
{
    cryo_offer_cancel_button.Flags |= 0x0001;
}

void reset_cryo_screen_boxes_flags(void)
{
    cryo_agent_list_box.Flags = 0x0001 | 0x0100 | 0x0200;
}

/******************************************************************************/
