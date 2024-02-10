/******************************************************************************/
// Bullfrog Ariadne Pathfinding Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file delaunay.c
 *     Triangulation optimization support using Delaunay algorithm.
 * @par Purpose:
 *     Implement the Delaunay mesh optimization for triangulation purposes.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     29 Sep 2023 - 02 Jan 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "delaunay.h"

#include <limits.h>
#include <stdlib.h>
#include "bfmemut.h"
#include "triangls.h"
#include "tringops.h"
#include "trstate.h"
#include "trlog.h"
/******************************************************************************/

extern ubyte tag_current; // = 255;
extern ubyte routeTags[8000];

int optimise_heuristic(int tri, int cor)
{
#if 0 // TODO not defined in ASM
    int ret;
    asm volatile ("call ASM_optimise_heuristic\n"
        : "=r" (ret) : "a" (tri), "d" (cor));
    return ret;
#endif
}

void delaunay_step(void)
{
#if 0
    asm volatile ("call ASM_delaunay_step\n"
        :  :  : "eax" );
#else
    uint n_optimised;
    int tri;
    int cor;

    tag_current++;
    if (tag_current == 0) {
        LbMemorySet(routeTags, 0, 8000);
        tag_current = 1;
    }

    do
    {
        n_optimised = 0;
        for (tri = 0; tri < triangulation[0].ix_Triangles; tri++)
        {
            struct TrTriangle *p_tri;

            p_tri = &triangulation[0].Triangles[tri];
            if (p_tri->solid == 255)
                continue;
            for (cor = 0; cor < 3; cor++)
            {
                if (optimise_heuristic(tri, cor)) {
                    break;
                }
            }
            if (cor >= 3)
                continue;
            edge_rotateAC(tri, cor);
            ++n_optimised;
            routeTags[tri] = tag_current;
        }
    }
    while (n_optimised > 0);
#endif
}

/******************************************************************************/
