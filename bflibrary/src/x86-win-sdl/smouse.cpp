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

TbBool volatile lbMouseInstalled = false;
TbBool volatile lbMouseOffline = false;

int LbMousePlace()
{
// code at 0001:000a6810
}

int LbMouseRemove()
{
// code at 0001:000a6968
}

int LbMouseChangeSpriteOffset(unsigned long hsX, unsigned long hsY)
{
// code at 0001:000a6a44
}

int LbMouseChangeSprite(struct TbSprite *spr)
{
// code at 0001:000a6a7c
}

int LbMouseChangeMoveRatio()
{
// code at 0001:000a6c04
}

int LbMouseSetup()
{
// code at 0001:000a6c7c
}

int LbMouseReset()
{
// code at 0001:000a6e24
}

int LbMouseSuspend()
{
// code at 0001:000a6ea8
}

int LbMouseSetWindow()
{
// code at 0001:000a6f0c
}

int LbMouseSetPosition()
{
// code at 0001:000a6f78
}

int LbMouseUpdatePosition()
{
// code at 0001:000a7004
}

/******************************************************************************/
