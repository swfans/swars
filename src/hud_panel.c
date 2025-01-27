/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file hud_panel.c
 *     Ingame Heads-Up Display panel draw.
 * @par Purpose:
 *     Implement functions drawing the HUD panel.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 Aug 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "hud_panel.h"

#include <assert.h>
#include "bfbox.h"
#include "bfgentab.h"
#include "bfline.h"
#include "bfmemut.h"
#include "bfplanar.h"
#include "bfscreen.h"
#include "bfsprite.h"
#include "bfutility.h"
#include "ssampply.h"

#include "app_sprite.h"
#include "bflib_render_drspr.h"
#include "bigmap.h"
#include "display.h"
#include "engintrns.h"
#include "game_data.h"
#include "game_speed.h"
#include "game_sprts.h"
#include "game.h"
#include "guiboxes.h"
#include "hud_panecfg.h"
#include "keyboard.h"
#include "lvobjctv.h"
#include "mouse.h"
#include "mydraw.h"
#include "packet.h"
#include "player.h"
#include "scandraw.h"
#include "sound.h"
#include "thing.h"
#include "swlog.h"
/******************************************************************************/
extern long dword_1DC36C;

/** Over which agent weapon the cursor is currently placed.
 *
 * Used just for the graphical detail of highliting current weapon when mouse over.
 */
sbyte agent_with_mouse_over_weapon = -1;

/** Momentary flags - filled and used only while updating the panel, then forgitten.
 */
enum PanelMomentaryFlags {
    PaMF_NONE = 0,
    PaMF_EXISTS = 0x01, /**< The panel exists on screen. */
    PaMF_DISABLED = 0x02, /**< The panel is for a disabled agent who cannot accept input (executing automated commands, unconscious, dying). */
    PaMF_SUBORDNT = 0x04, /**< The panel is for an agent with no direct control (subordinate due to grouping). */
};

struct GamePanel *game_panel;
struct TbPoint *game_panel_shifts;

TbBool panel_exists(short panel)
{
    struct GamePanel *p_panel;

    p_panel = &game_panel[panel];
    return (p_panel->Spr[0] != -1);
}

TbBool panel_for_speciifc_agent(short panel)
{
    struct GamePanel *p_panel;

    p_panel = &game_panel[panel];
    return (p_panel->Type == PanT_AgentBadge || p_panel->Type == PanT_AgentMood ||
      p_panel->Type == PanT_AgentHealth || p_panel->Type == PanT_AgentEnergy ||
      p_panel->Type == PanT_AgentMedi || p_panel->Type == PanT_AgentWeapon);
}

TbResult load_pop_sprites_for_current_mode(void)
{
    PathInfo *pinfo;
    const char *name;
    short styleno;
    short i, max_detail;
    TbResult ret;

    max_detail = 0;
    for (i = 0; i <= MAX_SUPPORTED_SCREEN_HEIGHT/180; i++) {
        if ((320 * (i+1) > lbDisplay.GraphicsScreenWidth) || (180 * (i+1) > lbDisplay.GraphicsScreenHeight))
            break;
        max_detail = i;
    }

    pinfo = &game_dirs[DirPlace_Data];
    if (ingame.PanelPermutation >= 0) {
        styleno = ingame.PanelPermutation;
        name = "panel";
    } else {
        styleno = -ingame.PanelPermutation - 1;
        name = "pop";
    }
    ret = load_pop_sprites_up_to(pinfo->directory, name, styleno, max_detail);
    setup_pop_sprites();
    size_panels_for_detail(pop1_sprites_scale - 1);
    return ret;
}

//TODO not the best location for agent state update
void update_dropped_item_under_agent_exists(short agent)
{
    struct SimpleThing *p_pickup;
    struct Thing *p_agent;
    ThingIdx thing;

    p_agent = &things[agent];

    if ((p_agent->Flag & TngF_Unkn08000000) == 0)
        return;

    thing = p_agent->U.UPerson.Vehicle; // Seem to be weapon standing over rather than vehicle
    if (thing < 0)
        p_pickup = &sthings[thing];
    else
        p_pickup = NULL;
    if ((p_pickup == NULL) || (p_pickup->Type != SmTT_DROPPED_ITEM)) {
        p_agent->Flag &= ~TngF_Unkn08000000;
    }
}

/* draws a sprite scaled to double size; remove pending */
void SCANNER_unkn_func_200(struct TbSprite *p_spr, int x, int y, ubyte col)
{
    int xwind_beg;
    int xwind_end;
    int xwind_start;
    sbyte *inp;
    ubyte *oline;
    int opitch;
    int h;
    TbBool needs_window_bounding;

    xwind_beg = lbDisplay.GraphicsWindowX;
    xwind_end = lbDisplay.GraphicsWindowX + lbDisplay.GraphicsWindowWidth;
    xwind_start = lbDisplay.GraphicsWindowX + x;
    inp = (sbyte *)p_spr->Data;
    opitch = lbDisplay.GraphicsScreenWidth;
    oline = &lbDisplay.WScreen[opitch * (lbDisplay.GraphicsWindowY + y) + lbDisplay.GraphicsWindowX + x];
    if (xwind_start < lbDisplay.GraphicsWindowX) {
        if (xwind_start + 2 * p_spr->SWidth <= lbDisplay.GraphicsWindowX)
            return;
        needs_window_bounding = true;
    } else {
        if (xwind_start >= xwind_end)
            return;
        needs_window_bounding = (xwind_start + 2 * p_spr->SWidth > xwind_end);
    }

    if (!needs_window_bounding)
    {
        // Simplified and faster drawing when we do not have to check bounds
        for (h = 0; h < p_spr->SHeight; h++)
        {
            ubyte *o;

            o = oline;
            while (*inp)
            {
                int ival;
                int i;

                ival = *inp;
                if (ival < 0)
                {
                    inp++;
                    o -= 2 * ival;
                    continue;
                }
                inp += ival + 1;
                for (i = 0; i < ival; i++)
                {
                    o[0] = col;
                    o[opitch + 0] = col;
                    o[1] = col;
                    o[opitch + 1] = col;
                    o += 2;
                }
            }
            inp++;
            oline += 2 * opitch;
        }
    }
    else
    {
        for (h = 0; h < p_spr->SHeight; h++)
        {
            ubyte *o;
            int xwind_curr;

            o = oline;
            xwind_curr = xwind_start;
            while (*inp)
            {
                int ival;
                int i;

                ival = *inp;
                if (ival < 0)
                {
                    inp++;
                    o -= 2 * ival;
                    xwind_curr -= 2 * ival;
                    continue;
                }
                inp += ival + 1;
                for (i = 0; i < ival; i++)
                {
                    if (xwind_curr >= xwind_beg && xwind_curr < xwind_end) {
                        o[0] = col;
                        o[opitch] = col;
                    }
                    xwind_curr++;
                    o++;
                    if (xwind_curr >= xwind_beg && xwind_curr < xwind_end) {
                        o[0] = col;
                        o[opitch] = col;
                    }
                    xwind_curr++;
                    o++;
                }
            }
            inp++;
            oline += 2 * opitch;
        }
    }
}

void SCANNER_unkn_func_203(int a1, int a2, int a3, int a4, ubyte a5, int a6, int a7)
{
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_SCANNER_unkn_func_203\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5), "g" (a6), "g" (a7));
}

int SCANNER_text_draw(const char *text, int start_x, int height)
{
    const ubyte *str;
    struct TbSprite *p_spr;
    int x;
    ubyte sel_c1;
    ubyte ch;
    TbPixel col;

    str = (const ubyte *)text;
    sel_c1 = SCANNER_colour[0];
    x = start_x;
    if (lbDisplay.GraphicsScreenHeight >= 400)
    {
      int chr_width, chr_height;

      while (*str != '\0')
      {
        if (*str == '\1') {
          str++;
          sel_c1 = *str;
        } else {
          ch = my_char_to_upper(*str);
          col = pixmap.fade_table[56 * PALETTE_8b_COLORS + sel_c1];
          p_spr = &small_font[ch - 31];
          chr_width = p_spr->SWidth * height / 9;
          chr_height = p_spr->SHeight * height / 9;
          LbSpriteDrawScaledOneColour(x, 2, p_spr, chr_width, chr_height, col);
          x += chr_width;
        }
        str++;
      }
    }
    else
    {
      while (*str != '\0')
      {
        if (*str == '\1') {
          str++;
          sel_c1 = *str;
        } else {
          ch = my_char_to_upper(*str);
          col = pixmap.fade_table[56 * PALETTE_8b_COLORS + sel_c1];
          p_spr = &small_font[ch - 31];
          LbSpriteDrawOneColour(x, 1, p_spr, col);
          x += p_spr->SWidth;
        }
        str++;
      }
    }
    return x;
}

short panel_state_to_player_agent(ushort panstate)
{
    if ((panstate >= PANEL_STATE_WEP_SEL_ONE) && (panstate <= PANEL_STATE_MOOD_SET_GRP + 3))
        return (panstate - 1) & 3;
    return -1;
}

void SCANNER_move_objective_info(int width, int height, int end_pos)
{
    PlayerInfo *p_locplayer;

    p_locplayer = &players[local_player_no];
    if (in_network_game && p_locplayer->PanelState[mouser] == PANEL_STATE_SEND_MESSAGE)
    {
      if ( end_pos < lbDisplay.PhysicalScreenWidth - (lbDisplay.PhysicalScreenWidth >> 2) )
          scanner_unkn370 = -20;
      if (end_pos > lbDisplay.PhysicalScreenWidth - 16)
          scanner_unkn370 = 10;
      if (scanner_unkn370 > 0)
      {
          scanner_unkn370--;
          scanner_unkn3CC -= 1 * height / 9;
      }
      if (scanner_unkn370 < 0)
      {
          scanner_unkn370++;
          scanner_unkn3CC += 1 * height / 9;
          if (scanner_unkn3CC > 0)
              scanner_unkn3CC = 0;
      }
    }
    else
    {
        if (end_pos < 0)
            scanner_unkn3CC = width;
        scanner_unkn3CC -= 2 * height / 9;
    }
}

void draw_text_linewrap1(int base_x, int *p_pos_y, int plyr, const char *text)
{
    int pos_x, pos_y;
    const char *str;
    int base_shift;
    TbPixel col2;

    str = text;
    pos_x = base_x;
    pos_y = *p_pos_y;
    col2 = byte_1C5C30[plyr];
    base_shift = -180;
    while (*str != 0)
    {
        if (*str == 32)
        {
            if (pos_x + 2 * font_word_length(str + 1) < lbDisplay.PhysicalScreenWidth - 16) {
                pos_x += 8;
            } else {
                pos_x = base_x;
                pos_y += 12;
            }
        }
        else
        {
            struct TbSprite *p_spr;
            int fd;
            ubyte ch;
            TbPixel col1;

            ch = my_char_to_upper(*str);
            p_spr = &small_font[ch - 31];
            fd = base_shift + 4 * player_unkn0C9[plyr];
            if (fd > 63)
                fd = 63 - (fd - 63);
            if (fd > 63)
                fd = 63;
            if (fd < 0)
                fd = 0;
            col1 = pixmap.fade_table[256 * fd + colour_lookup[8]];
            SCANNER_unkn_func_200(p_spr, pos_x + 1, pos_y + 1, col1);
            SCANNER_unkn_func_200(p_spr, pos_x, pos_y, col2);
            pos_x += 2 * p_spr->SWidth;
        }
        str++;
        base_shift++;
    }
    pos_y += 12;
    *p_pos_y = pos_y;
}

