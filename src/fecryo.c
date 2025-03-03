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
#include "bfscrcopy.h"
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
#include "game_data.h"
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

short equip_blokey_height[] = {
    197,  50, 148, 197, 295,
};

short equip_blokey_width[] = {
    93, 47, 139, 93, 139,
};

struct ScreenPoint equip_blokey_static_pos[] = {
    {23,  0},
    {46,  0},
    { 0, 49},
    {23, 98},
    { 0,  0},
};

short equip_blokey_static_height[] = {
    197, 50, 148, 197, 295,
};

short equip_blokey_static_width[] = {
     93, 47, 139, 93, 139,
};

void update_cybmod_cost_text(void)
{
    struct ModDef *mdef;
    int cost;

    if (selected_mod == -1) // No mod selected
    {
        equip_cost_text[0] = 0;
        return;
    }

    mdef = &mod_defs[selected_mod + 1];
    cost = 10 * (int)mdef->Cost;
    sprintf(equip_cost_text, "%d", cost);
}

void update_cybmod_name_text(void)
{
    ushort mtype;
    ushort mdstr_id, lvstr_id;
    ubyte modgrp, modlv;

    if (selected_mod == -1) // No mod selected
    {
        cybmod_name_text[0] = 0;
        return;
    }

    mtype = selected_mod + 1;

    modgrp = cybmod_group_type(mtype);
    modlv = cybmod_version(mtype);
    mdstr_id = 70 + byte_1551F4[modgrp];
    if (modgrp != MODGRP_EPIDERM)
       lvstr_id = 76;
    else
       lvstr_id = 75;
    sprintf(cybmod_name_text, "%s %s %d", gui_strings[mdstr_id], gui_strings[lvstr_id], modlv);
}

/** Get global text pointer to a mod level string.
 * @see loctext_to_gtext()
 */
static const char *cryo_gtext_cybmod_list_item_level(ushort mtype)
{
    char locstr[48];
    ubyte modlv;
    ushort lvstr_id;

    modlv = cybmod_version(mtype);

    if (cybmod_group_type(mtype) != MODGRP_EPIDERM)
        lvstr_id = 76;
    else
        lvstr_id = 75;
    sprintf(locstr, "%s %d", gui_strings[lvstr_id], modlv);
    return loctext_to_gtext(locstr);
}

TbBool cybmod_has_display_anim(ubyte mod)
{
    return (1 << (mod - 1) < 0x1000);
}

void cryo_display_box_redraw(struct ScreenTextBox *p_box)
{
    ubyte real_dbcontent;

    real_dbcontent = cybmod_has_display_anim(selected_mod + 1) ? display_box_content : DiBoxCt_TEXT;
    switch (real_dbcontent)
    {
    case DiBoxCt_TEXT:
        p_box->Flags |= GBxFlg_Unkn0080;
        // Re-add scroll bars
        p_box->Flags |= GBxFlg_RadioBtn;

        p_box->field_38 = 0;
        p_box->Lines = 0;
        p_box->Text = &weapon_text[cybmod_text_index[selected_mod]];
        lbFontPtr = small_font;
        p_box->LineHeight = byte_197160 + font_height('A');
        lbFontPtr = p_box->Font;
        p_box->TextFadePos = -5;
        break;
    case DiBoxCt_ANIM:
        // Remove scroll bars
        p_box->Flags &= ~GBxFlg_RadioBtn;

        init_weapon_anim(selected_mod + 32);
        // Negative value saves the background before starting animation
        p_box->TextFadePos = -2;
        break;
    }
}

void cryo_update_for_selected_cybmod(void)
{
    update_cybmod_name_text();
    update_cybmod_cost_text();

    if (selected_mod == -1) // No mod selected
    {
        cryo_cybmod_list_box.Flags |= GBxFlg_Unkn0080;
        // Re-add scroll bars
        cryo_cybmod_list_box.Flags |= GBxFlg_RadioBtn;
        cryo_cybmod_list_box.Text = NULL;
        return;
    }

    equip_name_box.TextFadePos = -5;
    switch_equip_offer_to_buy();
    cryo_display_box_redraw(&cryo_cybmod_list_box);
}

ubyte do_cryo_offer_cancel(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_cryo_offer_cancel\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    selected_mod = -1;
    cryo_update_for_selected_cybmod();
    refresh_equip_list = 1;
    return 0;
}

void reset_mod_draw_states_flag08(void)
{
    ushort part;
    for (part = 0; part < 4; part++)
    {
        mod_draw_states[part] = 0;
        if (flic_mods[part] != 0)
            mod_draw_states[part] |= ModDSt_Unkn08;
    }
}

void set_mod_draw_states_flag08(void)
{
    ushort part;
    for (part = 0; part < 4; part++)
    {
        if (old_flic_mods[part] != flic_mods[part])
            mod_draw_states[part] |= ModDSt_Unkn08;
    }
}

