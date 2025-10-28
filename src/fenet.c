/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
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

#include "bfkeybd.h"
#include "bfmemut.h"
#include "bfscrcopy.h"
#include "bfsprite.h"
#include "bfstrut.h"
#include "bftext.h"
#include "bfutility.h"

#include "campaign.h"
#include "guiboxes.h"
#include "guitext.h"
#include "display.h"
#include "femain.h"
#include "feshared.h"
#include "game_save.h"
#include "game_sprts.h"
#include "game.h"
#include "network.h"
#include "purpldrw.h"
#include "purpldrwlst.h"
#include "player.h"
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
extern struct ScreenBox net_grpaint;
extern struct ScreenBox net_protocol_box;
extern struct ScreenButton net_protocol_option_button;

extern char net_unkn40_text[];
extern char net_baudrate_text[8];
extern char net_proto_param_text[8];
extern ubyte byte_155174; // = 166;
extern ubyte byte_155175[];
extern ubyte byte_155180; // = 109;
extern ubyte byte_155181[];
extern struct TbSprite *fe_icons_sprites;
extern int unkn_rate; // = 19200;

ubyte ac_do_net_protocol_option(ubyte click);
ubyte ac_do_net_unkn40(ubyte click);
ubyte ac_do_serial_speed_switch(ubyte click);
ubyte ac_do_net_SET2(ubyte click);
ubyte ac_do_net_SET(ubyte click);
ubyte ac_do_net_INITIATE(ubyte click);
ubyte ac_do_net_groups_LOGON(ubyte click);
ubyte ac_do_unkn8_EJECT(ubyte click);
ubyte ac_show_net_benefits_box(struct ScreenBox *box);
ubyte ac_show_net_grpaint(struct ScreenBox *box);
ubyte ac_show_net_comms_box(struct ScreenBox *box);
ubyte ac_do_net_protocol_select(ubyte click);
ubyte ac_show_net_protocol_box(struct ScreenBox *box);


void net_service_unkstruct04_clear(void)
{
    int i;

    LbMemorySet(unkstruct04_arr, 0, sizeof(unkstruct04_arr));
    byte_1C6D48 = 0;
    for (i = 0; i < 8; i++) {
        unkn2_names[i][0] = '\0';
    }
}

void net_service_gui_switch(void)
{
    const char *text;

    switch (nsvc.I.Type)
    {
    case NetSvc_IPX:
        net_protocol_option_button.Text = net_proto_param_text;
        net_protocol_option_button.CallBackFn = ac_do_net_protocol_option;
        text = gui_strings[497 + nsvc.I.Type];
        net_protocol_select_button.Text = text;
        net_service_unkstruct04_clear();
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        net_protocol_option_button.Text = net_baudrate_text;
        net_protocol_option_button.CallBackFn = ac_do_serial_speed_switch;
        if (byte_1C4A6F)
            text = gui_strings[522 + nsvc.I.Type];
        else
            text = gui_strings[497 + nsvc.I.Type];
        net_protocol_select_button.Text = text;
        break;
    }
}

ubyte do_net_protocol_option(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_net_protocol_option\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    short param, dt;

    if (byte_1C4A7C)
    {
        LbNetworkReset();
        byte_1C4A7C = 0;
    }
    dt = 0x01;
    if ((lbShift & KMod_SHIFT) != 0)
        dt = 0x10;
    if ((lbShift & KMod_CONTROL) != 0)
        dt = 0x100;

    param = nsvc.I.Param;
    if (click)
    {
        param -= dt;
        if (param < 0)
            param = 2746;
    }
    else
    {
        param += dt;
        if (param > 2746)
            param = 0;
    }
    nsvc.I.Param = param;

    sprintf(net_proto_param_text, "%04x", (int)nsvc.I.Param);
    LbStringToUpper(net_proto_param_text);

    net_service_unkstruct04_clear();

    if (LbNetworkServiceStart(&nsvc.I) != Lb_SUCCESS)
    {
        LOGERR("Failed on LbNetworkServiceStart");
        nsvc.I.Type = NetSvc_COM1;
        net_service_gui_switch();
        alert_box_text_fmt("%s", gui_strings[568]);
        return 1;
    }

    byte_1C4A7C = 1;
    return 1;
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

ubyte net_unkn_func_32(void)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_net_unkn_func_32\n"
        : "=r" (ret) : );
    return ret;
