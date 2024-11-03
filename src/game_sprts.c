/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file game_sprts.c
 *     Lists of sprites used by the game.
 * @par Purpose:
 *     Defines references to heads, tails and data buffers of 2D sprites.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Feb 2024 - 02 May 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "game_sprts.h"

#include <assert.h>
#include "bffile.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bfsprite.h"

#include "game_data.h"
#include "swlog.h"

/******************************************************************************/


/******************************************************************************/

void setup_mouse_pointers(void)
{
    struct TbSprite *spr;

    LbSpriteSetup(pointer_sprites, pointer_sprites_end, pointer_data);
    // Make mouse pointer sprite 1 an empty (zero size) sprite
    spr = &pointer_sprites[1];
    spr->SWidth = 0;
    spr->SHeight = 0;
}

void reset_mouse_pointers(void)
{
    LbSpriteReset(pointer_sprites, pointer_sprites_end, pointer_data);
}

TbResult load_sprites_icons(ubyte **pp_buf, const char *dir)
{
    char locstr[52];
    ubyte *p_buf;
    long len;
    TbResult ret;

    p_buf = *pp_buf;
    ret = Lb_OK;

    sprites_Icons0_0_data = p_buf;
    sprintf(locstr, "%s/icons0-0.dat", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 0;
    }
    p_buf += len;
    sprites_Icons0_0 = (struct TbSprite *)p_buf;
    sprintf(locstr, "%s/icons0-0.tab", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 32 * sizeof(struct TbSprite);
        LbMemorySet(p_buf, '\0', len);
    }
    p_buf += len;
    sprites_Icons0_0_end = (struct TbSprite *)p_buf;

    *pp_buf = p_buf;
    return ret;
}

TbResult load_sprites_wicons(ubyte **pp_buf, const char *dir)
{
    char locstr[52];
    ubyte *p_buf;
    long len;
    TbResult ret;

    p_buf = *pp_buf;
    ret = Lb_OK;

    unk1_sprites_data = p_buf;
    sprintf(locstr, "%s/w-icons.dat", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 0;
    }
    p_buf += len;
    unk1_sprites = (struct TbSprite *)p_buf;
    sprintf(locstr, "%s/w-icons.tab", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 32 * sizeof(struct TbSprite);
        LbMemorySet(p_buf, '\0', len);
    }
    p_buf += len;
    unk1_sprites_end = (struct TbSprite *)p_buf;

    *pp_buf = p_buf;
    return ret;
}

TbResult load_sprites_panel(ubyte **pp_buf, const char *dir)
{
    char locstr[52];
    ubyte *p_buf;
    long len;
    TbResult ret;

    p_buf = *pp_buf;
    ret = Lb_OK;

    unk2_sprites_data = p_buf;
    sprintf(locstr, "%s/panel0-0.dat", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 0;
    }
    p_buf += len;
    unk2_sprites = (struct TbSprite *)p_buf;
    sprintf(locstr, "%s/panel0-0.tab", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 32 * sizeof(struct TbSprite);
        LbMemorySet(p_buf, '\0', len);
    }
    p_buf += len;
    unk2_sprites_end = (struct TbSprite *)p_buf;

    *pp_buf = p_buf;
    return ret;
}

TbResult load_sprites_mouse(ubyte **pp_buf, const char *dir)
{
    char locstr[52];
    ubyte *p_buf;
    long len;
    TbResult ret;

    p_buf = *pp_buf;
    ret = Lb_OK;

    unk3_sprites_data = p_buf;
    sprintf(locstr, "%s/mouse-0.dat", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 0;
    }
    p_buf += len;
    unk3_sprites = (struct TbSprite *)p_buf;
    sprintf(locstr, "%s/mouse-0.tab", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 32 * sizeof(struct TbSprite);
        LbMemorySet(p_buf, '\0', len);
    }
    p_buf += len;
    unk3_sprites_end = (struct TbSprite *)p_buf;

    *pp_buf = p_buf;
    return ret;
}

TbResult load_sprites_med_font(ubyte **pp_buf, const char *dir)
{
    char locstr[52];
    ubyte *p_buf;
    long len;
    TbResult ret;

    p_buf = *pp_buf;
    ret = Lb_OK;

    med_font_data = p_buf;
    sprintf(locstr, "%s/font0-1.dat", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 0;
    }
    p_buf += len;
    med_font = (struct TbSprite *)p_buf;
    sprintf(locstr, "%s/font0-1.tab", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 128 * sizeof(struct TbSprite);
        LbMemorySet(p_buf, '\0', len);
    }
    p_buf += len;
    med_font_end = (struct TbSprite *)p_buf;

    *pp_buf = p_buf;
    return ret;
}

TbResult load_sprites_big_font(ubyte **pp_buf, const char *dir)
{
    char locstr[52];
    ubyte *p_buf;
    long len;
    TbResult ret;

    p_buf = *pp_buf;
    ret = Lb_OK;

    big_font_data = p_buf;
    sprintf(locstr, "%s/font0-2.dat", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 0;
    }
    p_buf += len;
    big_font = (struct TbSprite *)p_buf;
    sprintf(locstr, "%s/font0-2.tab", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 128 * sizeof(struct TbSprite);
        LbMemorySet(p_buf, '\0', len);
    }
    p_buf += len;
    big_font_end = (struct TbSprite *)p_buf;

    *pp_buf = p_buf;
    return ret;
}

TbResult load_sprites_small_med_font(ubyte **pp_buf, const char *dir)
{
    char locstr[52];
    ubyte *p_buf;
    long len;
    TbResult ret;

    p_buf = *pp_buf;
    ret = Lb_OK;

    small_med_font_data = p_buf;
    sprintf(locstr, "%s/font0-3.dat", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 0;
    }
    p_buf += len;
    small_med_font = (struct TbSprite *)p_buf;
    sprintf(locstr, "%s/font0-3.tab", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 128 * sizeof(struct TbSprite);
        LbMemorySet(p_buf, '\0', len);
    }
    p_buf += len;
    small_med_font_end = (struct TbSprite *)p_buf;

    *pp_buf = p_buf;
    return ret;
}

TbResult load_sprites_med2_font(ubyte **pp_buf, const char *dir)
{
    char locstr[52];
    ubyte *p_buf;
    long len;
    TbResult ret;

    p_buf = *pp_buf;
    ret = Lb_OK;

    med2_font_data = p_buf;
    sprintf(locstr, "%s/font0-4.dat", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 0;
    }
    p_buf += len;
    med2_font = (struct TbSprite *)p_buf;
    sprintf(locstr, "%s/font0-4.tab", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 128 * sizeof(struct TbSprite);
        LbMemorySet(p_buf, '\0', len);
    }
    p_buf += len;
    med2_font_end = (struct TbSprite *)p_buf;

    *pp_buf = p_buf;
    return ret;
}

TbResult load_sprites_small2_font(ubyte **pp_buf, const char *dir)
{
    char locstr[52];
    ubyte *p_buf;
    long len;
    TbResult ret;

    p_buf = *pp_buf;
    ret = Lb_OK;

    small2_font_data = p_buf;
    sprintf(locstr, "%s/font0-5.dat", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 0;
    }
    p_buf += len;
    small2_font = (struct TbSprite *)p_buf;
    sprintf(locstr, "%s/font0-5.tab", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 128 * sizeof(struct TbSprite);
        LbMemorySet(p_buf, '\0', len);
    }
    p_buf += len;
    small2_font_end = (struct TbSprite *)p_buf;

    *pp_buf = p_buf;
    return ret;
}

/******************************************************************************/