void draw_text_linewrap2(int base_x, int *p_pos_y, int plyr, const char *text)
{
    int pos_x, pos_y;
    const char *str;
    int base_shift;
    TbPixel col2;

    str = text;
    pos_x = base_x;
    pos_y = *p_pos_y;
    col2 = byte_1C5C30[plyr];
    base_shift = -180;
    while (*str != 0)
    {
        if (*str == 32)
        {
            if (font_word_length(str + 1) + pos_x < lbDisplay.PhysicalScreenWidth - 8) {
                pos_x += 4;
            } else {
                pos_x = base_x;
                pos_y += 6;
            }
        }
        else
        {
            struct TbSprite *p_spr;
            int fd;
            ubyte ch;
            TbPixel col1;

            ch = my_char_to_upper(*str);
            p_spr = &small_font[ch - 31];
            fd = base_shift + 4 * (ubyte)player_unkn0C9[plyr];
            if (fd > 63)
                fd = 63 - (fd - 63);
            if (fd > 63)
                fd = 63;
            if (fd < 0)
                fd = 0;
            col1 = pixmap.fade_table[256 * fd + colour_lookup[8]];
            LbSpriteDrawOneColour(pos_x + 1, pos_y + 1, p_spr, col1);
            LbSpriteDrawOneColour(pos_x, pos_y, p_spr, col2);
            pos_x += p_spr->SWidth;
        }
        str++;
        base_shift++;
    }
    pos_y += 6;
    *p_pos_y = pos_y;
}

void draw_players_chat_talk(int x, int y)
{
    char locstr[164];
    int plyr;
    int base_x, pos_y;

    base_x = x;
    pos_y = y;

    for (plyr = 0; plyr < PLAYERS_LIMIT; plyr++)
    {
        char *plname;

        if (player_unkn0C9[plyr] == 0)
            continue;

        plname = unkn2_names[plyr];
        if (player_unknCC9[plyr][0] != '\0')
        {
            if (plname[0] != '\0')
                sprintf(locstr, "%s: %s", plname, player_unknCC9[plyr]);
            else
                sprintf(locstr, "%s", player_unknCC9[plyr]);
        }
        else
        {
            sprintf(locstr, "%s said nothing.", plname);
        }

        if (lbDisplay.GraphicsScreenHeight >= 400)
        {
            draw_text_linewrap1(base_x, &pos_y, plyr, locstr);
        }
        else
        {
            draw_text_linewrap2(base_x, &pos_y, plyr, locstr);
        }

        if ( !--player_unkn0C9[plyr] ) {
            player_unknCC9[plyr][0] = '\0';
        }
    }
}

void SCANNER_draw_objective_info(int x, int y, int width)
{
#if 0
    asm volatile (
      "call ASM_SCANNER_draw_objective_info\n"
        : : "a" (x), "d" (y), "b" (width));
#endif
    int v48;
    int end_pos;
    struct TbAnyWindow bkpwnd;
    int height;
    int i;

    height = panel_get_objective_info_height(lbDisplay.GraphicsScreenHeight);
    v48 = y;
    for (i = 0; i < height; i++)
    {
        SCANNER_unkn_func_203(x, v48, x + width - 1, v48, SCANNER_colour[0],
          ingame.Scanner.Brightness, ingame.Scanner.Contrast);
        ++v48;
    }

    LbScreenStoreGraphicsWindow(&bkpwnd);
    LbScreenSetGraphicsWindow(x + 1, y, width - 2, height);

    end_pos = SCANNER_text_draw(scroll_text, scanner_unkn3CC, height);

    SCANNER_move_objective_info(width, height, end_pos);

    LbScreenLoadGraphicsWindow(&bkpwnd);

    // TODO it would make sense to move this to higher level function
    if (in_network_game)
    {
        short x, y;

        if (lbDisplay.GraphicsScreenHeight >= 400) {
            x = 22;
            y = 51;
        } else {
            x = 11;
            y = 26;
        }
        draw_players_chat_talk(x, y);
    }
}

void SCANNER_unkn_func_205(void)
{
    asm volatile ("call ASM_SCANNER_unkn_func_205\n"
        :  :  : "eax" );
}

TbBool check_scanner_input(void)
{
    int map_x, map_y, map_z;

    SCANNER_find_position(lbDisplay.MouseX, lbDisplay.MouseY, &map_z, &map_x);
    if (MAPCOORD_TO_TILE(map_x) >= 0 && MAPCOORD_TO_TILE(map_x) < 128
      && MAPCOORD_TO_TILE(map_z) >= 0 && MAPCOORD_TO_TILE(map_z) < 128)
    {
        PlayerInfo *p_locplayer;

        p_locplayer = &players[local_player_no];

        if (lbDisplay.LeftButton)
        {
            struct SpecialUserInput *p_usrinp;
            struct Packet *p_pckt;
            TbBool can_control;
            short dcthing;

            dcthing = p_locplayer->DirectControl[mouser];
            can_control = person_can_accept_control(dcthing);
            p_usrinp = &p_locplayer->UserInput[mouser];
            p_pckt = &packets[local_player_no];

            lbDisplay.LeftButton = 0;
            p_usrinp->ControlMode |= 0x8000;
            if ((p_locplayer->DoubleMode) || ((p_usrinp->ControlMode & 0x1FFF) == 1))
            {
                map_y = (alt_at_point(map_x, map_z) >> 8) + 20;
                if ((gameturn & 0x7FFF) - p_usrinp->Turn >= 7)
                {
                    p_usrinp->Turn = gameturn & 0x7FFF;
                    if (can_control)
                        my_build_packet(p_pckt, PAct_AGENT_GOTO_GND_PT_ABS, dcthing, map_x, map_y, map_z);
                }
                else
                {
                    p_usrinp->Turn = 0;
                    if (can_control)
                        my_build_packet(p_pckt, PAct_AGENT_GOTO_GND_PT_ABS_FF, dcthing, map_x, map_y, map_z);
                }
            }
            else
            {
                do_change_mouse(8);
                build_packet(p_pckt, PAct_CONTROL_MODE, 1, 0, 0, 0);
            }
            return true;
        }

        if (lbDisplay.RightButton)
        {
            struct SpecialUserInput *p_usrinp;
            struct Packet *p_pckt;
            TbBool can_control;
            short dcthing;
            short cwep;

            dcthing = p_locplayer->DirectControl[mouser];
            can_control = person_can_accept_control(dcthing);
            p_usrinp = &p_locplayer->UserInput[mouser];

            lbDisplay.RightButton = 0;
            p_usrinp->ControlMode |= 0x4000;
            if (!p_locplayer->DoubleMode)
            {
                map_y = (alt_at_point(map_x, map_z) >> 8) + 20;
                dcthing = p_locplayer->DirectControl[mouser];
                cwep = things[dcthing].U.UPerson.CurrentWeapon;
                p_pckt = &packets[local_player_no];

                if ((cwep == WEP_ELEMINE) || (cwep == WEP_EXPLMINE))
                {
                    if ((gameturn & 0x7FFF) - p_usrinp->Turn >= 7)
                    {
                        p_usrinp->Turn = gameturn & 0x7FFF;
                        if (can_control)
                        {
                            if (p_locplayer->TargetType == 3)
                                my_build_packet(p_pckt, PAct_PLANT_MINE_AT_FACE_PT, dcthing, map_x, p_locplayer->Target, map_z);
                            else
                                my_build_packet(p_pckt, PAct_PLANT_MINE_AT_GND_PT, dcthing, map_x, map_y, map_z);
                        }
                    }
                    else
                    {
                        p_usrinp->Turn = 0;
                        if (can_control)
                        {
                            if (p_locplayer->TargetType == 3)
                                my_build_packet(p_pckt, PAct_PLANT_MINE_AT_FACE_PT_FF, dcthing, map_x, p_locplayer->Target, map_z);
                            else
                                my_build_packet(p_pckt, PAct_PLANT_MINE_AT_GND_PT_FF, dcthing, map_x, map_y, map_z);
                        }
                    }
                }
                else if ((cwep == WEP_RAZORWIRE) || (cwep == WEP_EXPLWIRE))
                {
                    if ((gameturn & 0x7FFF) - p_usrinp->Turn >= 7)
                    {
                        p_usrinp->Turn = gameturn & 0x7FFF;
                        if (can_control)
                        {
                            if (p_locplayer->TargetType == 3)
                                my_build_packet(p_pckt, PAct_SHOOT_AT_FACE_POINT, dcthing, map_x, p_locplayer->Target, map_z);
                            else
                                my_build_packet(p_pckt, PAct_SHOOT_AT_GND_POINT, dcthing, map_x, map_y, map_z);
                        }
                    }
                    else
                    {
                        p_usrinp->Turn = 0;
                        if (can_control)
                        {
                            if (p_locplayer->TargetType == 3)
                                my_build_packet(p_pckt, PAct_SHOOT_AT_FACE_POINT_FF, dcthing, map_x, p_locplayer->Target, map_z);
                            else
                                my_build_packet(p_pckt, PAct_SHOOT_AT_GND_POINT_FF, dcthing, map_x, map_y, map_z);
                        }
                    }
                }
                else
                {
                    if (can_control)
                    {
                        if (p_locplayer->TargetType == 3)
                            my_build_packet(p_pckt, PAct_SHOOT_AT_FACE_POINT, dcthing, map_x, p_locplayer->Target, map_z);
                        else
                            my_build_packet(p_pckt, PAct_SHOOT_AT_GND_POINT, dcthing, map_x, map_y, map_z);
                    }
                }
            }
            return true;
        }
    }
    return false;
}

/**
 * Draw the panel button with standard palette, for selectable items.
 * @param px
 * @param py
 * @param spr_id
 */
void draw_new_panel_sprite_std(int px, int py, ulong spr_id)
{
    struct TbSprite *p_spr;

    p_spr = &pop1_sprites[spr_id];
    dword_1DC36C = ingame.Scanner.Brightness;

    if (ingame.PanelPermutation == -1) {
        lbDisplay.DrawFlags |= Lb_SPRITE_TRANSPAR4;
        ApSpriteDrawLowTransGreyRemap(px, py, p_spr,
          &pixmap.fade_table[0 * PALETTE_8b_COLORS]);
        lbDisplay.DrawFlags &= ~Lb_SPRITE_TRANSPAR4;
    } else {
        // We do not want to scale brightness of non-transparent panels - using a standard function
        LbSpriteDraw(px, py, p_spr);
    }
}

/**
 * Draw the rescaled panel button with standard palette, for selectable items.
 * @param px
 * @param py
 * @param spr_id
 */
void draw_new_panel_sprite_scaled_std(int px, int py, ulong spr_id, int dest_width, int dest_height)
{
    struct TbSprite *p_spr;

    p_spr = &pop1_sprites[spr_id];
    dword_1DC36C = ingame.Scanner.Brightness;

    if (ingame.PanelPermutation == -1) {
        lbDisplay.DrawFlags |= Lb_SPRITE_TRANSPAR4;
        ApSpriteDrawScaledLowTransGreyRemap(px, py, p_spr, dest_width, dest_height,
          &pixmap.fade_table[0 * PALETTE_8b_COLORS]);
        lbDisplay.DrawFlags &= ~Lb_SPRITE_TRANSPAR4;
    } else {
        LbSpriteDrawScaled(px, py, p_spr, dest_width, dest_height);
    }
}

/**
 * Draw the panel button with darkened palette, like the item is unavailable.
 * @param px
 * @param py
 * @param spr_id
 */
void draw_new_panel_sprite_dark(int px, int py, ulong spr_id)
{
    struct TbSprite *p_spr;

    p_spr = &pop1_sprites[spr_id];
    dword_1DC36C = 8;

    if (ingame.PanelPermutation == -1) {
        lbDisplay.DrawFlags |= Lb_SPRITE_TRANSPAR4;
        ApSpriteDrawLowTransGreyRemap(px, py, p_spr,
          &pixmap.fade_table[0 * PALETTE_8b_COLORS]);
        lbDisplay.DrawFlags &= ~Lb_SPRITE_TRANSPAR4;
    } else {
        LbSpriteDrawRemap(px, py, p_spr, &pixmap.fade_table[16 * PALETTE_8b_COLORS]);
    }
}

