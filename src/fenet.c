/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file fenet.c
 *     Front-end desktop and menu system, multiplayer screen.
 * @par Purpose:
 *     Implement functions for multiplayer screen in front-end desktop.
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
#include "fenet.h"

#include "guiboxes.h"
#include "guitext.h"
#include "display.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/

extern struct ScreenButton net_INITIATE_button;
extern struct ScreenButton net_groups_LOGON_button;
extern struct ScreenButton unkn8_EJECT_button;
extern struct ScreenButton net_protocol_select_button;
extern struct ScreenButton net_unkn40_button;
extern struct ScreenButton net_SET2_button;
extern struct ScreenButton net_SET_button;
extern struct ScreenBox net_groups_box;
extern struct ScreenBox net_users_box;
extern struct ScreenBox net_faction_box;
extern struct ScreenBox net_team_box;
extern struct ScreenBox net_benefits_box;
extern struct ScreenBox net_comms_box;
extern struct ScreenBox net_unkn21;
extern struct ScreenBox net_protocol_box;
extern struct ScreenButton net_protocol_option_button;

extern char net_unkn40_text[];
extern char unkn_opt_number_text[];

ubyte ac_do_net_protocol_option(ubyte click);
ubyte ac_do_net_unkn40(ubyte click);
ubyte ac_do_serial_speed_switch(ubyte click);
ubyte ac_do_net_SET2(ubyte click);
ubyte ac_do_net_SET(ubyte click);
ubyte ac_do_net_INITIATE(ubyte click);
ubyte ac_do_net_groups_LOGON(ubyte click);
ubyte ac_do_unkn8_EJECT(ubyte click);
ubyte ac_show_net_benefits_box(struct ScreenBox *box);
ubyte ac_show_net_unkn21(struct ScreenBox *box);
ubyte ac_show_net_comms_box(struct ScreenBox *box);
ubyte ac_do_net_protocol_select(ubyte click);
ubyte ac_show_net_protocol_box(struct ScreenBox *box);
ubyte ac_show_net_faction_box(struct ScreenBox *box);
ubyte ac_show_net_team_box(struct ScreenBox *box);
ubyte ac_show_net_groups_box(struct ScreenBox *box);
ubyte ac_show_net_users_box(struct ScreenBox *box);

