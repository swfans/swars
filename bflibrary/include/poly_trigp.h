/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file poly_trigp.h
 *     Header file for poly_trigp.c.
 * @par Purpose:
 *     Textured 2D triangle rendring, clipping and preparation.
 * @par Comment:
 *     To be used internally; to draw a triangle, use API from `poly.h`.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef POLY_TRIGP_H_
#define POLY_TRIGP_H_

#include "poly.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

enum RenderingStartType {
    RendStart_NO = 0,
    RendStart_LL,
    RendStart_RL,
    RendStart_FB,
    RendStart_FT,
};

/* State transferred from trig_??_start() functions to trig_render_md??().
 * Only a part of it is probably required - to be simplified or removed
 * when everything is remade in C.
 */
struct TrigLocals {
    ubyte *var_24; // 0x00 trig_??_start() + trig_render_md??()
    long var_28; // 0x04 trig_??_start()
    long var_2C; // 0x08 trig_??_start()
    long var_30; // 0x0C trig_??_start()
    long var_34; // 0x10 trig_??_start()
    long var_38; // 0x14 trig_??_start()
    long var_3C; // 0x18 trig_??_start()
    long var_40; // 0x1C trig_??_start()
    long var_44; // 0x20 trig_??_start() + trig_render_md??()
    long var_48; // 0x24 trig_??_start() + trig_render_md??()
    long var_4C; // 0x28 trig_??_start()
    long var_50; // 0x2C trig_??_start()
    long var_54; // 0x30 trig_??_start() + trig_render_md??()
    long var_58; // 0x34 trig_??_start()
    long var_5C; // 0x38 trig_??_start()
    long var_60; // 0x3C trig_??_start() + trig_render_md??()
    long var_64; // 0x40 trig_??_start()
    long var_68; // 0x44 trig_??_start()
    long var_6C; // 0x48 trig_??_start()
    long var_70; // 0x4C trig_render_md??()
    long var_74; // 0x50 trig_render_md??()
    long var_78; // 0x54 trig_??_start()
    long var_7C; // 0x58 trig_render_md??()
    long var_80; // 0x5C trig_??_start() + trig_render_md??()
    ubyte var_88; // 0x64 trig_render_md??()
    ubyte var_8A; // 0x66 trig_??_start()
    ubyte var_8B; // 0x67 trig_??_start()
    ubyte var_8C; // 0x68 trig_??_start()
};

#pragma pack()

ubyte trig_reorder_input_points(struct PolyPoint **opt_a,
  struct PolyPoint **opt_b, struct PolyPoint **opt_c);
int trig_ll_start(struct TrigLocals *lv, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c);
int trig_rl_start(struct TrigLocals *lv, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c);
int trig_fb_start(struct TrigLocals *lv, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c);
int trig_ft_start(struct TrigLocals *lv, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c);


#ifdef __cplusplus
};
#endif

#endif // POLY_TRIGP_H_
/******************************************************************************/