/**
 * Draw the rescaled panel button with darkened palette, like the item is unavailable.
 * @param px
 * @param py
 * @param spr_id
 */
void draw_new_panel_sprite_scaled_dark(int px, int py, ulong spr_id, int dest_width, int dest_height)
{
    struct TbSprite *p_spr;

    p_spr = &pop1_sprites[spr_id];
    dword_1DC36C = 8;

    if (ingame.PanelPermutation == -1) {
        lbDisplay.DrawFlags |= Lb_SPRITE_TRANSPAR4;
        ApSpriteDrawScaledLowTransGreyRemap(px, py, p_spr, dest_width, dest_height,
          &pixmap.fade_table[0 * PALETTE_8b_COLORS]);
        lbDisplay.DrawFlags &= ~Lb_SPRITE_TRANSPAR4;
    } else {
        LbSpriteDrawScaledRemap(px, py, p_spr, dest_width, dest_height,
          &pixmap.fade_table[16 * PALETTE_8b_COLORS]);
    }
}

/**
 * Draw the button with standard palette, version for old single weapon list.
 * @param px
 * @param py
 * @param spr_id
 */
void draw_new_panel_sprite_prealp(int px, int py, ulong spr_id)
{
    struct TbSprite *p_spr;

    p_spr = &pop1_sprites[spr_id];
    dword_1DC36C = ingame.Scanner.Brightness;

    if (ingame.PanelPermutation == -1) {
        lbDisplay.DrawFlags |= Lb_SPRITE_TRANSPAR4;
        ApSpriteDrawLowTransGreyRemap(px, py, p_spr,
          &pixmap.fade_table[0 * PALETTE_8b_COLORS]);
        lbDisplay.DrawFlags &= ~Lb_SPRITE_TRANSPAR4;
    }
}

/**
 * Draws squares for fourpacks, given specific screen coords and amount to fill.
 *
 * @param x
 * @param y
 * @param amount
 */
void draw_fourpack_amount(short x, ushort y, ushort amount)
{
    int i;
    TbPixel col;

    if (ingame.PanelPermutation == -3)
        col = 26;
    else
        col = 247;

    for (i = 0; i < min(amount,8); i++)
    {
        struct TbPoint *p_shift;
        struct TbPoint *p_size;

        p_shift = &game_panel_shifts[PaSh_WEP_FOURPACK_SLOTS + i];
        p_size = &game_panel_shifts[PaSh_WEP_FOURPACK_SIZE];
        LbDrawBox(x + p_shift->x, y + p_shift->y, p_size->x, p_size->y, col);
    }
}

TbBool panel_agents_weapon_highlighted(PlayerInfo *p_locplayer, ushort plagent, short weptype)
{
    return ((weptype == p_locplayer->PanelItem[mouser]) && (agent_with_mouse_over_weapon == plagent));
}

/**
 * For weapons which contain up to four itemized parts, draw the items.
 *
 * @param x
 * @param y
 * @param plagent
 * @param weptype
 */
void draw_fourpack_items(short x, short y, short plagent, short weptype)
{
    ushort fp;

    fp = weapon_fourpack_index(weptype);
    if (fp < WFRPK_COUNT) {
        PlayerInfo *p_locplayer;
        p_locplayer = &players[local_player_no];
        draw_fourpack_amount(x, y, p_locplayer->FourPacks[fp][plagent]);
    }
}

TbBool draw_panel_pickable_thing_below_agent(struct Thing *p_agent)
{
    struct SimpleThing *p_pickup;
    ThingIdx thing;
    TbBool drawn;

    if ((p_agent->Flag & TngF_Unkn08000000) == 0)
        return false;

    thing = p_agent->U.UPerson.Vehicle; // Seem to be weapon standing over rather than vehicle
    if (thing < 0)
        p_pickup = &sthings[thing];
    else
        p_pickup = NULL;

    drawn = false;
    if ((p_pickup != NULL) && (p_pickup->Type == SmTT_DROPPED_ITEM))
    {
        ushort weptype;
        short x, y;
        ushort spr;

        {
            struct TbSprite *p_spr;

            p_spr = &pop1_sprites[12];
            x = lbDisplay.GraphicsScreenWidth - 8 * pop1_sprites_scale - p_spr->SWidth;
            y = lbDisplay.GraphicsScreenHeight - 8 * pop1_sprites_scale - p_spr->SHeight;
        }
        lbDisplay.DrawFlags = 0;
        weptype = p_pickup->U.UWeapon.WeaponType;

        draw_new_panel_sprite_std(x, y, 12);
        if (weptype)
            spr = weapon_sprite_index(weptype, false);
        else
            spr = 70;
        x += game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].x;
        y += game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].y;
        draw_new_panel_sprite_std(x, y, spr);
        drawn = true;
    }
    return drawn;
}

TbBool draw_panel_pickable_thing_player_targeted(PlayerInfo *p_locplayer)
{
    struct SimpleThing *p_pickup;
    ThingIdx thing;
    TbBool drawn;

    if (p_locplayer->TargetType != 5)
        return false;

    thing = p_locplayer->field_102;
    if (thing < 0)
        p_pickup = &sthings[thing];
    else
        p_pickup = NULL;

    drawn = false;
    if (p_pickup != NULL)
    {
        ushort weptype;
        short x, y;
        ushort spr;

        {
            struct TbSprite *p_spr;

            p_spr = &pop1_sprites[12];
            x = lbDisplay.GraphicsScreenWidth - 8 * pop1_sprites_scale - p_spr->SWidth;
            y = lbDisplay.GraphicsScreenHeight - 8 * pop1_sprites_scale - p_spr->SHeight;
        }
        lbDisplay.DrawFlags = 0;
        weptype = p_pickup->U.UWeapon.WeaponType;

        draw_new_panel_sprite_std(x, y, 12);
        if (weptype)
            spr = weapon_sprite_index(weptype, false);
        else
            spr = 70;
        x += game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].x;
        y += game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].y;
        draw_new_panel_sprite_std(x, y, spr);
        drawn = true;
    }
    return drawn;
}

/**
 * Counts weapons in given flags, returning total, amount below given weapon and above it.
 * To be used for displaying scrollable list of weapons where current weapon is always visible.
 *
 * @param p_ncarr_below
 * @param p_ncarr_above
 * @param weapons_carried
 * @param current_weapon
 * @return
 */
int count_weapons_in_flags(int *p_ncarr_below, int *p_ncarr_above, ulong weapons_carried, short current_weapon)
{
    int ncarried, ncarr_below, ncarr_above;
    ulong wepflags;
    ushort nweptype;

    ncarried = 0;
    ncarr_above = 0;
    ncarr_below = 0;
    wepflags = weapons_carried;

    for (nweptype = 1; nweptype < WEP_TYPES_COUNT; nweptype++, wepflags >>= 1)
    {
        if (wepflags == 0)
            break;
        if (wepflags & 1)
        {
            ncarried++;
            if (nweptype > current_weapon)
                ncarr_above++;
            if (nweptype < current_weapon)
                ncarr_below++;
        }
    }

    *p_ncarr_below = ncarr_below;
    *p_ncarr_above = ncarr_above;
    return ncarried;
}

void draw_agent_carried_weapon(PlayerInfo *p_locplayer, ushort plagent, short slot, TbBool ready, short weptype, short cx, short cy)
{
    TbBool wep_highlight;
    TbBool recharging;
    short x, y;
    ushort spr;

    recharging = p_locplayer->WepDelays[plagent][weptype] != 0;
    wep_highlight = panel_agents_weapon_highlighted(p_locplayer, plagent, weptype);

    lbDisplay.DrawFlags = 0;
    if (!recharging || (gameturn & 1))
    {
        if (slot == 1) { // First weapon on list entry starts its sprite a bit earlier
            x = cx + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_DECOR].x;
            y = cy + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_DECOR].y;
            spr = 13;
        } else {
            x = cx + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_DECOR].x;
            y = cy + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_DECOR].y;
            spr = 94;
        }
        draw_new_panel_sprite_std(x, y, spr);

        if (slot == 1) {
            x = cx + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_SYMBOL].x;
            y = cy + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_SYMBOL].y;
        } else {
            x = cx + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_SYMBOL].x;
            y = cy + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_SYMBOL].y;
        }
        draw_new_panel_sprite_std(x, y, weapon_sprite_index(weptype, ready));
    }

    if (wep_highlight)
    {
        lbDisplay.DrawFlags = 0;
        if (slot == 1) {// The first on list longer sprite has its own highlight
            x = cx + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_DECOR].x;
            y = cy + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_DECOR].y;
            spr = 92;
        } else {
            x = cx + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_DECOR].x;
            y = cy + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_DECOR].y;
            spr = 93;
        }
        draw_new_panel_sprite_std(x, y, spr);
    }

    if (slot == 1) {
        x = cx + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_SYMBOL].x;
        y = cy + game_panel_shifts[PaSh_WEP_FRST_BTN_TO_SYMBOL].y;
    } else {
        x = cx + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_SYMBOL].x;
        y = cy + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_SYMBOL].y;
    }

    draw_fourpack_items(x, y, plagent, weptype);
}

void draw_agent_current_weapon(PlayerInfo *p_locplayer, ushort plagent, short slot, TbBool darkened, TbBool ready, short weptype, short cx, short cy)
{
    TbBool wep_highlight;
    TbBool recharging;
    short x, y;

    recharging = p_locplayer->WepDelays[plagent][weptype] != 0;
    wep_highlight = panel_agents_weapon_highlighted(p_locplayer, plagent, weptype);

    if (!recharging || (gameturn & 1))
    {
        x = cx + game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].x;
        y = cy + game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].y;
        if (ready)
            draw_new_panel_sprite_prealp(x, y, 14);

        if (!darkened) {
            draw_new_panel_sprite_std(x, y, weapon_sprite_index(weptype, ready));
        } else {
            draw_new_panel_sprite_dark(x, y, weapon_sprite_index(weptype, ready));
        }
    }

    // Additional border highlight around button on mouse over
    if (wep_highlight)
    {
        draw_new_panel_sprite_std(cx, cy, 90);
    }

    x = cx + game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].x;
    y = cy + game_panel_shifts[PaSh_WEP_CURR_BTN_TO_SYMBOL].y;
    draw_fourpack_items(x, y, plagent, weptype);
}

void draw_agent_carried_weapon_prealp_list(PlayerInfo *p_locplayer, ushort plagent, short slot, TbBool ready, short weptype, short cx, short cy)
{
    TbBool wep_highlight;
    TbBool recharging;
    short x, y;

    recharging = p_locplayer->WepDelays[plagent][weptype] != 0;
    wep_highlight = panel_agents_weapon_highlighted(p_locplayer, plagent, weptype);

    x = cx + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_SYMBOL].x;
    y = cy + game_panel_shifts[PaSh_WEP_NEXT_BTN_TO_SYMBOL].y;

    lbDisplay.DrawFlags = 0;
    if (!recharging || (gameturn & 1))
    {
        if (slot == 6)
            draw_new_panel_sprite_std(cx, cy, 13);
        else
            draw_new_panel_sprite_std(cx, cy, 12);

        draw_new_panel_sprite_std(x, y, weapon_sprite_index(weptype, false));
    }
    if (ready) {
        draw_new_panel_sprite_std(x, y, weapon_sprite_index(weptype, true));
    }

    if (wep_highlight)
    {
        draw_new_panel_sprite_std(cx, cy, 90);
    }

    draw_fourpack_items(x, y, plagent, weptype);
}

TbBool panel_mouse_over_weapon(short box_x, short box_y, short box_w, short box_h, int panstate, short box_no)
{
    short msx, msy;

    if ((panstate >= PANEL_STATE_WEP_SEL_GRP) && (panstate < PANEL_STATE_WEP_SEL_GRP + 4))
    {
        if (!lbDisplay.MRightButton) {
            msx = lbDisplay.RMouseX;
            msy = lbDisplay.RMouseY;
            if (over_box(msx, msy, box_x, box_y, box_w, box_h))
                return true;
        }
    }
    {
        msx = lbDisplay.MMouseX;
        msy = lbDisplay.MMouseY;
        if (over_box(msx, msy, box_x, box_y, box_w, box_h))
            return true;
    }
    return false;
}

