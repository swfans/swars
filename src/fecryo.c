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

#include <assert.h>
#include "bfkeybd.h"
#include "bfmemut.h"
#include "bfsprite.h"
#include "bftext.h"
#include "ssampply.h"
#include "specblit.h"
#include "campaign.h"
#include "cybmod.h"
#include "femain.h"
#include "feequip.h"
#include "guiboxes.h"
#include "guitext.h"
#include "display.h"
#include "game.h"
#include "player.h"
#include "research.h"
#include "sound.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenBox cryo_blokey_box;
extern struct ScreenTextBox cryo_agent_list_box;
extern struct ScreenTextBox cryo_cybmod_list_box;
extern struct ScreenButton cryo_offer_cancel_button;

extern char cybmod_name_text[];

extern ubyte current_frame;
extern short word_15511E; // = -1;
extern ubyte cheat_research_cybmods;
extern ubyte byte_1C4978;
extern ubyte byte_1C4979;

// Shared with equip screen
extern char equip_cost_text[20];
extern struct ScreenTextBox equip_name_box;
extern struct ScreenTextBox equip_list_box;
extern struct ScreenInfoBox equip_cost_box;
extern struct ScreenButton equip_offer_buy_button;
extern struct ScreenButton equip_all_agents_button;
extern struct ScreenShape unk11_menu[5];

extern struct TbSprite *sprites_Icons0_0;

ubyte ac_do_cryo_offer_cancel(ubyte click);
ubyte ac_show_cryo_agent_list(struct ScreenTextBox *box);
ubyte ac_show_cryo_cybmod_list_box(struct ScreenTextBox *box);
ubyte ac_show_cryo_blokey(struct ScreenBox *box);

struct ScreenPoint equip_blokey_pos[] = {
    {23,  0},
    {46,  0},
    { 0, 49},
    {23, 98},
    { 0,  0},
};

ubyte equip_blokey_height[] = {
    197,  50, 148, 197, 197,
};

ubyte equip_blokey_width[] = {
    93, 47, 139, 93, 139,
};

struct ScreenPoint equip_blokey_static_pos[] = {
    {23,  0},
    {46,  0},
    { 0, 49},
    {23, 98},
};

ubyte equip_blokey_static_height[] = {
    197, 50, 148, 197,
};

ubyte equip_blokey_static_width[] = {
     93, 47, 139, 93,
};

