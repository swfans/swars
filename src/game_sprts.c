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
#include "game.h"
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
    char locstr[DISKPATH_SIZE];
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

void setup_sprites_icons(void)
{
    LbSpriteSetup(sprites_Icons0_0, sprites_Icons0_0_end, sprites_Icons0_0_data);
}

void reset_sprites_icons(void)
{
    LbSpriteReset(sprites_Icons0_0, sprites_Icons0_0_end, sprites_Icons0_0_data);
}

TbResult load_sprites_wicons(ubyte **pp_buf, const char *dir)
{
    char locstr[DISKPATH_SIZE];
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

void setup_sprites_wicons(void)
{
    LbSpriteSetup(unk1_sprites, unk1_sprites_end, unk1_sprites_data);
}

void reset_sprites_wicons(void)
{
    LbSpriteReset(unk1_sprites, unk1_sprites_end, unk1_sprites_data);
}

TbResult load_sprites_panel(ubyte **pp_buf, const char *dir)
{
    char locstr[DISKPATH_SIZE];
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

void setup_sprites_panel(void)
{
    LbSpriteSetup(unk2_sprites, unk2_sprites_end, unk2_sprites_data);
}

void reset_sprites_panel(void)
{
    LbSpriteReset(unk2_sprites, unk2_sprites_end, unk2_sprites_data);
}

TbResult load_sprites_mouse(ubyte **pp_buf, const char *dir)
{
    char locstr[DISKPATH_SIZE];
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

void setup_sprites_mouse(void)
{
    LbSpriteSetup(unk3_sprites, unk3_sprites_end, unk3_sprites_data);
}

void reset_sprites_mouse(void)
{
    LbSpriteReset(unk3_sprites, unk3_sprites_end, unk3_sprites_data);
}

TbResult load_sprites_med_font(ubyte **pp_buf, const char *dir)
{
    char locstr[DISKPATH_SIZE];
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

void setup_sprites_med_font(void)
{
    LbSpriteSetup(med_font, med_font_end, med_font_data);
}

void reset_sprites_med_font(void)
{
    LbSpriteReset(med_font, med_font_end, med_font_data);
}

TbResult load_sprites_big_font(ubyte **pp_buf, const char *dir)
{
    char locstr[DISKPATH_SIZE];
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

void setup_sprites_big_font(void)
{
    LbSpriteSetup(big_font, big_font_end, big_font_data);
}

void reset_sprites_big_font(void)
{
    LbSpriteReset(big_font, big_font_end, big_font_data);
}

void setup_sprites_small_font(void)
{
    LbSpriteSetup(small_font, small_font_end, small_font_data);
}

void reset_sprites_small_font(void)
{
    LbSpriteReset(small_font, small_font_end, small_font_data);
}

TbResult load_sprites_small_med_font(ubyte **pp_buf, const char *dir)
{
    char locstr[DISKPATH_SIZE];
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

void setup_sprites_small_med_font(void)
{
    LbSpriteSetup(small_med_font, small_med_font_end, small_med_font_data);
}

void reset_sprites_small_med_font(void)
{
    LbSpriteReset(small_med_font, small_med_font_end, small_med_font_data);
}

TbResult load_sprites_med2_font(ubyte **pp_buf, const char *dir)
{
    char locstr[DISKPATH_SIZE];
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

void setup_sprites_med2_font(void)
{
    LbSpriteSetup(med2_font, med2_font_end, med2_font_data);
}

void reset_sprites_med2_font(void)
{
    LbSpriteReset(med2_font, med2_font_end, med2_font_data);
}

TbResult load_sprites_small2_font(ubyte **pp_buf, const char *dir)
{
    char locstr[DISKPATH_SIZE];
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

void setup_sprites_small2_font(void)
{
    LbSpriteSetup(small2_font, small2_font_end, small2_font_data);
}

void reset_sprites_small2_font(void)
{
    LbSpriteReset(small2_font, small2_font_end, small2_font_data);
}

TbResult load_multicolor_sprites(const char *dir)
{
    char locstr[DISKPATH_SIZE];
    long len;
    TbResult ret;

    ret = Lb_OK;

    sprintf(locstr, "%s/mspr-%d.dat", dir, ingame.TrenchcoatPreference);
    len = LbFileLoadAt(locstr, m_spr_data);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 0;
    }
    // additional 512 bytes are always reserved by LbDataLoad()
    assert(m_spr_data_end + 512 >= m_spr_data + len);
    sprintf(locstr, "%s/mspr-%d.tab", dir, ingame.TrenchcoatPreference);
    len = LbFileLoadAt(locstr, m_sprites);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 128 * sizeof(struct TbSprite);
        LbMemorySet(m_sprites, '\0', len);
    }
    m_sprites_end = (struct TbSprite *)((ubyte *)m_sprites + len);

    return ret;
}

void setup_multicolor_sprites(void)
{
    LbSpriteSetup(m_sprites, m_sprites_end, m_spr_data);
}

void reset_multicolor_sprites(void)
{
    LbSpriteReset(m_sprites, m_sprites_end, m_spr_data);
}

void debug_multicolor_sprite(int idx)
{
    int i;
    char strdata[100];
    char *str;
    struct TbSprite *spr;
    unsigned char *ptr;
    spr = &m_sprites[idx];
    str = strdata;
    sprintf(str, "spr %d width %d height %d ptr 0x%lx data",
      idx, (int)spr->SWidth, (int)spr->SHeight, (ulong)spr->Data);
    ptr = spr->Data;
    for (i = 0; i < 10; i++)
    {
        str = strdata + strlen(strdata);
        sprintf(str, " %02x", (int)*ptr);
        ptr++;
    }
    LOGDBG("m_sprites: %s", strdata);
}

TbResult load_pop_sprites(const char *dir)
{
    char locstr[DISKPATH_SIZE];
    long len;
    TbResult ret;

    ret = Lb_OK;

    sprintf(locstr, "%s/pop%d-0.dat", dir, -ingame.PanelPermutation - 1);
    len = LbFileLoadAt(locstr, pop1_data);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 0;
    }
    sprintf(locstr, "%s/pop%d-0.tab", dir, -ingame.PanelPermutation - 1);
    len = LbFileLoadAt(locstr, pop1_sprites);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 128 * sizeof(struct TbSprite);
        LbMemorySet(pop1_sprites, '\0', len);
    }
    pop1_sprites_end = &pop1_sprites[len/sizeof(struct TbSprite)];

    return ret;
}

void setup_pop_sprites(void)
{
    LbSpriteSetup(pop1_sprites, pop1_sprites_end, pop1_data);
}

void reset_pop_sprites(void)
{
    LbSpriteReset(pop1_sprites, pop1_sprites_end, pop1_data);
}

/******************************************************************************/