/** Updates a list of weapons in pre-alpha style.
 *
 * This function is intended to loop through weapons in the same way
 * as draw_weapons_list_prealp(), but update state instead of drawing.
 */
TbBool update_weapons_list_prealp(PlayerInfo *p_locplayer, ushort plagent, ulong weapons_carried, short current_weapon)
{
    struct GamePanel *p_panel;
    ushort nshown;
    int weptype;
    int nchecked;
    ulong wepflags;
    short cx, cy;
    int ncarried;
    int ncarr_below;
    int ncarr_above;
    TbBool ret;

    ret = false;
    ncarried = count_weapons_in_flags(&ncarr_below, &ncarr_above, weapons_carried, current_weapon);
    // Show at least 4 weapons below the currently selected
    if (ncarried <= 6)
        ncarr_below = 0;
    else if (ncarr_below <= 4)
        ncarr_below = 0;
    else
        ncarr_below -= 4;
    p_panel = &game_panel[26];
    cx = p_panel->pos.X;
    cy = p_panel->pos.Y;

    nshown = 0;
    wepflags = weapons_carried;
    nchecked = 0;
    for (weptype = 1; weptype < WEP_TYPES_COUNT; weptype++, wepflags >>= 1)
    {
        if (wepflags == 0)
            break;
        if ((wepflags & 1) == 0)
            continue;
        if (nshown >= ncarr_below)
        {
            TbBool wep_highlight;
            short w, h;

            w = max(game_panel_shifts[PaSh_WEP_NEXT_BUTTON_AREA].x + 2, game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].x - 1);
            h = max(game_panel_shifts[PaSh_WEP_NEXT_BUTTON_AREA].y, game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].y - 1);
            wep_highlight = panel_mouse_over_weapon(cx - 1, cy, w, h, PANEL_STATE_WEP_SEL_GRP + plagent, 0);

            if (wep_highlight)
            {
                p_locplayer->PanelItem[mouser] = weptype;
                agent_with_mouse_over_weapon = plagent;
                ret = true;
                break;
            }

            cx += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].x;
            cy += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].y;
            ++nchecked;
        }
        nshown++;
        if (nchecked == 6)
            break;
    }
    return ret;
}

void draw_weapons_list_prealp(PlayerInfo *p_locplayer, ushort plagent, ulong weapons_carried, short current_weapon)
{
    struct GamePanel *p_panel;
    ushort nshown;
    ushort weptype;
    int nchecked;
    ulong wepflags;
    short cx, cy;
    int ncarried;
    int ncarr_below;
    int ncarr_above;

    ncarried = count_weapons_in_flags(&ncarr_below, &ncarr_above, weapons_carried, current_weapon);
    // Show at least 4 weapons below the currently selected
    if (ncarried <= 6)
        ncarr_below = 0;
    else if (ncarr_below <= 4)
        ncarr_below = 0;
    else
        ncarr_below -= 4;
    p_panel = &game_panel[26];
    cx = p_panel->pos.X;
    cy = p_panel->pos.Y;

    nshown = 0;
    wepflags = weapons_carried;
    nchecked = 0;
    for (weptype = 1; weptype < WEP_TYPES_COUNT; weptype++, wepflags >>= 1)
    {
        if (wepflags == 0)
            break;
        if ((wepflags & 1) == 0)
            continue;
        if (nshown >= ncarr_below)
        {
            draw_agent_carried_weapon_prealp_list(p_locplayer, plagent, nshown, (weptype == current_weapon), weptype, cx, cy);

            cx += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].x;
            cy += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].y;
            ++nchecked;
        }
        nshown++;
        if (nchecked == 6)
            break;
    }
}

TbBool panel_update_weapon_current(PlayerIdx plyr, short nagent, ubyte flags)
{
    PlayerInfo *p_player;
    struct Thing *p_agent;
    struct GamePanel *p_panel;
    ushort panstate;
    ushort curwep, prevwep;
    short cx, cy;
    TbBool wep_highlight;
    short w, h;

    // If PaMF_EXISTS not set, do not draw and do not access the agent as it may be invalid
    if ((flags & PaMF_EXISTS) == 0)
        return false;
    // If the panel is drawn but disabled, disallow interaction
    if ((flags & PaMF_DISABLED) != 0)
        return false;

    p_player = &players[plyr];
    p_panel = &game_panel[20 + nagent];

    cx = p_panel->pos.X;
    cy = p_panel->pos.Y;
    p_agent = p_player->MyAgent[nagent];

    panstate = p_player->PanelState[mouser];
    // If we have panel state opened for another agent, disallow interaction
    if ((panstate != PANEL_STATE_NORMAL) && (nagent != panel_state_to_player_agent(panstate)))
        return false;

    curwep = p_agent->U.UPerson.CurrentWeapon;
    prevwep = p_player->PrevWeapon[nagent];
    if (curwep == 0 && prevwep == 0) {
        prevwep = find_nth_weapon_held(p_agent->ThingOffset, 1);
        p_player->PrevWeapon[nagent] = prevwep;
    }

    if ((curwep == 0) && (prevwep == 0)) {
        return false;
    }

    w = game_panel_shifts[PaSh_WEP_CURR_BUTTON_AREA].x;
    h = game_panel_shifts[PaSh_WEP_CURR_BUTTON_AREA].y;
    wep_highlight = panel_mouse_over_weapon(cx, cy, w, h, panstate, 0);

    if (wep_highlight)
    {
        if (curwep != 0)
            p_player->PanelItem[mouser] = curwep;
        else
            p_player->PanelItem[mouser] = prevwep;

        agent_with_mouse_over_weapon = nagent;
    }

    return wep_highlight;
}

short draw_current_weapon_button(PlayerInfo *p_locplayer, short nagent, ubyte flags)
{
    struct Thing *p_agent;
    struct GamePanel *p_panel;
    ushort curwep, prevwep;
    short cx, cy;
    TbBool darkened;

    // If PaMF_EXISTS not set, do not draw and do not access the agent as it may be invalid
    if ((flags & PaMF_EXISTS) == 0)
        return 0;

    p_agent = p_locplayer->MyAgent[nagent];
    p_panel = &game_panel[20 + nagent];

    cx = p_panel->pos.X;
    cy = p_panel->pos.Y;
    curwep = p_agent->U.UPerson.CurrentWeapon;
    prevwep = p_locplayer->PrevWeapon[nagent];

    darkened = ((flags & (PaMF_DISABLED|PaMF_SUBORDNT)) != 0);
    if (curwep != 0) // Is ready/drawn weapon - draw lighted weapon shape
    {
        draw_agent_current_weapon(p_locplayer, nagent, 0, darkened, true, curwep, cx, cy);
    }
    else if (prevwep != 0) // Weapon is carried but hidden - draw with dark weapon shape
    {
        curwep = prevwep;
        draw_agent_current_weapon(p_locplayer, nagent, 0, darkened, false, curwep, cx, cy);
    }
    return curwep;
}

/** Updates a list of weapons below current weapon panel.
 *
 * This function is intended to loop through weapons in the same way
 * as draw_agent_weapons_selection(), but update state instead of drawing.
 */
TbBool update_agent_weapons_selection(PlayerIdx plyr, short nagent)
{
    PlayerInfo *p_player;
    struct Thing *p_agent;
    struct GamePanel *p_panel;
    ulong wepflags;
    ushort panstate;
    int cx, cy;
    short cur_ready_wep, weptype;
    ushort plagent;
    ushort nshown, nchecked;
    TbBool ret;

    p_player = &players[plyr];
    p_agent = p_player->MyAgent[nagent];
    // Protect from damaged / unfinished levels
    if ((p_agent->Type != TT_PERSON) || (p_agent->Flag & TngF_Destroyed) != 0) {
        p_player->PanelState[mouser] = PANEL_STATE_NORMAL;
        return false;
    }

    ret = false;
    panstate = p_player->PanelState[mouser];
    plagent = p_agent->U.UPerson.ComCur & 3;
    wepflags = p_agent->U.UPerson.WeaponsCarried;
    p_panel = &game_panel[20 + plagent];

    cx = p_panel->pos.X;
    cy = p_panel->pos.Y;
    cx += game_panel_shifts[PaSh_AGENT_WEAPON_TO_LIST].x;
    cy += game_panel_shifts[PaSh_AGENT_WEAPON_TO_LIST].y;
    { // Shift the input a bit up to avoid having a do-nothin area between current weapon and the weapon list
        struct TbSprite *p_spr;

        p_spr = &pop1_sprites[12];
        cy -= max(game_panel_shifts[PaSh_AGENT_WEAPON_TO_LIST].y - p_spr->SHeight, 0);
    }

    cur_ready_wep = player_agent_current_or_prev_weapon(plyr, plagent);

    // Some weapons are not selectable
    wepflags &= ~(1 << (WEP_ENERGYSHLD-1));
    nchecked = 0;
    nshown = 0;
    for (weptype = 1; weptype < WEP_TYPES_COUNT; weptype++, wepflags >>= 1)
    {
        TbBool wep_visible;

        if (wepflags == 0)
            break;
        if ((wepflags & 1) == 0)
            continue;
        wep_visible = (cur_ready_wep != weptype);

        if (wep_visible)
        {
            TbBool wep_highlight;
            short w, h;

            w = max(game_panel_shifts[PaSh_WEP_NEXT_BUTTON_AREA].x + 2, game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].x - 1);
            h = max(game_panel_shifts[PaSh_WEP_NEXT_BUTTON_AREA].y, game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].y - 1);
            wep_highlight = panel_mouse_over_weapon(cx - 1, cy, w, h, panstate, nshown + 1);

            if (wep_highlight) {
                p_player->PanelItem[mouser] = weptype;
                agent_with_mouse_over_weapon = plagent;
                ret = true;
                break;
            }

            cx += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].x;
            cy += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].y;
            nshown++;
        }

        nchecked++;
        if (nshown == 12)
            break;
    }
    return ret;
}

/** Draws a list of weapons below current weapon panel.
 */
void draw_agent_weapons_selection(PlayerIdx plyr, short nagent)
{
    PlayerInfo *p_player;
    struct Thing *p_agent;
    struct GamePanel *p_panel;
    ulong wepflags;
    int cx, cy;
    short cur_ready_wep, weptype;
    ushort plagent;
    ushort nshown, nchecked;

    p_player = &players[plyr];
    p_agent = p_player->MyAgent[nagent];
    // Assured by update_agent_weapons_selection()
    assert(p_agent->Type == TT_PERSON);

    plagent = p_agent->U.UPerson.ComCur & 3;
    wepflags = p_agent->U.UPerson.WeaponsCarried;
    p_panel = &game_panel[20 + plagent];

    cx = p_panel->pos.X;
    cy = p_panel->pos.Y;
    cx += game_panel_shifts[PaSh_AGENT_WEAPON_TO_LIST].x;
    cy += game_panel_shifts[PaSh_AGENT_WEAPON_TO_LIST].y;

    cur_ready_wep = player_agent_current_or_prev_weapon(plyr, plagent);

    // Some weapons are not selectable
    wepflags &= ~(1 << (WEP_ENERGYSHLD-1));
    nchecked = 0;
    nshown = 0;
    for (weptype = 1; weptype < WEP_TYPES_COUNT; weptype++, wepflags >>= 1)
    {
        TbBool wep_visible;

        if (wepflags == 0)
            break;
        if ((wepflags & 1) == 0)
            continue;
        wep_visible = (cur_ready_wep != weptype);

        if (wep_visible)
        {
            draw_agent_carried_weapon(p_player, plagent, nshown + 1, false, weptype, cx, cy);

            cx += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].x;
            cy += game_panel_shifts[PaSh_WEP_NEXT_DISTANCE].y;
            nshown++;
        }

        nchecked++;
        if (nshown == 12)
            break;
    }
}

