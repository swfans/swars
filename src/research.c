/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file research.c
 *     Routines implementing weapons and mods research.
 * @par Purpose:
 *     Implement general functions for handling research progress.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "research.h"

#include "bfutility.h"
#include "weapon.h"
#include "cybmod.h"
#include "display.h"
#include "game.h"
#include "wadfile.h"
#include "swlog.h"
/******************************************************************************/

void load_scientist_lost_reason(ushort reason_no)
{
    int totlen;
    char *s;
    char c;
    ushort i;

    totlen = load_file_alltext("lost.txt", back_buffer + text_buf_pos);
    if (totlen == -1) {
        return;
    }
    s = (char *)(back_buffer + text_buf_pos);
    for (i = 0; i != reason_no; i++)
    {
        do
            c = *s++;
        while (c != '\n');
    }
    for (i = 0; i < sizeof(scientist_lost_reason)-1; i++,s++)
    {
        c = *s;
        if ((c == '\r') || (c == '\0'))
            break;
        scientist_lost_reason[i] = c;
    }
    scientist_lost_reason[i] = '\0';
}

void research_wep_store_daily_progress(int progress)
{
    short cwep;
    ushort cday;

    cwep = research.CurrentWeapon;
    if (research.WeaponDaysDone[cwep] < RESEARCH_DAYS_STORED-1)
    {
        cday = research.WeaponDaysDone[cwep] + 1;
    } else
    {
        // Shift the data left to make place for the next day
        for (cday = 0; cday < RESEARCH_DAYS_STORED-1; cday++) {
            research.WeaponProgress[cwep][cday] = research.WeaponProgress[cwep][cday + 1];
        }
        cday = RESEARCH_DAYS_STORED-1;
    }
    if (cday > 0)
        research.WeaponProgress[cwep][cday] = research.WeaponProgress[cwep][cday - 1];
    else
        research.WeaponProgress[cwep][cday] = 0;
    research.WeaponProgress[cwep][cday] += progress;
}

int research_wep_get_progress(short cwep)
{
    ushort cday;

    if (research.WeaponDaysDone[cwep] >= RESEARCH_DAYS_STORED-1)
        cday = RESEARCH_DAYS_STORED-1;
    else
        cday = research.WeaponDaysDone[cwep] + 1;
    return research.WeaponProgress[cwep][cday];
}

void research_wep_completed(void)
{
    short cwep;

    cwep = research.CurrentWeapon;
    research.ModsCompleted |= 1 << cwep;
    research.WeaponsAllowed &= ~(1 << cwep);
    research_unkn21_box.Lines = 0;
    research.CurrentWeapon = -1;
    refresh_equip_list = 1;
}

int research_wep_next_type(void)
{
    int wptype;

    wptype = 0;
    while ((research.WeaponsAllowed & (1 << wptype)) == 0)
    {
        wptype++;
        if (wptype >= WEP_TYPES_COUNT)
            return -1;
    }
    if ((research.CurrentWeapon == -1) ||
      (weapon_tech_level[wptype + 1] > weapon_tech_level[research.CurrentWeapon + 1])) {
        research.CurrentWeapon = wptype;
        return wptype;
    }
    return -1;
}

void research_cymod_store_daily_progress(int progress)
{
    short cmod;
    ushort cday;

    cmod = research.CurrentMod;
    if (research.ModDaysDone[cmod] < RESEARCH_DAYS_STORED-1)
    {
        cday = research.ModDaysDone[cmod] + 1;
    } else
    {
        // Shift the data left to make place for the next day
        for (cday = 0; cday < RESEARCH_DAYS_STORED-1; cday++) {
            research.ModProgress[cmod][cday] = research.ModProgress[cmod][cday + 1];
        }
        cday = RESEARCH_DAYS_STORED-1;
    }
    if (cday > 0)
        research.ModProgress[cmod][cday] = research.ModProgress[cmod][cday - 1];
    else
        research.ModProgress[cmod][cday] = 0;
    research.ModProgress[cmod][cday] += progress;
}

int research_cymod_get_progress(short cmod)
{
    ushort cday;

    if (research.ModDaysDone[cmod] >= RESEARCH_DAYS_STORED-1)
        cday = RESEARCH_DAYS_STORED-1;
    else
        cday = research.ModDaysDone[cmod] + 1;
    return research.ModProgress[cmod][cday];
}

void research_cymod_completed(void)
{
    short cmod;

    cmod = research.CurrentMod;
    research.ModsCompleted |= 1 << cmod;
    research.ModsAllowed &= ~(1 << cmod);
    //TODO CONFIG store required research in mod config (and weapon config as well)
    // Allow research of next level of the mod
    if (research.CurrentMod % 3 != 2 && cmod + 1 < MOD_EPIDERM1)
        research.ModsAllowed |= 1 << (cmod + 1);
    research_unkn21_box.Lines = 0;
    research.CurrentMod = -1;
    refresh_equip_list = 1;
}