#else
    int i, ret;
    TbBool modem_on_line;

    modem_on_line = 0;
    if (LbNetworkSessionActive())
        return 0;

    if (nsvc.I.Type == NetSvc_IPX)
        goto skip_modem_init;

    net_service_unkstruct04_clear();

    if (LbNetworkServiceStart(&nsvc.I) != Lb_SUCCESS)
    {
        LOGERR("Failed on LbNetworkServiceStart");
        alert_box_text_fmt("%s", gui_strings[568]);
        goto out_fail;
    }

    LbNetworkSetBaud(unkn_rate);
    players[local_player_no].DoubleMode = 0;
    byte_1C4A7C = 1;
    if (!byte_1C4A6F)
        goto skip_modem_init;

    if (LbNetworkInit() != Lb_SUCCESS) {
        LOGERR("Failed on LbNetworkInit");
        alert_box_text_fmt("%s", gui_strings[579]);
        goto out_fail;
    }

    ret = LbNetworkDial(net_unkn2_text);
    if (ret == -7 || ret == -1) {
        LOGERR("Failed on LbNetworkAnswer, ret=%d", ret);
        alert_box_text_fmt("%s", gui_strings[579]);
        modem_on_line = true;
        goto out_fail;
    }
    if (ret == 7) {
        LOGERR("Failed on LbNetworkAnswer, ret=%d", ret);
        alert_box_text_fmt("%s", gui_strings[579]);
        goto out_fail;
    }
    if (ret == 4 || ret == 5 || ret == 6) {
        LOGERR("Failed on LbNetworkAnswer, ret=%d", ret);
        alert_box_text_fmt("%s", gui_strings[579]);
        modem_on_line = true;
        goto out_fail;
    }
    if (ret == 1) {
        modem_on_line = true;
    }

skip_modem_init:
    LbMemoryCopy(nsvc.S.Name, login_name, min(sizeof(nsvc.S.Name),sizeof(login_name)));
    nsvc.S.MaxPlayers = 8;
    nsvc.S.HostPlayerNumber = 0;
    if (LbNetworkSessionCreate(&nsvc.S, nsvc.S.Name) != Lb_SUCCESS)
    {
        LOGERR("Failed on LbNetworkSessionCreate");
        alert_box_text_fmt("%s", gui_strings[579]);
        goto out_fail;
    }
    login_control__State = 5;
    net_host_player_no = LbNetworkHostPlayerNumber();
    net_players_num = LbNetworkSessionNumberPlayers();
    byte_15516C = -1;
    byte_15516D = -1;
    if (nsvc.I.Type != NetSvc_IPX)
        players[local_player_no].DoubleMode = 0;
    load_missions(99);
    for (i = 0; i < 8; i++) {
        network_players[i].Type = 17;
    }
    return 1;

out_fail:
    if (modem_on_line)
        LbNetworkHangUp();
    if (nsvc.I.Type == NetSvc_IPX)
    {
        if (!byte_1C4A7C) {
            nsvc.I.Type = NetSvc_COM1;
            net_service_gui_switch();
        }
    }
    else
    {
        if (byte_1C4A7C) {
            LbNetworkReset();
        }
        byte_1C4A7C = 0;
    }
    return 0;
#endif
}

ubyte net_unkn_func_31(struct TbNetworkSession *p_nsession)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_net_unkn_func_31\n"
        : "=r" (ret) : "a" (p_nsession));
    return ret;
#else
    TbBool modem_on_line;
    int i, ret;

    modem_on_line = false;
    if (nsvc.I.Type == NetSvc_IPX)
      goto skip_modem_init;

    net_service_unkstruct04_clear();

    if (LbNetworkServiceStart(&nsvc.I) != Lb_SUCCESS)
    {
        LOGERR("Failed on LbNetworkServiceStart");
        alert_box_text_fmt("%s", gui_strings[568]);
        goto out_fail;
    }

    LbNetworkSetBaud(unkn_rate);
    byte_1C4A7C = 1;
    players[local_player_no].DoubleMode = 0;
    if (!byte_1C4A6F)
        goto skip_modem_init;

    if (LbNetworkInit() != Lb_SUCCESS) {
        LOGERR("Failed on LbNetworkInit");
        alert_box_text_fmt("%s", gui_strings[579]);
        goto out_fail;
    }

    ret = LbNetworkAnswer();
    if (ret == -7 || ret == -1) {
        LOGERR("Failed on LbNetworkAnswer, ret=%d", ret);
        alert_box_text_fmt("%s", gui_strings[579]);
        modem_on_line = true;
        goto out_fail;
    }
    if (ret == 7) {
        LOGERR("Failed on LbNetworkAnswer, ret=%d", ret);
        alert_box_text_fmt("%s", gui_strings[579]);
        goto out_fail;
    }
    if (ret == 4 || ret == 5 || ret == 6) {
        LOGERR("Failed on LbNetworkAnswer, ret=%d", ret);
        alert_box_text_fmt("%s", gui_strings[579]);
        modem_on_line = true;
        goto out_fail;
    }
    if (ret == 1) {
      modem_on_line = true;
    }

