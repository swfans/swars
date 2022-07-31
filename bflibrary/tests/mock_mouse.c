/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mock_mouse.c
 *     Implementation which only pretends to do stuff, for test purposes.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     05 Nov 2021 - 29 Jul 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "mock_bfmouse.h"
#include "bfmouse.h"
#include "bfscreen.h"
#include "bfplanar.h"
#include "bftstlog.h"

/******************************************************************************/
struct TbPoint mock_hotspot;
/******************************************************************************/

TbBool MockMouseIsInstalled(void)
{
    if (!lbMouseInstalled)
        return false;
    return true;
}

TbResult MockMouseChangeSprite(const struct TbSprite *pointer_spr)
{
    if (!lbMouseInstalled)
        return Lb_FAIL;
    lbDisplay.MouseSprite = pointer_spr;
    return Lb_SUCCESS;
}

TbResult MockMouseChangeSpriteOffset(long hot_x, long hot_y)
{
    if (!lbMouseInstalled)
        return Lb_FAIL;
    mock_hotspot.x = -hot_x;
    mock_hotspot.y = -hot_y;
    return Lb_SUCCESS;
}

TbResult MockMouseGetSpriteOffset(long *hot_x, long *hot_y)
{
    *hot_x = -mock_hotspot.x;
    *hot_y = -mock_hotspot.y;
    return Lb_SUCCESS;
}

/******************************************************************************/
