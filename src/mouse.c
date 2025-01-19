/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file mouse.c
 *     Handling mouse position and mouse pointer sprites.
 * @par Purpose:
 *     Functions which wrap bflibrary routines for higher level mouse functions.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     11 Oct 2024 - 21 Nov 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "mouse.h"

#include <stdbool.h>
#include <stdint.h>
#include "bfscreen.h"
#include "bfplanar.h"
#include "bfsprite.h"
#include "bfmouse.h"
#include "bfutility.h"

#include "display.h"
#include "game_data.h"
#include "game_sprts.h"
#include "swlog.h"
/******************************************************************************/

#pragma pack(1)

struct PointerHotspot {
    sbyte x;
    sbyte y;
    sbyte z;
};

struct PointerHotspot pointer_hotspot[] = {
  {  0,  0, 0,},
  {  0,  0, 0,},
  { -7, -7, 0,},
  { -7, -7, 0,},
  { -5, -6, 0,},
  { -2, -2, 0,},
  { -7,-13, 0,},
  { -9, -1, 0,}, // mission ptr over platform
  {  0,  0, 0,}, // mission ptr normal
  { -7, -7, 0,},
  { -7, -7, 0,},
};

#pragma pack()

/******************************************************************************/
extern struct TbSprite *pointer_sprites;

TbResult load_mouse_pointers_sprites_for_current_mode(void)
{
    PathInfo *pinfo;
    short max_detail;
    TbResult ret;

    pinfo = &game_dirs[DirPlace_Data];
    max_detail = min(lbDisplay.GraphicsScreenHeight / 192, lbDisplay.GraphicsScreenWidth / 320);
    if (max_detail > 0)
        max_detail--;

    ret = load_sprites_mouse_pointers_up_to(pinfo->directory, 0, max_detail);
    setup_mouse_pointers();
    return ret;
}

void do_change_mouse(ushort mouse)
{
  LbMouseChangeSprite(&pointer_sprites[mouse]);
  LbMouseChangeSpriteOffset(pointer_hotspot[mouse].x, pointer_hotspot[mouse].y);
}

ubyte process_mouse_imputs(void)
{
    int ret;
    asm volatile ("call ASM_process_mouse_imputs\n"
        : "=r" (ret) : );
    return ret;
}