skip_modem_init:
    p_nsession->MaxPlayers = 8;
    if (LbNetworkSessionJoin(p_nsession, login_name) != Lb_SUCCESS)
    {
        LOGERR("Failed on LbNetworkSessionJoin");
        alert_box_text_fmt("%s", gui_strings[579]);
        goto out_fail;
    }
    login_control__State = 5;
    net_host_player_no = LbNetworkHostPlayerNumber();
    net_players_num = LbNetworkSessionNumberPlayers();
    byte_1C6D4A = 1;
    LbMemoryCopy(&nsvc.F, p_nsession, 0x28u);
    if (nsvc.I.Type != NetSvc_IPX) {
        players[local_player_no].DoubleMode = 0;
    }
    load_missions(99);

    for (i = 0; i < 8; i++) {
        network_players[i].Type = 17;
    }

    return 1;

out_fail:
    if (modem_on_line)
        LbNetworkHangUp();
    if (nsvc.I.Type == NetSvc_IPX)
    {
        if (!byte_1C4A7C) {
            nsvc.I.Type = NetSvc_COM1;
            net_service_gui_switch();
        }
    }
    else
    {
        if (byte_1C4A7C)
            LbNetworkReset();
        byte_1C4A7C = 0;
    }
    return 0;
#endif
}

void netgame_state_enter_5(void)
{
    const char *text;
    PlayerIdx plyr;

    net_INITIATE_button.Text = gui_strings[387];
    if (byte_1C4A6F)
        text = gui_strings[520];
    else
        text = gui_strings[388];
    net_groups_LOGON_button.Text = text;
    init_variables();
    init_agents();
    srm_reset_research();
    login_control__State = 5;
    for (plyr = 0; plyr < 8; plyr++) {
        player_mission_agents_reset(plyr);
    }
}


ubyte do_net_INITIATE(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_net_INITIATE\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    if (nsvc.I.Type == NetSvc_IPX && !byte_1C4A7C) {
        LOGWARN("Cannot init protocol %d - not ready", (int)nsvc.I.Type);
        return 0;
    }
    if (login_control__State == 6)
    {
        if (net_unkn_func_32())
        {
          netgame_state_enter_5();
      }
    }
    else if (login_control__State == 5)
    {
        int plyr;
        plyr = LbNetworkPlayerNumber();
        if (plyr == net_host_player_no)
        {
            if (login_control__City == -1) {
                LOGWARN("Cannot init protocol %d player %d - city not selected", (int)nsvc.I.Type, plyr);
                return 0;
            }
            byte_15516D = -1;
            byte_15516C = -1;
            network_players[plyr].Type = 2;
        }
    }
    return 1;
}

ubyte do_net_groups_LOGON(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_net_groups_LOGON\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    int plyr;

    if (nsvc.I.Type == NetSvc_IPX && !byte_1C4A7C) {
        LOGWARN("Cannot abort protocol %d - not ready", (int)nsvc.I.Type);
        return 0;
    }
      if (login_control__State == 5)
      {
        plyr = LbNetworkPlayerNumber();
        network_players[plyr].Type = 13;
        byte_15516D = -1;
        net_INITIATE_button.Text = gui_strings[385];
        byte_15516C = -1;
        net_groups_LOGON_button.Text = gui_strings[386];
        net_unkn_func_33();
      }
    else if (login_control__State == 6)
    {
        if (byte_15516C != -1 || nsvc.I.Type != NetSvc_IPX)
        {
            if (net_unkn_func_31(&unkstruct04_arr[byte_15516C].Session))
            {
                netgame_state_enter_5();
            }
        }
    }
    return 1;
}

TbBool mouse_down_over_unkn1(short x, short y, short width, short height)
{
    short ms_x, ms_y;

    ms_y = (lbDisplay.GraphicsScreenHeight < 400) ? 2 * lbDisplay.MouseY : lbDisplay.MouseY;
    ms_x = (lbDisplay.GraphicsScreenHeight < 400) ? 2 * lbDisplay.MouseX : lbDisplay.MouseX;
    if ((ms_y >= y) && (ms_y <= y + height))
    {
        short sum_min, sum_max;

        sum_min = x + y + height;
        sum_max = x + y + width;
        return (ms_x + ms_y >= sum_min) && (ms_x + ms_y <= sum_max);
    }
    return false;
}

