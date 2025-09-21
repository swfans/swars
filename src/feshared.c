/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file feshared.c
 *     Front-end desktop and menu system, items shared between screens.
 * @par Purpose:
 *     Implement functions for parts shared between screens in front-end desktop.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2024 - 03 Sep 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "feshared.h"

#include "bfscreen.h"

#include "femain.h"
#include "guiboxes.h"
#include "guitext.h"
#include "purpldrw.h"
#include "swlog.h"
/******************************************************************************/

TbBool is_sys_scr_shared_header_flag01(void)
{
    return ((system_screen_shared_header_box.Flags & 0x0001) != 0);
}

void mark_sys_scr_shared_header_box_redraw(void)
{
    system_screen_shared_header_box.Flags &= ~(GBxFlg_BkgndDrawn | GBxFlg_TextRight | GBxFlg_BkCopied);
}

void update_sys_scr_shared_header(ubyte sysscrn_no)
{
    system_screen_shared_header_box.Text = gui_strings[377 + sysscrn_no];
    mark_sys_scr_shared_header_box_redraw();
}

ubyte show_sys_scr_shared_header(void)
{
    ubyte drawn;

    //drawn = system_screen_shared_header_box.DrawFn(&system_screen_shared_header_box); -- incompatible calling convention
    asm volatile ("call *%2\n"
        : "=r" (drawn) : "a" (&system_screen_shared_header_box), "g" (system_screen_shared_header_box.DrawFn));
    return drawn;
}

void init_sys_scr_shared_content_box(void)
{
    init_screen_box(&system_screen_shared_content_box, 213u, 72u, 420u, 354, 6);

    system_screen_shared_content_box.X = unkn13_SYSTEM_button.X + unkn13_SYSTEM_button.Width + 9;
}

void mark_sys_scr_shared_content_box_redraw(void)
{
    system_screen_shared_content_box.Flags &= ~(GBxFlg_BkgndDrawn|GBxFlg_TextRight|GBxFlg_BkCopied);
}

void reset_sys_scr_shared_boxes_flags(void)
{
    system_screen_shared_header_box.Flags = GBxFlg_Unkn0001;
    system_screen_shared_content_box.Flags = GBxFlg_Unkn0001;
}

void set_flag02_sys_scr_shared_boxes(void)
{
    system_screen_shared_header_box.Flags |= GBxFlg_Unkn0002;
    system_screen_shared_content_box.Flags |= GBxFlg_Unkn0002;
}

/******************************************************************************/