ubyte do_cryo_offer_cancel(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_cryo_offer_cancel\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

#define PURPLE_MOD_AREA_WIDTH 139
#define PURPLE_MOD_AREA_HEIGHT 295

void init_next_blokey_flic(void)
{
#if 0
    asm volatile ("call ASM_init_next_blokey_flic\n"
        :  :  : "eax" );
#else
    struct Campaign *p_campgn;
    const char *campgn_mark;
    const char *flic_dir;
    ushort cmod, stage;
    int k;

    p_campgn = &campaigns[background_type];
    campgn_mark = p_campgn->ProjectorFnMk;
    // TODO FNAMES the convention with mark char is broken for "s"
    if (strcmp(campgn_mark, "s") == 0)
        campgn_mark = "m";

    flic_dir = "qdata/equip";

    stage = 0;

    if (stage == 0)
    {
        for (cmod = 1; cmod < 4; cmod = (cmod+1) % 4)
        {
            if (((mod_draw_states[cmod] & 0x08) != 0) &&
              ((mod_draw_states[cmod] & 0x04) != 0)) {
                stage = 1;
                break;
            }
            if (cmod == 0)
                break;
        }
    }

    if (stage == 0)
    {
        for (cmod = 0; cmod < 4; cmod++)
        {
            if (((mod_draw_states[cmod] & 0x08) != 0) &&
              ((mod_draw_states[cmod] & 0x04) == 0) &&
              (flic_mods[cmod] != 0)) {
                stage = 2;
                break;
            }
        }
    }

    if (stage == 0)
    {
        if (current_drawing_mod == 4) {
            stage = 3;
        } else
        for (cmod = 0; cmod < 4; cmod++)
        {
            if (flic_mods[cmod] == 0) {
                stage = 3;
                break;
            }
        }
    }

    switch (stage)
    {
    case 0:
        if (!byte_1DDC40)
        {
            byte_1DDC40 = 1;
            play_sample_using_heap(0, 134, 127, 64, 100, 0, 3u);
        }
        else if (!IsSamplePlaying(0, 134, 0))
        {
            k = anim_slots[8];
            sprintf(animations[k].Filename, "%s/%s%da%d.fli", flic_dir, campgn_mark, flic_mods[0], flic_mods[2]);
            flic_unkn03(8u);
            play_sample_using_heap(0, 126, 127, 64, 100, 0, 1u);
            current_frame = 0;
            new_current_drawing_mod = 4;
            byte_1DDC40 = 0;
        }
        break;
    case 1:
        k = anim_slots[3];
        switch (cmod)
        {
        case 0:
            sprintf(animations[k].Filename, "%s/%s%dbo.fli", flic_dir, campgn_mark, old_flic_mods[0]);
            break;
        case 1:
            sprintf(animations[k].Filename, "%s/%s%dbbo.fli", flic_dir, campgn_mark, old_flic_mods[0]);
            break;
        case 2:
            sprintf(animations[k].Filename, "%s/%s%da%do.fli", flic_dir, campgn_mark, old_flic_mods[0], old_flic_mods[2]);
            break;
        case 3:
            sprintf(animations[k].Filename, "%s/%s%dl%do.fli", flic_dir, campgn_mark, old_flic_mods[0], old_flic_mods[3]);
            break;
        default:
            assert(!"unreachable");
            break;
        }
        flic_unkn03(3);
        new_current_drawing_mod = cmod;
        mod_draw_states[cmod] |= 0x02;
        cryo_blokey_box.Flags &= ~0x0100;
        play_sample_using_heap(0, 132, 127, 64, 100, 0, 3);
        byte_1DDC40 = 0;
        break;
    case 2:
        k = anim_slots[3];
        switch (cmod)
        {
          case 0:
            sprintf(animations[k].Filename, "%s/%s%dbi.fli", flic_dir, campgn_mark, flic_mods[0]);
            break;
          case 1:
            sprintf(animations[k].Filename, "%s/%s%dbbi.fli", flic_dir, campgn_mark, flic_mods[0]);
            break;
          case 2:
            sprintf(animations[k].Filename, "%s/%s%da%di.fli", flic_dir, campgn_mark, flic_mods[0], flic_mods[2]);
            break;
          case 3:
            sprintf(animations[k].Filename, "%s/%s%dl%di.fli", flic_dir, campgn_mark, flic_mods[0], flic_mods[3]);
            break;
          default:
            assert(!"unreachable");
            break;
        }
        flic_unkn03(3);
        new_current_drawing_mod = cmod;
        mod_draw_states[cmod] |= 0x01;
        mod_draw_states[cmod] &= ~0x08;
        old_flic_mods[cmod] = flic_mods[cmod];
        cryo_blokey_box.Flags &= ~0x0100;
        byte_1DDC40 = 0;
        break;
    case 3:
        // No further action
        break;
    }
#endif
}

void purple_mods_data_to_screen(void)
{
#if 0
    asm volatile ("call ASM_purple_mods_data_to_screen\n"
        :  :  : "eax" );
#else
    struct Campaign *p_campgn;
    const char *campgn_mark;
    const char *flic_dir;
    char str[52];
    short x, y;
    ubyte *buf;
    ubyte *o[2];

    p_campgn = &campaigns[background_type];
    campgn_mark = p_campgn->ProjectorFnMk;
    // TODO FNAMES the convention with mark char is broken for "s"
    if (strcmp(campgn_mark, "s") == 0)
        campgn_mark = "";

    flic_dir = "qdata/equip";

    sprintf(str, "%s/bgman%s.dat", flic_dir, campgn_mark);

    buf = back_buffer - PURPLE_MOD_AREA_WIDTH*PURPLE_MOD_AREA_HEIGHT;
    LbFileLoadAt(str, buf);

    o[1] = back_buffer;
    o[0] = lbDisplay.WScreen;

    x = cryo_blokey_box.X + 63;
    y = cryo_blokey_box.Y + 1;
    copy_buffer_to_double_bufs(buf, PURPLE_MOD_AREA_WIDTH, PURPLE_MOD_AREA_HEIGHT,
        o, x, y, lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);
#endif
}

void blokey_flic_data_to_screen(void)
{
#if 0
    asm volatile ("call ASM_blokey_flic_data_to_screen\n"
        :  :  : "eax" );
#else
    ubyte cdm;
    ubyte *iline;
    ubyte *oline;
    ubyte *inp;
    ubyte *o;
    ushort dy, dx;

    cdm = current_drawing_mod;
    iline = unkn_buffer_05 + 0x8000;
    dx = cryo_blokey_box.X + 63 + equip_blokey_pos[cdm].X;
    dy = cryo_blokey_box.Y + 1 + equip_blokey_pos[cdm].Y;
    oline = &lbDisplay.WScreen[dx + lbDisplay.GraphicsScreenWidth * dy];

    for (dy = 0; dy < equip_blokey_height[cdm]; dy++)
    {
        inp = iline;
        o = oline;
        for (dx = 0; dx < equip_blokey_width[cdm]; dx++)
        {
            if (*inp != '\0')
                *o = *inp;
            inp++;
            o++;
        }
        oline += lbDisplay.GraphicsScreenWidth;
        iline += equip_blokey_width[cdm];
    }
#endif
}

void blokey_static_flic_data_to_screen(void)
{
#if 0
    asm volatile ("call ASM_blokey_static_flic_data_to_screen\n"
        :  :  : "eax" );
#else
    struct Campaign *p_campgn;
    const char *campgn_mark;
    const char *flic_dir;
    char str[52];
    ubyte *buf;
    ubyte *o[2];
    ubyte cdm;

    p_campgn = &campaigns[background_type];
    campgn_mark = p_campgn->ProjectorFnMk;
    // TODO FNAMES the convention with mark char is broken for "s"
    if (strcmp(campgn_mark, "s") == 0)
        campgn_mark = "m";

    flic_dir = "qdata/equip";

    o[1] = back_buffer;
    o[0] = lbDisplay.WScreen;

    for (cdm = 0; cdm < 4; cdm++)
    {
        long len;
        short x, y;

        if (flic_mods[cdm] == 0)
            continue;

        switch (cdm)
        {
        case 0:
            sprintf(str, "%s/%s%db.dat", flic_dir, campgn_mark, flic_mods[0]);
            break;
        case 1:
            sprintf(str, "%s/%s%dbb.dat", flic_dir, campgn_mark, flic_mods[0]);
            break;
        case 2:
            sprintf(str, "%s/%s%da%d.dat", flic_dir, campgn_mark, flic_mods[0], flic_mods[2]);
            break;
        case 3:
            sprintf(str, "%s/%s%dl%d.dat", flic_dir, campgn_mark, flic_mods[0], flic_mods[3]);
            break;
        }

        buf = unkn_buffer_05 + 0x8000;
        len = LbFileLoadAt(str, buf);
        if (len < 4) {
            LbMemorySet(buf, 0, equip_blokey_static_width[cdm] * equip_blokey_static_height[cdm]);
        }

        x = cryo_blokey_box.X + 63 + equip_blokey_static_pos[cdm].X;
        y = cryo_blokey_box.Y + 1 + equip_blokey_static_pos[cdm].Y;
        copy_buffer_to_double_bufs_with_trans(buf, equip_blokey_static_width[cdm], equip_blokey_static_height[cdm],
          o, x, y, lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight, 0);

        mod_draw_states[cdm] = 4;
    }
#endif
}

void update_cybmod_cost_text(void)
{
    int cost;

    cost = 10 * (int)mod_defs[selected_mod + 1].Cost;
    sprintf(equip_cost_text, "%d", cost);
}

ubyte cryo_blokey_mod_level(ubyte cdm)
{
    PlayerInfo *p_locplayer;
    ubyte cybmod_lv;

    switch (cdm)
    {
    case 0:
        cybmod_lv = flic_mods[1];
        break;
    case 1:
        cybmod_lv = flic_mods[2];
        break;
    case 2:
        cybmod_lv = flic_mods[0];
        break;
    case 3:
        p_locplayer = &players[local_player_no];
        if (selected_agent == 4)
        {
            cybmod_lv = cybmod_skin_level(&p_locplayer->Mods[0]);
            if ((cybmod_skin_level(&p_locplayer->Mods[1]) != cybmod_lv)
              || (cybmod_skin_level(&p_locplayer->Mods[2]) != cybmod_lv)
              || (cybmod_skin_level(&p_locplayer->Mods[3]) != cybmod_lv))
            {
              cybmod_lv = 0;
            }
        }
        else
        {
            cybmod_lv = cybmod_skin_level(&p_locplayer->Mods[selected_agent]);
        }
        break;
    case 4:
        cybmod_lv = flic_mods[3];
        break;
    }
    return cybmod_lv;
}

void update_flic_mods(ubyte *mods)
{
    asm volatile ("call ASM_update_flic_mods\n"
        : : "a" (mods));
}

void draw_body_mods(void)
{
    asm volatile ("call ASM_draw_body_mods\n"
        :  :  : "eax" );
}

ubyte show_cryo_blokey(struct ScreenBox *box)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_show_cryo_blokey\n"
        : "=r" (ret) : "a" (box));
    return ret;