void show_net_benefits_sub1(short x0, short y0, TbPixel colour)
{
    ushort spridx;
    short dx, dy;
    short i;

    dx = x0;
    dy = y0;
    spridx = 114;
    for (i = 0; i < 8; i++)
    {
        struct TbSprite *p_sprite;
        short delta;

        p_sprite = &fe_icons_sprites[spridx];
        lbDisplay.DrawColour = colour;
        draw_sprite_purple_list(dx, dy, p_sprite);
        if (i < 2)
            delta = p_sprite->SWidth - 11;
        else if (i < 5)
            delta = p_sprite->SWidth - 25;
        else
            delta = p_sprite->SWidth - 5;
        dx += delta;
        if (i == 0)
            delta = 2;
        else if (i == 1)
            delta = -14;
        else
            delta = 0;
        dy += delta;
        spridx++;
    }
}

void show_net_benefits_sub2(short x0, short y0, TbPixel *colours)
{
    ushort spridx;
    short dx, dy;
    short i;

    dx = x0;
    dy = y0;
    spridx = 114;
    for (i = 0; i < 8; i++)
    {
        struct TbSprite *p_sprite;
        short delta;

        p_sprite = &fe_icons_sprites[spridx];
        if (i < login_control__TechLevel)
        {
            lbDisplay.DrawFlags = Lb_TEXT_ONE_COLOR;
            lbDisplay.DrawColour = colours[i];
            draw_sprite_purple_list(dx, dy, p_sprite);
            lbDisplay.DrawFlags = 0;
        }
        if (mouse_down_over_unkn1(dx, dy, p_sprite->SWidth, p_sprite->SHeight))
        {
            if (lbDisplay.LeftButton)
            {
                lbDisplay.LeftButton = 0;
                if (is_unkn_current_player() && ((unkn_flags_08 & 0x02) == 0)
                  && (login_control__State == 5))
                    login_control__TechLevel = i + 1;
            }
        }
        if (i < 2)
            delta = p_sprite->SWidth - 11;
        else if (i < 5)
            delta = p_sprite->SWidth - 25;
        else
            delta = p_sprite->SWidth - 5;
        dx += delta;
        if (i == 0)
            delta = 2;
        else if (i == 1)
            delta = -14;
        else
            delta = 0;
        dy += delta;
        spridx++;
    }
}

void show_net_benefits_sub3(struct ScreenBox *box)
{
    struct ScreenBoxBase box1 = {box->X + 5, box->Y + 16, 9, 14};

    lbDisplay.DrawFlags = (0x8000|Lb_SPRITE_TRANSPAR4);
    if (mouse_move_over_box(&box1))
    {
        if (lbDisplay.LeftButton)
        {
            if (mouse_down_over_box(&box1))
            {
                lbDisplay.LeftButton = 0;
                if (is_unkn_current_player() && ((unkn_flags_08 & 0x02) == 0)
                  && (login_control__State == 5))
                {
                    login_control__TechLevel--;
                    if (login_control__TechLevel < 1)
                        login_control__TechLevel = 1;
                }
            }
        }
        lbDisplay.DrawFlags &= ~Lb_SPRITE_TRANSPAR4;
    }
    draw_sprite_purple_list(box1.X, box1.Y, &fe_icons_sprites[108]);
}

void show_net_benefits_sub4(struct ScreenBox *box)
{
    struct ScreenBoxBase box2 = {box->X + 242, box->Y + 5, 9, 14};

    lbDisplay.DrawFlags = (0x8000|Lb_SPRITE_TRANSPAR4);
    if (mouse_move_over_box(&box2))
    {
        if (lbDisplay.LeftButton)
        {
            if (mouse_down_over_box(&box2))
            {
                lbDisplay.LeftButton = 0;
                if (is_unkn_current_player() && ((unkn_flags_08 & 0x02) == 0)
                    && (login_control__State == 5))
                {
                    login_control__TechLevel++;
                    if (login_control__TechLevel > 8)
                        login_control__TechLevel = 8;
                }
            }
        }
        lbDisplay.DrawFlags &= ~Lb_SPRITE_TRANSPAR4;
    }
    draw_sprite_purple_list(box2.X - 7, box2.Y, &fe_icons_sprites[109]);
}

ubyte get_current_starting_cash_level(void)
{
    int i, lv_curr;

    lv_curr = 0;
    for (i = 0; i < 8; i++)
    {
      if (login_control__Money == starting_cash_amounts[i])
          break;
      lv_curr++;
    }
    return lv_curr;
}

uint reinit_starting_credits(sbyte change)
{
#if 0
    ulong ret;
    asm volatile ("call ASM_reinit_starting_credits\n"
        : "=r" (ret) : "a" (change));
    return ret;
#endif
  int lv, lv_curr;
  uint creds;

    lv_curr = get_current_starting_cash_level();
    lv = lv_curr + change;
    if (lv < 0)
        lv = 0;
    if (lv > 7)
        lv = 7;

    creds = starting_cash_amounts[lv];
    login_control__Money = creds;
    ingame.Credits = creds;
    ingame.CashAtStart = creds;
    ingame.Expenditure = 0;

    return creds;
}

