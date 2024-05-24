/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file building.c
 *     Buildings related functions.
 * @par Purpose:
 *     Implement functions for handling buildings.
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
#include "building.h"

#include "bfmemory.h"
#include "game.h"
#include "thing.h"
#include "swlog.h"
/******************************************************************************/

struct Thing *create_building_thing(int x, int y, int z, ushort obj, ushort nobj, ushort a6)
{
    struct Thing *ret;
    asm volatile (
      "push %6\n"
      "push %5\n"
      "call ASM_create_building_thing\n"
        : "=r" (ret) : "a" (x), "d" (y), "b" (z), "c" (obj), "g" (nobj), "g" (a6));
    return ret;
}

void set_dome_col(struct Thing *p_building, ubyte flag)
{
    struct ColVectList *p_cvlist;
    ushort cvl_beg, cvl_num;
    ushort  cvl;

    cvl_beg = p_building->U.UObject.BuildStartVect;
    cvl_num = p_building->U.UObject.BuildNumbVect;
    if (flag)
    {
        for (cvl = cvl_beg; cvl < cvl_beg + cvl_num; cvl++)
        {
            p_cvlist = &game_col_vects_list[cvl];
            p_cvlist->NextColList |= 0x8000;
        }
    }
    else
    {
        for (cvl = cvl_beg; cvl < cvl_beg + cvl_num; cvl++)
        {
            p_cvlist = &game_col_vects_list[cvl];
            p_cvlist->NextColList &= ~0x8000;
        }
    }
}

void do_dome_rotate1(struct Thing *p_building)
{
    asm volatile ("call ASM_do_dome_rotate1\n"
        : : "a" (p_building));
}

void process_dome1(struct Thing *p_building)
{
    asm volatile ("call ASM_process_dome1\n"
        : : "a" (p_building));
}

/******************************************************************************/
