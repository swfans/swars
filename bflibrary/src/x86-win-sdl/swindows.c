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
#include "bfpalette.h"
#include "privbflog.h"

#include <SDL/SDL.h>

#if defined(WIN32)
#  include <synchapi.h>
#endif

volatile TbBool lbLibInitialised = false;
volatile TbBool lbUserQuit = false;
volatile TbBool lbAppActive;

void LbRegisterStandardVideoModes(void);
TbResult MEvent(const SDL_Event *ev);
TbResult KEvent(const SDL_Event *ev);

extern SDL_Color lbPaletteColors[];

TbResult LbBaseInitialise(void)
{
    char buf[32];
    // Clear global variables
    lbScreenInitialised = false;
    lbScreenSurface = NULL;
    lbDrawSurface = NULL;
    lbAppActive = true;
    LbMouseChangeMoveRatio(256, 256);
    // Register default video modes
    if (lbScreenModeInfoNum == 0) {
        LbRegisterStandardVideoModes();
    }
    // SDL environment variables
/* TODO - enable or remove; wo we really want to control this instead of allowing users to use env vars?
    if (lbVideoDriver[0] != '\0') {
        sprintf(buf,"SDL_VIDEODRIVER=%s",lbVideoDriver);
        putenv(buf);
    }*/
    // Initialize SDL library
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE |
      SDL_INIT_JOYSTICK) < 0) {
        LOGERR("SDL init: %s",SDL_GetError());
        return Lb_FAIL;
    }
    SDL_ShowCursor(SDL_DISABLE);
    // Setup the atexit() call to un-initialize
    atexit(SDL_Quit);
    lbLibInitialised = true;
    return Lb_SUCCESS;
}

TbResult LbInputRestate(void)
{
    return Lb_SUCCESS;
}

/** @internal
 * Triggers control function for given SDL window event.
 * @return SUCCESS if the event was processed, FAIL if it isn't supported, OK if no window event.
 */
TbResult WEvent(const SDL_Event *ev)
{
    //TbWindowAction action;

    switch (ev->type)
    {
    case SDL_ACTIVEEVENT:
        if (ev->active.state & SDL_APPACTIVE) {
            lbAppActive = (ev->active.gain != 0);
            //SYNCDBG(10, "Active = %d",(int)lbAppActive);
            LbInputRestate();
        }
        if ((lbAppActive) && (lbDisplay.Palette != NULL)) {
            // Switching windows in 8-bit modes often leads to palette loss
            // Below is the faster version of LbPaletteSet(lbDisplay.Palette);
            SDL_SetColors(lbDrawSurface,lbPaletteColors, 0, PALETTE_8b_COLORS);
        }
        return Lb_SUCCESS;
    case SDL_SYSWMEVENT:
        break;
    }
    return Lb_OK;
}

void LbDoMultitasking(void)
{
#if defined(WIN32)
    Sleep(LB_LARGE_DELAY_TIME>>1); // This switches to other tasks
#else
    sleep(LB_LARGE_DELAY_TIME);
#endif
}

static void LbI_ProcessEvent(const SDL_Event *ev)
{
    LOGNO("Starting for ev type %d", (int)ev->type);

    switch (ev->type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        KEvent(ev);
        break;

    case SDL_MOUSEMOTION:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        MEvent(ev);
        break;

    case SDL_ACTIVEEVENT:
    case SDL_SYSWMEVENT:
        WEvent(ev);
        break;

    case SDL_JOYAXISMOTION:
    case SDL_JOYBALLMOTION:
    case SDL_JOYHATMOTION:
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
        //TODO INPUT make joypad support
        //JEvent(ev);
        break;

    case SDL_VIDEORESIZE:
        break;

    case SDL_VIDEOEXPOSE:
        break;

    case SDL_QUIT:
        lbUserQuit = true;
        break;
    }
}

TbBool LbWindowsControl(void)
{
    SDL_Event ev;
    //process events until event queue is empty
    while (SDL_PollEvent(&ev)) {
        LbI_ProcessEvent(&ev);
    }
    return !lbUserQuit;
}

TbResult LbBaseReset(void)
{
    lbLibInitialised = false;
    SDL_Quit();
    return Lb_SUCCESS;
}

/******************************************************************************/