void draw_panel_pickable_item(void)
{
    TbBool ret;
    PlayerInfo *p_locplayer;
    struct Thing *p_agent;
    short dcthing;

    p_locplayer = &players[local_player_no];
    dcthing = direct_control_thing_for_player(local_player_no);
    // FIXME a strange place for fixing state of an agent; should be moved to game world update
    update_dropped_item_under_agent_exists(dcthing);
    p_agent = &things[dcthing];

    ret = draw_panel_pickable_thing_below_agent(p_agent);
    if (!ret)
        draw_panel_pickable_thing_player_targeted(p_locplayer);
}

TbBool func_1caf8(ubyte *panel_wep)
{
    TbBool ret;
    PlayerInfo *p_locplayer;
    struct Thing *p_agent;
    short dcthing;

    p_locplayer = &players[local_player_no];
    dcthing = direct_control_thing_for_player(local_player_no);
    p_agent = &things[dcthing];

    // Clearing active weapon so it can be re-set
    p_locplayer->PanelItem[mouser] = WEP_NULL;
    if (ingame.PanelPermutation >= 0)
    {
        ushort plagent;
        plagent = p_agent->U.UPerson.ComCur & 3;
        ret = update_weapons_list_prealp(p_locplayer, plagent,
            p_agent->U.UPerson.WeaponsCarried, p_agent->U.UPerson.CurrentWeapon);
    }
    else
    {
        short nagent;
        ushort panstate;

        agent_with_mouse_over_weapon = -1;
        ret = false;
        for (nagent = 0; nagent < playable_agents; nagent++)
        {
            ret |= panel_update_weapon_current(local_player_no, nagent, panel_wep[nagent]);
        }
        panstate = p_locplayer->PanelState[mouser];
        nagent = panel_state_to_player_agent(panstate);
        if ((panstate >= PANEL_STATE_WEP_SEL_ONE) && (panstate <= PANEL_STATE_WEP_SEL_GRP + 3) && (nagent >= 0))
        {
            ret |= update_agent_weapons_selection(local_player_no, nagent);
        }
    }

    if (ingame.PanelPermutation >= 0)
    {
        ushort plagent;
        plagent = p_agent->U.UPerson.ComCur & 3;
        draw_weapons_list_prealp(p_locplayer, plagent,
            p_agent->U.UPerson.WeaponsCarried, p_agent->U.UPerson.CurrentWeapon);
    }
    else
    {
        short nagent;
        ushort panstate;

        for (nagent = 0; nagent < playable_agents; nagent++)
        {
            draw_current_weapon_button(p_locplayer, nagent, panel_wep[nagent]);
        }

        panstate = p_locplayer->PanelState[mouser];
        nagent = panel_state_to_player_agent(panstate);
        if ((panstate >= PANEL_STATE_WEP_SEL_ONE) && (panstate <= PANEL_STATE_WEP_SEL_GRP + 3) && (nagent >= 0))
        {
            draw_agent_weapons_selection(local_player_no, nagent);
        }
    }

    lbDisplay.DrawFlags = 0;
    return ret;
}

void draw_agent_grouping_bars(short panel)
{
    struct Thing *p_thing;
    short dcthing;
    short i, n;

    n = 0;
    for (i = 0; i < playable_agents; i++)
    {
        p_thing = players[local_player_no].MyAgent[i];
        if (p_thing->Type != TT_PERSON) continue;
        dcthing = players[local_player_no].DirectControl[byte_153198-1];
        if ((p_thing->State != PerSt_PROTECT_PERSON) || (p_thing->GotoThingIndex != dcthing)) {
            if (((p_thing->Flag2 & TgF2_Unkn10000000) == 0) || (p_thing->Owner != dcthing))
                continue;
        }
        n++;
    }

    for (n--; n >= 0; n--)
    {
        struct GamePanel *p_panel;
        short x, y;

        p_panel = &game_panel[panel];
        x = p_panel->pos.X;
        y = p_panel->pos.Y;
        x += game_panel_shifts[PaSh_GROUP_PANE_AGENTS + n].x;
        y += game_panel_shifts[PaSh_GROUP_PANE_AGENTS + n].y;
        draw_new_panel_sprite_std(x, y, 69);
    }
}

void func_702c0(int a1, int a2, int a3, int a4, int a5, ubyte a6)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_func_702c0\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5), "g" (a6));
}

void draw_transparent_slant_bar(short x, short y, ushort w, ushort h)
{
    long waftx, wafty;
    ushort tmx, tmy;
    struct EnginePoint point4;
    struct EnginePoint point2;
    struct EnginePoint point1;
    struct EnginePoint point3;
    short sh_x;

    sh_x = 3;
    if (lbDisplay.GraphicsScreenHeight < 400)
        sh_x /= 2;
    point1.pp.X = x;
    point1.pp.Y = y;
    point4.pp.X = (x + w);
    point4.pp.Y = y;
    point2.pp.Y = (y + h);
    point2.pp.X = (x + w - sh_x);
    point3.pp.Y = (y + h);
    point3.pp.X = (x - sh_x);

    // The shield bar is animated, even if it's not possible to see
    waftx = waft_table[(gameturn >> 3) & 31];
    wafty = waft_table[(gameturn + 16) & 31];
    tmx = ((waftx + 30) >> 1);
    tmy = ((wafty + 30) >> 3) + 64;
    point1.pp.U = tmx << 16;
    point4.pp.U = (tmx + 64) << 16;
    point2.pp.U = (tmx + 64) << 16;
    point1.pp.V = tmy << 16;
    point4.pp.V = tmy << 16;
    point2.pp.V = (tmy + 8) << 16;
    point3.pp.U = tmx << 16;
    point3.pp.V = (tmy + 8) << 16;

    point1.pp.S = 0;
    point2.pp.S = 0;
    point3.pp.S = 0;
    point4.pp.S = 0;

    vec_mode = 18;
    vec_map = dword_1AA280;
    draw_trigpoly(&point1.pp, &point4.pp, &point3.pp);
    if (vec_mode == 2)
        vec_mode = 27;
    draw_trigpoly(&point4.pp, &point2.pp, &point3.pp);
}

void draw_health_level(short x, short y, ushort w, ushort h, short lv, ushort lvmax, ubyte col, ubyte transp)
{
    short cw;

    if ((lv <= 0) || (lvmax == 0))
        return;

    cw = w * lv / lvmax;
    if (transp)
    {
        draw_transparent_slant_bar(x, y, cw, h);
    }
    else
    {
        ApDrawSlantBox(x, y, cw, h, col);
    }
}

void draw_wep_energy_level(short x, short y, ushort w, ushort h, short lv, ushort lvmax, ubyte col, ubyte transp)
{
    short cw, ch;

    if ((lv <= 0) || (lvmax == 0))
        return;

    ch = h * lv / lvmax;

    short cx, cy;
    cx = x;
    cy = y;
    for (cw = w; cw > 0; cw--)
    {
        short cy1, cy2;

        cy1 = h + cy;
        cy2 = h + cy - ch;
        SCANNER_unkn_func_203(cx, cy1, cx, cy2, col,
            ingame.Scanner.Contrast, ingame.Scanner.Brightness);
        ++cx;
        ++cy;
    }
}

void draw_mood_level(short x, short y, ushort w, int h, short value)
{
    short cent_x;
    short x1, y1, x2;
    TbPixel col;
    short fade;
    short i;

    fade = value >> 2;
    if (value >= 0)
        col = pixmap.fade_table[PALETTE_8b_COLORS * (63 - fade) + colour_lookup[ColLU_RED]];
    else
        col = pixmap.fade_table[PALETTE_8b_COLORS * (63 + fade) + colour_lookup[ColLU_BLUE]];

    cent_x = x + (w >> 1);
    x1 = cent_x;
    x2 = x1 + (w >> 1) * value / 88;
    y1 = y;

    for (i = h; i > 0; i--)
    {
        SCANNER_unkn_func_203(x1, y1, x2, y1,
            col, ingame.Scanner.Contrast, ingame.Scanner.Brightness);
        x1--;
        x2--;
        y1++;
    }
}

void draw_mood_limits(short x, short y, short w, short h, short value, short maxval)
{
    short scaled_val;
    short curr_x, sh_x;
    TbPixel col;

    if (value <= 0)
        return;

    sh_x = h;

    col = colour_lookup[ColLU_WHITE];
    scaled_val = (w * value / maxval) >> 1;

    curr_x = x + (w >> 1) - scaled_val;
    LbDrawLine(curr_x, y, curr_x - sh_x, (y + h), col);

    curr_x = x + (w >> 1) + scaled_val;
    LbDrawLine(curr_x, y, curr_x - sh_x, (y + h), col);
}

void draw_energy_bar(int x1, int y1, short w, short h, int value, int maxval)
{
    short scaled_val;
    short x2, y2;
    short i;
    TbPixel col;

    if (maxval == 0)
        return;

    col = colour_lookup[ColLU_WHITE];
    scaled_val = h * value / maxval;

    x2 = x1 + scaled_val;
    y2 = y1 - scaled_val;
    for (i = w; i > 0; i--)
    {
        LbDrawLine(x1, y1, x2, y2, col);
        x1++;
        x2++;
    }
}


TbBool panel_active_based_on_target(short panel)
{
    struct GamePanel *p_panel;
    PlayerInfo *p_locplayer;
    struct Thing *p_agent;

    p_panel = &game_panel[panel];

    if ((p_panel->Type == PanT_WeaponEnergy) || (p_panel->Type == PanT_Grouping))
        return true;

    if (p_panel->ID >= playable_agents)
        return true;

    p_locplayer = &players[local_player_no];
    p_agent = p_locplayer->MyAgent[p_panel->ID];

    if (p_agent->Type != TT_PERSON)
        return false;

    return ((p_agent->Flag & TngF_Destroyed) == 0);
}

void panel_get_size(short *p_width, short *p_height, short panel)
{
    struct GamePanel *p_panel;
    short w, h;

    p_panel = &game_panel[panel];

    if ((p_panel->pos.Width == 0) && (p_panel->pos.Height == 0))
    {
        struct TbSprite *p_spr;
        short spr;
        short i;

        w = 0;
        h = 0;
        spr = p_panel->Spr[0];
        if (spr >= 0)
        {
            p_spr = &pop1_sprites[spr];
            w = p_spr->SWidth;
            h = p_spr->SHeight;

            if ((p_panel->Flags & PanF_SPRITES_IN_LINE_HORIZ) != 0)
            {
                for (i = 1; i < 3; i++)
                {
                    spr = p_panel->Spr[i];
                    if (spr <= 0) {
                        if (spr == 0)
                            continue;
                        break;
                    }
                    p_spr = &pop1_sprites[spr];
                    w += p_spr->SWidth;
                }
            }
            if ((p_panel->Flags & PanF_SPRITES_IN_LINE_VERTC) != 0)
            {
                for (i = 1; i < 3; i++)
                {
                    spr = p_panel->Spr[i];
                    if (spr <= 0) {
                        if (spr == 0)
                            continue;
                        break;
                    }
                    p_spr = &pop1_sprites[spr];
                    h += p_spr->SHeight;
                }
            }
        }
    } else {
        w = p_panel->pos.Width;
        h = p_panel->pos.Height;
    }

   *p_width = w;
   *p_height = h;
}

TbBool mouse_move_over_panel(short panel)
{
    struct GamePanel *p_panel;
    short x, y, w, h;

    p_panel = &game_panel[panel];

    x = p_panel->pos.X;
    y = p_panel->pos.Y;
    panel_get_size(&w, &h, panel);

    if (!panel_active_based_on_target(panel))
        return false;

    // We are only interested in panels for control of agents
    if (p_panel->ID >= playable_agents)
        return false;

    return mouse_move_over_box_coords(x, y, x + w, y + h);
}

