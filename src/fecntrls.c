/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file fecntrls.c
 *     Front-end desktop and menu system, research screen.
 * @par Purpose:
 *     Implement functions for research screen in front-end desktop.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "fecntrls.h"

#include "guiboxes.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenBox controls_keylist_box;
extern struct ScreenBox controls_joystick_box;

ubyte ac_show_controls_joystick_box(struct ScreenBox *box);

ubyte show_controls_joystick_box(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_controls_joystick_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

void init_controls_screen_boxes(void)
{
    init_screen_box(&controls_keylist_box, 213u, 72u, 420u, 354, 6);

    init_screen_box(&controls_joystick_box, 7u, 252u, 197u, 174, 6);
    controls_joystick_box.SpecialDrawFn = ac_show_controls_joystick_box;
}

void reset_controls_screen_boxes_flags(void)
{
    controls_keylist_box.Flags = 0x0001;
    controls_joystick_box.Flags = 0x0001;
}

void clear_someflags_controls_screen_boxes(void)
{
    controls_keylist_box.Flags &= ~(0x8000|0x2000|0x0004);
}

/******************************************************************************/