void show_net_benefits_sub5(short x0, short y0, TbPixel *colours)
{
    ushort spridx;
    short dx, dy;
    short i;

    dx = x0;
    dy = y0;
    spridx = 114;
    for (i = 0; i < 8; i++)
    {
        struct TbSprite *p_sprite;
        short delta;

        p_sprite = &fe_icons_sprites[spridx];
        if (login_control__Money >= starting_cash_amounts[i])
        {
            lbDisplay.DrawFlags = Lb_TEXT_ONE_COLOR;
            lbDisplay.DrawColour = colours[i];
            draw_sprite_purple_list(dx, dy, p_sprite);
            lbDisplay.DrawFlags = 0;
        }
        if (mouse_down_over_unkn1(dx, dy, p_sprite->SWidth, p_sprite->SHeight))
        {
            if (lbDisplay.LeftButton)
            {
                lbDisplay.LeftButton = 0;
                if (is_unkn_current_player() && ((unkn_flags_08 & 0x01) == 0)
                  && (login_control__State == 5))
                {
                    login_control__Money = starting_cash_amounts[i];
                    ingame.Credits = login_control__Money;
                }
            }
        }
        if (i < 2)
            delta = p_sprite->SWidth - 11;
        else if (i < 5)
            delta = p_sprite->SWidth - 25;
        else
            delta = p_sprite->SWidth - 5;
        dx += delta;
        if (i == 0)
            delta = 2;
        else if (i == 1)
            delta = -14;
        else
            delta = 0;
        dy += delta;
        spridx++;
    }
}

void show_net_benefits_sub6(struct ScreenBox *box)
{
    struct ScreenBoxBase box1 = {box->X + 5, box->Y + 47, 9, 14};

    lbDisplay.DrawFlags = (0x8000|Lb_SPRITE_TRANSPAR4);
    if (mouse_move_over_box(&box1))
    {
        if (lbDisplay.LeftButton)
        {
            if (mouse_down_over_box(&box1))
            {
                lbDisplay.LeftButton = 0;
                if (is_unkn_current_player() && ((unkn_flags_08 & 0x01) == 0)
                    && (login_control__State == 5))
                {
                    reinit_starting_credits(-1);
                }
            }
        }
        lbDisplay.DrawFlags &= ~Lb_SPRITE_TRANSPAR4;
    }
    lbDisplay.DrawFlags |= Lb_TEXT_ONE_COLOR;
    lbDisplay.DrawColour = 87;
    draw_sprite_purple_list(box1.X, box1.Y, &fe_icons_sprites[108]);
}

void show_net_benefits_sub7(struct ScreenBox *box)
{
    struct ScreenBoxBase box2 = {box->X + 242, box->Y + 36, 9, 14};

    lbDisplay.DrawFlags = (0x8000|Lb_SPRITE_TRANSPAR4);
    if (mouse_move_over_box(&box2))
    {
        if (lbDisplay.LeftButton)
        {
            if (mouse_down_over_box(&box2))
            {
                lbDisplay.LeftButton = 0;
                if (is_unkn_current_player() && ((unkn_flags_08 & 0x01) == 0)
                    && (login_control__State == 5))
                {
                    reinit_starting_credits(1);
                }
            }
        }
        lbDisplay.DrawFlags &= ~Lb_SPRITE_TRANSPAR4;
    }
    lbDisplay.DrawFlags |= Lb_TEXT_ONE_COLOR;
    lbDisplay.DrawColour = 87;
    draw_sprite_purple_list(box2.X - 7, box2.Y, &fe_icons_sprites[109]);
}


ubyte show_net_benefits_box(struct ScreenBox *box)
{
    ubyte drawn = true;

    my_set_text_window(box->X + 4, box->Y + 4, box->Width - 8, box->Height - 8);
    if ((box->Flags & GBxFlg_TextCopied) == 0)
    {
        lbFontPtr = med2_font;
        draw_text_purple_list2(30, 1, gui_strings[401], 0);
        draw_text_purple_list2(27, 32, gui_strings[402], 0);
        lbDisplay.DrawFlags = Lb_TEXT_ONE_COLOR;
        show_net_benefits_sub1(box->X + 8, box->Y + 16, byte_155174);
        show_net_benefits_sub1(box->X + 8, box->Y + 47, byte_155180);
        lbDisplay.DrawFlags = 0;
        copy_box_purple_list(box->X - 3, box->Y - 3,
            box->Width + 6, box->Height + 6);
        box->Flags |= GBxFlg_TextCopied;
    }

    show_net_benefits_sub2(box->X + 8, box->Y + 16, byte_155175);
    show_net_benefits_sub3(box);
    show_net_benefits_sub4(box);

    lbDisplay.DrawFlags = 0;

    show_net_benefits_sub5(box->X + 8, box->Y + 47, byte_155181);
    show_net_benefits_sub6(box);
    show_net_benefits_sub7(box);

    lbDisplay.DrawFlags = 0;
    if (is_unkn_current_player() && (login_control__State == 5))
    {
        //net_SET2_button.DrawFn(&net_SET2_button); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&net_SET2_button), "g" (net_SET2_button.DrawFn));
        //net_SET_button.DrawFn(&net_SET_button); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&net_SET_button), "g" (net_SET_button.DrawFn));
    }
    return drawn;
}