TbBool mod_draw_update_on_change(ushort mtype)
{
    switch (cybmod_group_type(mtype))
    {
    case MODGRP_LEGS:
        mod_draw_states[3] |= ModDSt_Unkn08;
        update_flic_mods(flic_mods);
        break;
    case MODGRP_ARMS:
        mod_draw_states[2] |= ModDSt_Unkn08;
        update_flic_mods(flic_mods);
        break;
    case MODGRP_CHEST:
        mod_draw_states[0] |= ModDSt_Unkn08;
        update_flic_mods(flic_mods);
        break;
    case MODGRP_BRAIN:
        mod_draw_states[1] |= ModDSt_Unkn08;
        update_flic_mods(flic_mods);
        break;
    default:
        return false;
    }
    return true;
}

ubyte do_equip_offer_buy_cybmod(ubyte click)
{
    struct ModDef *mdef;
    ubyte nbought;

    mdef = &mod_defs[selected_mod + 1];
    nbought = 0;

    if (selected_agent != 4)
    {
        long cost;
        short nagent;
        TbBool added;

        cost = 10 * mdef->Cost;
        nagent = selected_agent;

        if (ingame.Credits - cost < 0)
            added = false;
        else
            added = player_cryo_add_cybmod(nagent, selected_mod + 1);

        if (added) {
            mod_draw_update_on_change(selected_mod + 1);
        }

        if (added) {
            ingame.Expenditure += cost;
            ingame.Credits -= cost;
            nbought++;
        }
    }
    else
    {
        long cost;
        short nagent;
        TbBool added;

        cost = 10 * mdef->Cost;

        for (nagent = 0; nagent < 4; nagent++)
        {
            if (ingame.Credits - cost < 0)
                break;

            added = player_cryo_add_cybmod(nagent, selected_mod + 1);

            if (added) {
                mod_draw_update_on_change(selected_mod + 1);
            }

            if (added) {
                ingame.Credits -= cost;
                ingame.Expenditure += cost;
                nbought++;
            }
        }
    }

    if (nbought > 0)
    {
        if ((login_control__State == 5) && ((unkn_flags_08 & 0x08) != 0)) {
            net_players_copy_cryo();
        }
        selected_mod = -1;
        cryo_update_for_selected_cybmod();
        refresh_equip_list = 1;
    }

    return (nbought > 0);
}

void sprint_cryo_cyborg_mods_static_fname(char *str, ubyte part, ubyte *mods_arr)
{
    struct Campaign *p_campgn;
    const char *campgn_mark;
    PathInfo *pinfo;

    p_campgn = &campaigns[background_type];
    // TODO FNAMES the convention with mark char is broken for "s"
    campgn_mark = p_campgn->ProjectorFnMk;

    pinfo = &game_dirs[DirPlace_QEquip];

    switch (part)
    {
    case ModDPt_CHEST:
        if (strcmp(campgn_mark, "s") == 0)
            campgn_mark = "m";
        sprintf(str, "%s/%s%db.dat", pinfo->directory, campgn_mark, mods_arr[0]);
        break;
    case ModDPt_BRAIN:
        if (strcmp(campgn_mark, "s") == 0)
            campgn_mark = "m";
        sprintf(str, "%s/%s%dbb.dat", pinfo->directory, campgn_mark, mods_arr[0]);
        break;
    case ModDPt_ARMS:
        if (strcmp(campgn_mark, "s") == 0)
            campgn_mark = "m";
        sprintf(str, "%s/%s%da%d.dat", pinfo->directory, campgn_mark, mods_arr[0], mods_arr[2]);
        break;
    case ModDPt_LEGS:
        if (strcmp(campgn_mark, "s") == 0)
            campgn_mark = "m";
        sprintf(str, "%s/%s%dl%d.dat", pinfo->directory, campgn_mark, mods_arr[0], mods_arr[3]);
        break;
    case ModDPt_BKGND:
        if (strcmp(campgn_mark, "s") == 0)
            campgn_mark = "";
        sprintf(str, "%s/bgman%s.dat", pinfo->directory, campgn_mark);
        break;
    default:
        str[0] = '\0';
        break;
    }
}

