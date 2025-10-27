/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mock_windows.c
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
#include "mock_bfwindows.h"
#include "mock_bfpalette.h"
#include "bfwindows.h"
#include "bfutility.h"
#include "bfmouse.h"
#include "bfscreen.h"
#include "bftstlog.h"

void LbRegisterStandardVideoModes(void);

TbResult MockBaseInitialise(void)
{
    // Clear global variables
    lbScreenInitialised = false;
    lbAppActive = true;
    LbMouseChangeMoveRatio(NORMAL_MOUSE_MOVE_RATIO, NORMAL_MOUSE_MOVE_RATIO);
    // Register default video modes
    if (lbScreenModeInfoNum == 0) {
        LbRegisterStandardVideoModes();
    }
    lbLibInitialised = true;
    return Lb_SUCCESS;
}

/******************************************************************************/