void panel_sprites_switch(short panel, TbBool sw_on)
{
    struct GamePanel *p_panel;
    short loc_spr[3];

    p_panel = &game_panel[panel];
    loc_spr[0] = p_panel->Spr[0];
    loc_spr[1] = p_panel->Spr[1];
    loc_spr[2] = p_panel->Spr[2];

    if (sw_on && ((p_panel->Flags & PanF_SPR_TOGGLED_ON) == 0))
    {
        p_panel->Spr[0] = p_panel->ExtraSpr[0];
        p_panel->Spr[1] = p_panel->ExtraSpr[1];
        p_panel->Spr[2] = p_panel->ExtraSpr[2];

        p_panel->Flags |= PanF_SPR_TOGGLED_ON;
        p_panel->ExtraSpr[0] = loc_spr[0];
        p_panel->ExtraSpr[1] = loc_spr[1];
        p_panel->ExtraSpr[2] = loc_spr[2];
    }
    else if (!sw_on && ((p_panel->Flags & PanF_SPR_TOGGLED_ON) != 0))
    {
        p_panel->Spr[0] = p_panel->ExtraSpr[0];
        p_panel->Spr[1] = p_panel->ExtraSpr[1];
        p_panel->Spr[2] = p_panel->ExtraSpr[2];

        p_panel->Flags &= ~PanF_SPR_TOGGLED_ON;
        p_panel->ExtraSpr[0] = loc_spr[0];
        p_panel->ExtraSpr[1] = loc_spr[1];
        p_panel->ExtraSpr[2] = loc_spr[2];
    }
}

void update_game_panel(void)
{
    PlayerInfo *p_locplayer;
    short panel;

    p_locplayer = &players[local_player_no];
    for (panel = 0; panel < GAME_PANELS_LIMIT; panel++)
    {
        struct GamePanel *p_panel;
        struct Thing *p_agent;

        if (!panel_exists(panel))
            break;
        p_panel = &game_panel[panel];

        switch (p_panel->Type)
        {
        case PanT_AgentMedi:
            // If an agent has a medkit, use the sprite with lighted cross
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            panel_sprites_switch(panel, (p_agent->Type == TT_PERSON) && person_carries_any_medikit(p_agent));
            break;
        case PanT_WeaponEnergy:
            // If supershield is enabled for the current agent, draw energy bar in red
            p_agent = &things[p_locplayer->DirectControl[0]];
            panel_sprites_switch(panel, (p_agent->Type == TT_PERSON) && (p_agent->Flag & TngF_Unkn0100) != 0);
            break;
        }
    }
}

TbBool mouse_over_infrared_slant_box(short panel)
{
    struct GamePanel *p_panel;
    short x, y;
    short ms_x, ms_y;
    short delta_x, delta_y;

    p_panel = &game_panel[panel];

    x = p_panel->pos.X;
    y = p_panel->pos.Y;
    x += game_panel_shifts[PaSh_GROUP_PANE_TO_THERMAL_BOX].x;
    y += game_panel_shifts[PaSh_GROUP_PANE_TO_THERMAL_BOX].y;

    ms_x = lbDisplay.MouseX;
    ms_y = lbDisplay.MouseY;
    delta_x = (ms_x - x);
    delta_y = (ms_y - y);

    return (ms_y > y) && (delta_x + 22 - delta_y < 22);
}

short panel_mouse_move_mood_value(short panel)
{
    struct GamePanel *p_panel;
    short i;
    short x;

    p_panel = &game_panel[panel];
    x = p_panel->pos.X;
    i = mouse_move_position_horizonal_over_bar_coords(x, p_panel->pos.Width);
    i = 2 * (i * 88 / p_panel->pos.Width) - 88;
    if (i < -88) i = -88;
    if (i > 88) i = 88;

    return i;
}

/** Thermal vision button light.
 */
void draw_panel_thermal_button(short panel)
{
    if ((ingame.Flags & GamF_ThermalView) != 0)
    {
        struct GamePanel *p_panel;
        short x, y;

        p_panel = &game_panel[panel];
        x = p_panel->pos.X;
        y = p_panel->pos.Y;
        x += game_panel_shifts[PaSh_GROUP_PANE_TO_THERMAL_SPR].x;
        y += game_panel_shifts[PaSh_GROUP_PANE_TO_THERMAL_SPR].y;
        draw_new_panel_sprite_std(x, y, 91);
    }
}

/** Objective text, or net players list.
 */
void draw_panel_objective_info(void)
{
    int x, y, w;
    x = ingame.Scanner.X1 - 1;
    if (x < 0)
        x = 0;
    y = lbDisplay.GraphicsScreenHeight - panel_get_objective_info_height(lbDisplay.GraphicsScreenHeight);
    if (in_network_game) {
        SCANNER_unkn_func_205();
        w = lbDisplay.GraphicsScreenWidth;
    } else {
        // original width 67 low res, 132 high res
        w = ingame.Scanner.X2 - ingame.Scanner.X1 + 3;
    }
    SCANNER_draw_objective_info(x, y, w);
}

void draw_weapon_energy_bar(short panel)
{
    struct GamePanel *p_panel;
    struct Thing *p_agent;
    int lv, lvmax, col;
    short x, y, w, h;
    ThingIdx dcthing;

    p_panel = &game_panel[panel];
    dcthing = direct_control_thing_for_player(local_player_no);
    p_agent = &things[dcthing];
    if ((p_agent->Flag & TngF_PlayerAgent) == 0) {
        LOGNO("Agent %d unexpected flags", plagent);
        return;
    }
    x = p_panel->dyn.X;
    y = p_panel->dyn.Y;
    w = p_panel->dyn.Width;
    h = p_panel->dyn.Height;

    if ((p_agent->U.UPerson.Energy < 50) && (gameturn & 1))
        col = ColLU_RED;
    else
        col = ColLU_WHITE;
    lvmax = p_agent->U.UPerson.MaxEnergy;
    lv = p_agent->U.UPerson.Energy;
    draw_wep_energy_level(x, y, w, h, lv, lvmax, colour_lookup[col], 0);
}

void draw_new_panel_badge_overlay(short panel, ushort plagent, TbBool darkened)
{
    // Blink the number of active agent
    if (gameturn & 4)
    {
        short dcthing;
        struct Thing *p_agent;

        dcthing = direct_control_thing_for_player(local_player_no);
        p_agent = &things[dcthing];
        if ((p_agent->Flag & TngF_Destroyed) == 0 && !person_is_executing_commands(p_agent->ThingOffset) &&
          (plagent == (p_agent->U.UPerson.ComCur & 3)))
        {
            struct GamePanel *p_panel;
            short x, y;

            p_panel = &game_panel[panel];
            x = p_panel->dyn.X;
            y = p_panel->dyn.Y;
            draw_new_panel_sprite_std(x, y, 6 + plagent);
        }
    }
}

void draw_new_panel_health_overlay(short panel, ushort plagent, TbBool darkened)
{
    struct GamePanel *p_panel;
    struct Thing *p_agent;
    PlayerInfo *p_locplayer;
    int lv, lvmax;
    short x, y, w, h;

    p_panel = &game_panel[panel];
    p_locplayer = &players[local_player_no];
    p_agent = p_locplayer->MyAgent[plagent];
    if ((p_agent->Flag & TngF_PlayerAgent) == 0) {
        LOGNO("Agent %d unexpected flags", plagent);
        return;
    }
    x = p_panel->dyn.X;
    y = p_panel->dyn.Y;
    w = p_panel->dyn.Width;
    h = p_panel->dyn.Height;

    // Draw health level
    lv = p_agent->Health;
    lvmax = p_agent->U.UPerson.MaxHealth;
    if (lv <= lvmax) { // Normal health amount
        draw_health_level(x, y, w, h, lv, lvmax, colour_lookup[ColLU_WHITE], 0);
    } else { // Health reinforced beyond max is drawn in red
        draw_health_level(x, y, w, h, lvmax, lvmax, colour_lookup[ColLU_WHITE], 0);
        draw_health_level(x, y, w, h, lv - lvmax, lvmax, colour_lookup[ColLU_RED], 0);
    }
    // Draw shield level over health
    lv = p_agent->U.UPerson.ShieldEnergy;
    draw_health_level(x, y, w, h, lv, 0x400, colour_lookup[ColLU_WHITE], 1);
}

void draw_new_panel_mood_overlay(short panel, ushort plagent, TbBool darkened)
{
    struct GamePanel *p_panel;
    struct Thing *p_agent;
    PlayerInfo *p_locplayer;
    int lv, lvmax;
    short x, y, w, h;

    p_panel = &game_panel[panel];
    p_locplayer = &players[local_player_no];
    p_agent = p_locplayer->MyAgent[plagent];
    if ((p_agent->Flag & TngF_PlayerAgent) == 0) {
        LOGNO("Agent %d unexpected flags", plagent);
        return;
    }
    x = p_panel->dyn.X;
    y = p_panel->dyn.Y;
    w = p_panel->dyn.Width;
    h = p_panel->dyn.Height;

    // Draw drug level aka mood (or just a red line if no drugs)
    draw_mood_level(x, y, w, h, p_agent->U.UPerson.Mood);
    // Draw stamina level which caps the mood level
    h = h * 3 / 4;
    lv = p_agent->U.UPerson.Stamina;
    lvmax = p_agent->U.UPerson.MaxStamina;
    draw_mood_limits(x, y, w, h, lv, lvmax);
}

void draw_new_panel_energy_overlay(short panel, ushort plagent, TbBool darkened)
{
    struct GamePanel *p_panel;
    struct Thing *p_agent;
    PlayerInfo *p_locplayer;
    int lv, lvmax;
    short x, y, w, h;

    p_panel = &game_panel[panel];
    p_locplayer = &players[local_player_no];
    p_agent = p_locplayer->MyAgent[plagent];
    if ((p_agent->Flag & TngF_PlayerAgent) == 0) {
        LOGNO("Agent %d unexpected flags", plagent);
        return;
    }
    x = p_panel->dyn.X;
    y = p_panel->dyn.Y;
    w = p_panel->dyn.Width;
    h = p_panel->dyn.Height;

    // Draw weapon energy level
    lv = p_agent->U.UPerson.Energy;
    lvmax = p_agent->U.UPerson.MaxEnergy;
    draw_energy_bar(x, y, w, h, lv, lvmax);
}

void draw_new_panel_weapon_overlay(short panel, ushort plagent, TbBool darkened)
{
}

