/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file lvwalk.c
 *     Routines for handling walk items on a map.
 * @par Purpose:
 *     Implement functions storing and handling walk items list.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 24 Sep 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "lvwalk.h"

#include <assert.h>

#include "swlog.h"
/******************************************************************************/

ushort next_walk_header = 1;
ushort next_walk_item = 1;

/******************************************************************************/