void cryo_cyborg_mods_anim_set_fname(ubyte anislot, ubyte part, ubyte stage)
{
    struct Campaign *p_campgn;
    struct Animation *p_anim;
    const char *campgn_mark;
    PathInfo *pinfo;
    int k;

    p_campgn = &campaigns[background_type];
    campgn_mark = p_campgn->ProjectorFnMk;
    // TODO FNAMES the convention with mark char is broken for "s"
    if (strcmp(campgn_mark, "s") == 0)
        campgn_mark = "m";

    k = anim_slots[anislot];
    p_anim = &animations[k];

    pinfo = &game_dirs[DirPlace_QEquip];

    switch (stage)
    {
    case ModDSt_BRT:
        anim_flic_set_fname(p_anim, "%s/%s%da%d.fli", pinfo->directory, campgn_mark, flic_mods[0], flic_mods[2]);
        break;
    case ModDSt_OUT:
        switch (part)
        {
        case ModDPt_CHEST:
            anim_flic_set_fname(p_anim, "%s/m%dbo.fli", pinfo->directory, old_flic_mods[0]);
            break;
        case ModDPt_BRAIN:
            anim_flic_set_fname(p_anim, "%s/m%dbbo.fli", pinfo->directory, old_flic_mods[0]);
            break;
        case ModDPt_ARMS:
            anim_flic_set_fname(p_anim, "%s/m%da%do.fli", pinfo->directory, old_flic_mods[0], old_flic_mods[2]);
            break;
        case ModDPt_LEGS:
            anim_flic_set_fname(p_anim, "%s/m%dl%do.fli", pinfo->directory, old_flic_mods[0], old_flic_mods[3]);
            break;
        default:
            assert(!"unreachable");
            break;
        }
        break;
    case ModDSt_IN:
        switch (part)
        {
          case ModDPt_CHEST:
            anim_flic_set_fname(p_anim, "%s/m%dbi.fli", pinfo->directory, flic_mods[0]);
            break;
          case ModDPt_BRAIN:
            anim_flic_set_fname(p_anim, "%s/m%dbbi.fli", pinfo->directory, flic_mods[0]);
            break;
          case ModDPt_ARMS:
            anim_flic_set_fname(p_anim, "%s/m%da%di.fli", pinfo->directory, flic_mods[0], flic_mods[2]);
            break;
          case ModDPt_LEGS:
            anim_flic_set_fname(p_anim, "%s/m%dl%di.fli", pinfo->directory, flic_mods[0], flic_mods[3]);
            break;
          default:
            assert(!"unreachable");
            break;
        }
        break;
    case 3:
        // No animation
        break;
    }
}

/** Returns scanline width for given RAW image width.
 *
 * RAW images are padded, meaning amount of bytes between lines
 * is not neccessarily equal to image width.
 */
short raw_file_scanline(short w)
{
    return w;
}

uint cryo_cyborg_framebuf_max_size(void)
{
    short h, scanln;

    h = equip_blokey_height[ModDPt_BKGND];
    scanln = raw_file_scanline(equip_blokey_width[ModDPt_BKGND]);

    return scanln * h;
}

inline static ubyte *cryo_cyborg_framebuf_back_ptr(void)
{
    return back_buffer - cryo_cyborg_framebuf_max_size();
}

void cryo_cyborg_mods_blokey_bkgnd_clear(ubyte *framebuf)
{
    short h, scanln;
    ubyte part;

    part = ModDPt_BKGND;
    scanln = raw_file_scanline(equip_blokey_width[part]);
    h = equip_blokey_height[part];

    LbMemorySet(framebuf, 0, scanln * h);
}

void cryo_cyborg_mods_blokey_bkgnd_to_buffer(ubyte *framebuf)
{
    long len;
    short h, scanln;
    ubyte part;

    part = ModDPt_BKGND;
    scanln = raw_file_scanline(equip_blokey_width[part]);
    h = equip_blokey_height[part];

    {
        char locstr[52];
        sprint_cryo_cyborg_mods_static_fname(locstr, part, flic_mods);
        len = LbFileLoadAt(locstr, framebuf);
    }
    if (len < 4) {
        LbMemorySet(framebuf, 0, scanln * h);
    }
}

void cryo_cyborg_mods_blokey_static_part_to_buffer(ubyte *partbuf, ubyte *mods_arr, ubyte part)
{
    long len;
    short w, h;

    w = equip_blokey_width[part];
    h = equip_blokey_height[part];

    {
        char locstr[52];
        sprint_cryo_cyborg_mods_static_fname(locstr, part, mods_arr);
        len = LbFileLoadAt(locstr, partbuf);
    }
    if (len < 4) {
        LbMemorySet(partbuf, 0, w * h);
    }
}

void cryo_cyborg_mods_blokey_static_to_buffer(ubyte *framebuf, ubyte *scratchbuf, ubyte *mods_arr)
{
    short frame_scanln;
    ubyte part;

    frame_scanln = raw_file_scanline(equip_blokey_width[ModDPt_BKGND]);

    for (part = 0; part < 4; part++)
    {
        ubyte *ldbuf;
        ubyte *blbuf;
        short w, h;

        if (mods_arr[part] == 0)
            continue;

        w = equip_blokey_width[part];
        h = equip_blokey_height[part];
        cryo_cyborg_mods_blokey_static_part_to_buffer(scratchbuf, mods_arr, part);

        // Blit the current part image onto framebuf
        ldbuf = scratchbuf;
        blbuf = framebuf;
        blbuf += (equip_blokey_pos[part].X - equip_blokey_pos[ModDPt_BKGND].X);
        blbuf += (equip_blokey_pos[part].Y - equip_blokey_pos[ModDPt_BKGND].Y) * frame_scanln;
        ApScreenCopyRectColorKey(ldbuf, blbuf, w, frame_scanln, h, 0);
    }
}