void purple_unkn3_data_to_screen(void)
{
    LbScreenSetGraphicsWindow(net_grpaint.X + 4, net_grpaint.Y + 4,
      255, 96);
    LbScreenCopy(dword_1C6DE8, lbDisplay.GraphicsWindowPtr, lbDisplay.GraphicsWindowHeight);
    LbScreenSetGraphicsWindow(0, 0, lbDisplay.GraphicsScreenWidth,
        lbDisplay.GraphicsScreenHeight);
}

void purple_unkn4_data_to_screen(void)
{
    LbScreenSetGraphicsWindow(net_grpaint.X + 4, net_grpaint.Y + 4,
      255, 96);
    LbScreenCopy(dword_1C6DE4, lbDisplay.GraphicsWindowPtr, lbDisplay.GraphicsWindowHeight);
    LbScreenSetGraphicsWindow(0, 0, lbDisplay.GraphicsScreenWidth,
        lbDisplay.GraphicsScreenHeight);
}

ubyte show_net_grpaint(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_net_grpaint\n"
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
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_net_protocol_select\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    short proto;
    short pos_x;

    if (byte_1C4A7C)
    {
      LbNetworkReset();
      byte_1C4A7C = 0;
    }

    pos_x = net_protocol_select_button.X - 12 + net_protocol_select_button.Width + 4;

    proto = nsvc.I.Type;
    if (click)
    {
        proto--;
        if (proto <= NetSvc_NONE)
        {
            proto = NetSvc_COM4;
            if (data_1c4a70)
            {
                net_protocol_select_button.X -= 12;
                net_unkn40_button.X = pos_x;
                byte_1C4A6F = 1;
            }
        }
        else if (proto == NetSvc_IPX) // IPX needs to be accepted
        {
            if (byte_1C4A6F)
            {
                byte_1C4A6F = 0;
                proto = NetSvc_COM4;
                net_protocol_select_button.X += 12;
            }
        }
        nsvc.I.Type = proto;
    }
    else
    {
        proto++;
        if (proto > NetSvc_COM4)
        {
            if (byte_1C4A6F || !data_1c4a70)
            {
                proto = NetSvc_IPX;
                if (byte_1C4A6F)
                {
                    byte_1C4A6F = 0;
                    net_protocol_select_button.X += 12;
                }
            }
            else
            {
                proto = NetSvc_COM1;
                net_protocol_select_button.X -= 12;
                net_unkn40_button.X = pos_x;
                byte_1C4A6F = 1;
            }
        }
        nsvc.I.Type = proto;
    }

    switch (nsvc.I.Type)
    {
    case NetSvc_NONE:
    default:
        break;
    case NetSvc_IPX:
        net_service_gui_switch();

        if (LbNetworkServiceStart(&nsvc.I) != Lb_SUCCESS)
        {
            LOGERR("Failed on LbNetworkServiceStart");
            nsvc.I.Type = NetSvc_COM1;
            net_service_gui_switch();
            alert_box_text_fmt("%s", gui_strings[568]);
            break;
        }
        byte_1C4A7C = 1;
        byte_15516C = -1;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        net_service_gui_switch();
        byte_15516C = 0;
        break;
    }
    return 1;
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
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_unkn8_EJECT\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    int plyr;

    plyr = LbNetworkPlayerNumber();
    if (byte_15516D == plyr)
        return 0;
    network_players[plyr].Type = 12;
    return 1;
}

void show_netgame_unkn_case1(void)
{
    asm volatile (
      "call ASM_show_netgame_unkn_case1\n"
        :  :  : "eax" );
}

