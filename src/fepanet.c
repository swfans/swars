/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file fepanet.c
 *     Front-end desktop and menu system, P. A. Network screen.
 * @par Purpose:
 *     Implement functions for Public Access Network screen in front-end desktop.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     05 Apr 2024 - 22 May 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "fepanet.h"

#include "femain.h"
#include "guiboxes.h"
#include "guitext.h"
#include "display.h"
#include "swlog.h"
/******************************************************************************/

ubyte show_panet_screen(void)
{
    ubyte drawn = true;

    // Draw sequentially
    if (drawn)
        drawn = draw_heading_box();

    //TODO PANET screen not implemented

    return drawn;
}

/******************************************************************************/
