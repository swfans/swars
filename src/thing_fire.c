/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file thing_fire.c
 *     Support for creating fires burning on the map.
 * @par Purpose:
 *     Implement creation and updating of the representation of fire.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Aug 2025 - 25 Aug 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "thing_fire.h"

#include "swlog.h"
#include "thing.h"

/******************************************************************************/

void set_static_on_fire(struct SimpleThing *p_static)
{
    asm volatile (
      "call ASM_set_static_on_fire\n"
        :  : "a" (p_static));
}

/******************************************************************************/