void init_net_screen_boxes(void)
{
    ScrCoord scr_w, scr_h, start_x, start_y;
    short space_w, space_h, border;

    // Border value represents how much the box background goes
    // out of the box area.
    border = 3;
    scr_w = lbDisplay.GraphicsWindowWidth;
#ifdef EXPERIMENTAL_MENU_CENTER_H
    scr_h = global_apps_bar_box.Y;
#else
    scr_h = 432;
#endif

    init_screen_box(&net_groups_box, 213u, 72u, 171u, 155, 6);
    init_screen_box(&net_users_box, 393u, 72u, 240u, 155, 6);

    init_screen_box(&net_faction_box, 213u, 236u, 73u, 67, 6);
    init_screen_box(&net_team_box, 295u, 236u, 72u, 67, 6);
    init_screen_box(&net_benefits_box, 376u, 236u, 257u, 67, 6);
    init_screen_box(&net_protocol_box, 7u, 252u, 197u, 51, 6);

    init_screen_box(&net_grpaint, 7u, 312u, 279u, 104, 6);
    init_screen_box(&net_comms_box, 295u, 312u, 336u, 104, 6);

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
    init_screen_button(&net_protocol_option_button, 7u, 275u,
        net_proto_param_text, 6, med2_font, 1, 0);

    net_groups_LOGON_button.Width = 85;
    net_INITIATE_button.Width = 85;
    net_unkn40_button.Width = 21;
    net_protocol_select_button.Width = 157;
    net_protocol_option_button.Width = net_protocol_select_button.Width;
    net_protocol_option_button.CallBackFn = ac_do_net_protocol_option;
    net_INITIATE_button.CallBackFn = ac_do_net_INITIATE;
    net_faction_box.SpecialDrawFn = show_net_faction_box;
    net_team_box.SpecialDrawFn = show_net_team_box;
    net_groups_box.Flags |= GBxFlg_RadioBtn|GBxFlg_IsMouseOver;

    net_groups_box.SpecialDrawFn = show_net_groups_box;
    net_users_box.SpecialDrawFn = show_net_users_box;
    net_benefits_box.SpecialDrawFn = show_net_benefits_box;
    net_unkn40_button.CallBackFn = ac_do_net_unkn40;
    net_SET_button.CallBackFn = ac_do_net_SET;
    net_protocol_select_button.CallBackFn = ac_do_net_protocol_select;
    unkn8_EJECT_button.CallBackFn = ac_do_unkn8_EJECT;
    net_comms_box.SpecialDrawFn = show_net_comms_box;
    net_users_box.Flags |= GBxFlg_RadioBtn|GBxFlg_IsMouseOver;
    net_groups_LOGON_button.CallBackFn = ac_do_net_groups_LOGON;
    net_grpaint.SpecialDrawFn = show_net_grpaint;
    net_SET2_button.CallBackFn = ac_do_net_SET2;
    net_protocol_box.SpecialDrawFn = show_net_protocol_box;

    // Reposition the components to current resolution

    start_x = unkn13_SYSTEM_button.X + unkn13_SYSTEM_button.Width;
    // On the X axis, we're going for centering on the screen. So subtract the previous
    // button position two times - once for the left, and once to make the same space on
    // the right.
    space_w = scr_w - start_x - unkn13_SYSTEM_button.X - net_groups_box.Width - net_users_box.Width;

    start_y = system_screen_shared_header_box.Y + system_screen_shared_header_box.Height;
    // On the top, we're aligning to spilled border of previous box; same goes inside.
    // But on the bottom, we're aligning to hard border, without spilling. To compensate
    // for that, add pixels for such border to the space.
    space_h = scr_h - start_y - net_users_box.Height - net_benefits_box.Height - net_comms_box.Height + border;

    // There are 2 boxes to position in X axis, and no space is needed after - the
    // available empty space is divided into 2.
    net_groups_box.X = start_x + space_w / 2;
    // There are 3 boxes to position in Y axis, so space goes into 4 parts - before, between and after.
    net_groups_box.Y = start_y + space_h / 4;
    net_users_box.X = net_groups_box.X + net_groups_box.Width + space_w - space_w / 2;
    net_users_box.Y = net_groups_box.Y;

    // Next row - re-compute space in one dimension
    space_w = scr_w - start_x - unkn13_SYSTEM_button.X - net_faction_box.Width - net_team_box.Width - net_benefits_box.Width;

    net_faction_box.X = start_x + space_w / 3;
    net_faction_box.Y = net_groups_box.Y + net_groups_box.Height + space_h / 4;
    net_team_box.X = net_faction_box.X + net_faction_box.Width  + space_w / 3;
    net_team_box.Y = net_faction_box.Y;
    net_benefits_box.X = net_team_box.X + net_team_box.Width + space_w - 2 * (space_w / 3);
    net_benefits_box.Y = net_faction_box.Y;

    // The remaining components are positioned below the system menu
    start_x = unkn13_SYSTEM_button.X;

    net_protocol_box.X = start_x;
    net_protocol_box.Y = net_faction_box.Y + net_faction_box.Height - net_protocol_box.Height;

    // Next row - re-compute space in one dimension
    space_w = scr_w - start_x - unkn13_SYSTEM_button.X - net_grpaint.Width - net_comms_box.Width;

    net_grpaint.X = start_x;
    net_grpaint.Y = net_benefits_box.Y + net_benefits_box.Height + space_h / 4;
    net_comms_box.X = net_grpaint.X + net_grpaint.Width + space_w;
    net_comms_box.Y = net_grpaint.Y;

    // Two buttons on top of each other
    net_protocol_select_button.X = net_protocol_box.X +
      (net_protocol_box.Width - net_protocol_select_button.Width) / 2;
    net_protocol_select_button.Y = net_protocol_box.Y + net_protocol_box.Height - 43;
    net_unkn40_button.X = net_protocol_box.X +
      (net_protocol_box.Width - net_unkn40_button.Width) / 2;
    net_unkn40_button.Y = net_protocol_box.Y + net_protocol_box.Height - 43;

    net_protocol_option_button.X = net_protocol_box.X +
      (net_protocol_box.Width - net_protocol_option_button.Width) / 2;
    net_protocol_option_button.Y = net_protocol_box.Y + net_protocol_box.Height - 24;

    net_INITIATE_button.X = net_groups_box.X + 5;
    net_INITIATE_button.Y = net_groups_box.Y + net_groups_box.Height - 42;
    net_groups_LOGON_button.X = net_groups_box.X + 5;
    net_groups_LOGON_button.Y = net_groups_box.Y + net_groups_box.Height - 21;

    unkn8_EJECT_button.X = net_groups_box.X + net_groups_box.Width - 76;
    unkn8_EJECT_button.Y = net_groups_box.Y + net_groups_box.Height - 21;
    net_SET2_button.X = net_benefits_box.X + net_benefits_box.Width - 71;
    net_SET2_button.Y = net_benefits_box.Y + net_benefits_box.Height - 52;
    net_SET_button.X = net_benefits_box.X + net_benefits_box.Width - 71;
    net_SET_button.Y = net_benefits_box.Y + net_benefits_box.Height - 19;
}

