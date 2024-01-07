/******************************************************************************/
// Bullfrog Ariadne Pathfinding Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file tringops.c
 *     Pathfinding related operations concerning multiple triangles.
 * @par Purpose:
 *     Implement functions for performing operations on triangles.
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
#include "tringops.h"

#include <string.h>
#include "triangls.h"
#include "trstate.h"
#include "trlog.h"
/******************************************************************************/
const short MOD3[] = {0, 1, 2, 0, 1, 2};

static TrTriangId tri_split2(TrTriangId tri, TrTipId cor,
  TrCoord pt_x, TrCoord pt_y, TrPointId pt)
{
    struct TrTriangle *p_tri1;
    struct TrTriangle *p_tri2;
    TrTriangId tri2, tri3;
    TrTipId cor2, cor3;

    tri2 = tri_new();
    if (tri2 < 0) {
        return -1;
    }
    p_tri1 = &triangulation[0].Triangles[tri];
    p_tri2 = &triangulation[0].Triangles[tri2];
    memcpy(p_tri2, p_tri1, sizeof(struct TrTriangle));

    p_tri2->point[cor] = pt;

    cor2 = MOD3[cor + 1];
    p_tri1->point[cor2] = pt;
    p_tri1->tri[cor2] = tri2;
    p_tri1->enter |= 1 << cor2;
    p_tri1->enter &= ~(1 << (cor2 + 3));

    cor3 = MOD3[cor + 2];
    p_tri2->tri[cor3] = tri;
    p_tri2->enter |= 1 << cor3;
    p_tri2->enter &= ~(1 << (cor3 + 3));

    tri3 = p_tri2->tri[cor2];
    if (tri3 != -1)
    {
        TrTipId cor4;
        cor4 = link_find(tri3, tri);
        if (cor4 >= 0) {
            struct TrTriangle *p_tri3;
            p_tri3 = &triangulation[0].Triangles[tri3];
            p_tri3->tri[cor4] = tri2;
        } else {
            LOGERR("no two-way link between triangles %d and %d", tri3, tri);
        }
    }
#if ARIADNE_REGIONS
    int regid;
    regid = get_triangle_region_id(tri);
    if (regid > 0) {
        region_unset(tri, regid);
    }
    p_tri2->field_E = 0;
    edgelen_set(tri);
    edgelen_set(tri2);
#endif
    return tri2;
}

