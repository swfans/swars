/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file weapon.c
 *     Weapons related functions.
 * @par Purpose:
 *     Implement functions for handling weapons.
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
#include "weapon.h"
#include "thing.h"
#include "player.h"
#include "game.h"
/******************************************************************************/
struct WeaponDef weapon_defs[] = {
    { 0,    0,  0,  0,   0,  0, 0, 0, 0,    0,     0,     0,  0},
    { 5,   50,  4,  5,   8, 10, 1, 1, 0,  272,    40,    40, 10},
    { 8,  100,  3,  8,  20, 10, 1, 1, 0,  273,   120,   120, 10},
    { 9,  200,  5,  8,   2, 10, 2, 2, 3, 1811,   370,   370, 10},
    { 9,  200,  5,  8,   2, 10, 2, 2, 3, 1812,   420,   420, 10},
    {12,  800, 30,  8, 300, 10, 1, 1, 0,  280,   750,   750, 10},
    { 6,  800, 40,  8, 600, 10, 1, 1, 0,  789,  1000,  1000, 10},
    { 1,    0, 10,  8,   2, 10, 1, 1, 0, 2319,   150,   150, 10},
    { 4,   80,  1, 16,   6,  5, 0, 0, 0,  535,   160,   160, 10},
    { 0, 9999,  3, 16,   4,  5, 0, 0, 0, 1821,   200,   200, 10},
    { 6,  800, 40,  8, 600, 10, 0, 0, 0,  545,   100,   100, 10},
    { 6,  800, 40,  8, 600, 10, 0, 0, 0,  542,    80,    80, 10},
    { 4,  300,  5, 16,   4,  5, 0, 0, 0, 1817,   620,   620, 10},
    { 4, 9999, 40, 16, 200,  5, 0, 0, 0,  283,   480,   480, 10},
    { 4, 9999,  5, 16,   4,  5, 0, 0, 0,  537,   400,   400, 10},
    {16, 1300, 80,  8, 200, 10, 1, 1, 0,  284,   300,   300, 10},
    { 4, 9999,  5, 16,   4,  5, 0, 0, 0,  803,  1050,  1050, 10},
    {10,  550,  2, 16,   4,  5, 0, 0, 0, 2586,  2000,  2000, 10},
    {10,   50,  1,  8,   4,  5, 0, 0, 0,  274,   100,   100, 10},
    { 4, 9999,  1, 16,   4,  5, 0, 0, 0, 1560,   850,   850, 10},
    {11, 1500, 10, 16,   4,  5, 0, 0, 0, 1574, 65535, 65535, 10},
    { 2,    0, 10,  8,   2, 10, 1, 1, 0, 2345,   450,   450, 10},
    { 8, 9999, 20, 16, 200,  5, 0, 0, 0, 1575,  1100,  1100, 10},
    { 0, 9999,  1, 16,   4,  5, 0, 0, 0, 2574,    80,    80, 10},
    { 4, 9999,  1, 16, 200,  5, 0, 0, 0, 1573,  1200,  1200, 10},
    { 8,   25, 80, 16, 600,  5, 0, 0, 0, 1576,  1300,  1300, 10},
    { 2,  500, 50, 16, 600,  5, 0, 0, 0, 1572,   650,   650, 10},
    { 1, 9999,  1, 16,  60,  5, 0, 0, 0, 1302,    30,    30, 10},
    { 1, 9999,  1, 16,  60,  5, 0, 0, 0, 1311,   130,   130, 10},
    {10,   50,  1,  8,   4,  5, 0, 0, 0,  288,   950,   950, 10},
    { 4, 9999,  1, 16,   4,  5, 0, 0, 0, 1314,  1000,  1000, 10},
    { 0,    0,  0, 16,   4,  5, 0, 0, 0, 1352,  3000,    10,  8},
    { 0,    0,  0, 16,   4,  5, 0, 0, 0, 1352,  3000,    10,  8},
};

void do_weapon_quantities_net_to_player(struct Thing *p_person)
{
    ushort plyr, cc2;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    cc2 = (p_person->U.UPerson.ComCur & 3);

    if (person_carries_weapon(p_person, WEP_NUCLGREN))
        players[plyr].FourPacks[WFRPK_NUCLGREN][cc2] = net_agents__FourPacks[plyr][cc2][WFRPK_NUCLGREN];
    if (person_carries_weapon(p_person, WEP_ELEMINE))
        players[plyr].FourPacks[WFRPK_ELEMINE][cc2] = net_agents__FourPacks[plyr][cc2][WFRPK_ELEMINE];
    if (person_carries_weapon(p_person, WEP_EXPLMINE))
        players[plyr].FourPacks[WFRPK_EXPLMINE][cc2] = net_agents__FourPacks[plyr][cc2][WFRPK_EXPLMINE];
    if (person_carries_weapon(p_person, WEP_KOGAS))
        players[plyr].FourPacks[WFRPK_KOGAS][cc2] = net_agents__FourPacks[plyr][cc2][WFRPK_KOGAS];
    if (person_carries_weapon(p_person, WEP_CRAZYGAS))
        players[plyr].FourPacks[WFRPK_CRAZYGAS][cc2] = net_agents__FourPacks[plyr][cc2][WFRPK_CRAZYGAS];
}

void do_weapon_quantities1(struct Thing *p_person)
{
#if 0
    asm volatile ("call ASM_do_weapon_quantities1\n"
        : : "a" (p_person));
#endif
    ushort plyr, cc2;

    plyr = (p_person->U.UObject.MatrixIndex & 0x1C) >> 2;
    cc2 = p_person->U.UObject.MatrixIndex & 3;
    if (in_network_game)
        return;

    if (person_carries_weapon(p_person, WEP_NUCLGREN))
        players[plyr].FourPacks[WFRPK_NUCLGREN][cc2] = 4;
    if (person_carries_weapon(p_person, WEP_ELEMINE))
        players[plyr].FourPacks[WFRPK_ELEMINE][cc2] = 4;
    if (person_carries_weapon(p_person, WEP_EXPLMINE))
        players[plyr].FourPacks[WFRPK_EXPLMINE][cc2] = 4;
    if (person_carries_weapon(p_person, WEP_KOGAS))
        players[plyr].FourPacks[WFRPK_KOGAS][cc2] = 4;
    if (person_carries_weapon(p_person, WEP_CRAZYGAS))
        players[plyr].FourPacks[WFRPK_CRAZYGAS][cc2] = 4;
}

void do_weapon_quantities_proper1(struct Thing *p_person)
{
    asm volatile ("call ASM_do_weapon_quantities_proper1\n"
        : : "a" (p_person));
}

/******************************************************************************/
