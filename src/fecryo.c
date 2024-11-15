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
#include "bflib_joyst.h"
#include "ssampply.h"
#include "specblit.h"
#include "campaign.h"
#include "cybmod.h"
#include "display.h"
#include "femain.h"
#include "feequip.h"
#include "guiboxes.h"
#include "guitext.h"
#include "keyboard.h"
#include "game_sprts.h"
#include "game.h"
#include "player.h"
#include "purpldrw.h"
#include "purpldrwlst.h"
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
extern ubyte byte_155174; // = 166;
extern ubyte byte_155175[];
extern ubyte byte_155180; // = 109;
extern ubyte byte_155181[];
extern ubyte byte_1551F4[5];
extern ubyte cheat_research_cybmods;
extern char byte_1C495C[20];
extern ubyte byte_1C4978;
extern ubyte byte_1C4979;
extern ubyte byte_1C4AA0;

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
ubyte ac_do_cryo_all_agents_set(ubyte click);
void ac_weapon_flic_data_to_screen(void);
ubyte ac_do_equip_offer_buy(ubyte click);

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
        cryo_blokey_box.Flags &= ~GBxFlg_RadioBtn;
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
        cryo_blokey_box.Flags &= ~GBxFlg_RadioBtn;
        byte_1DDC40 = 0;
        break;
    case 3:
        // No further action
        break;
    }
}

void purple_mods_data_to_screen(void)
{
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
}

void blokey_flic_data_to_screen(void)
{
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
}

void blokey_static_flic_data_to_screen(void)
{
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

        mod_draw_states[cdm] = 0x04;
    }
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

void reset_mod_draw_states_flag08(void)
{
    ushort cdm;
    for (cdm = 0; cdm < 4; cdm++)
    {
        mod_draw_states[cdm] = 0;
        if (flic_mods[cdm] != 0)
            mod_draw_states[cdm] |= 0x08;
    }
}

void set_mod_draw_states_flag08(void)
{
    ushort cdm;
    for (cdm = 0; cdm < 4; cdm++)
    {
        if (old_flic_mods[cdm] != flic_mods[cdm])
            mod_draw_states[cdm] |= 0x08;
    }
}