void draw_new_panel(void)
{
    PlayerInfo *p_locplayer;
    short panel;
    ubyte panel_wep[AGENTS_SQUAD_MAX_COUNT];

    update_game_panel();

    p_locplayer = &players[local_player_no];
    LbMemorySet(panel_wep, 0, sizeof(panel_wep));

    for (panel = 0; true; panel++)
    {
        struct GamePanel *p_panel;
        TbBool is_visible;
        TbBool is_disabled, is_subordnt;

        p_panel = &game_panel[panel];
        if (p_panel->Spr[0] < 0)
          break;
        lbDisplay.DrawFlags = 0;

        if (!panel_for_speciifc_agent(panel))
        {
            is_visible = (p_panel->Spr[0] != 0);
            is_disabled = false;
            is_subordnt = false;
        }
        else
        {
            is_visible = true;
            if (p_panel->ID >= playable_agents)
            {
                is_visible = false;
            }
            if (!player_agent_is_alive(local_player_no, p_panel->ID))
            {
                is_visible = false;
            }
            else if (p_panel->Type == PanT_AgentWeapon)
            {
                if (!player_agent_is_executing_commands(local_player_no, p_panel->ID))
                {
                    ubyte weapon;
                    short wep_delay;

                    weapon = player_agent_current_or_prev_weapon(local_player_no, p_panel->ID);
                    wep_delay = player_agent_weapon_delay(local_player_no, p_panel->ID, weapon);

                    is_visible = (weapon != 0) && ((wep_delay == 0) || (gameturn & 1));
                }
                else
                {
                    // If executing predefined commands rather than being controlled by player
                    is_visible = false;
                }
            }

            {
                struct Thing *p_agent;
                p_agent = p_locplayer->MyAgent[p_panel->ID];

                is_subordnt = (p_agent->State == PerSt_PROTECT_PERSON);
                is_disabled = (((p_agent->Flag2 & TgF2_Unkn10000000) != 0) ||
                  person_is_executing_commands(p_agent->ThingOffset));
            }
        }

        if (!is_visible)
            continue;

        if (p_panel->Spr[0] != 0)
        {
            short x, y;
            short spr;

            spr = p_panel->Spr[0];
            x = p_panel->pos.X;
            y = p_panel->pos.Y;
            if (is_disabled || is_subordnt)
                draw_new_panel_sprite_dark(x, y, spr);
            else
                draw_new_panel_sprite_std(x, y, spr);

            if ((p_panel->Flags & PanF_SPRITES_IN_LINE_HORIZ) != 0)
            {
                struct TbSprite *p_spr;
                short real_spr1_width;

                spr = p_panel->Spr[0];
                p_spr = &pop1_sprites[spr];
                x += p_spr->SWidth;

                if ((p_panel->Flags & PanF_RESIZE_MIDDLE_SPR) != 0) {
                    short const_width;

                    const_width = p_spr->SWidth;

                    spr = p_panel->Spr[2];
                    p_spr = &pop1_sprites[spr];
                    const_width += p_spr->SWidth;

                    real_spr1_width = p_panel->SprWidth - const_width;
                } else {
                    spr = p_panel->Spr[1];
                    p_spr = &pop1_sprites[spr];
                    real_spr1_width = p_spr->SWidth;
                }

                spr = p_panel->Spr[1];
                p_spr = &pop1_sprites[spr];
                if (real_spr1_width == p_spr->SWidth)
                {
                    if (is_disabled || is_subordnt)
                        draw_new_panel_sprite_dark(x, y, spr);
                    else
                        draw_new_panel_sprite_std(x, y, spr);
                }
                else
                {
                    if (is_disabled || is_subordnt)
                        draw_new_panel_sprite_scaled_dark(x, y, spr, real_spr1_width, p_spr->SHeight);
                    else
                        draw_new_panel_sprite_scaled_std(x, y, spr, real_spr1_width, p_spr->SHeight);
                }
                x += real_spr1_width;

                spr = p_panel->Spr[2];
                if (is_disabled || is_subordnt)
                    draw_new_panel_sprite_dark(x, y, spr);
                else
                    draw_new_panel_sprite_std(x, y, spr);
            }

            if ((p_panel->Flags & PanF_SPRITES_IN_LINE_VERTC) != 0)
            {
                struct TbSprite *p_spr;
                short real_spr1_height;

                spr = p_panel->Spr[0];
                p_spr = &pop1_sprites[spr];
                y += p_spr->SHeight;

                if ((p_panel->Flags & PanF_RESIZE_MIDDLE_SPR) != 0) {
                    short const_height;

                    const_height = p_spr->SHeight;

                    spr = p_panel->Spr[2];
                    p_spr = &pop1_sprites[spr];
                    const_height += p_spr->SHeight;

                    real_spr1_height = p_panel->SprHeight - const_height;
                } else {
                    spr = p_panel->Spr[1];
                    p_spr = &pop1_sprites[spr];
                    real_spr1_height = p_spr->SHeight;
                }

                spr = p_panel->Spr[1];
                p_spr = &pop1_sprites[spr];
                if (real_spr1_height == p_spr->SHeight)
                {
                    if (is_disabled || is_subordnt)
                        draw_new_panel_sprite_dark(x, y, spr);
                    else
                        draw_new_panel_sprite_std(x, y, spr);
                }
                else
                {
                    if (is_disabled || is_subordnt)
                        draw_new_panel_sprite_scaled_dark(x, y, spr, p_spr->SWidth, real_spr1_height);
                    else
                        draw_new_panel_sprite_scaled_std(x, y, spr, p_spr->SWidth, real_spr1_height);
                }
                y += real_spr1_height;

                spr = p_panel->Spr[2];
                if (is_disabled || is_subordnt)
                    draw_new_panel_sprite_dark(x, y, spr);
                else
                    draw_new_panel_sprite_std(x, y, spr);
            }
        }

        // Fill additional info on top of the sprites
        switch (p_panel->Type)
        {
        case PanT_AgentBadge:
            draw_new_panel_badge_overlay(panel, p_panel->ID, is_disabled || is_subordnt);
            break;
        case PanT_AgentHealth:
            draw_new_panel_health_overlay(panel, p_panel->ID, is_disabled || is_subordnt);
            break;
        case PanT_AgentMood:
            draw_new_panel_mood_overlay(panel, p_panel->ID, is_disabled || is_subordnt);
            break;
        case PanT_AgentEnergy:
            draw_new_panel_energy_overlay(panel, p_panel->ID, is_disabled || is_subordnt);
            break;
        case PanT_AgentMedi:
            // Medi sprite gets switched when we have medikit, so no need for update
            break;
        case PanT_AgentWeapon:
            panel_wep[p_panel->ID] |= PaMF_EXISTS;
            if (is_disabled)
                panel_wep[p_panel->ID] |= PaMF_DISABLED;
            if (is_subordnt)
                panel_wep[p_panel->ID] |= PaMF_SUBORDNT;
            break;
        case PanT_WeaponEnergy:
            // Fill the left energy bar
            draw_weapon_energy_bar(panel);
            break;
        case PanT_Grouping:
            draw_agent_grouping_bars(panel);
            draw_panel_thermal_button(panel);
            break;
        }
    }

    lbDisplay.DrawFlags = 0;

    draw_panel_pickable_item();

    if (!func_1caf8(panel_wep))
    {
        if (ingame.Flags & GamF_Unkn0200) {
            ulong md, y;
            md = p_locplayer->UserInput[0].ControlMode & 0x1FFF;
            if (md == 1 && pktrec_mode != PktR_PLAYBACK) {
                y = alt_at_point(mouse_map_x, mouse_map_z);
                func_702c0(mouse_map_x, PRCCOORD_TO_YCOORD(y), mouse_map_z, 64, 64, colour_lookup[ColLU_RED]);
            }
        }
    }

    ingame.Scanner.MX = engn_xc >> 7;
    ingame.Scanner.MZ = engn_zc >> 7;
    ingame.Scanner.Angle = 2047 - ((engn_anglexz >> 5) & 0x7FF);
    SCANNER_draw_new_transparent();

    draw_panel_objective_info();
}

TbBool process_panel_state_one_agent_weapon(ushort agent)
{
    PlayerInfo *p_locplayer;
    struct Packet *p_pckt;
    short pnitm;

    p_locplayer = &players[local_player_no];
    p_pckt = &packets[local_player_no];
    pnitm = p_locplayer->PanelItem[mouser];

    if (lbDisplay.RightButton)
    {
        // Right click while holding left weapon drop
        struct Thing *p_agent;

        lbDisplay.RightButton = 0;
        p_agent = p_locplayer->MyAgent[agent];

        if ((p_agent->Type == TT_PERSON) && (pnitm != 0))
        {
            p_locplayer->UserInput[mouser].ControlMode |= 0x4000;
            my_build_packet(p_pckt, PAct_DROP_HELD_WEAPON_SECR, p_agent->ThingOffset, pnitm, 0, 0);
            p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
            return true;
        }
    }
    if (!lbDisplay.MLeftButton)
    {
        struct Thing *p_agent;

        p_agent = p_locplayer->MyAgent[agent];

        if (lbDisplay.MRightButton)
        {
            // Hold left, hold right, release left weapon drop
            lbDisplay.RightButton = 0;
            if ((p_agent->Type == TT_PERSON) && (pnitm != 0))
            {
                p_locplayer->UserInput[mouser].ControlMode |= 0x4000;
                my_build_packet(p_pckt, PAct_DROP_HELD_WEAPON_SECR, p_agent->ThingOffset, pnitm, 0, 0);
                p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
                return true;
            }
        }

        // release button while in weapon selection mode
        {
            if ((p_agent != NULL) && (pnitm != 0))
            {
                my_build_packet(p_pckt, PAct_SELECT_SPECIFIC_WEAPON, p_agent->ThingOffset, pnitm, 0, 0);
                p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
                lbDisplay.RightButton = 0;
                lbDisplay.LeftButton = 0;
                p_locplayer->UserInput[mouser].ControlMode &= ~(0x4000|0x8000);
                return true;
            }
        }
        p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
    }
    return false;
}

TbBool process_panel_state_grp_agents_weapon(ushort agent)
{
    PlayerInfo *p_locplayer;
    struct Packet *p_pckt;
    short pnitm;

    p_locplayer = &players[local_player_no];
    p_pckt = &packets[local_player_no];
    pnitm = p_locplayer->PanelItem[mouser];

    if (lbDisplay.LeftButton)
    {
        // Left click while holding right weapon drop
        struct Thing *p_agent;

        lbDisplay.LeftButton = 0;
        p_agent = p_locplayer->MyAgent[agent];
        if ((p_agent->Type == TT_PERSON) && (pnitm != 0))
        {
            p_locplayer->UserInput[mouser].ControlMode |= 0x8000;
            my_build_packet(p_pckt, PAct_DROP_HELD_WEAPON_SECR, p_agent->ThingOffset, pnitm, 0, 0);
            p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
            return true;
        }
    }
    if (!lbDisplay.MRightButton)
    {
        struct Thing *p_agent;

        p_agent = p_locplayer->MyAgent[agent];
        // release button while in weapon selection mode
        if ((p_agent->Type == TT_PERSON) && (pnitm != 0))
        {
            my_build_packet(p_pckt, PAct_SELECT_GRP_SPEC_WEAPON, p_agent->ThingOffset, pnitm, 0, 0);
            p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
            lbDisplay.RightButton = 0;
            lbDisplay.LeftButton = 0;
            p_locplayer->UserInput[mouser].ControlMode &= ~(0x4000|0x8000);
            return true;
        }
        p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
    }
    return false;
}

/** Process panel in regard to a previously enter mood alteration state.
 *
 * @param main_panel Index of the main panel linked to the current state.
 * @param agent Agent index whose panel is currenly under mouse.
 * @param can_control Whether the agent currently under mouse can receive control commands.
 */
TbBool process_panel_state_one_agent_mood(ushort main_panel, ushort agent)
{
    PlayerInfo *p_locplayer;
    struct Packet *p_pckt;

    p_locplayer = &players[local_player_no];
    p_pckt = &packets[local_player_no];

    if (lbDisplay.MLeftButton)
    {
        // Left button hold mood control
        struct Thing *p_agent;
        short dcthing;
        short mood;
        TbBool can_control;

        p_agent = p_locplayer->MyAgent[agent];
        mood = panel_mouse_move_mood_value(main_panel);
        dcthing = p_locplayer->DirectControl[mouser];
        can_control = person_can_accept_control(dcthing);

        if ((p_agent->Type == TT_PERSON) && (can_control))
            build_packet(p_pckt, PAct_AGENT_SET_MOOD, p_agent->ThingOffset, mood, 0, 0);
        return true;
    }
    p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;
    return false;
}