ubyte do_net_protocol_option(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_net_protocol_option\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_net_unkn40(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_net_unkn40\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_serial_speed_switch(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_serial_speed_switch\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_net_SET2(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_net_SET2\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_net_SET(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_net_SET\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_net_INITIATE(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_net_INITIATE\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_net_groups_LOGON(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_net_groups_LOGON\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte show_net_benefits_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_benefits_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_net_unkn21(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_unkn21\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_net_comms_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_comms_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte do_net_protocol_select(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_net_protocol_select\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte show_net_protocol_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_protocol_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_net_faction_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_faction_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_net_team_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_team_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_net_groups_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_groups_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_net_users_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_users_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte do_unkn8_EJECT(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_unkn8_EJECT\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

void init_net_screen_boxes(void)
{
    init_screen_button(&net_INITIATE_button, 218u, 185u, gui_strings[385], 6,
        med2_font, 1, 0);
    init_screen_button(&net_groups_LOGON_button, 218u, 206u, gui_strings[386],
        6, med2_font, 1, 0);
    init_screen_button(&unkn8_EJECT_button, 308u, 206u, gui_strings[403], 6,
        med2_font, 1, 0);
    init_screen_button(&net_SET2_button, 562u, 251u, gui_strings[440], 6,
        med2_font, 1, 0);
    init_screen_button(&net_SET_button, 562u, 284u, gui_strings[440], 6,
        med2_font, 1, 0);
    init_screen_button(&net_protocol_select_button, 37u, 256u, gui_strings[498],
        6, med2_font, 1, 0);
    init_screen_button(&net_unkn40_button, 37u, 256u, net_unkn40_text, 6,
        med2_font, 1, 0);
    net_groups_LOGON_button.Width = 85;
    net_INITIATE_button.Width = 85;
    net_unkn40_button.Width = 21;
    net_protocol_select_button.Width = 157;
    init_screen_box(&net_groups_box, 213u, 72u, 171u, 155, 6);
    init_screen_box(&net_users_box, 393u, 72u, 240u, 155, 6);
    init_screen_box(&net_faction_box, 213u, 236u, 73u, 67, 6);
    init_screen_box(&net_team_box, 295u, 236u, 72u, 67, 6);
    init_screen_box(&net_benefits_box, 376u, 236u, 257u, 67, 6);
    init_screen_box(&net_comms_box, 295u, 312u, 336u, 104, 6);
    init_screen_box(&net_unkn21, 7u, 312u, 279u, 104, 6);
    init_screen_box(&net_protocol_box, 7u, 252u, 197u, 51, 6);
    init_screen_button(&net_protocol_option_button, 7u, 275u,
        unkn_opt_number_text, 6, med2_font, 1, 0);
    net_protocol_option_button.Width = net_protocol_select_button.Width;
    net_protocol_select_button.X = ((net_protocol_box.Width
        - net_protocol_select_button.Width) >> 1) + 7;
    net_protocol_option_button.X = ((net_protocol_box.Width
        - net_protocol_select_button.Width) >> 1) + 7;
    net_unkn40_button.X = net_protocol_select_button.Width
        + ((net_protocol_box.Width - net_protocol_select_button.Width) >> 1) + 7
        + 4;
    net_protocol_option_button.CallBackFn = ac_do_net_protocol_option;
    net_INITIATE_button.CallBackFn = ac_do_net_INITIATE;
    net_faction_box.SpecialDrawFn = ac_show_net_faction_box;
    net_team_box.SpecialDrawFn = ac_show_net_team_box;
    net_groups_box.Flags |= 0x0300;

    net_groups_box.SpecialDrawFn = ac_show_net_groups_box;
    net_users_box.SpecialDrawFn = ac_show_net_users_box;
    net_benefits_box.SpecialDrawFn = ac_show_net_benefits_box;
    net_unkn40_button.CallBackFn = ac_do_net_unkn40;
    net_SET_button.CallBackFn = ac_do_net_SET;
    net_protocol_select_button.CallBackFn = ac_do_net_protocol_select;
    unkn8_EJECT_button.CallBackFn = ac_do_unkn8_EJECT;
    net_comms_box.SpecialDrawFn = ac_show_net_comms_box;
    net_users_box.Flags |= 0x0300;
    net_groups_LOGON_button.CallBackFn = ac_do_net_groups_LOGON;
    net_unkn21.SpecialDrawFn = ac_show_net_unkn21;
    net_SET2_button.CallBackFn = ac_do_net_SET2;
    net_protocol_box.SpecialDrawFn = ac_show_net_protocol_box;
}

void reset_net_screen_boxes_flags(void)
{
    net_users_box.Flags = 0x0001;
    net_groups_box.Flags = 0x0001;
    net_benefits_box.Flags = 0x0001;
    net_team_box.Flags = 0x0001;
    net_faction_box.Flags = 0x0001;
    net_comms_box.Flags = 0x0001;
    net_unkn21.Flags = 0x0001;
    net_protocol_box.Flags = 0x0001;
}

void reset_net_screen_EJECT_flags(void)
{
    unkn8_EJECT_button.Flags = 0x0001;
}

void set_flag01_net_screen_boxes(void)
{
    net_SET2_button.Flags |= 0x0001;
    net_SET_button.Flags |= 0x0001;
    net_INITIATE_button.Flags |= 0x0001;
    net_protocol_option_button.Flags |= 0x0001;
    net_protocol_select_button.Flags |= 0x0001;
    net_unkn40_button.Flags |= 0x0001;
    unkn8_EJECT_button.Flags |= 0x0001;
    net_groups_LOGON_button.Flags |= 0x0001;
}

void switch_net_screen_boxes_to_initiate(void)
{
    net_INITIATE_button.Flags = 1;
    net_INITIATE_button.Text = gui_strings[385];
    net_groups_LOGON_button.Text = gui_strings[386];
}

void switch_net_screen_boxes_to_execute(void)
{
    net_INITIATE_button.Text = gui_strings[387];
    if (byte_1C4A6F)
        net_groups_LOGON_button.Text = gui_strings[520];
    else
        net_groups_LOGON_button.Text = gui_strings[388];
}

/******************************************************************************/
