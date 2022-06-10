/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file swindows.cpp
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
#include <unistd.h>
#include "bfwindows.h"
#include "bfscreen.h"
#include "bfscrsurf.h"
#include "bfmouse.h"

#if defined(WIN32)
#  include <synchapi.h>
#endif

void LbRegisterStandardVideoModes(void);

TbBool lbLibInitialised = false;

TbResult LbBaseInitialise(void)
{
    char buf[32];
    // Clear global variables
    lbScreenInitialised = false;
    lbScreenSurface = NULL;
    lbDrawSurface = NULL;
    //lbAppActive = true;
    LbMouseChangeMoveRatio(256, 256);
    // Register default video modes
    if (lbScreenModeInfoNum == 0) {
        LbRegisterStandardVideoModes();
    }
    // SDL environment variables
/*    if (lbVideoDriver[0] != '\0') {
        sprintf(buf,"SDL_VIDEODRIVER=%s",lbVideoDriver);
        putenv(buf);
    }
    // Initialize SDL library
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE|SDL_INIT_AUDIO) < 0) {
        ERRORLOG("SDL init: %s",SDL_GetError());
        return Lb_FAIL;
    }
    // Setup the atexit() call to un-initialize
    atexit(SDL_Quit);*/
    lbLibInitialised = true;
    return Lb_SUCCESS;
}

void LbDoMultitasking(void)
{
#if defined(WIN32)
    Sleep(LB_LARGE_DELAY_TIME>>1); // This switches to other tasks
#else
    sleep(LB_LARGE_DELAY_TIME);
#endif
}

int LbWindowsControl_UNUSED()
{
// code at 0001:000b1350
}

TbResult LbBaseReset(void)
{
    lbLibInitialised = false;
    return Lb_SUCCESS;
}

/******************************************************************************/