void reset_net_screen_boxes_flags(void)
{
    net_users_box.Flags = GBxFlg_Unkn0001;
    net_groups_box.Flags = GBxFlg_Unkn0001;
    net_benefits_box.Flags = GBxFlg_Unkn0001;
    net_team_box.Flags = GBxFlg_Unkn0001;
    net_faction_box.Flags = GBxFlg_Unkn0001;
    net_comms_box.Flags = GBxFlg_Unkn0001;
    net_grpaint.Flags = GBxFlg_Unkn0001;
    net_protocol_box.Flags = GBxFlg_Unkn0001;
}

void reset_net_screen_EJECT_flags(void)
{
    unkn8_EJECT_button.Flags = GBxFlg_Unkn0001;
}

void set_flag01_net_screen_boxes(void)
{
    net_SET2_button.Flags |= GBxFlg_Unkn0001;
    net_SET_button.Flags |= GBxFlg_Unkn0001;
    net_INITIATE_button.Flags |= GBxFlg_Unkn0001;
    net_protocol_option_button.Flags |= GBxFlg_Unkn0001;
    net_protocol_select_button.Flags |= GBxFlg_Unkn0001;
    net_unkn40_button.Flags |= GBxFlg_Unkn0001;
    unkn8_EJECT_button.Flags |= GBxFlg_Unkn0001;
    net_groups_LOGON_button.Flags |= GBxFlg_Unkn0001;
}

void skip_flashy_draw_net_screen_boxes(void)
{
    net_INITIATE_button.Flags |= GBxFlg_Unkn0002;
    net_groups_LOGON_button.Flags |= GBxFlg_Unkn0002;
    net_SET_button.Flags |= GBxFlg_Unkn0002;
    net_SET2_button.Flags |= GBxFlg_Unkn0002;
    net_groups_box.Flags |= GBxFlg_Unkn0002;
    net_users_box.Flags |= GBxFlg_Unkn0002;
    net_faction_box.Flags |= GBxFlg_Unkn0002;
    net_team_box.Flags |= GBxFlg_Unkn0002;
    net_grpaint.Flags |= GBxFlg_Unkn0002;
    net_benefits_box.Flags |= GBxFlg_Unkn0002;
    net_protocol_box.Flags |= GBxFlg_Unkn0002;
    net_protocol_select_button.Flags |= GBxFlg_Unkn0002;
    net_unkn40_button.Flags |= GBxFlg_Unkn0002;
    net_protocol_option_button.Flags |= GBxFlg_Unkn0002;
    net_comms_box.Flags |= GBxFlg_Unkn0002;
}

void switch_net_screen_boxes_to_initiate(void)
{
    net_INITIATE_button.Flags = GBxFlg_Unkn0001;
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