int research_cymod_next_type(void)
{
    int mdtype;

    mdtype = 0;
    while ((research.ModsAllowed & (1 << mdtype)) == 0)
    {
        mdtype++;
        if (mdtype >= MOD_TYPES_COUNT)
            return -1;
    }
    if ((research.CurrentMod == -1) ||
      (mod_tech_level[mdtype + 1] > mod_tech_level[research.CurrentMod + 1])) {
        research.CurrentMod = mdtype;
        return mdtype;
    }
    return -1;
}

int research_unkn_func_004(ushort percent_per_day, int expect_funding, int real_funding)
{
#if 0
    int ret;
    asm volatile ("call ASM_research_unkn_func_004\n"
        : "=r" (ret) : "a" (percent_per_day), "d" (expect_funding), "b" (real_funding));
    return ret;
#else
    int n_remain, overhead;
    int points_total, ppd, delta;
    int points_by_group;

    // Adjust points to number of scientists
    points_total = percent_per_day << 8;
    n_remain = research.Scientists;
    delta = 4;
    ppd = points_total;
    if (n_remain > delta)
    {
        overhead = 2;
        while (n_remain > delta)
        {
            if (overhead >= 0x40000000)
                break;
            n_remain -= delta;
            if (n_remain <= delta)
                points_by_group = n_remain * (long long)ppd / delta;
            else
                points_by_group = ppd;
            points_total += points_by_group / overhead;
            overhead *= 2;
        }
    }
    else
    {
        points_total = n_remain * (long long)ppd / delta;
    }

    // Adjust points to amount of funding
    // The integers multiplied here may go beyond 16-bit - so the result
    // may not fit 32-bit variable; use 64-bit for intermediate values
    n_remain = real_funding;
    delta = 100 * expect_funding;
    ppd = points_total;
    if (n_remain > delta)
    {
        // Our current value of points_total is as if we've already
        // passed the iteration for overhead=1; so start with 2
        overhead = 2;
        while (n_remain > delta)
        {
            if (overhead >= 0x40000000)
                break;
            n_remain -= delta;
            if (n_remain <= delta)
                points_by_group = n_remain * (long long)ppd / delta;
            else
                points_by_group = ppd;
            points_total += points_by_group / overhead;
            overhead *= 2;
        }
    }
    else
    {
        points_total = n_remain * (long long)ppd / delta;
    }
    return points_total;
#endif
}

int research_daily_progress_for_type(ubyte rstype)
{
#if 0
    int ret;
    asm volatile ("call ASM_research_daily_progress_for_type\n"
        : "=r" (ret) : "a" (rstype));
    return ret;
#endif
    int real_funding;
    short progress;
    struct ModDef *mdef;
    struct WeaponDef *wdef;
    int scientists_died;

    real_funding = 0;

    switch (rstype)
    {
    case 0:
        if (research.CurrentWeapon == -1)
            break;
        real_funding = research.WeaponFunding;
        if (ingame.Credits < real_funding)
            real_funding = ingame.Credits;
        wdef = &weapon_defs[research.CurrentWeapon + 1];
        progress = research_unkn_func_004(wdef->PercentPerDay, wdef->Funding, real_funding);
        research_wep_store_daily_progress(progress);
        if (research_wep_get_progress(research.CurrentWeapon) < RESEARCH_COMPLETE_POINTS)
        {
            if (research.WeaponFunding && research.Scientists && research.NumBases)
                research.WeaponDaysDone[research.CurrentWeapon]++;
        }
        else
        {
            research_wep_completed();
            if (ingame.fld_unk7DE)
                research_wep_next_type();
        }
        break;

    case 1:
        if (research.CurrentMod == -1)
            break;
        real_funding = research.ModFunding;
        if (ingame.Credits < real_funding)
            real_funding = ingame.Credits;
        mdef = &mod_defs[research.CurrentMod + 1];
        progress = research_unkn_func_004(mdef->PercentPerDay, mdef->Funding, real_funding);
        research_cymod_store_daily_progress(progress);
        if (research_cymod_get_progress(research.CurrentMod) < RESEARCH_COMPLETE_POINTS)
        {
            if (research.ModFunding && research.Scientists && research.NumBases)
                research.ModDaysDone[research.CurrentMod]++;
        }
        else
        {
            research_cymod_completed();
            if (ingame.fld_unk7DE)
                research_cymod_next_type();
        }
        break;

    default:
        break;
    }
    scientists_died = 0;
    if ((real_funding > 0) && (progress > 0))
    {
        ingame.Credits -= real_funding;
        ingame.Expenditure += real_funding;
        //TODO CONFIG make research accident chance a config option
        if (LbRandomAnyShort() % 100 < 2)
        {
            scientists_died++;
            if (LbRandomAnyShort() & 1)
                scientists_died++;
            if (scientists_died > research.Scientists)
                scientists_died = research.Scientists;
            research.Scientists -= scientists_died;
        }
    }
    return scientists_died;
}

/******************************************************************************/
