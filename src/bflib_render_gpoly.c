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

/******************************************************************************/