TrPointId tri_split3(TrTriangId btri, TrCoord pt_x, TrCoord pt_y)
{
    TrTriangId tri1, tri2;
    struct TrTriangle *p_tri1;
    struct TrTriangle *p_tri2;
    struct TrTriangle *p_btri;

    tri1 = tri_new();
    if (tri1 < 0) {
        return -1;
    }
    tri2 = tri_new();
    if (tri2 < 0) {
        tri_dispose(tri1);
        return -1;
    }
    p_tri1 = &triangulation[0].Triangles[tri1];
    p_tri2 = &triangulation[0].Triangles[tri2];
    p_btri = &triangulation[0].Triangles[btri];
    memcpy(p_tri1, p_btri, sizeof(struct TrTriangle));
    memcpy(p_tri2, p_btri, sizeof(struct TrTriangle));

    TrPointId pt;
    pt = point_set_new_or_reuse(pt_x, pt_y);
    if (pt < 0) {
        tri_dispose(tri1);
        tri_dispose(tri2);
        return -1;
    }

    p_btri->point[2] = pt;
    p_tri1->point[0] = pt;
    p_tri2->point[1] = pt;

    ubyte prev_btri_enter;
    prev_btri_enter = p_btri->enter;

    p_btri->enter |= 6;
    p_btri->tri[1] = tri1;
    p_btri->tri[2] = tri2;
    // TODO In DK, condition is always met, `jump` is intact and `enter` mask is 0x0F - verify
    if ((prev_btri_enter & 0x18) != 0x18)
    {
        p_btri->jump = -1;
        p_btri->enter &= 0x07;
    }

    p_tri1->enter |= 5;
    p_tri1->tri[0] = btri;
    p_tri1->tri[2] = tri2;
    // TODO In DK, condition is always met, `jump` is intact and `enter` mask is 0x17 - verify
    if ((prev_btri_enter & 0x28) != 0x28)
    {
        p_tri1->jump = -1;
        p_tri1->enter &= 0x07;
    }

    p_tri2->enter |= 3;
    p_tri2->tri[0] = btri;
    p_tri2->tri[1] = tri1;
    // TODO In DK, condition is always met, `jump` is intact and `enter` mask is 0x27 - verify
    if ((prev_btri_enter & 0x48) != 0x48)
    {
        p_tri2->jump = -1;
        p_tri2->enter &= 0x07;
    }

    TrTriangId ttri;
    ttri = p_tri1->tri[1];
    if (ttri != -1)
    {
        TrTipId lcor;
        lcor = link_find(ttri, btri);
        if (lcor >= 0) {
            struct TrTriangle *p_ttri;
            p_ttri = &triangulation[0].Triangles[ttri];
            p_ttri->tri[lcor] = tri1;
        } else {
            LOGERR("no two-way link between triangles %d and %d", ttri, btri);
        }
    }
    ttri = p_tri2->tri[2];
    if (ttri != -1)
    {
        TrTipId lcor;
        lcor = link_find(ttri, btri);
        if (lcor >= 0) {
            struct TrTriangle *p_ttri;
            p_ttri = &triangulation[0].Triangles[ttri];
            p_ttri->tri[lcor] = tri2;
        } else {
            LOGERR("no two-way link between triangles %d and %d", ttri, btri);
        }
    }
#if ARIADNE_REGIONS
    int regid;
    regid = get_triangle_region_id(btri);
    if (regid > 0) {
        region_unset(btri, regid);
    }
    p_tri1->field_E = 0;
    p_tri2->field_E = 0;
    edgelen_set(btri);
    edgelen_set(tri1);
    edgelen_set(tri2);
#endif
    return pt;
}

TrPointId edge_split(TrTriangId tri, TrTipId cor, TrCoord pt_x, TrCoord pt_y)
{
    struct TrTriangle *p_tri;
    struct TrTriangle *p_tri_sec;
    TrPointId pt;
    TrTriangId tri_sec;
    TrTipId cor_sec;
    TrTriangId tri_sp1, tri_sp2;

    // Create and fill new point
    pt = point_set_new_or_reuse(pt_x, pt_y);
    if (pt < 0) {
        return -1;
    }
    p_tri = &triangulation[0].Triangles[tri];
    // Find second tri and cor which links it back to the first
    tri_sec = p_tri->tri[cor];
    cor_sec = link_find(tri_sec, tri);
    if (cor_sec < 0) {
        LOGERR("no two-way link between triangles %d and %d", tri, tri_sec);
        point_dispose(pt);
        return -1;
    }
    // Do the splitting
    tri_sp1 = tri_split2(tri, cor, pt_x, pt_y, pt);
    tri_sp2 = tri_split2(tri_sec, cor_sec, pt_x, pt_y, pt);
    p_tri_sec = &triangulation[0].Triangles[tri_sec];
    p_tri_sec->tri[cor_sec] = tri_sp1;
    p_tri->tri[cor] = tri_sp2;
    return pt;
}