/** Clears output buffer of the animation at given slot.
 *
 * The animation must be opened, but its frame buffer
 * doesn't have to be set for this function to work.
 */
void flic_clear_output_buffer(ubyte anislot)
{
    struct Animation *p_anim;
    int k;

    k = anim_slots[anislot];
    p_anim = &animations[k];
    if (anim_is_opened(p_anim))
    {
        ubyte *obuf;
        short h;

        obuf = anim_type_get_output_buffer(p_anim->Type);

        for (h = p_anim->FLCFileHeader.Height; h > 0; h--)
        {
            LbMemorySet(obuf, '\0', p_anim->FLCFileHeader.Width);
            obuf += p_anim->FLCFileHeader.Width;
        }
    }
}

/** Background fill output buffer of the animation at given slot.
 */
void flic_bkgnd_fill_output_buffer_anim_out(ubyte anislot, ubyte skip_part)
{
    struct Animation *p_anim;
    int k;

    k = anim_slots[anislot];
    p_anim = &animations[k];
    if (anim_is_opened(p_anim))
    {
        ubyte *obuf;
        ubyte *bkgbuf;
        ubyte bkgn_part;

        bkgbuf = anim_type_get_output_buffer(AniSl_BKGND);
        obuf = anim_type_get_output_buffer(p_anim->Type);

        cryo_cyborg_mods_blokey_bkgnd_to_buffer(obuf);
        cryo_cyborg_mods_blokey_static_to_buffer(obuf, bkgbuf, old_flic_mods);

        // Now blit part of the background which we care about to the bkgbuf,
        // knowing we will play "out" anim for skip_part using that buffer
        bkgn_part = ModDPt_BKGND;
        obuf += equip_blokey_pos[skip_part].X - equip_blokey_pos[bkgn_part].X;
        obuf += (equip_blokey_pos[skip_part].Y - equip_blokey_pos[bkgn_part].Y) * equip_blokey_width[bkgn_part];
        ApScreenCopyRectColorKey(obuf, bkgbuf, equip_blokey_width[bkgn_part],
          equip_blokey_width[skip_part], equip_blokey_height[skip_part], 0);
    }
}

ubyte cryo_cyborg_mods_anim_get_stage(ubyte *p_part)
{
    ubyte part, stage;

    stage = ModDSt_BRT;

    if (stage == ModDSt_BRT)
    {
        for (part = 1; part < 4; part = (part+1) % 4)
        {
            if (((mod_draw_states[part] & ModDSt_Unkn08) != 0) &&
              ((mod_draw_states[part] & ModDSt_Unkn04) != 0)) {
                stage = ModDSt_OUT;
                break;
            }
            if (part == 0)
                break;
        }
    }

    if (stage == ModDSt_BRT)
    {
        for (part = 0; part < 4; part++)
        {
            if (((mod_draw_states[part] & ModDSt_Unkn08) != 0) &&
              ((mod_draw_states[part] & ModDSt_Unkn04) == 0) &&
              (flic_mods[part] != 0)) {
                stage = ModDSt_IN;
                break;
            }
        }
    }

    if (stage == ModDSt_BRT)
    {
        if (current_drawing_mod == 4) {
            part = current_drawing_mod;
            stage = 3;
        } else
        for (part = 0; part < 4; part++)
        {
            if (flic_mods[part] == 0) {
                stage = ModDSt_END;
                break;
            }
        }
    }

    *p_part = part;
    return stage;
}

void init_next_blokey_flic(void)
{
    ubyte part, stage;
    ubyte anislot;

    stage = cryo_cyborg_mods_anim_get_stage(&part);

    switch (stage)
    {
    case ModDSt_BRT:
        anislot = AniSl_CYBORG_BRTH;
        if (!byte_1DDC40)
        {
            byte_1DDC40 = 1;
            play_sample_using_heap(0, 134, 127, 64, 100, 0, 3u);
        }
        else if (!IsSamplePlaying(0, 134, 0))
        {
            cryo_cyborg_mods_anim_set_fname(anislot, part, stage);
            flic_unkn03(anislot);
            flic_clear_output_buffer(anislot);
            play_sample_using_heap(0, 126, 127, 64, 100, 0, 1u);
            current_frame = 0;
            new_current_drawing_mod = 4;
            byte_1DDC40 = 0;
        }
        break;
    case ModDSt_OUT:
        anislot = AniSl_CYBORG_INOUT;
        cryo_cyborg_mods_anim_set_fname(anislot, part, stage);
        flic_unkn03(anislot);
        old_flic_mods[part] = 0;
        flic_bkgnd_fill_output_buffer_anim_out(anislot, part);
        new_current_drawing_mod = part;
        mod_draw_states[part] |= ModDSt_ModAnimOut;
        play_sample_using_heap(0, 132, 127, 64, 100, 0, 3);
        cryo_blokey_box.Flags &= ~GBxFlg_RadioBtn;
        byte_1DDC40 = 0;
        break;
    case ModDSt_IN:
        anislot = AniSl_CYBORG_INOUT;
        cryo_cyborg_mods_anim_set_fname(anislot, part, stage);
        flic_unkn03(anislot);
        flic_clear_output_buffer(anislot);
        new_current_drawing_mod = part;
        mod_draw_states[part] |= ModDSt_ModAnimIn;
        mod_draw_states[part] &= ~ModDSt_Unkn08;
        old_flic_mods[part] = flic_mods[part];
        cryo_blokey_box.Flags &= ~GBxFlg_RadioBtn;
        byte_1DDC40 = 0;
        break;
    case ModDSt_END:
        // No further action
        break;
    }
}

