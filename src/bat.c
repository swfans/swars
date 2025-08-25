/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file bat.c
 *     Routines implementing Breakout minigame.
 * @par Purpose:
 *     Implement functions for handling breakout-like minigame.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 22 Sep 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bat.h"

#include "swlog.h"
/******************************************************************************/

void BAT_play(void)
{
    asm volatile ("call ASM_BAT_play\n"
        :  :  : "eax" );
}

/******************************************************************************/