int edge_rotateAC(TrTriangId tri1, int cor1)
{
    struct TrTriangle *p_tri1;
    p_tri1 = &triangulation[0].Triangles[tri1];

    TrTipId cor_t1t2 = cor1;

    TrTriangId tri2 = p_tri1->tri[cor_t1t2];
    if (tri2 == -1) {
        return false;
    }
    TrTipId cor_t2t1 = link_find(tri2, tri1);
    if (cor_t2t1 == -1) {
        LOGERR("no two-way link between triangles %d and %d", tri2, tri1);
    }

    TrTipId cor_t1t3 = MOD3[cor_t1t2 + 1];
    TrTipId cor_t2t4 = MOD3[cor_t2t1 + 1];

    struct TrTriangle *p_tri2;
    p_tri2 = &triangulation[0].Triangles[tri2];

    TrTriangId tri3 = p_tri1->tri[cor_t1t3];
    TrTriangId tri4 = p_tri2->tri[cor_t2t4];

#if 0
    { // Not in SW, but it is in DK; `enter` is `field_D` in DK
        ushort tri1_fld = p_tri1->enter;
        ushort tri2_fld = p_tri2->enter;
        if ( ((1 << (cor_t1t2 + 3)) & tri1_fld) || ((1 << (cor_t2t1 + 3)) & tri2_fld) ) {
            return false;
        }
        p_tri1->enter &= ~(1 << (cor_t1t3 + 3));
        p_tri1->enter &= ~(1 << (cor_t1t2 + 3));
        p_tri1->enter |= ((((1 << (cor_t2t4 + 3)) & tri2_fld) != 0) << (cor_t1t2 + 3));

        p_tri2->enter &= ~(1 << (cor_t2t4 + 3));
        p_tri2->enter &= ~(1 << (cor_t2t1 + 3));
        p_tri2->enter |= ((((1 << (cor_t1t3 + 3)) & tri1_fld) != 0) << (cor_t2t1 + 3));
    }
#endif

    TrTipId cor_t1t5 = MOD3[cor_t1t2 + 2];
    TrTipId cor_t2t6 = MOD3[cor_t2t1 + 2];

    TrPointId pt1 = p_tri1->point[cor_t1t2];
    TrPointId pt2 = p_tri1->point[cor_t1t5];
    TrPointId pt3 = p_tri2->point[cor_t2t6];
    TrPointId pt4 = p_tri2->point[cor_t2t1];

    if (compare_point_cross_distances(pt1, pt2, pt3) <= 0) {
        return false;
    }

    if (compare_point_cross_distances(pt4, pt2, pt3) >= 0) {
        return false;
    }

    p_tri1->point[cor_t1t3] = pt3;
    p_tri2->point[cor_t2t4] = pt2;

    p_tri1->tri[cor_t1t2] = tri4;
    p_tri1->tri[cor_t1t3] = tri2;

    p_tri2->tri[cor_t2t1] = tri3;
    p_tri2->tri[cor_t2t4] = tri1;
    if (tri3 != -1)
    {
        TrTipId tmcor = link_find(tri3, tri1);
        if (tmcor >= 0) {
            struct TrTriangle *p_tri3;
            p_tri3 = &triangulation[0].Triangles[tri3];
            p_tri3->tri[tmcor] = tri2;
        } else {
            LOGERR("no two-way link between triangles %d and %d", tri3, tri1);
        }
    }
    if (tri4 != -1)
    {
        TrTipId tmcor = link_find(tri4, tri2);
        if (tmcor >= 0) {
            struct TrTriangle *p_tri4;
            p_tri4 = &triangulation[0].Triangles[tri4];
            p_tri4->tri[tmcor] = tri1;
        } else {
            LOGERR("no two-way link between triangles %d and %d", tri4, tri2);
        }
    }

    ubyte enter_t1t2 = ((1 << cor_t1t2) & p_tri1->enter) != 0;
    ubyte enter_t1t3 = ((1 << cor_t1t3) & p_tri1->enter) != 0;
    ubyte enter_t2t1 = ((1 << cor_t2t1) & p_tri2->enter) != 0;
    ubyte enter_t2t4 = ((1 << cor_t2t4) & p_tri2->enter) != 0;

    p_tri1->enter &= ~(1 << cor_t1t2);
    p_tri1->enter |= (enter_t2t4 << cor_t1t2);
    p_tri1->enter &= ~(1 << cor_t1t3);
    p_tri1->enter |= (enter_t1t2 << cor_t1t3);

    p_tri2->enter &= ~(1 << cor_t2t1);
    p_tri2->enter |= (enter_t1t3 << cor_t2t1);
    p_tri2->enter &= ~(1 << cor_t2t4);
    p_tri2->enter |= (enter_t2t1 << cor_t2t4);

#if ARIADNE_REGIONS
    edgelen_set(tri1);
    edgelen_set(tri2);
#endif
    return true;
}

/******************************************************************************/
