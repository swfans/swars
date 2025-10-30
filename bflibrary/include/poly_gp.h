/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file poly_gp.h
 *     Header file for poly_gp.c.
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
#ifndef POLY_GP_H_
#define POLY_GP_H_

#include "poly.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

struct gpoly_state {
    int ratioCA_X2Y;
    int ratioBA_X2Y;
    int ratioCB_X2Y;
    int incA_S;
    int incB_S;
    int ptA_Y;
    int ptA_X;
    int ptA_X_prc;
    int ptA_S;
    int ptA_U;
    int ptA_V;
    int ptB_Y;
    int ptB_X;
    int ptB_X_prc;
    int ptB_S;
    int ptB_U;
    int ptB_V;
    int ptC_Y;
    int ptC_X;
    int ptC_X_prc;
    int ptC_S;
    int ptC_U;
    int ptC_V;
    int var_134;
    int inc_S2;
    int inc_S4;
    int inc_S3;
    int var_0BC;
    int var_0B8;
    int var_0B4;
    int var_0A8;
    int var_0A4;
    int var_0A0;
    int var_09C;
    int var_098;
    int var_094;
    int var_090;
    int var_08C;
    int var_088;
    int var_084;
    int var_080;
    int var_07C;
    int var_078;
    int inc_S1;
};

#pragma pack()

ubyte gpoly_reorder_input_points(struct PolyPoint **opt_a,
  struct PolyPoint **opt_b, struct PolyPoint **opt_c);
void gpoly_init_state(struct gpoly_state *st, struct PolyPoint *point_a,
  struct PolyPoint *point_b, struct PolyPoint *point_c);

void gpoly_sta_md03(struct gpoly_state *st);
void gpoly_sta_md04(struct gpoly_state *st);
void gpoly_sta_md05(struct gpoly_state *st);
void gpoly_sta_md27(struct gpoly_state *st);
void gpoly_sta_md28(struct gpoly_state *st);

/**
 * Renders shaded gpoly triangle using given state, checking window bounds.
 */
void gpoly_rasterize_shaded_bound(struct gpoly_state *st);

/**
 * Renders shaded gpoly triangle using given state, without bounds check.
 */
void gpoly_rasterize_shaded_nobound(struct gpoly_state *st);

/**
 * Renders const brightness gpoly triangle using given state, checking window bounds.
 */
void gpoly_rasterize_noshade_bound(struct gpoly_state *st);

/**
 * Renders const brightness gpoly triangle using given state, without bounds check.
 */
void gpoly_rasterize_noshade_nobound(struct gpoly_state *st);


#ifdef __cplusplus
};
#endif

#endif // POLY_GP_H_
/******************************************************************************/