/** Blit cyborg mod screen background to screen and back buffer.
 *
 * Requires the additive background image to be already loaded
 * into framebuf_back buffer. Should be executed once per screen
 * refresh - in consecutive frames, the background from back_buffer
 * is used, and this function updates the content of back buffer.
 */
void blokey_bkgnd_data_to_screen(void)
{
    TbPixel *p_inp;
    short scr_x, scr_y;
    short w, h;

    scr_x = cryo_blokey_box.X + 63;
    scr_y = cryo_blokey_box.Y + 1;
    w = equip_blokey_width[ModDPt_BKGND];
    h = equip_blokey_height[ModDPt_BKGND];

    p_inp = cryo_cyborg_framebuf_back_ptr();

    LbScreenSetGraphicsWindow(scr_x, scr_y, w, h);

    ApScreenCopyColorKey(p_inp, lbDisplay.GraphicsWindowPtr,
        lbDisplay.GraphicsWindowHeight, 0);

    LbScreenSetGraphicsWindow(0, 0, lbDisplay.GraphicsScreenWidth,
        lbDisplay.GraphicsScreenHeight);

    // Copy to back buffer - the back buffer should contain background shape,
    // but any mods on it should be redrawn each frame
    LbScreenCopyBox(lbDisplay.WScreen, back_buffer,
        scr_x, scr_y, scr_x, scr_y, w, h);
}

/** Blit a color keyed frame of cyborg animation from buffer to screen.
 */
void blokey_flic_data_to_screen(void)
{
    ubyte *inp;
    short scr_x, scr_y;
    short w, h;
    ubyte part;

    part = current_drawing_mod;
    scr_x = cryo_blokey_box.X + 63 + equip_blokey_pos[part].X;
    scr_y = cryo_blokey_box.Y + 1 + equip_blokey_pos[part].Y;
    w = equip_blokey_width[part];
    h = equip_blokey_height[part];

    inp = anim_type_get_output_buffer(AniSl_CYBORG_INOUT);

    LbScreenSetGraphicsWindow(scr_x, scr_y, w, h);

    ApScreenCopyColorKey(inp, lbDisplay.GraphicsWindowPtr,
        lbDisplay.GraphicsWindowHeight, 0);

    LbScreenSetGraphicsWindow(0, 0, lbDisplay.GraphicsScreenWidth,
        lbDisplay.GraphicsScreenHeight);
}

void blokey_static_flic_framebuf_back_make(void);

void blokey_static_flic_data_to_screen(void)
{
    TbPixel *p_inp;
    short scr_x, scr_y;
    short w, h;

    // TODO fill the buffer outside of drawlist
    blokey_static_flic_framebuf_back_make();

    scr_x = cryo_blokey_box.X + 63;
    scr_y = cryo_blokey_box.Y + 1;
    w = equip_blokey_width[ModDPt_BKGND];
    h = equip_blokey_height[ModDPt_BKGND];

    p_inp = cryo_cyborg_framebuf_back_ptr();

    LbScreenSetGraphicsWindow(scr_x, scr_y, w, h);

    ApScreenCopyColorKey(p_inp, lbDisplay.GraphicsWindowPtr,
        lbDisplay.GraphicsWindowHeight, 0);

    LbScreenSetGraphicsWindow(0, 0, lbDisplay.GraphicsScreenWidth,
        lbDisplay.GraphicsScreenHeight);

    // TODO REMOVE back buffer copy of mods foreground
    LbScreenCopyBox(lbDisplay.WScreen, back_buffer,
        scr_x, scr_y, scr_x, scr_y, w, h);
}

void blokey_static_flic_framebuf_back_make(void)
{
    TbPixel *p_framebuf;
    TbPixel *p_scratch;
    ubyte part;

    p_framebuf = cryo_cyborg_framebuf_back_ptr();
    p_scratch = anim_type_get_output_buffer(AniSl_CYBORG_INOUT);
    cryo_cyborg_mods_blokey_bkgnd_clear(p_framebuf);
    cryo_cyborg_mods_blokey_static_to_buffer(p_framebuf, p_scratch, flic_mods);

    for (part = 0; part < 4; part++)
    {
        if (flic_mods[part] == 0)
            continue;
        mod_draw_states[part] = ModDSt_Unkn04;
    }
}

