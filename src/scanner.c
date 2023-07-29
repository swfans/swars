/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file scanner.c
 *     Ingame scanner (minimap/radar) support.
 * @par Purpose:
 *     Implement functions for handling the scanner map and its state.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "scanner.h"

#include "campaign.h"
/******************************************************************************/

void SCANNER_init(void)
{
    asm volatile ("call ASM_SCANNER_init\n"
        :  :  : "eax" );
}

void SCANNER_set_colour(ubyte col)
{
    asm volatile ("call ASM_SCANNER_set_colour\n"
        :  : "a" ((long)col));
}

void SCANNER_fill_in(void)
{
    asm volatile ("call ASM_SCANNER_fill_in\n"
        :  :  : "eax" );
}

ushort do_group_scanner(struct Objective *p_objectv, ushort signal_count)
{
    ushort ret;
    asm volatile ("call ASM_do_group_scanner\n"
        : "=r" (ret) : "a" (p_objectv), "d" (signal_count));
    return ret;
}

/******************************************************************************/