#else
    short cx, cy;
    short hline;
    ubyte cdm;

    if ((box->Flags & 0x8000) == 0)
    {
        draw_flic_purple_list(purple_mods_data_to_screen);
        box->Flags |= 0x8000;
        update_flic_mods(old_flic_mods);
        update_flic_mods(flic_mods);
        for (cdm = 0; cdm < 4; cdm++)
        {
            mod_draw_states[cdm] = 0;
            if (flic_mods[cdm] != 0)
                mod_draw_states[cdm] = 8;
        }
        current_drawing_mod = 0;
    }

    if (word_15511E != selected_agent)
        box->Flags &= ~0x0100;
    word_15511E = selected_agent;
    lbFontPtr = small_med_font;
    my_set_text_window(0, 0, lbDisplay.GraphicsScreenWidth,
        lbDisplay.GraphicsScreenHeight);
    cx = box->X + 4;
    cy = box->Y + 20;
    hline = font_height('A');
    //if (selected_agent != -1) -- this is always set to 0..4
    {
        draw_body_mods();
        for (cdm = 0; cdm < 5; cdm++)
        {
            ubyte cybmod_lv;
            char locstr[54];
            char *text;

            cybmod_lv = cryo_blokey_mod_level(cdm);

            if (cybmod_lv == 0)
            {
                if (cdm == 3)
                    cy += 2 * hline + 70;
                else
                    cy += 2 * hline + 37;
                continue;
            }

            lbDisplay.DrawColour = 247;
            draw_text_purple_list2(cx, cy, gui_strings[70 + cdm], 0);
            cy += hline + 3;
            if (cdm == 3)
                snprintf(locstr, sizeof(locstr), "%s %d", gui_strings[75], cybmod_lv);
            else
                snprintf(locstr, sizeof(locstr), "%s %d", gui_strings[76], cybmod_lv);
            text = (char *)back_buffer + text_buf_pos;
            strcpy(text, locstr);
            draw_text_purple_list2(cx, cy, text, 0);
            lbDisplay.DrawFlags = 0;
            text_buf_pos += strlen(locstr) + 1;
            if (cdm == 3)
            {
                lbDisplay.DrawFlags = 0x0010;
                draw_box_purple_list(cx, cy + hline + 3, 40, 40, lbDisplay.DrawColour);
                draw_sprite_purple_list(cx + 1, cy + hline + 4, &sprites_Icons0_0[163 + cybmod_lv]);
                lbDisplay.DrawFlags = 0;
                cy += hline + 67;
            }
            else
            {
                cy += hline + 34;
            }
        }
    }
    return 0;