ushort cryo_ordpart_to_mod_type(ubyte ordpart, ubyte mver)
{
    ushort mgroup;

    switch (ordpart)
    {
    default:
    case 0:
        mgroup = MODGRP_BRAIN;
        break;
    case 1:
        mgroup = MODGRP_ARMS;
        break;
    case 2:
        mgroup = MODGRP_CHEST;
        break;
    case 3:
        mgroup = MODGRP_EPIDERM;
        break;
    case 4:
        mgroup = MODGRP_LEGS;
        break;
    }
    return cybmod_type(mgroup, mver);
}

ubyte cryo_blokey_mod_level(ubyte ordpart)
{
    PlayerInfo *p_locplayer;
    ubyte mver;

    if (selected_agent < 0)
        return 0;

    switch (ordpart)
    {
    case 0:
        mver = flic_mods[ModDPt_BRAIN];
        break;
    case 1:
        mver = flic_mods[ModDPt_ARMS];
        break;
    case 2:
        mver = flic_mods[ModDPt_CHEST];
        break;
    case 3:
        p_locplayer = &players[local_player_no];
        if (selected_agent == 4)
        {
            mver = cybmod_skin_level(&p_locplayer->Mods[0]);
            if ((cybmod_skin_level(&p_locplayer->Mods[1]) != mver)
              || (cybmod_skin_level(&p_locplayer->Mods[2]) != mver)
              || (cybmod_skin_level(&p_locplayer->Mods[3]) != mver))
            {
              mver = 0;
            }
        }
        else
        {
            mver = cybmod_skin_level(&p_locplayer->Mods[selected_agent]);
        }
        break;
    case 4:
        mver = flic_mods[ModDPt_LEGS];
        break;
    }
    return mver;
}

void update_flic_mods(ubyte *mods)
{
    asm volatile ("call ASM_update_flic_mods\n"
        : : "a" (mods));
}

/** Draws body mods, either images or anims, on pre-drawn background.
 *
 * A background with person outline is expected to be already drawn
 * when this function is called. This function draws the actual mods only.
 */
void draw_blokey_body_mods(void)
{
#if 0
    asm volatile ("call ASM_draw_blokey_body_mods\n"
        :  :  : "eax" );
#endif
    int part;
    TbBool done, still_playing;

    current_drawing_mod = new_current_drawing_mod;
    if ((lbKeyOn[KC_SPACE] || game_projector_speed) && (cryo_blokey_box.Flags & GBxFlg_RadioBtn) == 0)
    {
        lbKeyOn[KC_SPACE] = 0;
        //TODO get rid of background blitting at this point
        draw_flic_purple_list(blokey_bkgnd_data_to_screen);
        {
            ubyte *inp;
            inp = cryo_cyborg_framebuf_back_ptr();
            cryo_cyborg_mods_blokey_bkgnd_to_buffer(inp);
        }
        //TODO blit the static data on every frame
        draw_flic_purple_list(blokey_static_flic_data_to_screen);
        update_flic_mods(old_flic_mods);
        update_flic_mods(flic_mods);

        for (part = 0; part < 4; part++)
            mod_draw_states[part] = 0;
        new_current_drawing_mod = 0;
        current_drawing_mod = 0;
        current_frame = 0;
        cryo_blokey_box.Flags |= GBxFlg_RadioBtn;
    }
    if ((mod_draw_states[0] & ModDSt_Unkn08) != 0)
    {
        if ((mod_draw_states[1] & ModDSt_Unkn04) != 0)
            mod_draw_states[1] |= ModDSt_Unkn08;
        if ((mod_draw_states[2] & ModDSt_Unkn04) != 0)
            mod_draw_states[2] |= ModDSt_Unkn08;
        if ((mod_draw_states[3] & ModDSt_Unkn04) != 0)
            mod_draw_states[3] |= ModDSt_Unkn08;
    }

    still_playing = 0;

    if (!still_playing)
    {
        for (part = 0; part < 4; part++)
        {
            if ((mod_draw_states[part] & ModDSt_ModAnimIn) == 0)
                continue;
            done = xdo_next_frame(AniSl_CYBORG_INOUT);
            still_playing = 1;
            draw_flic_purple_list(blokey_flic_data_to_screen);
            if (done != 0)
            {
                mod_draw_states[part] &= ~(ModDSt_ModAnimIn | ModDSt_Unkn04);
                mod_draw_states[part] |= ModDSt_Unkn04;
                if (old_flic_mods[part] != flic_mods[part])
                    mod_draw_states[part] |= (ModDSt_Unkn08 | ModDSt_Unkn04);
                copy_box_purple_list(cryo_blokey_box.X - 3, cryo_blokey_box.Y - 3,
                  cryo_blokey_box.Width + 6, cryo_blokey_box.Height + 6);
            }
            break;
        }
    }

    if (!still_playing)
    {
        for (part = 0; part < 4; part++)
        {
            if ((mod_draw_states[part] & ModDSt_ModAnimOut) == 0)
                continue;
            done = xdo_prev_frame(AniSl_CYBORG_INOUT);
            still_playing = 1;
            draw_flic_purple_list(blokey_flic_data_to_screen);
            if (done)
            {
                mod_draw_states[part] &= ~(ModDSt_ModAnimOut | ModDSt_Unkn04);
                if (flic_mods[part] != 0)
                    mod_draw_states[part] |= ModDSt_Unkn08;
                copy_box_purple_list(cryo_blokey_box.X - 3, cryo_blokey_box.Y - 3,
                  cryo_blokey_box.Width + 6, cryo_blokey_box.Height + 6);
            }
        }
    }

    if (!still_playing && (current_drawing_mod == 4))
    {
        done = xdo_next_frame(AniSl_CYBORG_BRTH);
        still_playing = !done;
        current_frame++;
        if (current_frame == 26) {
            play_sample_using_heap(0, 127, 127, 64, 100, 0, 1);
        } else if (current_frame == 52) {
            current_frame = 0;
            play_sample_using_heap(0, 126, 127, 64, 100, 0, 1);
        }
        draw_flic_purple_list(blokey_flic_data_to_screen);
    }
    if (!still_playing) {
        init_next_blokey_flic();
    }
}

