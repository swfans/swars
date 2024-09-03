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

#include "bfbox.h"
#include "bfgentab.h"
#include "bfline.h"
#include "bfscreen.h"
#include "bfsprite.h"
#include "bfutility.h"
#include "bigmap.h"
#include "display.h"
#include "engintrns.h"
#include "game_speed.h"
#include "game.h"
#include "player.h"
#include "thing.h"
#include "swlog.h"
/******************************************************************************/
extern ubyte byte_1CAB64[];
extern ubyte byte_1DB088[];
extern long dword_1DC36C;

//TODO not the best location for agent state update
void update_dropped_item_under_agent_exists(struct Thing *p_agent)
{
    struct SimpleThing *p_pickup;
    ThingIdx thing;

    if (p_agent->Flag & TngF_Unkn08000000)
    {
        thing = p_agent->U.UPerson.Vehicle; // Seem to be weapon standing over rather than vehicle
        if (thing < 0)
            p_pickup = &sthings[thing];
        else
            p_pickup = NULL;
        if ((p_pickup == NULL) || (p_pickup->Type != SmTT_DROPPED_ITEM)) {
            p_agent->Flag &= ~TngF_Unkn08000000;
        }
    }
}

void SCANNER_unkn_func_201(struct TbSprite *spr, int x, int y, ubyte *fade)
{
    ubyte *oline;
    ubyte *dt;
    int ich;
    ubyte *o;

    oline = &lbDisplay.WScreen[lbDisplay.GraphicsScreenWidth * y + x];
    dt = spr->Data;
    for (ich = spr->SHeight; ich > 0; ich--)
    {
        o = oline;
        while (1)
        {
            ushort ftidx;
            sbyte len;

            len = *dt;
            if (!len)
                break;
            if (len > 0)
            {
                ++dt;
                while (len)
                {
                    ftidx = *dt;
                    *o = fade[ftidx];
                    ++dt;
                    ++o;
                    len--;
                }
            }
            else
            {
                len = -len;
                o += len;
                ++dt;
            }
        }
        ++dt;
        oline += lbDisplay.GraphicsScreenWidth;
    }
}