ubyte show_cryo_blokey(struct ScreenBox *box)
{
    short cx, cy;
    short hline;

    if ((box->Flags & GBxFlg_BkgndDrawn) == 0)
    {
        draw_flic_purple_list(purple_mods_data_to_screen);
        box->Flags |= GBxFlg_BkgndDrawn;
        update_flic_mods(old_flic_mods);
        update_flic_mods(flic_mods);
        reset_mod_draw_states_flag08();
        current_drawing_mod = 0;
    }

    if (word_15511E != selected_agent)
        box->Flags &= ~GBxFlg_RadioBtn;
    word_15511E = selected_agent;
    lbFontPtr = small_med_font;
    my_set_text_window(0, 0, lbDisplay.GraphicsScreenWidth,
        lbDisplay.GraphicsScreenHeight);
    cx = box->X + 4;
    cy = box->Y + 20;
    hline = font_height('A');
    //if (selected_agent != -1) -- this is always set to 0..4
    {
        ubyte cdm;

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
                lbDisplay.DrawFlags = Lb_SPRITE_OUTLINE;
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
}

ubyte show_cryo_agent_list(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_cryo_agent_list\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

TbBool cybmod_available_for_purchase(short mtype)
{
    PlayerInfo *p_locplayer;

    p_locplayer = &players[local_player_no];

    if (!is_research_cymod_completed(mtype)
      && ((login_control__State != 5) || mod_tech_level[mtype] > login_control__TechLevel))
        return false;

    //if (selected_agent != -1) -- this is always set to 0..4
    if (selected_agent == 4)
    {
        ushort plagent;

        // If not purcheasing chest mods, require chest mods on all agents
        if (cybmod_group_type(mtype) != MODGRP_CHEST)
        {
            for (plagent = 0; plagent < 4; plagent++)
            {
                if (!cybmod_chest_level(&p_locplayer->Mods[plagent]))
                    break;
            }
            if (plagent < 4)
                return false;
        }
        return true;
    }
    {
        // If not purcheasing chest mod, require chest mod
        if (cybmod_group_type(mtype) != MODGRP_CHEST)
        {
            if (!cybmod_chest_level(&p_locplayer->Mods[selected_agent]))
                return false;
        }
    }
    return true;
}

ubyte show_cryo_cybmod_list_box(struct ScreenTextBox *box)
{
    ubyte modstrings[5];

    memcpy(modstrings, byte_1551F4, 5);
    struct ScreenBoxBase power_box = {box->X + 8, box->Y + 152, 192, 17};
    struct ScreenBoxBase resil_box = {box->X + 8, box->Y + 177, 192, 17};
    struct ScreenBoxBase addit_box = {box->X + 8, box->Y + 200, 192, 19};

    if ((box->Flags & GBxFlg_BkgndDrawn) == 0)
    {
        lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        draw_box_purple_list(text_window_x1, text_window_y1,
          text_window_x2 - text_window_x1 + 1, text_window_y2 - text_window_y1 + 1, 56);
        draw_box_purple_list(box->X + 4, box->Y + 149, box->Width - 8, 48, 56);
        draw_box_purple_list(box->X + 4, box->Y + 210, box->Width - 8, 13, 56);

        draw_discrete_rects_bar_bk(&power_box, gui_strings[432], byte_155174);
        draw_discrete_rects_bar_bk(&resil_box, gui_strings[433], byte_155180);
        draw_text_property_bk(&addit_box, gui_strings[434]);

        lbDisplay.DrawFlags = 0;
        box->Flags |= GBxFlg_BkgndDrawn;
        copy_box_purple_list(box->X + 4, box->Y + 4 + box->ScrollWindowOffset,
          box->Width - 20, box->ScrollWindowHeight + 23);

        copy_box_purple_list(box->X + 4, box->Y + 149, box->Width - 8, box->Height - 146);

        my_set_text_window(box->X + 4, box->ScrollWindowOffset + box->Y + 4,
          box->Width - 20, box->ScrollWindowHeight + 23);
        lbFontPtr = small_med_font;
    }

    if (selected_mod == -1) // No mod selected - show list of available ones
    {
        ushort mtype;
        short text_h;
        short cy;

        cy = 3;
        text_h = font_height('A');
        for (mtype = box->field_38+1; mtype < MOD_TYPES_COUNT; mtype++)
        {
            if (text_h + cy >= box->ScrollWindowHeight + 23)
                return 0;
            if (cybmod_available_for_purchase(mtype))
            {
                  char *text;
                  ubyte modgrp, modlv;
                  ushort mdstr_id, lvstr_id;

                  if (mouse_down_over_box_coords(text_window_x1, cy + text_window_y1 - 1,
                    text_window_x2, cy + text_window_y1 + 1 + text_h) && lbDisplay.LeftButton)
                  {
                        lbDisplay.LeftButton = 0;
                        selected_mod = mtype - 1;
                        equip_name_box.TextFadePos = -5;
                        modgrp = cybmod_group_type(mtype);
                        modlv = cybmod_version(mtype);
                        mdstr_id = 70 + modstrings[modgrp];
                        if (cybmod_group_type(mtype) != MODGRP_EPIDERM)
                           lvstr_id = 76;
                        else
                           lvstr_id = 75;
                        sprintf(cybmod_name_text, "%s %s %d", gui_strings[mdstr_id], gui_strings[lvstr_id], modlv);
                        sprintf(equip_cost_text, "%d", 10 * mod_defs[mtype].Cost);
                        equip_offer_buy_button.Text = gui_strings[436];
                        equip_offer_buy_button.CallBackFn = ac_do_equip_offer_buy;
                        if (byte_1C4AA0 || (1 << selected_mod >= 0x1000))
                        {
                            box->TextFadePos = -5;
                            box->field_38 = 0;
                            box->Text = &weapon_text[cybmod_text_index[selected_mod]];
                            box->Lines = 0;
                            box->Flags |= GBxFlg_Unkn0080;
                            lbFontPtr = small_font;
                            box->BGColour = byte_197160 + font_height('A');
                            lbFontPtr = box->Font;
                        }
                        else
                        {
                          init_weapon_anim(selected_mod + 32);
                        }
                  }
                  if (selected_mod == mtype - 1) {
                      lbDisplay.DrawFlags = Lb_TEXT_ONE_COLOR;
                      lbDisplay.DrawColour = 87;
                  } else {
                      lbDisplay.DrawFlags = 0;
                  }
                  lbDisplay.DrawFlags |= 0x8000;
                  modgrp = cybmod_group_type(mtype);
                  modlv = cybmod_version(mtype);
                  mdstr_id = 70 + modstrings[modgrp];
                  draw_text_purple_list2(3, cy + 1, gui_strings[mdstr_id], 0);
                  lbDisplay.DrawFlags &= ~(0x8000|Lb_TEXT_HALIGN_RIGHT);

                  lbDisplay.DrawFlags |= Lb_TEXT_HALIGN_RIGHT;
                  if (cybmod_group_type(mtype) != MODGRP_EPIDERM)
                      lvstr_id = 76;
                  else
                      lvstr_id = 75;
                  sprintf(byte_1C495C, "%s %d", gui_strings[lvstr_id], modlv);
                  text = (char *)back_buffer + text_buf_pos;
                  strcpy(text, byte_1C495C);
                  draw_text_purple_list2(-1, cy + 1, text, 0);
                  text_buf_pos += strlen(byte_1C495C) + 1;
                  lbDisplay.DrawFlags = 0;

                  cy += text_h + box->LineSpacing;
            }
        }
    }
    else
    {
        struct ModDef *mdef;

        mdef = &mod_defs[selected_mod + 1];

        draw_discrete_rects_bar_lv(&power_box, mdef->PowerOutput, 8, byte_155175);
        draw_discrete_rects_bar_lv(&resil_box, mdef->Resilience, 8, byte_155181);
        draw_text_property_lv(&addit_box, gui_strings[645 + mdef->AdditProp]);
        lbDisplay.DrawFlags = 0;

        // Add control hotspot for the view / description switch
        draw_hotspot_purple_list(box->X + box->Width / 2, box->Y + 104);

        if (byte_1C4AA0 || (1 << selected_mod >= 0x1000))
        {
            lbFontPtr = small_font;
            my_set_text_window(box->X + 4, box->ScrollWindowOffset + box->Y + 4,
              box->Width - 20, box->ScrollWindowHeight + 23);
            flashy_draw_text(0, 0, box->Text, box->TextSpeed, box->field_38, &box->TextFadePos, 0);
        }
        else
        {
            xdo_next_frame(2);
            draw_flic_purple_list(ac_weapon_flic_data_to_screen);
        }
        if (lbDisplay.LeftButton)
        {
            if (mouse_down_over_box_coords(box->X + 4,
              box->Y + 4, box->X + box->Width - 4, box->Y + 4 + 140))
            {
                lbDisplay.LeftButton = 0;
                byte_1C4AA0 = byte_1C4AA0 == 0;
                if (byte_1C4AA0 || (1 << selected_mod >= 0x1000))
                {
                    box->TextFadePos = -5;
                    box->Text = &weapon_text[cybmod_text_index[selected_mod]];
                    box->field_38 = 0;
                    box->Lines = 0;
                    box->Flags |= GBxFlg_Unkn0080;
                    lbFontPtr = small_font;
                    box->BGColour = byte_197160 + font_height('A');
                    lbFontPtr = box->Font;
                }
                else
                {
                    init_weapon_anim(selected_mod + 32);
                }
            }
        }
        //equip_offer_buy_button.DrawFn(&equip_offer_buy_button); -- incompatible calling convention
        asm volatile ("call *%1\n"
            : : "a" (&equip_offer_buy_button), "g" (equip_offer_buy_button.DrawFn));
        //cryo_offer_cancel_button.DrawFn(&cryo_offer_cancel_button); -- incompatible calling convention
        asm volatile ("call *%1\n"
            : : "a" (&cryo_offer_cancel_button), "g" (cryo_offer_cancel_button.DrawFn));
        //equip_cost_box.DrawFn(&equip_cost_box); -- incompatible calling convention
        asm volatile ("call *%1\n"
            : : "a" (&equip_cost_box), "g" (equip_cost_box.DrawFn));

        if (selected_mod == -1)
        {
            equip_cost_box.Flags = (GBxFlg_Unkn0008|GBxFlg_Unkn0001);
            equip_offer_buy_button.Flags |= GBxFlg_Unkn0001;
            cryo_offer_cancel_button.Flags |= GBxFlg_Unkn0001;
        }
    }
    return 0;
}

void set_flag02_cryo_screen_boxes(void)
{
    short i;

    cryo_agent_list_box.Flags |= GBxFlg_Unkn0002;
    cryo_blokey_box.Flags |= GBxFlg_Unkn0002;
    equip_offer_buy_button.Flags |= GBxFlg_Unkn0002;
    equip_name_box.Flags |= GBxFlg_Unkn0002;
    cryo_cybmod_list_box.Flags |= GBxFlg_Unkn0002;
    equip_cost_box.Flags |= GBxFlg_Unkn0002;
    cryo_offer_cancel_button.Flags |= GBxFlg_Unkn0002;
    equip_all_agents_button.Flags |= GBxFlg_Unkn0002;
    for (i = 0; i < 5; i++) {
        unk11_menu[i].Flags = GBxFlg_Unkn0002;
    }
}

ubyte do_cryo_all_agents_set(ubyte click)
{
    struct ScreenButton *button = &equip_all_agents_button;
    *(button->Radio) = button->RadioValue;
    check_buy_sell_button();
    update_flic_mods(flic_mods);
    set_mod_draw_states_flag08();
    return 1;
}

ubyte input_cryo_agent_panel_shape(struct ScreenShape *shape, sbyte nagent)
{
    ubyte gbstate;

    if (lbKeyOn[KC_1 + nagent])
    {
        lbKeyOn[KC_1 + nagent] = 0;
        if (nagent < cryo_agents.NumAgents)
        {
            selected_agent = nagent;
            check_buy_sell_button();
            update_flic_mods(flic_mods);
            set_mod_draw_states_flag08();
        }
    }

    if (mouse_over_agent_panel_shape(shape))
    {
        if ((shape->Flags & 0x0200) == 0) {
            play_sample_using_heap(0, 123, 127, 64, 100, 0, 1u);
            shape->Flags |= 0x0200;
        }
        if (lbDisplay.MLeftButton || (joy.Buttons[0] != 0))
        {
            lbDisplay.LeftButton = 0;
            shape->Flags |= 0x0400;
            gbstate = GBxSta_PUSHED;
        }
        else
        {
            if ((nagent >= cryo_agents.NumAgents) || (mo_weapon == -1))
            {
                if ((shape->Flags & 0x0400) != 0)
                {
                    if (nagent < cryo_agents.NumAgents)
                    {
                        play_sample_using_heap(0, 111, 127, 64, 100, 0, 2u);
                        selected_agent = nagent;
                        check_buy_sell_button();
                        update_flic_mods(flic_mods);
                        set_mod_draw_states_flag08();
                    }
                    else
                    {
                        play_sample_using_heap(0, 129, 127, 64, 100, 0, 2u);
                    }
                    shape->Flags &= ~0x0400;
                }
            }
            else
            {
                // No drag and drop ability in mods screen
                mo_weapon = -1;
                shape->Flags &= ~0x0400;
            }
            gbstate = GBxSta_HLIGHT2;
        }
    }
    else
    {
        if (shape->Flags & 0x0200)
            shape->Flags &= ~0x0200;
        if (shape->Flags & 0x0400)
            shape->Flags &= ~0x0400;
        if ((selected_agent == nagent) || (selected_agent == 4))
        {
            gbstate = GBxSta_HLIGHT1;
        }
        else
        {
            gbstate = GBxSta_NORMAL;
        }
    }

    return gbstate;
}

ubyte input_cryo_all_agents_button(struct ScreenButton *button)
{
    ubyte gbstate;
    sbyte nagent = 4;

    gbstate = GBxSta_NORMAL;
    if (lbKeyOn[KC_1 + nagent])
    {
        lbKeyOn[KC_1 + nagent] = 0;
        selected_agent = nagent;
        check_buy_sell_button();
        update_flic_mods(flic_mods);
        set_mod_draw_states_flag08();
        gbstate = GBxSta_HLIGHT2;
    }
    return gbstate;
}

ubyte show_cryo_chamber_screen(void)
{
    ubyte drawn = true;

    if ((unk11_menu[0].Flags & GBxFlg_Unkn0001) != 0)
    {
        byte_1C4978 = 0;
        byte_1C4979 = 0;
    }
    if (cryo_agent_list_box.Lines == 0)
        cryo_agent_list_box.Lines = cryo_agents.NumAgents;
    if (((game_projector_speed != 0) && is_heading_flag01()) ||
      (is_key_pressed(KC_SPACE, KMod_DONTCARE) && !edit_flag))
    {
        clear_key_pressed(KC_SPACE);
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
        cryo_cybmod_list_box.Flags |= GBxFlg_Unkn0080;
        cryo_cybmod_list_box.BGColour = 0;
        refresh_equip_list = 0;
    }

    if (drawn)
        drawn = draw_heading_box();

    if (drawn)
    {
        sbyte nagent;
        ubyte agnt[4];
        ubyte boxes_drawn;

        boxes_drawn = 1;
        for (nagent = 4; nagent >= 0; nagent--)
        {
            struct ScreenShape *shape;
            ubyte gbstate;

            shape = &unk11_menu[nagent];

            if (nagent == 4) // agent name box
            {
                ubyte name_drawn;
                // Agents grouping has little to do with name box, but it's convienient to put here
                gbstate = input_cryo_all_agents_button(&equip_all_agents_button);

                if (byte_1C4979 == 0)
                {
                    name_drawn = flashy_draw_draw_equip_agent_name_shape(shape, gbstate);
                }
                else if (byte_1C4979 == 1)
                {
                    draw_equip_agent_name_shape(shape, gbstate);
                    name_drawn = 3;
                }
                byte_1C4979 = (name_drawn == 3);
            }
            else
            {
                ubyte drawn;
                ushort spridx;

                gbstate = input_cryo_agent_panel_shape(shape, nagent);

                if (byte_1C4978 == 0)
                {
                    drawn = flashy_draw_agent_panel_shape(shape, gbstate);
                }
                else
                {
                    spridx = 140 + nagent;
                    draw_agent_panel_shape(shape, spridx, gbstate);
                    drawn = 3;
                }
                // Is the flashy draw finished for current button
                agnt[nagent] = (drawn == 3);
                // When all buttons started actually drawing, we can begin flashy draw of the panels below
                boxes_drawn &= (drawn > 1);
            }
        }
        if (byte_1C4978 == 0)
        {
            byte_1C4978 = agnt[0] && agnt[1] && agnt[2] && agnt[3];
        }
        drawn = boxes_drawn;
    }

    if (drawn)
    {
        //drawn = equip_all_agents_button.DrawFn(&equip_all_agents_button); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&equip_all_agents_button), "g" (equip_all_agents_button.DrawFn));
        //drawn = cryo_agent_list_box.DrawFn(&cryo_agent_list_box); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&cryo_agent_list_box), "g" (cryo_agent_list_box.DrawFn));
    }

    if (drawn)
    {
        //drawn = cryo_blokey_box.DrawFn(&cryo_blokey_box); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&cryo_blokey_box), "g" (cryo_blokey_box.DrawFn));
    }

    if (drawn)
    {
        //drawn = cryo_cybmod_list_box.DrawFn(&cryo_cybmod_list_box); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&cryo_cybmod_list_box), "g" (cryo_cybmod_list_box.DrawFn));
        //drawn = equip_name_box.DrawFn(&equip_name_box); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&equip_name_box), "g" (equip_name_box.DrawFn));
    }

    return drawn;
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
    cryo_agent_list_box.Flags |= (GBxFlg_RadioBtn|GBxFlg_IsMouseOver);
    cryo_agent_list_box.ScrollWindowHeight -= 27;

    init_screen_box(&cryo_blokey_box, 212u, 122u, 203u, 303, 6);
    cryo_blokey_box.SpecialDrawFn = ac_show_cryo_blokey;

    init_screen_text_box(&cryo_cybmod_list_box, 425u, 153u, 208u, 272,
      6, small_med_font, 1);
    cryo_cybmod_list_box.DrawTextFn = ac_show_cryo_cybmod_list_box;
    cryo_cybmod_list_box.Flags |= (GBxFlg_RadioBtn|GBxFlg_IsMouseOver);
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
    equip_all_agents_button.CallBackFn = ac_do_cryo_all_agents_set;
    if (selected_mod < 0)
        cybmod_name_text[0] = '\0';
    else
        init_weapon_anim(selected_mod + 32);
    switch_equip_offer_to_buy();
}

void set_flag01_cryo_screen_boxes(void)
{
    cryo_offer_cancel_button.Flags |= GBxFlg_Unkn0001;
}

void reset_cryo_screen_boxes_flags(void)
{
    cryo_cybmod_list_box.Flags = GBxFlg_Unkn0001 | GBxFlg_RadioBtn | GBxFlg_IsMouseOver;
    cryo_agent_list_box.Flags = GBxFlg_Unkn0001 | GBxFlg_RadioBtn | GBxFlg_IsMouseOver;
    cryo_blokey_box.Flags = GBxFlg_Unkn0001;
}

/******************************************************************************/