ubyte draw_blokey_body_mods_names(struct ScreenBox *p_box)
{
    short cx, cy;
    short hline;
    ubyte ordpart;

    cx = p_box->X + 4;
    cy = p_box->Y + 20;
    hline = font_height('A');

    for (ordpart = 0; ordpart < 5; ordpart++)
    {
        const char *text;
        ubyte mver;

        mver = cryo_blokey_mod_level(ordpart);

        if (mver == 0)
        {
            if (ordpart == 3)
                cy += 2 * hline + 70;
            else
                cy += 2 * hline + 37;
            continue;
        }

        text = gui_strings[70 + ordpart];
        lbDisplay.DrawColour = 247;
        draw_text_purple_list2(cx, cy, text, 0);
        cy += hline + 3;

        text = cryo_gtext_cybmod_list_item_level(cryo_ordpart_to_mod_type(ordpart, mver));
        draw_text_purple_list2(cx, cy, text, 0);
        lbDisplay.DrawFlags = 0;
        if (ordpart == 3)
        {
            lbDisplay.DrawFlags = Lb_SPRITE_OUTLINE;
            draw_box_purple_list(cx, cy + hline + 3, 40, 40, lbDisplay.DrawColour);
            draw_sprite_purple_list(cx + 1, cy + hline + 4, &sprites_Icons0_0[163 + mver]);
            lbDisplay.DrawFlags = 0;
            cy += hline + 67;
        }
        else
        {
            cy += hline + 34;
        }
    }
    return 0;
}

/** Draws cryo agent with his cybernetic mods.
 * The general frow is:
 * - store additive background (shape of the agent) in framebuf_back
 * - draw the background within drawlist, update back_buffer with
 *   the background image
 * - clear framebuf_back and store color keyed images/frames with mods
 *   into that buffer
 * - continue updating and blitting colour keyed framebuf_back
 *   on each frame
 */
ubyte show_cryo_blokey(struct ScreenBox *p_box)
{
    if ((p_box->Flags & GBxFlg_BkgndDrawn) == 0)
    {
        ubyte *inp;

        draw_flic_purple_list(blokey_bkgnd_data_to_screen);
        p_box->Flags |= GBxFlg_BkgndDrawn;
        update_flic_mods(old_flic_mods);
        update_flic_mods(flic_mods);
        reset_mod_draw_states_flag08();
        current_drawing_mod = 0;

        inp = cryo_cyborg_framebuf_back_ptr();
        cryo_cyborg_mods_blokey_bkgnd_to_buffer(inp);
    }

    if (word_15511E != selected_agent)
        p_box->Flags &= ~GBxFlg_RadioBtn;
    word_15511E = selected_agent;
    lbFontPtr = small_med_font;
    my_set_text_window(0, 0, lbDisplay.GraphicsScreenWidth,
        lbDisplay.GraphicsScreenHeight);

    if (selected_agent < 0)
        return 0;

    draw_blokey_body_mods();
    draw_blokey_body_mods_names(p_box);
    return 0;
}

ubyte show_cryo_agent_list(struct ScreenTextBox *p_box)
{
    ubyte ret;
    asm volatile ("call ASM_show_cryo_agent_list\n"
        : "=r" (ret) : "a" (p_box));
    return ret;
}

