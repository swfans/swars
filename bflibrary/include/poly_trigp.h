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
struct TrigLocalPrep {
    long var_28;
    long var_2C;
    long var_30;
    long var_34;
    long var_38;
    long var_3C;
    long var_40;
    long var_4C;
    long var_50;
    long var_58;
    long var_5C;
    long var_64;
    long var_68;
    long var_6C;
    long var_78;
    ubyte var_8A;
    ubyte var_8B;
    ubyte var_8C;
};

struct TrigLocalRend {
    ubyte *var_24;
    long var_44;
    long var_48;
    long var_54;
    long var_60;
};

#pragma pack()

ubyte trig_reorder_input_points(struct PolyPoint **opt_a,
  struct PolyPoint **opt_b, struct PolyPoint **opt_c);
int trig_ll_start(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c);
int trig_rl_start(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c);
int trig_fb_start(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c);
int trig_ft_start(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c);


#ifdef __cplusplus
};
#endif

#endif // POLY_TRIGP_H_
/******************************************************************************/