TbBool process_panel_state_grp_agents_mood(ushort main_panel, ushort agent)
{
    PlayerInfo *p_locplayer;
    struct Packet *p_pckt;

    p_locplayer = &players[local_player_no];
    p_pckt = &packets[local_player_no];

    if (lbDisplay.MRightButton)
    {
        // Right button hold mood control
        struct Thing *p_agent;
        short dcthing;
        short mood;
        TbBool can_control;

        p_agent = p_locplayer->MyAgent[agent];
        mood = panel_mouse_move_mood_value(main_panel);
        dcthing = p_locplayer->DirectControl[mouser];
        can_control = person_can_accept_control(dcthing);

        if ((p_agent->Type == TT_PERSON) && (can_control))
            build_packet(p_pckt, PAct_GROUP_SET_MOOD, p_agent->ThingOffset, mood, 0, 0);
        return true;
    }
    p_locplayer->UserInput[mouser].ControlMode &= ~0xC000;
    p_locplayer->PanelState[mouser] = PANEL_STATE_NORMAL;

    return false;
}

TbBool process_panel_state(void)
{
    PlayerInfo *p_locplayer;
    ubyte pnsta;

    p_locplayer = &players[local_player_no];
    pnsta = p_locplayer->PanelState[mouser];

    if ((ingame.Flags & GamF_Unkn00100000) != 0)
    {
        if ((pnsta < PANEL_STATE_WEP_SEL_GRP + 4) || (pnsta >= PANEL_STATE_SEND_MESSAGE))
        {
            while (IsSamplePlaying(0, 21, 0))
                stop_sample_using_heap(0, 21);
            ingame.Flags &= ~GamF_Unkn00100000;
        }
    }

    if ((pnsta >= PANEL_STATE_WEP_SEL_ONE) && (pnsta < PANEL_STATE_WEP_SEL_ONE + 4))
    {
        if (process_panel_state_one_agent_weapon((pnsta - PANEL_STATE_WEP_SEL_ONE) % 4))
            return 1;
    }
    else if ((pnsta >= PANEL_STATE_WEP_SEL_GRP) && (pnsta < PANEL_STATE_WEP_SEL_GRP + 4))
    {
        if (process_panel_state_grp_agents_weapon((pnsta - PANEL_STATE_WEP_SEL_GRP) % 4))
            return 1;
    }
    else if ((pnsta >= PANEL_STATE_MOOD_SET_ONE) && (pnsta < PANEL_STATE_MOOD_SET_ONE + 4))
    {
        if (process_panel_state_one_agent_mood(pnsta - 5, (pnsta - PANEL_STATE_MOOD_SET_ONE) % 4))
            return 1;
    }
    else if ((pnsta >= PANEL_STATE_MOOD_SET_GRP) && (pnsta < PANEL_STATE_MOOD_SET_GRP + 4))
    {
        if (process_panel_state_grp_agents_mood(pnsta - 9, (pnsta - PANEL_STATE_MOOD_SET_GRP) % 4))
            return 1;
    }
    else if (pnsta == PANEL_STATE_SEND_MESSAGE)
    {
        struct Packet *p_pckt;
        ushort i;

        p_pckt = &packets[local_player_no];
        i = next_buffered_key();
        if (i != 0)
        {
            if (lbShift & 1)
                i |= 0x0100;
            my_build_packet(p_pckt, PAct_CHAT_MESSAGE_KEY, i, 0, 0, 0);
            return 1;
        }
    }
    return 0;
}

TbBool check_panel_input(short panel)
{
    PlayerInfo *p_locplayer;
    int i;

    p_locplayer = &players[local_player_no];

    if (lbDisplay.LeftButton)
    {
        struct Packet *p_pckt;
        struct Thing *p_agent;
        struct GamePanel *p_panel;
        short dcthing;

        lbDisplay.LeftButton = 0;
        p_panel = &game_panel[panel];
        p_pckt = &packets[local_player_no];

        switch (p_panel->Type)
        {
        case PanT_AgentBadge:
        case PanT_AgentHealth:
        case PanT_AgentEnergy:
            // Select controlled agent
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            if ((p_agent->Type != TT_PERSON) || ((p_agent->Flag & TngF_Destroyed) != 0) || ((p_agent->Flag2 & TgF2_KnockedOut) != 0))
                return 0;
            if (p_locplayer->DoubleMode) {
                byte_153198 = p_panel->ID + 1;
            } else {
                dcthing = p_locplayer->DirectControl[0];
                build_packet(p_pckt, PAct_SELECT_AGENT, dcthing, p_agent->ThingOffset, 0, 0);
                p_locplayer->UserInput[0].ControlMode |= 0x8000;
            }
            return 1;
        case PanT_AgentMood:
            // Change mood / drugs level
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            if ((p_agent->Type == TT_PERSON) && (p_agent->State != PerSt_DEAD))
            {
                p_locplayer->UserInput[mouser].ControlMode |= 0x8000;
                i = panel_mouse_move_mood_value(panel);
                if (panel_active_based_on_target(panel))
                    my_build_packet(p_pckt, PAct_AGENT_SET_MOOD, p_agent->ThingOffset, i, 0, 0);
                p_locplayer->PanelState[mouser] = PANEL_STATE_MOOD_SET_ONE + p_panel->ID;
                if (!IsSamplePlaying(0, 21, 0))
                    play_sample_using_heap(0, 21, 127, 64, 100, -1, 1u);
                ingame.Flags |= GamF_Unkn00100000;
                return 1;
            }
            break;
        case PanT_AgentWeapon:
            // Weapon selection for single agent
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            if ((p_agent->Type == TT_PERSON) && person_can_accept_control(p_agent->ThingOffset))
            {
                p_locplayer->UserInput[mouser].ControlMode |= 0x8000;
                p_locplayer->PanelState[mouser] = PANEL_STATE_WEP_SEL_ONE + p_panel->ID;
                return 1;
            }
            break;
        case PanT_AgentMedi:
            // Use medikit
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            if ((p_agent->Type == TT_PERSON) && person_carries_any_medikit(p_agent))
            {
                my_build_packet(p_pckt, PAct_AGENT_USE_MEDIKIT, p_agent->ThingOffset, 0, 0, 0);
                return 1;
            }
            break;
        case PanT_WeaponEnergy:
            // Enable supershield
            if (p_locplayer->DoubleMode && byte_153198 - 1 != mouser)
                break;
            if (p_locplayer->DoubleMode)
                break;
            dcthing = p_locplayer->DirectControl[mouser];
            if ((things[dcthing].Flag & TngF_Destroyed) != 0)
                break;
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            if (p_agent->Type != TT_PERSON)
                break;
            build_packet(p_pckt, PAct_SHIELD_TOGGLE, dcthing, p_agent->ThingOffset, 0, 0);
            p_locplayer->UserInput[mouser].ControlMode |= 0x8000;
            return 1;
        case PanT_Grouping:
            if (mouse_over_infrared_slant_box(panel))
            {
                // Toggle thermal view
                if ((ingame.Flags & GamF_ThermalView) == 0)
                {
                    dcthing = p_locplayer->DirectControl[mouser];
                    if (things[dcthing].U.UPerson.Energy > 100)
                    {
                        ingame.Flags |= GamF_ThermalView;
                        play_sample_using_heap(0, 35, 127, 64, 100, 0, 1);
                        ingame_palette_reload();
                    }
                }
                else
                {
                    ingame.Flags &= ~GamF_ThermalView;
                    change_brightness(0);
                }
            }
            else
            {
                // Increase agent grouping
                dcthing = p_locplayer->DirectControl[mouser];
                p_locplayer->UserInput[mouser].ControlMode |= 0x8000;
                if (panel_active_based_on_target(panel))
                    my_build_packet(p_pckt, PAct_PROTECT_INC, dcthing, 0, 0, 0);
            }
            return 1;
        default:
            break;
        }
    }

    if (lbDisplay.RightButton)
    {
        struct Packet *p_pckt;
        struct Thing *p_agent;
        struct GamePanel *p_panel;

        lbDisplay.RightButton = 0;
        p_panel = &game_panel[panel];
        p_pckt = &packets[local_player_no];

        switch (p_panel->Type)
        {
        case PanT_AgentMood:
            // Change mood / drugs level
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            if ((p_agent->Type == TT_PERSON) && (p_agent->State != PerSt_DEAD))
            {
                p_locplayer->UserInput[mouser].ControlMode |= 0x4000;
                i = panel_mouse_move_mood_value(panel);
                if (panel_active_based_on_target(panel))
                    my_build_packet(p_pckt, PAct_GROUP_SET_MOOD, p_agent->ThingOffset, i, 0, 0);
                p_locplayer->PanelState[mouser] = PANEL_STATE_MOOD_SET_GRP + p_panel->ID;
                if (!IsSamplePlaying(0, 21, 0))
                    play_sample_using_heap(0, 21, 127, 64, 100, -1, 1u);
                ingame.Flags |= GamF_Unkn00100000;
                return 1;
            }
            break;
        case PanT_AgentWeapon:
            // Weapon selection for all grouped agent
            p_agent = p_locplayer->MyAgent[p_panel->ID];
            if ((p_agent->Type == TT_PERSON) && person_can_accept_control(p_agent->ThingOffset))
            {
                p_locplayer->UserInput[mouser].ControlMode |= 0x4000;
                p_locplayer->PanelState[mouser] = PANEL_STATE_WEP_SEL_GRP + p_panel->ID;
                return 1;
            }
            break;
        case PanT_Grouping:
            // Switch grouping fully on or fully off
            p_locplayer->UserInput[mouser].ControlMode |= 0x4000;
            if (panel_active_based_on_target(panel))
            {
                short dcthing;
                dcthing = p_locplayer->DirectControl[mouser];
                my_build_packet(p_pckt, PAct_PROTECT_TOGGLE, dcthing, 0, 0, 0);
            }
            return 1;
        default:
            break;
        }
    }

    if (lbDisplay.MRightButton)
    {
        struct Packet *p_pckt;
        struct Thing *p_agent;
        struct GamePanel *p_panel;

        p_panel = &game_panel[panel];
        p_pckt = &packets[local_player_no];

        switch (p_panel->Type)
        {
        case PanT_AgentBadge:
            // Center view on the selected agent
            if (!p_locplayer->DoubleMode)
            {
                p_agent = p_locplayer->MyAgent[p_panel->ID];
                if ((p_agent->Type == TT_PERSON) && ((p_agent->Flag & TngF_Destroyed) == 0))
                {
                    ushort dcthing;

                    dcthing = p_locplayer->DirectControl[mouser];
                    if ((things[dcthing].Flag & TngF_Unkn0400) == 0)
                    {
                        game_set_cam_track_thing_xz(p_agent->ThingOffset);
                        engn_yc = PRCCOORD_TO_MAPCOORD(p_agent->Y);
                        build_packet(p_pckt, PAct_SELECT_AGENT, dcthing, p_agent->ThingOffset, 0, 0);
                        if (p_agent->ThingOffset == dcthing)
                        {
                            engn_xc = PRCCOORD_TO_MAPCOORD(p_agent->X);
                            engn_zc = PRCCOORD_TO_MAPCOORD(p_agent->Z);
                        }
                        return 1;
                    }
                }
            }
            break;
        default:
            break;
        }
    }
    return 0;
}

TbBool check_panel_button(void)
{
    short panel;

    if (lbDisplay.LeftButton && lbDisplay.RightButton)
    {
        struct Packet *p_pckt;
        PlayerInfo *p_locplayer;
        short dcthing;

        lbDisplay.LeftButton = 0;
        lbDisplay.RightButton = 0;
        p_locplayer = &players[local_player_no];
        p_pckt = &packets[local_player_no];
        dcthing = p_locplayer->DirectControl[mouser];
        my_build_packet(p_pckt, PAct_PEEPS_SCATTER, dcthing,
            mouse_map_x, 0, mouse_map_z);
        return 1;
    }

    if (mouse_move_over_scanner())
    {
        if (check_scanner_input())
            return 1;
    }

    for (panel = GAME_PANELS_LIMIT - 1; panel >= 0; panel--)
    {
        if (!panel_exists(panel))
            continue;
        if (mouse_move_over_panel(panel))
        {
            if (check_panel_input(panel))
                return 1;
        }
    }
    return 0;
}

/******************************************************************************/