TbBool cybmod_available_for_purchase(short mtype)
{
    PlayerInfo *p_locplayer;

    p_locplayer = &players[local_player_no];

    if (!is_research_cymod_completed(mtype)
      && ((login_control__State != 5) || mod_tech_level[mtype] > login_control__TechLevel))
        return false;

    if (selected_agent < 0)
        return false;

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

void draw_display_box_content_mod(struct ScreenTextBox *p_box)
{
    ubyte real_dbcontent;

    real_dbcontent = cybmod_has_display_anim(selected_mod + 1) ? display_box_content : DiBoxCt_TEXT;
    switch (real_dbcontent)
    {
    case DiBoxCt_TEXT:
        lbFontPtr = small_font;
        my_set_text_window(p_box->X + 4, p_box->ScrollWindowOffset + p_box->Y + 4,
          p_box->Width - 20, p_box->ScrollWindowHeight + 23);
        flashy_draw_text(0, 0, p_box->Text, p_box->TextSpeed, p_box->field_38,
          &p_box->TextFadePos, 0);
        break;
    case DiBoxCt_ANIM:
        if (p_box->TextFadePos < 0)
            // Mark that we should start animation frames the next time
            p_box->TextFadePos++;
        else
            xdo_next_frame(AniSl_EQVIEW);
        draw_flic_purple_list(ac_weapon_flic_data_to_screen);
        break;
    }
}

TbBool input_display_box_content_mod(struct ScreenTextBox *p_box)
{
    if (mouse_down_over_box_coords(p_box->X + 4, p_box->Y + 4,
      p_box->X + p_box->Width - 4, p_box->Y + 4 + 140))
    {
        if (lbDisplay.LeftButton)
        {
            lbDisplay.LeftButton = 0;
            display_box_content_state_switch();
            cryo_display_box_redraw(p_box);
            return true;
        }
    }
    return false;
}

/** Get global text pointer to a mod group name string.
 */
static const char *cryo_gtext_cybmod_list_item_name(ushort mtype)
{
    ubyte modgrp;
    ushort mdstr_id;

    modgrp = cybmod_group_type(mtype);
    mdstr_id = 70 + byte_1551F4[modgrp];
    return gui_strings[mdstr_id];
}

ubyte show_cryo_cybmod_list_box(struct ScreenTextBox *box)
{
    struct ScreenBoxBase power_box = {box->X + 8, box->Y + 152, 192, 17};
    struct ScreenBoxBase resil_box = {box->X + 8, box->Y + 177, 192, 17};
    struct ScreenBoxBase addit_box = {box->X + 8, box->Y + 200, 192, 19};

    if ((box->Flags & GBxFlg_BkgndDrawn) == 0)
    {
        lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        // Highlight behind both power_box and resil_box
        draw_box_purple_list(box->X + 4, box->Y + 149, box->Width - 8, 48, 56);
        // Highlight in bottom part of additiional info box
        draw_box_purple_list(addit_box.X - 4, addit_box.Y + 10, addit_box.Width + 8, addit_box.Height - 10 + 4, 56);

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

        lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        // Highlight behind the main list
        draw_box_purple_list(text_window_x1, text_window_y1,
          text_window_x2 - text_window_x1 + 1, text_window_y2 - text_window_y1 + 1, 56);

        cy = 3;
        text_h = font_height('A');
        for (mtype = box->field_38+1; mtype < MOD_TYPES_COUNT; mtype++)
        {
            if (text_h + cy >= box->ScrollWindowHeight + 23)
                return 0;
            if (cybmod_available_for_purchase(mtype))
            {
                  const char *text;

                  if (mouse_down_over_box_coords(text_window_x1, cy + text_window_y1 - 1,
                    text_window_x2, cy + text_window_y1 + 1 + text_h))
                  {
                      if (lbDisplay.LeftButton) {
                          lbDisplay.LeftButton = 0;
                          selected_mod = mtype - 1;
                          cryo_update_for_selected_cybmod();
                      }
                  }
                  if (selected_mod == mtype - 1) {
                      lbDisplay.DrawFlags = Lb_TEXT_ONE_COLOR;
                      lbDisplay.DrawColour = 87;
                  } else {
                      lbDisplay.DrawFlags = 0;
                  }

                  text = cryo_gtext_cybmod_list_item_name(mtype);
                  lbDisplay.DrawFlags |= 0x8000;
                  draw_text_purple_list2(3, cy + 1, text, 0);
                  lbDisplay.DrawFlags &= ~(0x8000|Lb_TEXT_HALIGN_RIGHT);

                  text = cryo_gtext_cybmod_list_item_level(mtype);
                  lbDisplay.DrawFlags |= Lb_TEXT_HALIGN_RIGHT;
                  draw_text_purple_list2(-1, cy + 1, text, 0);
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
        draw_display_box_content_mod(box);
        input_display_box_content_mod(box);

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
            equip_cost_box.Flags = (GBxFlg_NoBkCopy|GBxFlg_Unkn0001);
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
        cryo_cybmod_list_box.LineHeight = 0;
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
    cryo_agent_list_box.LineHeight = 25;
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

    update_cybmod_name_text();
    switch_equip_offer_to_buy();
    cryo_display_box_redraw(&cryo_cybmod_list_box);
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
