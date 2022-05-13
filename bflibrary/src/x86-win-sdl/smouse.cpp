/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file smouse.cpp
 *     Implementation of related functions.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfmouse.h"

#include <stdbool.h>
#include "bfscreen.h"

#if 0
volatile TbBool lbMouseInstalled = false;
#endif
volatile TbBool lbMouseOffline = false;
volatile TbBool lbInteruptMouse = false;

int LbMousePlace_UNUSED()
{
// code at 0001:000a6810
}

int LbMouseRemove_UNUSED()
{
// code at 0001:000a6968
}

int LbMouseChangeSpriteOffset_UNUSED(unsigned long hsX, unsigned long hsY)
{
// code at 0001:000a6a44
}

int LbMouseChangeSprite_UNUSED(struct TbSprite *spr)
{
// code at 0001:000a6a7c
}

int LbMouseChangeMoveRatio_UNUSED()
{
// code at 0001:000a6c04
}

TbResult LbMouseSetup_UNUSED(const struct TbSprite *pointer_spr, int ratio_x, int ratio_y)
{
// code at 0001:000a6c7c
}

int LbMouseReset_UNUSED()
{
// code at 0001:000a6e24
}

int LbMouseSuspend_UNUSED()
{
// code at 0001:000a6ea8
}

int LbMouseSetWindow_UNUSED()
{
// code at 0001:000a6f0c
}

int LbMouseSetPosition_UNUSED()
{
// code at 0001:000a6f78
}

int LbMouseUpdatePosition_UNUSED()
{
// code at 0001:000a7004
}

/******************************************************************************/