void SCANNER_unkn_func_202(struct TbSprite *spr, int x, int y, int ctr, int bri)
{
    ubyte *oline;
    ubyte *dt;
    int ich;
    ubyte *o;

    if ((x < 0) || (x > lbDisplay.PhysicalScreenWidth))
        return;
    if ((y < 0) || (y > lbDisplay.PhysicalScreenHeight))
        return;
    if ((x + spr->SWidth < 0) || (x + spr->SWidth > lbDisplay.PhysicalScreenWidth))
        return;
    if ((y + spr->SHeight < 0) || (y + spr->SHeight > lbDisplay.PhysicalScreenHeight))
        return;

    oline = &lbDisplay.WScreen[y * lbDisplay.GraphicsScreenWidth + x];
    dword_1DC36C = bri;
    dt = spr->Data;
    for (ich = spr->SHeight; ich > 0; ich--)
    {
        o = oline;
        while (1)
        {
            ushort ftsub, ftidx;
            sbyte len;

            len = *dt;
            if (!len)
                break;
            if (len > 0)
            {
                ++dt;
                while (len)
                {
                    ftidx = *dt++;
                    ftsub = dword_1DC36C + ((byte_1CAB64[ftidx] >> 1) + (byte_1CAB64[*o] >> 1));
                    ftidx |= byte_1DB088[ftsub] << 8;
                    *o++ = pixmap.fade_table[ftidx];
                    len--;
                }
            }
            else
            {
                len = -len;
                o += len;
                ++dt;
            }
        }
        ++dt;
        oline += lbDisplay.GraphicsScreenWidth;
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

void SCANNER_unkn_func_204(int a1, int a2, int a3)
{
    asm volatile (
      "call ASM_SCANNER_unkn_func_204\n"
        : : "a" (a1), "d" (a2), "b" (a3));
}

void SCANNER_unkn_func_205(void)
{
    asm volatile ("call ASM_SCANNER_unkn_func_205\n"
        :  :  : "eax" );
}

void SCANNER_draw_new_transparent(void)
{
    asm volatile ("call ASM_SCANNER_draw_new_transparent\n"
        :  :  : "eax" );
}

void LbSpriteDraw_1(int x, int y, struct TbSprite *spr)
{
    asm volatile (
      "call ASM_LbSpriteDraw_1\n"
        : : "a" (x), "d" (y), "b" (spr));
}

/**
 * Draw the button with standard palette, for selectable items.
 * @param px
 * @param py
 * @param spr_id
 */
void draw_new_panel_sprite_std(int px, int py, ulong spr_id)
{
    struct TbSprite *spr;
    int x, y;

    spr = &pop1_sprites[spr_id];
    if (lbDisplay.GraphicsScreenHeight >= 400) {
        x = px;
        y = py;
    } else {
        x = px >> 1;
        y = py >> 1;
    }
    if (ingame.PanelPermutation == -1)
        SCANNER_unkn_func_202(spr, x, y, ingame.Scanner.Contrast, ingame.Scanner.Brightness);
    else
        LbSpriteDraw_1(x, y, spr);
}

/**
 * Draw the button with darkened palette, like the item is unavailable.
 * @param px
 * @param py
 * @param spr_id
 */
void draw_new_panel_sprite_dark(int px, int py, ulong spr_id)
{
    struct TbSprite *spr;
    int x, y;

    spr = &pop1_sprites[spr_id];
    if (lbDisplay.GraphicsScreenHeight >= 400) {
        x = px;
        y = py;
    } else {
        x = px >> 1;
        y = py >> 1;
    }
    if (ingame.PanelPermutation == -1)
        SCANNER_unkn_func_202(spr, x, y, ingame.Scanner.Contrast, 8);
    else
        SCANNER_unkn_func_201(spr, x, y, &pixmap.fade_table[4096]);
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
    // We're expecting to draw 4 items; 8 are supported mostly to signal an issue
    static short dtx[] = {0+10, 0+10, 72-10, 72-10, 0+16, 0+16, 72-16, 72-16,};
    static short dty[] = {0+4, 22-4, 0+4, 22-4, 0+4, 22-4, 0+4, 22-4,};
    int i;
    TbPixel col;

    if (ingame.PanelPermutation == -3)
        col = 26;
    else
        col = 247;

    for (i = 0; i < min(amount,8); i++)
    {
      if (lbDisplay.GraphicsScreenHeight < 400)
          LbDrawBox((x + dtx[i]) >> 1, (y + dty[i]) >> 1, 2, 2, col);
      else
          LbDrawBox(x + dtx[i], y + dty[i], 4, 4, col);
    }
}

/**
 * For weapons which contain up to four itemized parts, draw the items.
 *
 * @param a1
 * @param y
 * @param plagent
 * @param wtype
 */
void draw_fourpack_items(int a1, ushort y, short plagent, short wtype)
{
    ushort fp;

    fp = weapon_fourpack_index(wtype);
    if (fp < WFRPK_COUNT) {
        PlayerInfo *p_locplayer;
        p_locplayer = &players[local_player_no];
        draw_fourpack_amount(a1, y, p_locplayer->FourPacks[fp][plagent]);
    }
}

TbBool draw_panel_pickable_thing_below_agent(struct Thing *p_agent)
{
    struct SimpleThing *p_pickup;
    ThingIdx thing;
    TbBool drawn;

    drawn = false;
    if (p_agent->Flag & TngF_Unkn08000000)
    {
        ushort weptype;
        thing = p_agent->U.UPerson.Vehicle; // Seem to be weapon standing over rather than vehicle
        if (thing < 0)
            p_pickup = &sthings[thing];
        else
            p_pickup = NULL;
        if ((p_pickup != NULL) && (p_pickup->Type == SmTT_DROPPED_ITEM)) {
            lbDisplay.DrawFlags = 0;
            weptype = p_pickup->U.UWeapon.WeaponType;
            if (weptype)
                draw_new_panel_sprite_std(548, 364, weapon_sprite_index(weptype, false));
            else
                draw_new_panel_sprite_std(548, 364, 70);
            draw_new_panel_sprite_std(540, 360, 12);
            drawn = true;
        }
    }
    return drawn;
}

TbBool draw_panel_pickable_thing_player_targeted(PlayerInfo *p_locplayer)
{
    ;
    struct SimpleThing *p_pickup;
    ThingIdx thing;
    TbBool drawn;

    drawn = false;
    if (p_locplayer->TargetType == 5)
    {
        ushort weptype;
        thing = p_locplayer->field_102;
        if (thing < 0)
        {
            lbDisplay.DrawFlags = 0;
            p_pickup = &sthings[thing];
            weptype = p_pickup->U.UWeapon.WeaponType;
            if (weptype)
                draw_new_panel_sprite_std(548, 364, weapon_sprite_index(weptype, false));
            else
                draw_new_panel_sprite_std(548, 364, 70);
            draw_new_panel_sprite_std(540, 360, 12);
            drawn = true;
        }
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

    for (nweptype = 0; nweptype < WEP_TYPES_COUNT; nweptype++)
    {
        if (!wepflags)
            break;
        if (wepflags & 1)
        {
            ncarried++;
            if (nweptype + 1 > current_weapon)
                ncarr_above++;
            if (nweptype + 1 < current_weapon)
                ncarr_below++;
        }
        wepflags >>= 1;
    }

    *p_ncarr_below = ncarr_below;
    *p_ncarr_above = ncarr_above;
    return ncarried;
}

TbBool draw_weapons_list_single(PlayerInfo *p_locplayer, ushort plagent, ulong weapons_carried, short current_weapon)
{
    ushort nshown;
    int weptype;
    int nchecked;
    ulong wepflags;
    int cy;
    int ncarried;
    int ncarr_below;
    int ncarr_above;
    TbBool ret;

    ret = false;
    ncarried = count_weapons_in_flags(&ncarr_below, &ncarr_above, weapons_carried, current_weapon);
    if (ncarried <= 6)
        ncarr_below = 0;
    else if (ncarr_below <= 4)
        ncarr_below = 0;
    else
        ncarr_below -= 4;

    nshown = 0;
    wepflags = weapons_carried;
    cy = 36;
    nchecked = 0;
    for (weptype = 1; weptype < WEP_TYPES_COUNT; weptype++, wepflags >>= 1)
    {
        if (wepflags == 0)
            break;
        if (wepflags & 1)
        {
            if (ncarr_below > nshown)
            {
                nshown++;
                if (nchecked == 6)
                    break;
                continue;
            }
            lbDisplay.DrawFlags = 0;

            if (!p_locplayer->WepDelays[plagent][weptype] || (gameturn & 1))
            {
                if (nshown == 6)
                    draw_new_panel_sprite_std(22, cy, 13);
                else
                    draw_new_panel_sprite_std(22, cy, 12);
            }

            if (!p_locplayer->WepDelays[plagent][weptype] || (gameturn & 1))
                draw_new_panel_sprite_std(30, cy + 4, weapon_sprite_index(weptype, false));
            if (weptype == current_weapon)
            {
                lbDisplay.DrawFlags = 0;
                draw_new_panel_sprite_std(30, cy + 4, weapon_sprite_index(weptype, true));
            }
            if (!lbDisplay.MRightButton)
            {
                short msy, msx;

                msy = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.RMouseY : lbDisplay.RMouseY;
                msx = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.RMouseX : lbDisplay.RMouseX;
                if (in_box(msx, msy, 22, cy, 76, 22))
                {
                    lbDisplay.DrawFlags = 0;
                    ret = true;
                    p_locplayer->PanelItem[mouser] = weptype;
                    draw_new_panel_sprite_std(22, cy, 90);
                }
            }
            draw_fourpack_items(22, cy, plagent, weptype);
            cy += 28;
            ++nchecked;
            ++nshown;
            if (nchecked == 6)
                break;
        }
    }
    return ret;
}

short draw_current_weapon_button(PlayerInfo *p_locplayer, short nagent)
{
    struct Thing *p_agent;
    ushort curwep, prevwep;
    short cx, cy;

    if (lbDisplay.GraphicsScreenHeight < 400) {
        cy = 28;
        cx = 158 * nagent + 66;
    } else {
        cy = 29;
        cx = 157 * nagent + 65;
    }
    p_agent = p_locplayer->MyAgent[nagent];
    // Protect from damaged / unfinished levels
    if (p_agent->Type != TT_PERSON)
        return 0;

    curwep = p_agent->U.UPerson.CurrentWeapon;
    prevwep = p_locplayer->PrevWeapon[nagent];
    if (!curwep && !prevwep) {
        prevwep = find_nth_weapon_held(p_agent->ThingOffset, 1);
        p_locplayer->PrevWeapon[nagent] = prevwep;
    }

    // Highlight button border on mouse over
    if (curwep || prevwep)
    {
        TbBool wep_highlight;
        short msx, msy;
        wep_highlight = false;
        if (!lbDisplay.MRightButton) {
            msx = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.RMouseX : lbDisplay.RMouseX;
            msy = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.RMouseY : lbDisplay.RMouseY;
            if (in_box(msx, msy, cx - 4, cy - 4, 76, 24))
                wep_highlight = true;
        }
        {
            msx = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
            msy = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
            if (in_box(msx, msy, cx - 4, cy - 4, 76, 24))
                wep_highlight = true;
        }
        if (wep_highlight)
        {
            if (curwep)
                p_locplayer->PanelItem[mouser] = curwep;
            else
                p_locplayer->PanelItem[mouser] = prevwep;
            draw_new_panel_sprite_std(cx - 8, cy - 4, 90);
        }
    }
    if (curwep) // Active weapon - draw lighted weapon shape
    {
        if (!p_locplayer->WepDelays[nagent][curwep] || (gameturn & 1))
        {
            if (p_agent->State == PerSt_PROTECT_PERSON)
            {
                struct TbSprite *spr;
                if (ingame.PanelPermutation == -1)
                {
                    spr = &pop1_sprites[14];
                    if (lbDisplay.GraphicsScreenHeight < 400)
                        SCANNER_unkn_func_202(spr, cx >> 1, cy >> 1,
                            ingame.Scanner.Contrast, ingame.Scanner.Brightness);
                    else
                        SCANNER_unkn_func_202(spr, cx, cy,
                            ingame.Scanner.Contrast, ingame.Scanner.Brightness);
                }
                draw_new_panel_sprite_dark(cx, cy, weapon_sprite_index(curwep, true));
            }
            else
            {
                struct TbSprite *spr;
                if (ingame.PanelPermutation == -1)
                {
                    spr = &pop1_sprites[14];
                    if (lbDisplay.GraphicsScreenHeight < 400)
                        SCANNER_unkn_func_202(spr, cx >> 1, cy >> 1,
                            ingame.Scanner.Contrast, ingame.Scanner.Brightness);
                    else
                        SCANNER_unkn_func_202(spr, cx, cy,
                            ingame.Scanner.Contrast, ingame.Scanner.Brightness);
                }
                draw_new_panel_sprite_std(cx, cy, weapon_sprite_index(curwep, true));
            }
            draw_fourpack_items(cx - 8, cy - 4, nagent, curwep);
        }
    }
    else
    { // Weapon is hidden - draw with dark weapon shape
        curwep = prevwep;
        if (curwep && (!p_locplayer->WepDelays[nagent][curwep] || (gameturn & 1)))
        {
            if (p_agent->State == PerSt_PROTECT_PERSON)
                draw_new_panel_sprite_dark(cx, cy, weapon_sprite_index(curwep, false));
            else
                draw_new_panel_sprite_std(cx, cy, weapon_sprite_index(curwep, false));
            draw_fourpack_items(cx - 8, cy - 4, nagent, curwep);
        }
    }
    return curwep;
}

TbBool draw_agent_weapons_selection(PlayerInfo *p_locplayer, struct Thing *p_agent, ubyte *cur_weapons, int nagent)
{
    int cx, cy;
    int weptype;
    int dcx, dcy;
    int nunk1;
    ulong wepflags;
    ushort plagent;
    ushort nshown;
    int nchecked;
    TbBool wep_highlight;
    TbBool ret;

    ret = false;
    nunk1 = 0;
    plagent = p_agent->U.UPerson.ComCur & 3;
    wepflags = p_agent->U.UPerson.WeaponsCarried;
    cy = 44;
    cx = 158 * nagent + 42;
    // Some weapons are not selectable
    wepflags &= ~(1 << (WEP_ENERGYSHLD-1));
    nshown = 0;
    nchecked = 0;
    for (weptype = 1; weptype < WEP_TYPES_COUNT; weptype++, wepflags >>= 1)
    {
        if (wepflags == 0)
            break;
        if ((wepflags & 1) == 0)
            continue;

        if (nunk1 > nshown || cur_weapons[plagent] == weptype)
        {
            ++nshown;
            if (nchecked == 12)
                break;
            continue;
        }
        lbDisplay.DrawFlags = 0;
        if (!p_locplayer->WepDelays[plagent][weptype] || (gameturn & 1))
        {
            if (!nchecked)
                draw_new_panel_sprite_std(cx, cy, 13);
            else
                draw_new_panel_sprite_std(cx, cy, 94);
        }

        if (!p_locplayer->WepDelays[plagent][weptype] || (gameturn & 1))
        {
            if (!nchecked)
                draw_new_panel_sprite_std(cx + 24, cy + 12, weapon_sprite_index(weptype, false));
            else
                draw_new_panel_sprite_std(cx + 8, cy + 8, weapon_sprite_index(weptype, false));
        }
        dcx = 0;
        dcy = 0;
        if (!nchecked)
        {
            dcx = 14;
            dcy = 8;
        }

        wep_highlight = false;
        if (!lbDisplay.MRightButton) {
            short dch;
            short msx, msy;
            dch = nchecked ? 0 : 8;
            msx = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.RMouseX : lbDisplay.RMouseX;
            msy = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.RMouseY : lbDisplay.RMouseY;
            if (in_box(msx, msy, cx + dcx, cy + dcy, 76, 30 - dch))
                wep_highlight = true;
        }
        if (!wep_highlight) {
            short dch;
            short msx, msy;
            dch = nchecked ? 0 : 8;
            msx = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
            msy = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
            if (in_box(msx, msy, cx + dcx, cy + dcy, 76, 30 - dch))
                wep_highlight = true;
        }
        if (wep_highlight)
        {
            lbDisplay.DrawFlags = 0;
            p_locplayer->PanelItem[mouser] = weptype;
            if (nchecked)
                draw_new_panel_sprite_std(cx, cy, 93);
            else
                draw_new_panel_sprite_std(cx, cy, 92);
            ret = 1;
        }

        if (nchecked)
            draw_fourpack_items(cx, cy + 4, plagent, weptype);
        else
            draw_fourpack_items(cx + 16, cy + 8, plagent, weptype);
        if (nchecked) {
            cy += 28;
        } else {
            cx += 16;
            cy += 32;
        }
        ++nchecked;
        ++nshown;
        if (nchecked == 12)
            break;
    }
    return ret;
}

TbBool func_1caf8(void)
{
    TbBool ret;
    PlayerInfo *p_locplayer;
    struct Thing *p_agent;
    ubyte cur_weapons[4];
    short dcthing;
    int nagent;
    int panstate;

    p_locplayer = &players[local_player_no];
    dcthing = direct_control_thing_for_player(local_player_no);
    p_agent = &things[dcthing];

    p_locplayer->PanelItem[mouser] = 0;
    // FIXME a strange place for fixing state of an agent; should be moved to game world update
    update_dropped_item_under_agent_exists(p_agent);
    ret = draw_panel_pickable_thing_below_agent(p_agent);
    if (!ret)
        draw_panel_pickable_thing_player_targeted(p_locplayer);

    if (ingame.PanelPermutation >= 0)
    {
        ushort plagent;
        plagent = p_agent->U.UPerson.ComCur & 3;
        ret = draw_weapons_list_single(p_locplayer, plagent,
            p_agent->U.UPerson.WeaponsCarried, p_agent->U.UPerson.CurrentWeapon);
    }
    else
    {
        for (nagent = 0; nagent < playable_agents; nagent++)
        {
            ushort curwep;
            p_agent = p_locplayer->MyAgent[nagent];
            if ((p_agent->Type != TT_PERSON) || (p_agent->Flag & TngF_Unkn0002) != 0) {
                cur_weapons[nagent] = 0;
                continue;
            }
            curwep = draw_current_weapon_button(p_locplayer, nagent);
            cur_weapons[nagent] = curwep;
        }

        ret = false;
        panstate = p_locplayer->PanelState[mouser];
        if (panstate >= 1 && panstate <= 8)
        {
            nagent = (panstate - 1) & 3;
            p_agent = p_locplayer->MyAgent[nagent];
            if (p_agent->Type == TT_PERSON)
                ret |= draw_agent_weapons_selection(p_locplayer, p_agent, cur_weapons, nagent);

        }
    }

    lbDisplay.DrawFlags = 0;
    return ret;
}

void draw_agent_grouping_bars(void)
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
        if (lbDisplay.GraphicsScreenHeight < 400)
        {
            if (ingame.PanelPermutation == -1)
                SCANNER_unkn_func_202(&pop1_sprites[69], 2, 2 * (107 - 6 * n) >> 1,
                  ingame.Scanner.Contrast, ingame.Scanner.Brightness);
            else
                LbSpriteDraw_1(2, 2 * (107 - 6 * n) >> 1, &pop1_sprites[69]);
        }
        else
        {
            if (ingame.PanelPermutation == -1)
                SCANNER_unkn_func_202(&pop1_sprites[69], 4, 89 + 2 * (107 - 6 * n),
                  ingame.Scanner.Contrast, ingame.Scanner.Brightness);
            else
                LbSpriteDraw_1(4, 89 + 2 * (107 - 6 * n), &pop1_sprites[69]);
        }
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

void draw_shield_level(short x, short y, ushort w, ushort h)
{
    ubyte m;
    long waftx, wafty;
    ushort tmx, tmy;
    struct EnginePoint point4;
    struct EnginePoint point2;
    struct EnginePoint point1;
    struct EnginePoint point3;

    m = 1;
    if (lbDisplay.GraphicsScreenHeight < 400)
        m = 0;
    point1.pp.X = x << m;
    point1.pp.Y = y << m;
    point4.pp.X = (x + w) << m;
    point4.pp.Y = y << m;
    point2.pp.Y = (y + h) << m;
    point2.pp.X = (x + w - 3) << m;
    point3.pp.Y = (y + h) << m;
    point3.pp.X = (x - 3) << m;

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

void draw_health_level(short x, short y, ushort w, ushort h, short lv, ushort lvmax, ubyte col, ubyte a8)
{
    short cw, ch;

    if ((lv <= 0) || (lvmax == 0))
        return;

    cw = w * lv / lvmax;
    if (a8)
    {
        draw_shield_level(x, y, cw, h);
    }
    else if (lbDisplay.GraphicsScreenHeight < 400)
    {
        short cx, cy;
        cx = x;
        cy = y;
        for (ch = h; ch > 0; ch--)
        {
            if (lbDisplay.GraphicsScreenHeight < 400)
                LbDrawLine(2 * cx >> 1, 2 * cy >> 1, 2 * (cx + cw) >> 1, 2 * cy >> 1, col);
            else
                LbDrawLine(2 * cx, 2 * cy, 2 * (cx + cw), 2 * cy, col);
            --cx;
            ++cy;
        }
    }
    else
    {
        short cx, cy;
        cx = 2 * x;
        cy = 2 * y;
        cw *= 2;
        for (ch = 2 * h; ch > 0; ch--)
        {
            if (lbDisplay.GraphicsScreenHeight < 400)
                LbDrawLine(cx >> 1, cy >> 1, (cx + cw) >> 1, cy >> 1, col);
            else
                LbDrawLine(cx, cy, cx + cw, cy, col);
            --cx;
            ++cy;
        }
    }
}

void func_1eae4(int x, short y, int w, ushort h, short lv, ushort lvmax, ubyte col, int a8)
{
    short cw, ch;

    if ((lv <= 0) || (lvmax == 0))
        return;

    ch = h * lv / lvmax;
    if (lbDisplay.GraphicsScreenHeight < 400)
    {
        short cx, cy;
        cx = x;
        cy = y;
        for (cw = w; cw > 0; cw--)
        {
            short cy1, cy2;
            cy1 = cy + h;
            cy2 = cy + h - ch;
            if (lbDisplay.GraphicsScreenHeight < 400)
                SCANNER_unkn_func_203(2 * cx >> 1, 2 * cy1 >> 1, 2 * cx >> 1, 2 * cy2 >> 1, col,
                    ingame.Scanner.Contrast, ingame.Scanner.Brightness);
            else
                SCANNER_unkn_func_203(2 * cx, 2 * cy1, 2 * cx, 2 * cy2, col,
                    ingame.Scanner.Contrast, ingame.Scanner.Brightness);
            ++cx;
            ++cy;
        }
    }
    else
    {
        short cx, cy;
        cx = 2 * x;
        cy = 2 * y;
        for (cw = 2 * w; cw > 0; cw--)
        {
            short cy1, cy2;
            cy1 = 2*h + cy;
            cy2 = 2*h + cy - 2*ch;
            if (lbDisplay.GraphicsScreenHeight < 400)
                LbDrawLine(cx >> 1, cy1 >> 1, cx >> 1, cy2 >> 1, col);
            else
                LbDrawLine(cx, cy1, cx, cy2, col);
            ++cx;
            ++cy;
        }
    }
}

void draw_mood_level(short x, short y, ushort w, int h, short value)
{
    short cent_x;
    short cur_x, cur_y;
    TbPixel col;
    short fade;
    short i;

    fade = value >> 2;
    if (value >= 0)
        col = pixmap.fade_table[PALETTE_8b_COLORS * (63 - fade) + colour_lookup[2]];
    else
        col = pixmap.fade_table[PALETTE_8b_COLORS * (63 + fade) + colour_lookup[4]];

    cent_x = x + (w >> 1);
    if (lbDisplay.GraphicsScreenHeight < 400)
    {
        cur_x = cent_x;
        cur_y = y;

        for (i = h; i > 0; i--)
        {
            SCANNER_unkn_func_203(2 * cur_x >> 1, 2 * cur_y >> 1,
                2 * (cur_x + (value >> 2)) >> 1, 2 * cur_y >> 1,
                col, ingame.Scanner.Contrast, ingame.Scanner.Brightness);
            cur_x--;
            cur_y++;
        }
    }
    else
    {
        cur_x = 2 * cent_x;
        cur_y = 2 * y;

        for (i = 2 * h; i > 0; i--)
        {
            SCANNER_unkn_func_203(cur_x, cur_y, cur_x + (value >> 1), cur_y,
                col, ingame.Scanner.Contrast, ingame.Scanner.Brightness);
            cur_x--;
            cur_y++;
        }
    }
}

void draw_mood_limits(short x, short y, short w, short h, short value, short maxval)
{
    short scaled_val;
    short curr_x;
    TbPixel col;

    if (value <= 0)
        return;

    col = colour_lookup[1];
    scaled_val = (w * value / maxval) >> 1;

    curr_x = 2 * (x + (w >> 1) - scaled_val);
    if (lbDisplay.GraphicsScreenHeight < 400)
        LbDrawLine(curr_x >> 1, 2 * y >> 1, (curr_x - 4) >> 1, 2 * (y + h) >> 1, col);
    else
        LbDrawLine(curr_x, 2 * y, curr_x - 4, 2 * (y + h), col);

    curr_x = 2 * (x + (w >> 1) + scaled_val);
    if (lbDisplay.GraphicsScreenHeight < 400)
        LbDrawLine(curr_x >> 1, 2 * y >> 1, (curr_x - 4) >> 1, 2 * (y + h) >> 1, col);
    else
        LbDrawLine(curr_x, 2 * y, curr_x - 4, 2 * (y + h), col);
}

void draw_energy_bar(int x1, int y1, int len_mul, int len_div)
{
    short scaled_val;
    short w, h;
    TbPixel col;

    if (len_div == 0)
        return;

    col = colour_lookup[1];
    scaled_val = 2 * (7 * len_mul / len_div);

    w = x1 + scaled_val;
    h = y1 - scaled_val;
    if (lbDisplay.GraphicsScreenHeight < 400)
        LbDrawLine((x1 + 0) >> 1, y1 >> 1, w >> 1, h >> 1, col);
    else
        LbDrawLine(x1 + 0, y1, w, h, col);
    w = x1 + 2 + scaled_val;
    h = y1 - scaled_val;
    if (lbDisplay.GraphicsScreenHeight < 400)
        LbDrawLine((x1 + 2) >> 1, y1 >> 1, w >> 1, h >> 1, col);
    else
        LbDrawLine(x1 + 2, y1, w, h, col);

    if (lbDisplay.GraphicsScreenHeight >= 400)
    {
        w = x1 + 1 + scaled_val;
        h = y1 - scaled_val;
        LbDrawLine(x1 + 1, y1, w, h, col);
        w = x1 + 3 + scaled_val;
        h = y1 - scaled_val;
        LbDrawLine(x1 + 3, y1, w, h, col);
    }
}


void draw_new_panel(void)
{
    int i;
    PlayerInfo *p_locplayer;

    p_locplayer = &players[local_player_no];
    // If an agent has a medkit, use the sprite with lighted cross
    for (i = 0; i < playable_agents; i++)
    {
        struct Thing *p_agent;
        p_agent = p_locplayer->MyAgent[i];
        if ((p_agent->Type == TT_PERSON) && person_carries_any_medikit(p_agent))
            game_panel[8+i].Spr = 96;
        else
            game_panel[8+i].Spr = 95;
    }

    { // If supershield is enabled for the current agent, draw energy bar in red
        struct Thing *p_agent;
        p_agent = &things[p_locplayer->DirectControl[0]];
        if ((p_agent->Type == TT_PERSON) && (p_agent->Flag & TngF_Unkn0100) != 0)
        {
            game_panel[16].Spr = 99;
            if (lbDisplay.GraphicsScreenHeight >= 400)
                game_panel[17].Spr = 106;
        }
        else
        {
            game_panel[16].Spr = 10;
            if (lbDisplay.GraphicsScreenHeight >= 400)
                game_panel[17].Spr = 105;
        }
    }

    for (i = 0; true; i++)
    {
        struct GamePanel *panel;

        panel = &game_panel[i];
        if (panel->Spr < 0)
          break;
        if (panel->Spr == 0)
          continue;
        lbDisplay.DrawFlags = 0;

        if ( panel->Type != 1 && panel->Type != 6 && panel->Type != 5 )
        {
            draw_new_panel_sprite_std(panel->X, panel->Y, panel->Spr);
        }
        else
        {
            TbBool is_visible;

            is_visible = true;
            if (panel->Type == 5 && panel->ID < playable_agents)
            {
                struct Thing *p_agent;
                ubyte weapon;

                p_agent = p_locplayer->MyAgent[panel->ID];
                if (p_agent->Type != TT_PERSON)
                    break;
                weapon = p_agent->U.UPerson.CurrentWeapon;
                if (weapon == 0)
                    weapon = p_locplayer->PrevWeapon[panel->ID];
                is_visible = !p_locplayer->WepDelays[panel->ID][weapon] || (gameturn & 1);
            }

            if (p_locplayer->DoubleMode)
            {
                struct Thing *p_agent;
                if (!is_visible)
                    continue;
                if (panel->ID >= playable_agents)
                    continue;
                p_agent = p_locplayer->MyAgent[panel->ID];
                if ((p_agent->Type != TT_PERSON) || (p_agent->Flag & TngF_Unkn0002))
                    continue;
                draw_new_panel_sprite_std(panel->X, panel->Y, panel->Spr);
            }
            else
            {
                struct Thing *p_agent;
                if (!is_visible)
                    continue;
                if (panel->ID >= playable_agents)
                    continue;
                p_agent = p_locplayer->MyAgent[panel->ID];
                if ((p_agent->Type != TT_PERSON) || (p_agent->Flag & TngF_Unkn0002))
                    continue;

                if (panel->Type == 5) {
                    ubyte weapon;

                    weapon = p_agent->U.UPerson.CurrentWeapon;
                    if (weapon == 0)
                        weapon = p_locplayer->PrevWeapon[panel->ID];
                    if (weapon == 0)
                        continue;
                }

                if ((p_agent->State == PerSt_PROTECT_PERSON) || (p_agent->Flag2 & TgF2_Unkn10000000))
                    draw_new_panel_sprite_dark(panel->X, panel->Y, panel->Spr);
                else
                    draw_new_panel_sprite_std(panel->X, panel->Y, panel->Spr);
            }
        }
    }

    if (gameturn & 4)
    {
        int x;
        short dcthing;
        struct Thing *p_agent;

        x = 0;
        dcthing = direct_control_thing_for_player(local_player_no);
        p_agent = &things[dcthing];
        if ((p_agent->Flag & TngF_Unkn0002) == 0 && (p_agent->Flag2 & TgF2_Unkn0800) == 0)
        {
            ushort plagent;

            plagent = p_agent->U.UPerson.ComCur & 3;
            if (lbDisplay.GraphicsScreenHeight < 400)
            {
                switch (plagent)
                {
                case 0:
                  x = 4;
                  break;
                case 1:
                  x = 154;
                  break;
                case 2:
                  x = 308;
                  break;
                case 3:
                  x = 472;
                  break;
                }
            }
            else
            {
                switch (plagent)
                {
                case 0:
                  x = 4;
                  break;
                case 1:
                  x = 153;
                  break;
                case 2:
                  x = 306;
                  break;
                case 3:
                  x = 469;
                  break;
                }
            }
            draw_new_panel_sprite_std(x, 2, 6 + plagent);
        }
    }
    lbDisplay.DrawFlags = 0;

    if (!func_1caf8())
    {
        if (ingame.Flags & GamF_Unkn0200) {
            ulong md, y;
            md = p_locplayer->UserInput[0].ControlMode & 0x1FFF;
            if (md == 1 && pktrec_mode != PktR_PLAYBACK) {
                y = alt_at_point(mouse_map_x, mouse_map_z);
                func_702c0(mouse_map_x, y >> 5, mouse_map_z, 64, 64, colour_lookup[2]);
            }
        }
    }
    draw_agent_grouping_bars();

    // Fill the left energy bar
    {
        short dcthing;
        struct Thing *p_agent;
        int lv, lvmax, col, w;

        dcthing = direct_control_thing_for_player(local_player_no);
        p_agent = &things[dcthing];
        if ((p_agent->U.UPerson.Energy < 50) && (gameturn & 1))
            col = 2;
        else
            col = 1;
        lvmax = p_agent->U.UPerson.MaxEnergy;
        lv = p_agent->U.UPerson.Energy;
        if (lbDisplay.GraphicsScreenHeight < 400)
            w = 0;
        else
            w = 45;
        func_1eae4(3, 30, 4, 54+w, lv, lvmax, colour_lookup[col], 0);
    }

    // Fill the agents bar at top
    for (i = 0; i < playable_agents; i++)
    {
        struct Thing *p_agent;
        int lv, lvmax, x, w;

        p_agent = p_locplayer->MyAgent[i];
        if ((p_agent->Type != TT_PERSON) || (p_agent->Flag & TngF_PlayerAgent) == 0) {
            LOGERR("Agent %d unexpected flags", i);
            return;
        }
        if ((p_agent->Flag & TngF_Unkn0002) == 0)
        {
            x = 79 * i + 27;
            // Draw health level
            lv = p_agent->Health;
            lvmax = p_agent->U.UPerson.MaxHealth;
            if (lv <= lvmax) { // Normal health amount
                draw_health_level(x, 2, 44, 2, lv, lvmax, colour_lookup[1], 0);
            } else { // Health reinforced beyond max is drawn in red
                draw_health_level(x, 2, 44, 2, lvmax, lvmax, colour_lookup[1], 0);
                draw_health_level(x, 2, 44, 2, lv - lvmax, lvmax, colour_lookup[2], 0);
            }
            // Draw shield level over health
            lv = p_agent->U.UPerson.ShieldEnergy;
            draw_health_level(x, 2, 44, 2, lv, 0x400, colour_lookup[1], 1);
            // Draw drug level aka mood (or just a red line if no drugs)
            w = game_panel[4+i].Width;
            x = game_panel[4+i].X >> 1;
            draw_mood_level(x, 6, w, 3, p_agent->U.UPerson.Mood);
            // Draw stamina level which caps the mood level
            lv = p_agent->U.UPerson.Stamina;
            lvmax = p_agent->U.UPerson.MaxStamina;
            draw_mood_limits(x, 6, w, 2, lv, lvmax);
            if (lbDisplay.GraphicsScreenHeight < 400)
                x = 158 * i + 54;
            else
                x = 157 * i + 54;
            lv = p_agent->U.UPerson.Energy;
            lvmax = p_agent->U.UPerson.MaxEnergy;
            draw_energy_bar(x + 80, 18, lv, lvmax);
        }
    }

    ingame.Scanner.MX = engn_xc >> 7;
    ingame.Scanner.MZ = engn_zc >> 7;
    ingame.Scanner.Angle = 2047 - ((engn_anglexz >> 5) & 0x7FF);
    SCANNER_draw_new_transparent();

    // Objective text, or net players list
    {
        int x, y, w;
        x = ingame.Scanner.X1 - 1;
        if (x < 0)
            x = 0;
        if (lbDisplay.GraphicsScreenWidth >= 640)
            y = lbDisplay.GraphicsScreenHeight - 18;
        else
            y = lbDisplay.GraphicsScreenHeight - 9;
        if (in_network_game) {
            SCANNER_unkn_func_205();
            w = lbDisplay.PhysicalScreenWidth;
        } else {
            // original width 67 low res, 132 high res
            w = ingame.Scanner.X2 - ingame.Scanner.X1 + 3;
        }
        SCANNER_unkn_func_204(x, y, w);
    }

    // Thermal vision button light
    if ((ingame.Flags & GamF_Unkn8000) != 0) {
        int x;
        x = 238;
        if (lbDisplay.GraphicsScreenHeight >= 400)
            x += 89;
        draw_new_panel_sprite_std(4, x, 91);
    }
}

/******************************************************************************/