#endif
}

ubyte show_cryo_agent_list(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_cryo_agent_list\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_cryo_cybmod_list_box(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_cryo_cybmod_list_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

TbBool cybmod_available_for_purchase(short mtype)
{
    PlayerInfo *p_locplayer;
    union Mod sgumod;

    p_locplayer = &players[local_player_no];

    if (!is_research_cymod_completed(mtype)
      && (login_control__State != 5 || mod_tech_level[mtype] > login_control__TechLevel))
        return false;

    //if (selected_agent != -1) -- this is always set to 0..4
    sgumod.Mods = 0;
    add_mod_to_flags(&sgumod, mtype);
    if (selected_agent == 4)
    {
        ushort plagent;

        // If not purcheasing chest mods, require chest mods on all agents
        if (!cybmod_chest_level(&sgumod))
        {
            for (plagent = 0; plagent < 4; plagent++)
            {
                if (!cybmod_chest_level(&p_locplayer->Mods[plagent]))
                    break;
            }
        }
        if (plagent < 4)
            return false;
        return true;
    }
    {
        // If not purcheasing chest mod, require chest mod
        if (!cybmod_chest_level(&sgumod))
        {
            if (!cybmod_chest_level(&p_locplayer->Mods[selected_agent]))
                return false;
        }
    }
    return true;
}

void set_flag02_cryo_screen_boxes(void)
{
    short i;

    cryo_agent_list_box.Flags |= 0x0002;
    cryo_blokey_box.Flags |= 0x0002;
    equip_offer_buy_button.Flags |= 0x0002;
    equip_name_box.Flags |= 0x0002;
    cryo_cybmod_list_box.Flags |= 0x0002;
    equip_cost_box.Flags |= 0x0002;
    cryo_offer_cancel_button.Flags |= 0x0002;
    equip_all_agents_button.Flags |= 0x0002;
    for (i = 0; i < 5; i++) {
        unk11_menu[i].Flags = 0x02;
    }
}

void show_cryo_chamber_screen(void)
{
#if 1
    asm volatile ("call ASM_show_cryo_chamber_screen\n"
        :  :  : "eax" );
#else
    if ((unk11_menu[0].Flags & 0x01) != 0)
    {
        byte_1C4978 = 0;
        byte_1C4979 = 0;
    }
    if (cryo_agent_list_box.Lines == 0)
        cryo_agent_list_box.Lines = cryo_agents.NumAgents;
    if (((game_projector_speed != 0) && is_heading_flag01()) ||
      (lbKeyOn[KC_SPACE] && !edit_flag))
    {
        set_flag02_heading_screen_boxes();
        set_flag02_cryo_screen_boxes();
        byte_1C4978 = 1;
        byte_1C4979 = 1;
    }
    if ((ingame.UserFlags & UsrF_Cheats) != 0)
    {
        if (lbKeyOn[KC_0])
        {
            lbKeyOn[KC_0] = 0;
            refresh_equip_list = 1;
            research_cymod_complete(cheat_research_cybmods + 1);
            cheat_research_cybmods++;
        }
    }

    if (refresh_equip_list && selected_mod == -1)
    {
        ushort mtype;

        cryo_cybmod_list_box.Lines = 0;
        for (mtype = 1; mtype < MOD_TYPES_COUNT; mtype++)
        {
            if (cybmod_available_for_purchase(mtype)) {
                cryo_cybmod_list_box.Lines++;
            }
        }
        cryo_cybmod_list_box.Flags |= 0x0080;
        cryo_cybmod_list_box.BGColour = 0;
        refresh_equip_list = 0;
    }

    if (draw_heading_box())
    {
        //TODO: remake the inside
    }
#endif
}

void init_cryo_screen_boxes(void)
{
    short scr_w, start_x;

    scr_w = lbDisplay.GraphicsWindowWidth;

    init_screen_text_box(&cryo_agent_list_box, 7u, 122u, 196u, 303, 6,
        small_med_font, 1);
    cryo_agent_list_box.BGColour = 25;
    cryo_agent_list_box.DrawTextFn = ac_show_cryo_agent_list;
    cryo_agent_list_box.ScrollWindowOffset += 27;
    cryo_agent_list_box.Flags |= 0x0300;
    cryo_agent_list_box.ScrollWindowHeight -= 27;

    init_screen_box(&cryo_blokey_box, 212u, 122u, 203u, 303, 6);
    cryo_blokey_box.SpecialDrawFn = ac_show_cryo_blokey;

    init_screen_text_box(&cryo_cybmod_list_box, 425u, 153u, 208u, 272,
      6, small_med_font, 1);
    cryo_cybmod_list_box.DrawTextFn = ac_show_cryo_cybmod_list_box;
    cryo_cybmod_list_box.Flags |= 0x0300;
    cryo_cybmod_list_box.ScrollWindowHeight = 117;

    init_screen_button(&cryo_offer_cancel_button, 628u, 404u,
      gui_strings[437], 6, med2_font, 1, 0x80);
    cryo_offer_cancel_button.CallBackFn = ac_do_cryo_offer_cancel;

    start_x = (scr_w - cryo_agent_list_box.Width - cryo_blokey_box.Width - cryo_cybmod_list_box.Width - 33) / 2;

    cryo_agent_list_box.X = start_x + 7;
    cryo_blokey_box.X = cryo_agent_list_box.X + cryo_agent_list_box.Width + 9;
    cryo_cybmod_list_box.X = cryo_blokey_box.X + cryo_blokey_box.Width + 10;
    cryo_offer_cancel_button.X = cryo_cybmod_list_box.X + cryo_cybmod_list_box.Width - cryo_offer_cancel_button.Width - 5;
}

void switch_shared_equip_screen_buttons_to_cybmod(void)
{
    set_heading_box_text(gui_strings[369]);
    equip_cost_box.X = cryo_cybmod_list_box.X + 5;
    equip_cost_box.Width = cryo_cybmod_list_box.Width - 10;
    equip_cost_box.Y = 383;
    equip_name_box.Text = cybmod_name_text;
    if (selected_mod < 0)
        cybmod_name_text[0] = '\0';
    else
        init_weapon_anim(selected_mod + 32);
    switch_equip_offer_to_buy();
}

void set_flag01_cryo_screen_boxes(void)
{
    cryo_offer_cancel_button.Flags |= 0x0001;
}

void reset_cryo_screen_boxes_flags(void)
{
    cryo_cybmod_list_box.Flags = 0x0001 | 0x0100 | 0x0200;
    cryo_agent_list_box.Flags = 0x0001 | 0x0100 | 0x0200;
    cryo_blokey_box.Flags = 0x0001;
}

/******************************************************************************/
