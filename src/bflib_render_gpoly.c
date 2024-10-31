/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_render_gpoly.c
 *     Rendering function draw_gpoly() for drawing 3D view elements.
 * @par Purpose:
 *     Function for rendering 3D elements.
 * @par Comment:
 *     Go away from here, you bad optimizer! Do not compile this with optimizations.
 * @author   Tomasz Lis
 * @date     20 Mar 2009 - 14 Feb 2010
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "poly.h"

#include "bfendian.h"
#include "bfgentab.h"
#include "poly_gp.h"
#include "poly_trigp.h"

#include "globals.h"
#include "swlog.h"

/******************************************************************************/

long gpoly_pro_enable_mode_ofs;

// Static variables used only inside draw_gpoly().
long gpoly_mode;

/******************************************************************************/
void gpoly_enable_pentium_pro(TbBool state)
{
  LOGSYNC("Pentium Pro polygon rendering %s",state?"on":"off");
  if (state)
    gpoly_pro_enable_mode_ofs = (1<<6);
  else
    gpoly_pro_enable_mode_ofs = 0;
}

void draw_trigpoly(struct PolyPoint *point_a, struct PolyPoint *point_b, struct PolyPoint *point_c)
{
    switch (vec_mode)
    {
    case 5:
    case 27:
      draw_gpoly(point_a, point_b, point_c);
      break;
    default:
      trig(point_a, point_b, point_c);
      break;
    }
}

void draw_gpoly(struct PolyPoint *point_a, struct PolyPoint *point_b, struct PolyPoint *point_c)
{
#if 0
    asm volatile (
      "call ASM_draw_gpoly_\n"
        : : "a" (point_a), "d" (point_b), "b" (point_c));
    return;
#endif
    struct gpoly_state st;
    ubyte start_type, cross_bound;

    gpoly_mode = vec_mode;

    cross_bound = (point_c->X | point_b->X | point_a->X) < 0
             || point_a->X > vec_window_width
             || point_b->X > vec_window_width
             || point_c->X > vec_window_width;


    start_type = gpoly_reorder_input_points(&point_a, &point_b, &point_c);
    if (start_type == RendStart_NO)
        return;

    gpoly_init_state(&st, point_a, point_b, point_c);

    switch (gpoly_mode)
    {
    case RendVec_mode02:
    case RendVec_mode03:
    case RendVec_mode12:
    case RendVec_mode13:
    case RendVec_mode18:
    case RendVec_mode19:
    case RendVec_mode22:
    case RendVec_mode23:
        gpoly_sta_md03(&st);
        break;
    case RendVec_mode04:
    case RendVec_mode16:
    case RendVec_mode17:
        gpoly_sta_md04(&st);
        break;
    case RendVec_mode05:
    case RendVec_mode06:
    case RendVec_mode20:
    case RendVec_mode21:
    case RendVec_mode24:
    case RendVec_mode25:
        gpoly_sta_md05(&st);
        break;
    case 0x1Bu:
    case 0x1Du:
    case 0x1Fu:
    case 0x20u:
    case 0x21u:
    case 0x22u:
    case 0x25u:
    case 0x26u:
        gpoly_sta_md27(&st);
        break;
    case 0x1Cu:
    case 0x1Eu:
    case 0x23u:
    case 0x24u:
    case 0x27u:
    case 0x28u:
        gpoly_sta_md28(&st);
        break;
    }

    switch (gpoly_mode)
    {
    case RendVec_mode05:
        if (cross_bound)
            gpoly_stb_md05uni_var040_nz(&st);
        else
            gpoly_stb_md05uni_var040_zr(&st);
        break;
    case 27:
        if (cross_bound)
            gpoly_stb_md05p64_var040_nz(&st);
        else
            gpoly_stb_md05p64_var040_zr(&st);
        break;
    }
}

/******************************************************************************/
