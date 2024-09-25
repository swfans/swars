/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file poly_trigp.c
 *     Implementation of related functions.
 * @par Purpose:
 *     Textured 2D triangle rendring, clipping and preparation.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "poly_trigp.h"

#include <assert.h>
#include "poly.h"
#include "privbflog.h"


/**
 * whether the subtraction (x-y) of two long ints would overflow
 */
static inline ubyte __OFSUBL__(long x, long y)
{
    return ((x < 0) ^ (y < 0)) & ((x < 0) ^ (x-y < 0));
}

ubyte trig_reorder_input_points(struct PolyPoint **opt_a,
  struct PolyPoint **opt_b, struct PolyPoint **opt_c)
{
    struct PolyPoint *ordpt_a;
    struct PolyPoint *ordpt_b;
    struct PolyPoint *ordpt_c;
    ubyte start_type;

    ordpt_a = *opt_a;
    ordpt_b = *opt_b;
    ordpt_c = *opt_c;
    if (ordpt_a->Y == ordpt_b->Y)
    {
        if (ordpt_a->Y == ordpt_c->Y)
            return RendStart_NO;
        if (ordpt_a->Y >= ordpt_c->Y) {
            if (ordpt_a->X <= ordpt_b->X)
                return RendStart_NO;
            ordpt_a = *opt_c;
            ordpt_b = *opt_a;
            ordpt_c = *opt_b;
            start_type = RendStart_FB;
        } else {
            if (ordpt_b->X <= ordpt_a->X)
                return RendStart_NO;
            start_type = RendStart_FT;
        }
    }
    else if (ordpt_a->Y > ordpt_b->Y)
    {
        if (ordpt_a->Y == ordpt_c->Y)
        {
            if (ordpt_c->X <= ordpt_a->X)
                return RendStart_NO;
            ordpt_a = *opt_b;
            ordpt_b = *opt_c;
            ordpt_c = *opt_a;
            start_type = RendStart_FB;
        }
        else if (ordpt_a->Y < ordpt_c->Y)
        {
            ordpt_a = *opt_b;
            ordpt_b = *opt_c;
            ordpt_c = *opt_a;
            start_type = RendStart_RL;
        }
        else if (ordpt_b->Y == ordpt_c->Y)
        {
            if (ordpt_c->X <= ordpt_b->X)
                return RendStart_NO;
            ordpt_a = *opt_b;
            ordpt_b = *opt_c;
            ordpt_c = *opt_a;
            start_type = RendStart_FT;
        }
        else if (ordpt_b->Y < ordpt_c->Y)
        {
            ordpt_a = *opt_b;
            ordpt_b = *opt_c;
            ordpt_c = *opt_a;
            start_type = RendStart_LL;
        }
        else
        {
            ordpt_a = *opt_c;
            ordpt_b = *opt_a;
            ordpt_c = *opt_b;
            start_type = RendStart_RL;
        }
    }
    else // if (ordpt_a->Y < ordpt_b->Y)
    {
        if (ordpt_a->Y == ordpt_c->Y)
        {
            if (ordpt_a->X <= ordpt_c->X)
                return RendStart_NO;
            ordpt_a = *opt_c;
            ordpt_b = *opt_a;
            ordpt_c = *opt_b;
            start_type = RendStart_FT;
        }
        else if (ordpt_a->Y >= ordpt_c->Y)
        {
            ordpt_a = *opt_c;
            ordpt_b = *opt_a;
            ordpt_c = *opt_b;
            start_type = RendStart_LL;
        }
        else if (ordpt_b->Y == ordpt_c->Y)
        {
            if (ordpt_b->X <= ordpt_c->X)
                return RendStart_NO;
            start_type = RendStart_FB;
        }
        else if (ordpt_b->Y <= ordpt_c->Y)
        {
            start_type = RendStart_LL;
        }
        else
        {
            start_type = RendStart_RL;
        }
    }

    *opt_a = ordpt_a;
    *opt_b = ordpt_b;
    *opt_c = ordpt_c;
    return start_type;
}

static inline int trig_ll_md00(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pYa, pYb;
    struct PolyPoint *pp;

    pX = opt_a->X << 16;
    pYa = opt_a->X << 16;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        // whether the addition (tlr->var_44 + tlp->var_78) would overflow
        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        if (tlp->var_6C - tlp->var_38 >= 0)
        {
            tlp->var_3C -= tlp->var_6C - tlp->var_38;
            tlp->var_6C -= tlp->var_38;
            pX += tlp->var_28 * tlp->var_6C + tlp->var_38 * tlp->var_28;
            pYb = tlp->var_30 * tlp->var_6C + tlp->var_40;
            if (tlp->var_8C)
            {
                tlp->var_3C = vec_window_height;
                tlr->var_44 = vec_window_height;
            }
            tlp->var_38 = 0;
        }
        else
        {
            tlp->var_38 -= tlp->var_6C;
            pX += tlp->var_28 * tlp->var_6C;
            pYa += tlp->var_6C * tlp->var_2C;
            if (tlp->var_8C)
            {
                tlr->var_44 = vec_window_height;
                if (tlp->var_8B) {
                    tlp->var_38 = vec_window_height;
                } else {
                    tlp->var_8B = vec_window_height <= tlp->var_38;
                    tlp->var_3C = vec_window_height - tlp->var_38;
                }
            }
            pYb = tlp->var_40;
        }
    }
    else
    {
        if (tlp->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - tlp->var_78;
            tlr->var_44 = dH;
            if (tlp->var_8B) {
                tlp->var_38 = dH;
            } else {
                // whether the subtraction (dH - tlp->var_38) would overflow
                eH_overflow = __OFSUBL__(dH, tlp->var_38);
                eH = dH - tlp->var_38;
                tlp->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                tlp->var_3C = eH;
            }
        }
        pYb = tlp->var_40;
    }
    pp = polyscans;
    for (; tlp->var_38; tlp->var_38--)
    {
        pp->X = pX;
        pX += tlp->var_28;
        pp->Y = pYa;
        pYa += tlp->var_2C;
        ++pp;
    }
    if (!tlp->var_8B)
    {
        for (; tlp->var_3C; tlp->var_3C--)
        {
            pp->X = pX;
            pX += tlp->var_28;
            pp->Y = pYb;
            pYb += tlp->var_30;
            ++pp;
        }
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

static inline int trig_ll_md01(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    struct PolyPoint *pp;
    long pX, pYa, pYb;
    long pS;
    long ratio_var_34;

    ratio_var_34 = (tlp->var_38 << 16) / tlp->var_34;
    {
        long dX, wX;
        long eX;
        TbBool eX_overflow;

        dX = opt_a->X - opt_c->X;
        wX = (ratio_var_34 * dX) >> 16;
        dX = opt_b->X - opt_a->X;
        // whether the addition (wX + dX) would overflow
        eX_overflow = __OFSUBL__(wX, -dX);
        eX = wX + dX;
        if ((eX < 0) ^ eX_overflow) {
            LOGNO("skip due to sum %ld %ld", (long)wX, (long)dX);
            return 0;
        }
        if (eX != 0) {
            long long dS, wS;
            dS = opt_a->S - opt_c->S;
            wS = (ratio_var_34 * dS) >> 16;
            tlr->var_60 = (opt_b->S + wS - opt_a->S) / (eX + 1);
        }
    }
    tlp->var_64 = (opt_c->S - opt_a->S) / tlp->var_34;
    pX = opt_a->X << 16;
    pYa = opt_a->X << 16;
    pS = opt_a->S;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        if (tlp->var_6C - tlp->var_38 >= 0)
        {
            tlp->var_3C -= tlp->var_6C - tlp->var_38;
            tlp->var_6C -= tlp->var_38;
            pX += tlp->var_28 * tlp->var_6C + tlp->var_38 * tlp->var_28;
            pYb = tlp->var_30 * tlp->var_6C + tlp->var_40;
            pS += tlp->var_6C * tlp->var_64 + tlp->var_38 * tlp->var_64;
            if (tlp->var_8C)
            {
              tlp->var_3C = vec_window_height;
              tlr->var_44 = vec_window_height;
            }
            tlp->var_38 = 0;
        }
        else
        {
            tlp->var_38 -= tlp->var_6C;
            pX += tlp->var_28 * tlp->var_6C;
            pYa += tlp->var_6C * tlp->var_2C;
            pS += tlp->var_6C * tlp->var_64;
            if (tlp->var_8C)
            {
                tlr->var_44 = vec_window_height;
                if (tlp->var_8B) {
                    tlp->var_38 = vec_window_height;
                } else {
                    tlp->var_8B = vec_window_height <= tlp->var_38;
                    tlp->var_3C = vec_window_height - tlp->var_38;
                }
            }
            pYb = tlp->var_40;
        }
    }
    else
    {
        if (tlp->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - tlp->var_78;
            tlr->var_44 = dH;
            if (tlp->var_8B) {
                tlp->var_38 = dH;
            } else {
                eH_overflow = __OFSUBL__(dH, tlp->var_38);
                eH = dH - tlp->var_38;
                tlp->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                tlp->var_3C = eH;
            }
        }
        pYb = tlp->var_40;
    }
    pp = polyscans;
    for (; tlp->var_38; tlp->var_38--)
    {
        pp->X = pX;
        pX += tlp->var_28;
        pp->Y = pYa;
        pYa += tlp->var_2C;
        pp->S = pS;
        pS += tlp->var_64;
        ++pp;
    }
    if (!tlp->var_8B)
    {
      for (; tlp->var_3C; tlp->var_3C--)
      {
          pp->X = pX;
          pX += tlp->var_28;
          pp->Y = pYb;
          pYb += tlp->var_30;
          pp->S = pS;
          pS += tlp->var_64;
          ++pp;
      }
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

static inline int trig_ll_md02(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pYa, pYb;
    long pU, pV;
    struct PolyPoint *pp;
    long ratio_var_34;

    ratio_var_34 = (tlp->var_38 << 16) / tlp->var_34;
    {
        long dX, wX;
        long eX;
        TbBool eX_overflow;

        dX = opt_a->X - opt_c->X;
        wX = ratio_var_34 * dX >> 16;
        dX = opt_b->X - opt_a->X;
        eX_overflow = __OFSUBL__(wX, -dX);
        eX = wX + dX;
        if ((eX < 0) ^ eX_overflow) {
            LOGNO("skip due to sum %ld %ld", (long)wX, (long)dX);
            return 0;
        }
        if (eX != 0) {
            long long dS, wS;
            dS = opt_a->U - opt_c->U;
            wS = (ratio_var_34 * dS) >> 16;
            tlr->var_48 = (opt_b->U + wS - opt_a->U) / (eX + 1);
            dS = opt_a->V - opt_c->V;
            wS = (ratio_var_34 * dS) >> 16;
            tlr->var_54 = (opt_b->V + wS - opt_a->V) / (eX + 1);
        }
    }
    tlp->var_4C = (opt_c->U - opt_a->U) / tlp->var_34;
    tlp->var_58 = (opt_c->V - opt_a->V) / tlp->var_34;
    pX = opt_a->X << 16;
    pYa = opt_a->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        if (tlp->var_6C - tlp->var_38 >= 0 )
        {
            tlp->var_3C -= tlp->var_6C - tlp->var_38;
            tlp->var_6C -= tlp->var_38;
            pX += tlp->var_28 * tlp->var_6C + tlp->var_38 * tlp->var_28;
            pYb = tlp->var_30 * tlp->var_6C + tlp->var_40;
            pU += tlp->var_6C * tlp->var_4C + tlp->var_38 * tlp->var_4C;
            pV += tlp->var_6C * tlp->var_58 + tlp->var_38 * tlp->var_58;
            if ( tlp->var_8C )
            {
                tlp->var_3C = vec_window_height;
                tlr->var_44 = vec_window_height;
            }
            tlp->var_38 = 0;
        }
        else
        {
            tlp->var_38 -= tlp->var_6C;
            pX += tlp->var_28 * tlp->var_6C;
            pYa += tlp->var_6C * tlp->var_2C;
            pU += tlp->var_6C * tlp->var_4C;
            pV += tlp->var_6C * tlp->var_58;
            if ( tlp->var_8C )
            {
                tlr->var_44 = vec_window_height;
                if (tlp->var_8B) {
                  tlp->var_38 = vec_window_height;
                } else {
                  tlp->var_8B = vec_window_height <= tlp->var_38;
                  tlp->var_3C = vec_window_height - tlp->var_38;
                }
            }
            pYb = tlp->var_40;
        }
    }
    else
    {
        if (tlp->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - tlp->var_78;
            tlr->var_44 = dH;
            if (tlp->var_8B) {
                tlp->var_38 = dH;
            } else {
                eH_overflow = __OFSUBL__(dH, tlp->var_38);
                eH = dH - tlp->var_38;
                tlp->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                tlp->var_3C = eH;
            }
        }
        pYb = tlp->var_40;
    }
    pp = polyscans;
    for (; tlp->var_38; tlp->var_38--)
    {
        pp->X = pX;
        pX += tlp->var_28;
        pp->Y = pYa;
        pYa += tlp->var_2C;
        pp->U = pU;
        pU += tlp->var_4C;
        pp->V = pV;
        pV += tlp->var_58;
        ++pp;
    }
    if (!tlp->var_8B)
    {
        for (; tlp->var_3C; tlp->var_3C--)
        {
            pp->X = pX;
            pX += tlp->var_28;
            pp->Y = pYb;
            pYb += tlp->var_30;
            pp->U = pU;
            pU += tlp->var_4C;
            pp->V = pV;
            pV += tlp->var_58;
            ++pp;
        }
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

static inline int trig_ll_md05(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pYa, pYb;
    long pU, pV, pS;
    struct PolyPoint *pp;
    long ratio_var_34;

    ratio_var_34 = (tlp->var_38 << 16) / tlp->var_34;
    {
        long dX, wX;
        long eX;
        TbBool eX_overflow;

        dX = opt_a->X - opt_c->X;
        wX = ratio_var_34 * dX >> 16;
        dX = opt_b->X - opt_a->X;
        eX_overflow = __OFSUBL__(wX, -dX);
        eX = wX + dX;
        if ((eX < 0) ^ eX_overflow) {
            LOGNO("skip due to sum %ld %ld", (long)wX, (long)dX);
            return 0;
        }
        if (eX != 0)
        {
            long long dS, wS;
            dS = opt_a->U - opt_c->U;
            wS = (ratio_var_34 * dS) >> 16;
            tlr->var_48 = (opt_b->U + wS - opt_a->U) / (eX + 1);
            dS = opt_a->V - opt_c->V;
            wS = (ratio_var_34 * dS) >> 16;
            tlr->var_54 = (opt_b->V + wS - opt_a->V) / (eX + 1);
            dS = opt_a->S - opt_c->S;
            wS = (ratio_var_34 * dS) >> 16;
            tlr->var_60 = (opt_b->S + wS - opt_a->S) / (eX + 1);
        }
    }
    tlp->var_4C = (opt_c->U - opt_a->U) / tlp->var_34;
    tlp->var_58 = (opt_c->V - opt_a->V) / tlp->var_34;
    tlp->var_64 = (opt_c->S - opt_a->S) / tlp->var_34;

    pX = opt_a->X << 16;
    pYa = opt_a->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    pS = opt_a->S;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        if (tlp->var_6C - tlp->var_38 >= 0)
        {
            tlp->var_3C -= tlp->var_6C - tlp->var_38;
            tlp->var_6C -= tlp->var_38;
            pX += tlp->var_28 * tlp->var_6C + tlp->var_38 * tlp->var_28;
            pYb = tlp->var_30 * tlp->var_6C + tlp->var_40;
            pU += tlp->var_6C * tlp->var_4C + tlp->var_38 * tlp->var_4C;
            pV += tlp->var_6C * tlp->var_58 + tlp->var_38 * tlp->var_58;
            pS += tlp->var_6C * tlp->var_64 + tlp->var_38 * tlp->var_64;
            if (tlp->var_8C) {
              tlp->var_3C = vec_window_height;
              tlr->var_44 = vec_window_height;
            }
            tlp->var_38 = 0;
        }
        else
        {
            tlp->var_38 -= tlp->var_6C;
            pX += tlp->var_28 * tlp->var_6C;
            pYa += tlp->var_6C * tlp->var_2C;
            pU += tlp->var_6C * tlp->var_4C;
            pV += tlp->var_6C * tlp->var_58;
            pS += tlp->var_6C * tlp->var_64;
            if (tlp->var_8C)
            {
                tlr->var_44 = vec_window_height;
                if (tlp->var_8B) {
                    tlp->var_38 = vec_window_height;
                } else {
                    tlp->var_8B = vec_window_height <= tlp->var_38;
                    tlp->var_3C = vec_window_height - tlp->var_38;
                }
            }
            pYb = tlp->var_40;
        }
    }
    else
    {
        if (tlp->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - tlp->var_78;
            tlr->var_44 = vec_window_height - tlp->var_78;
            if (tlp->var_8B) {
                tlp->var_38 = vec_window_height - tlp->var_78;
            } else {
                eH_overflow = __OFSUBL__(dH, tlp->var_38);
                eH = dH - tlp->var_38;
                tlp->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                tlp->var_3C = eH;
            }
        }
        pYb = tlp->var_40;
    }
    pp = polyscans;
    for (; tlp->var_38; tlp->var_38--)
    {
        pp->X = pX;
        pX += tlp->var_28;
        pp->Y = pYa;
        pYa += tlp->var_2C;
        pp->U = pU;
        pU += tlp->var_4C;
        pp->V = pV;
        pV += tlp->var_58;
        pp->S = pS;
        pS += tlp->var_64;
        ++pp;
    }
    if ( !tlp->var_8B )
    {
        for (; tlp->var_3C; tlp->var_3C--)
        {
          pp->X = pX;
          pX += tlp->var_28;
          pp->Y = pYb;
          pYb += tlp->var_30;
          pp->U = pU;
          pU += tlp->var_4C;
          pp->V = pV;
          pV += tlp->var_58;
          pp->S = pS;
          pS += tlp->var_64;
          ++pp;
        }
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

int trig_ll_start(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    long dX, dY;

    tlp->var_78 = opt_a->Y;
    if (opt_a->Y < 0) {
        tlr->var_24 = poly_screen;
        tlp->var_8A = 1;
    } else if (opt_a->Y < vec_window_height) {
        tlr->var_24 = poly_screen + vec_screen_width * opt_a->Y;
        tlp->var_8A = 0;
    } else {
        LOGNO("height %ld exceeded by opt_a Y %ld", (long)vec_window_height, (long)opt_a->Y);
        return 0;
    }

    tlp->var_8C = opt_c->Y > vec_window_height;
    dY = opt_c->Y - opt_a->Y;
    tlp->var_34 = dY;
    tlr->var_44 = dY;

    tlp->var_8B = opt_b->Y > vec_window_height;
    dY = opt_b->Y - opt_a->Y;
    tlp->var_38 = dY;
    dX = opt_c->X - opt_a->X;
    tlp->var_28 = (dX << 16) / tlp->var_34;
    dX = opt_b->X - opt_a->X;
    if ((dX << 16) / dY <= tlp->var_28) {
        LOGNO("value (%ld << 16) / %ld below min %ld", (long)dX, (long)dY, (long)tlp->var_28);
        return 0;
    }
    tlp->var_2C = (dX << 16) / dY;

    dY = opt_c->Y - opt_b->Y;
    dX = opt_c->X - opt_b->X;
    tlp->var_30 = (dX << 16) / dY;
    tlp->var_3C = dY;
    tlp->var_40 = opt_b->X << 16;

    ret = 0;
    switch (vec_mode) /* swars-final @ 0x120F07 */
    {
    case RendVec_mode00:
    case RendVec_mode14:
    case RendVec_mode15:
        ret = trig_ll_md00(tlp, tlr, opt_a, opt_b, opt_c);
        break;

    case RendVec_mode01:
    case RendVec_mode04:
    case RendVec_mode16:
    case RendVec_mode17:
        ret = trig_ll_md01(tlp, tlr, opt_a, opt_b, opt_c);
        break;

    case RendVec_mode02:
    case RendVec_mode03:
    case RendVec_mode07:
    case RendVec_mode08:
    case RendVec_mode09:
    case RendVec_mode10:
    case RendVec_mode11:
    case RendVec_mode12:
    case RendVec_mode13:
    case RendVec_mode18:
    case RendVec_mode19:
    case RendVec_mode22:
    case RendVec_mode23:
        ret = trig_ll_md02(tlp, tlr, opt_a, opt_b, opt_c);
        break;

    case RendVec_mode05:
    case RendVec_mode06:
    case RendVec_mode20:
    case RendVec_mode21:
    case RendVec_mode24:
    case RendVec_mode25:
    case RendVec_mode26:
        ret = trig_ll_md05(tlp, tlr, opt_a, opt_b, opt_c);
        break;
    }

    return ret;
}

static inline int trig_rl_md00(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    ulong pXa, pXb, pY;
    struct PolyPoint *pp;

    pXa = opt_a->X << 16;
    pY = opt_a->X << 16;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        if (tlp->var_6C - tlp->var_34 >= 0)
        {
            tlp->var_6C -= tlp->var_34;
            tlp->var_3C -= tlp->var_6C;
            pXb = tlp->var_30 * tlp->var_6C + tlp->var_40;
            pY += tlp->var_6C * tlp->var_2C + tlp->var_34 * tlp->var_2C;
            if (tlp->var_8C) {
              tlp->var_3C = vec_window_height;
              tlr->var_44 = vec_window_height;
            }
            tlp->var_34 = 0;
        }
        else
        {
            tlp->var_34 -= tlp->var_6C;
            pXa += tlp->var_28 * tlp->var_6C;
            pY += tlp->var_6C * tlp->var_2C;
            if (tlp->var_8C)
            {
                tlr->var_44 = vec_window_height;
                if (tlp->var_8B) {
                    tlp->var_34 = vec_window_height;
                } else {
                    tlp->var_8B = vec_window_height <= tlp->var_34;
                    tlp->var_3C = vec_window_height - tlp->var_34;
                }
            }
            pXb = tlp->var_40;
        }
    }
    else
    {
        if (tlp->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - tlp->var_78;
            tlr->var_44 = dH;
            if (tlp->var_8B) {
                tlp->var_34 = dH;
            } else {
                eH_overflow = __OFSUBL__(dH, tlp->var_34);
                eH = dH - tlp->var_34;
                tlp->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                tlp->var_3C = eH;
            }
        }
        pXb = tlp->var_40;
    }
    pp = polyscans;
    for (; tlp->var_34; tlp->var_34--)
    {
        pp->X = pXa;
        pXa += tlp->var_28;
        pp->Y = pY;
        pY += tlp->var_2C;
        ++pp;
    }
    if (!tlp->var_8B)
    {
        for (; tlp->var_3C; tlp->var_3C--)
        {
            pp->X = pXb;
            pXb += tlp->var_30;
            pp->Y = pY;
            pY += tlp->var_2C;
            ++pp;
        }
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

static inline int trig_rl_md01(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pXa, pXb, pY;
    long pS;
    struct PolyPoint *pp;
    long ratio_var_34;

    ratio_var_34 = (tlp->var_34 << 16) / tlp->var_38;
    {
        long dXa, wXb;
        long eX;
        TbBool eX_overflow;

        wXb = ratio_var_34 * (opt_b->X - opt_a->X) >> 16;
        dXa = opt_a->X - opt_c->X;
        eX_overflow = __OFSUBL__(wXb, -dXa);
        eX = wXb + dXa;
        if ((eX < 0) ^ eX_overflow) {
            LOGNO("skip due to sum %ld %ld", (long)wXb, (long)dXa);
            return 0;
        }
        if (eX != 0) {
            long long dS, wS;
            dS = opt_b->S - opt_a->S;
            wS = (ratio_var_34 * dS) >> 16;
            tlr->var_60 = (opt_a->S + wS - opt_c->S) / (eX + 1);
        }
    }
    tlp->var_64 = (opt_c->S - opt_a->S) / tlp->var_34;
    tlp->var_68 = (opt_b->S - opt_c->S) / tlp->var_3C;
    pXa = opt_a->X << 16;
    pY = opt_a->X << 16;
    pS = opt_a->S;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        if (tlp->var_6C - tlp->var_34 >= 0)
        {
            tlp->var_6C -= tlp->var_34;
            tlp->var_3C -= tlp->var_6C;
            pXb = tlp->var_30 * tlp->var_6C + tlp->var_40;
            pY += tlp->var_6C * tlp->var_2C + tlp->var_34 * tlp->var_2C;
            pS += tlp->var_6C * tlp->var_68 + tlp->var_34 * tlp->var_64;
            if (tlp->var_8C) {
                tlp->var_3C = vec_window_height;
                tlr->var_44 = vec_window_height;
            }
            tlp->var_34 = 0;
        }
        else
        {
            tlp->var_34 -= tlp->var_6C;
            pXa += tlp->var_28 * tlp->var_6C;
            pY += tlp->var_6C * tlp->var_2C;
            pS += tlp->var_6C * tlp->var_64;
            if ( tlp->var_8C )
            {
                tlr->var_44 = vec_window_height;
                if ( tlp->var_8B )
                {
                  tlp->var_34 = vec_window_height;
                }
                else
                {
                  tlp->var_8B = vec_window_height <= tlp->var_34;
                  tlp->var_3C = vec_window_height - tlp->var_34;
                }
            }
            pXb = tlp->var_40;
        }
    }
    else
    {
        if (tlp->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - tlp->var_78;
            tlr->var_44 = vec_window_height - tlp->var_78;
            if (tlp->var_8B) {
                tlp->var_34 = vec_window_height - tlp->var_78;
            } else {
                eH_overflow = __OFSUBL__(dH, tlp->var_34);
                eH = dH - tlp->var_34;
                tlp->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                tlp->var_3C = eH;
            }
        }
        pXb = tlp->var_40;
    }
    pp = polyscans;
    for (; tlp->var_34; tlp->var_34--)
    {
        pp->X = pXa;
        pXa += tlp->var_28;
        pp->Y = pY;
        pY += tlp->var_2C;
        pp->S = pS;
        pS += tlp->var_64;
        ++pp;
    }
    if (!tlp->var_8B)
    {
        for (; tlp->var_3C; tlp->var_3C--)
        {
            pp->X = pXb;
            pXb += tlp->var_30;
            pp->Y = pY;
            pY += tlp->var_2C;
            pp->S = pS;
            pS += tlp->var_68;
            ++pp;
        }
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

static inline int trig_rl_md02(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pXa, pXb, pY;
    long pU, pV;
    struct PolyPoint *pp;
    long ratio_var_34;

    ratio_var_34 = (tlp->var_34 << 16) / tlp->var_38;
    {
        long dXa, wXb;
        long eX;
        TbBool eX_overflow;

        wXb = ratio_var_34 * (opt_b->X - opt_a->X) >> 16;
        dXa = opt_a->X - opt_c->X;
        eX_overflow = __OFSUBL__(wXb, -dXa);
        eX = wXb + dXa;
        if ((eX < 0) ^ eX_overflow) {
            LOGNO("skip due to sum %ld %ld", (long)wXb, (long)dXa);
            return 0;
        }
        if (eX != 0) {
            long long dS, wS;

            dS = opt_b->U - opt_a->U;
            wS = (ratio_var_34 * dS) >> 16;
            tlr->var_48 = (opt_a->U + wS - opt_c->U) / (eX + 1);
            dS = opt_b->V - opt_a->V;
            wS = (ratio_var_34 * dS) >> 16;
            tlr->var_54 = (opt_a->V + wS - opt_c->V) / (eX + 1);
        }
    }
    tlp->var_4C = (opt_c->U - opt_a->U) / tlp->var_34;
    tlp->var_58 = (opt_c->V - opt_a->V) / tlp->var_34;
    tlp->var_50 = (opt_b->U - opt_c->U) / tlp->var_3C;
    tlp->var_5C = (opt_b->V - opt_c->V) / tlp->var_3C;
    pXa = opt_a->X << 16;
    pY = opt_a->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        if (tlp->var_6C - tlp->var_34 >= 0)
        {
            tlp->var_6C -= tlp->var_34;
            tlp->var_3C -= tlp->var_6C;
            pXb = tlp->var_30 * tlp->var_6C + tlp->var_40;
            pY += tlp->var_6C * tlp->var_2C + tlp->var_34 * tlp->var_2C;
            pU += tlp->var_6C * tlp->var_50 + tlp->var_34 * tlp->var_4C;
            pV += tlp->var_6C * tlp->var_5C + tlp->var_34 * tlp->var_58;
            if (tlp->var_8C) {
                tlp->var_3C = vec_window_height;
                tlr->var_44 = vec_window_height;
            }
            tlp->var_34 = 0;
        }
        else
        {
            tlp->var_34 -= tlp->var_6C;
            pXa += tlp->var_28 * tlp->var_6C;
            pY += tlp->var_6C * tlp->var_2C;
            pU += tlp->var_6C * tlp->var_4C;
            pV += tlp->var_6C * tlp->var_58;
            if ( tlp->var_8C )
            {
                tlr->var_44 = vec_window_height;
                if (tlp->var_8B) {
                    tlp->var_34 = vec_window_height;
                } else {
                    tlp->var_8B = vec_window_height <= tlp->var_34;
                    tlp->var_3C = vec_window_height - tlp->var_34;
                }
            }
            pXb = tlp->var_40;
        }
    }
    else
    {
        if (tlp->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - tlp->var_78;
            tlr->var_44 = dH;
            if (tlp->var_8B) {
                tlp->var_34 = dH;
            } else {
                eH_overflow = __OFSUBL__(dH, tlp->var_34);
                eH = dH - tlp->var_34;
                tlp->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                tlp->var_3C = eH;
            }
        }
        pXb = tlp->var_40;
    }
    pp = polyscans;
    for (; tlp->var_34; tlp->var_34--)
    {
        pp->X = pXa;
        pXa += tlp->var_28;
        pp->Y = pY;
        pY += tlp->var_2C;
        pp->U = pU;
        pU += tlp->var_4C;
        pp->V = pV;
        pV += tlp->var_58;
        ++pp;
    }
    if (!tlp->var_8B)
    {
        for (; tlp->var_3C; tlp->var_3C--)
        {
            pp->X = pXb;
            pXb += tlp->var_30;
            pp->Y = pY;
            pY += tlp->var_2C;
            pp->U = pU;
            pU += tlp->var_50;
            pp->V = pV;
            pV += tlp->var_5C;
            ++pp;
        }
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

static inline int trig_rl_md05(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pXa, pXb, pY;
    long pU, pV, pS;
    struct PolyPoint *pp;
    long ratio_var_34;

    ratio_var_34 = (tlp->var_34 << 16) / tlp->var_38;
    {
        long dXa, wXb;
        long eX;
        TbBool eX_overflow;

        wXb = ratio_var_34 * (opt_b->X - opt_a->X) >> 16;
        dXa = opt_a->X - opt_c->X;
        eX_overflow = __OFSUBL__(wXb, -dXa);
        eX = wXb + dXa;
        if ((eX < 0) ^ eX_overflow) {
            LOGNO("skip due to sum %ld %ld", (long)wXb, (long)dXa);
            return 0;
        }
        tlr->var_60 = wXb;
        if (eX != 0) {
            long long dS, wS;

            dS = opt_b->U - opt_a->U;
            wS = (ratio_var_34 * dS) >> 16;
            tlr->var_48 = (opt_a->U + wS - opt_c->U) / (eX + 1);
            dS = opt_b->V - opt_a->V;
            wS = (ratio_var_34 * dS) >> 16;
            tlr->var_54 = (opt_a->V + wS - opt_c->V) / (eX + 1);
            dS = opt_b->S - opt_a->S;
            wS = (ratio_var_34 * dS) >> 16;
            tlr->var_60 = (opt_a->S + wS - opt_c->S) / (eX + 1);
        }
    }
    tlp->var_4C = (opt_c->U - opt_a->U) / tlp->var_34;
    tlp->var_58 = (opt_c->V - opt_a->V) / tlp->var_34;
    tlp->var_64 = (opt_c->S - opt_a->S) / tlp->var_34;
    tlp->var_50 = (opt_b->U - opt_c->U) / tlp->var_3C;
    tlp->var_5C = (opt_b->V - opt_c->V) / tlp->var_3C;
    tlp->var_68 = (opt_b->S - opt_c->S) / tlp->var_3C;
    pXa = opt_a->X << 16;
    pY = opt_a->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    pS = opt_a->S;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        if (tlp->var_6C - tlp->var_34 >= 0)
        {
            tlp->var_6C -= tlp->var_34;
            tlp->var_3C -= tlp->var_6C;
            pXb = tlp->var_30 * tlp->var_6C + tlp->var_40;
            pY += tlp->var_6C * tlp->var_2C + tlp->var_34 * tlp->var_2C;
            pU += tlp->var_6C * tlp->var_50 + tlp->var_34 * tlp->var_4C;
            pV += tlp->var_6C * tlp->var_5C + tlp->var_34 * tlp->var_58;
            pS += tlp->var_6C * tlp->var_68 + tlp->var_34 * tlp->var_64;
            if (tlp->var_8C) {
                tlp->var_3C = vec_window_height;
                tlr->var_44 = vec_window_height;
            }
            tlp->var_34 = 0;
        }
        else
        {
            tlp->var_34 -= tlp->var_6C;
            pXa += tlp->var_28 * tlp->var_6C;
            pY += tlp->var_6C * tlp->var_2C;
            pU += tlp->var_6C * tlp->var_4C;
            pV += tlp->var_6C * tlp->var_58;
            pS += tlp->var_6C * tlp->var_64;
            if (tlp->var_8C) {
                tlr->var_44 = vec_window_height;
                if (tlp->var_8B) {
                    tlp->var_34 = vec_window_height;
                } else {
                    tlp->var_8B = vec_window_height <= tlp->var_34;
                    tlp->var_3C = vec_window_height - tlp->var_34;
                }
            }
            pXb = tlp->var_40;
        }
    }
    else
    {
        if (tlp->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - tlp->var_78;
            tlr->var_44 = dH;
            if (tlp->var_8B) {
                tlp->var_34 = dH;
            } else {
                eH_overflow = __OFSUBL__(dH, tlp->var_34);
                eH = dH - tlp->var_34;
                tlp->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                tlp->var_3C = eH;
            }
        }
        pXb = tlp->var_40;
    }
    pp = polyscans;
    for (; tlp->var_34; tlp->var_34--)
    {
        pp->X = pXa;
        pXa += tlp->var_28;
        pp->Y = pY;
        pY += tlp->var_2C;
        pp->U = pU;
        pU += tlp->var_4C;
        pp->V = pV;
        pV += tlp->var_58;
        pp->S = pS;
        pS += tlp->var_64;
        ++pp;
    }
    if (!tlp->var_8B)
    {
        for (; tlp->var_3C; tlp->var_3C--)
        {
          pp->X = pXb;
          pXb += tlp->var_30;
          pp->Y = pY;
          pY += tlp->var_2C;
          pp->U = pU;
          pU += tlp->var_50;
          pp->V = pV;
          pV += tlp->var_5C;
          pp->S = pS;
          pS += tlp->var_68;
          ++pp;
        }
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

int trig_rl_start(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    long dX, dY;

    tlp->var_78 = opt_a->Y;
    if (opt_a->Y < 0) {
        tlr->var_24 = poly_screen;
        tlp->var_8A = 1;
    } else if (opt_a->Y < vec_window_height) {
        tlr->var_24 = poly_screen + vec_screen_width * opt_a->Y;
        tlp->var_8A = 0;
    } else {
        LOGNO("height %ld exceeded by opt_a Y %ld", (long)vec_window_height, (long)opt_a->Y);
        return 0;
    }

    tlp->var_8B = opt_c->Y > vec_window_height;
    dY = opt_c->Y - opt_a->Y;
    tlp->var_34 = dY;

    tlp->var_8C = opt_b->Y > vec_window_height;
    dY = opt_b->Y - opt_a->Y;
    tlp->var_38 = dY;
    tlr->var_44 = dY;
    dX = opt_c->X - opt_a->X;
    tlp->var_28 = (dX << 16) / tlp->var_34;
    dX = opt_b->X - opt_a->X;
    if ((dX << 16) / dY <= tlp->var_28) {
        LOGNO("value (%ld << 16) / %ld below min %ld", (long)dX, (long)dY, (long)tlp->var_28);
        return 0;
    }
    tlp->var_2C = (dX << 16) / dY;

    dY = opt_b->Y - opt_c->Y;
    dX = opt_b->X - opt_c->X;
    tlp->var_30 = (dX << 16) / dY;
    tlp->var_3C = dY;
    tlp->var_40 = opt_c->X << 16;

    ret = 0;
    switch (vec_mode) /* swars-final @ 0x121814 */
    {
    case RendVec_mode00:
    case RendVec_mode14:
    case RendVec_mode15:
        ret = trig_rl_md00(tlp, tlr, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode01:
    case RendVec_mode04:
    case RendVec_mode16:
    case RendVec_mode17:
        ret = trig_rl_md01(tlp, tlr, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode02:
    case RendVec_mode03:
    case RendVec_mode07:
    case RendVec_mode08:
    case RendVec_mode09:
    case RendVec_mode10:
    case RendVec_mode11:
    case RendVec_mode12:
    case RendVec_mode13:
    case RendVec_mode18:
    case RendVec_mode19:
    case RendVec_mode22:
    case RendVec_mode23:
        ret = trig_rl_md02(tlp, tlr, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode05:
    case RendVec_mode06:
    case RendVec_mode20:
    case RendVec_mode21:
    case RendVec_mode24:
    case RendVec_mode25:
    case RendVec_mode26:
        ret = trig_rl_md05(tlp, tlr, opt_a, opt_b, opt_c);
        break;
    }

    return ret;
}

static inline int trig_fb_md00(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pY;
    struct PolyPoint *pp;

    pX = opt_a->X << 16;
    pY = opt_a->X << 16;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        tlp->var_34 += tlp->var_78;
        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        pX += tlp->var_28 * (-tlp->var_78);
        pY += (-tlp->var_78) * tlp->var_2C;
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height;
            tlp->var_34 = vec_window_height;
        }
    }
    else
    {
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height - tlp->var_78;
            tlp->var_34 = vec_window_height - tlp->var_78;
        }
    }
    pp = polyscans;
    for (; tlp->var_34; tlp->var_34--)
    {
        pp->X = pX;
        pX += tlp->var_28;
        pp->Y = pY;
        pY += tlp->var_2C;
        ++pp;
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

static inline int trig_fb_md01(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int pX, pY;
    int pS;
    struct PolyPoint *pp;

    {
        long dX;
        dX = opt_b->X - opt_c->X;
        tlr->var_60 = (opt_b->S - opt_c->S) / dX;
        tlp->var_64 = (opt_c->S - opt_a->S) / tlr->var_44;
    }
    pX = opt_a->X << 16;
    pY = opt_a->X << 16;
    pS = opt_a->S;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        tlp->var_34 += tlp->var_78;
        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        pX += tlp->var_28 * (-tlp->var_78);
        pY += (-tlp->var_78) * tlp->var_2C;
        pS += (-tlp->var_78) * tlp->var_64;
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height;
            tlp->var_34 = vec_window_height;
        }
    }
    else
    {
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height - tlp->var_78;
            tlp->var_34 = vec_window_height - tlp->var_78;
        }
    }
    pp = polyscans;
    for (; tlp->var_34; tlp->var_34--)
    {
        pp->X = pX;
        pX += tlp->var_28;
        pp->Y = pY;
        pY += tlp->var_2C;
        pp->S = pS;
        pS += tlp->var_64;
        ++pp;
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

static inline int trig_fb_md02(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pY;
    long pU, pV;
    struct PolyPoint *pp;

    {
        long dX;
        dX = opt_b->X - opt_c->X;
        tlr->var_48 = (opt_b->U - opt_c->U) / dX;
        tlr->var_54 = (opt_b->V - opt_c->V) / dX;
        tlp->var_4C = (opt_c->U - opt_a->U) / tlr->var_44;
        tlp->var_58 = (opt_c->V - opt_a->V) / tlr->var_44;
    }
    pX = opt_a->X << 16;
    pY = opt_a->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        tlp->var_34 += tlp->var_78;
        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        pX += tlp->var_28 * (-tlp->var_78);
        pY += (-tlp->var_78) * tlp->var_2C;
        pU += (-tlp->var_78) * tlp->var_4C;
        pV += (-tlp->var_78) * tlp->var_58;
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height;
            tlp->var_34 = vec_window_height;
        }
    }
    else
    {
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height - tlp->var_78;
            tlp->var_34 = vec_window_height - tlp->var_78;
        }
    }
    pp = polyscans;
    for (; tlp->var_34; tlp->var_34--)
    {
        pp->X = pX;
        pX += tlp->var_28;
        pp->Y = pY;
        pY += tlp->var_2C;
        pp->U = pU;
        pU += tlp->var_4C;
        pp->V = pV;
        pV += tlp->var_58;
        ++pp;
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

static inline int trig_fb_md05(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pY;
    long pU, pV, pS;
    struct PolyPoint *pp;

    {
        long dX;
        dX = opt_b->X - opt_c->X;
        tlr->var_48 = (opt_b->U - opt_c->U) / dX;
        tlr->var_54 = (opt_b->V - opt_c->V) / dX;
        tlr->var_60 = (opt_b->S - opt_c->S) / dX;
        tlp->var_4C = (opt_c->U - opt_a->U) / tlr->var_44;
        tlp->var_58 = (opt_c->V - opt_a->V) / tlr->var_44;
        tlp->var_64 = (opt_c->S - opt_a->S) / tlr->var_44;
    }
    pX = opt_a->X << 16;
    pY = opt_a->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    pS = opt_a->S;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        tlp->var_34 += tlp->var_78;
        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        pX += tlp->var_28 * (-tlp->var_78);
        pY += (-tlp->var_78) * tlp->var_2C;
        pU += (-tlp->var_78) * tlp->var_4C;
        pV += (-tlp->var_78) * tlp->var_58;
        pS += (-tlp->var_78) * tlp->var_64;
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height;
            tlp->var_34 = vec_window_height;
        }
    }
    else
    {
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height - tlp->var_78;
            tlp->var_34 = vec_window_height - tlp->var_78;
        }
    }
    pp = polyscans;
    for (; tlp->var_34; tlp->var_34--)
    {
        pp->X = pX;
        pX += tlp->var_28;
        pp->Y = pY;
        pY += tlp->var_2C;
        pp->U = pU;
        pU += tlp->var_4C;
        pp->V = pV;
        pV += tlp->var_58;
        pp->S = pS;
        pS += tlp->var_64;
        ++pp;
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

int trig_fb_start(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    long dX, dY;

    tlp->var_78 = opt_a->Y;
    if (opt_a->Y < 0) {
        tlr->var_24 = poly_screen;
        tlp->var_8A = 1;
    } else if (opt_a->Y < vec_window_height) {
        tlr->var_24 = poly_screen + vec_screen_width * opt_a->Y;
        tlp->var_8A = 0;
    } else {
        LOGNO("height %ld exceeded by opt_a Y %ld", (long)vec_window_height, (long)opt_a->Y);
        return 0;
    }

    tlp->var_8B = opt_c->Y > vec_window_height;
    dY = opt_c->Y - opt_a->Y;
    tlp->var_34 = dY;
    tlr->var_44 = dY;
    dX = opt_c->X - opt_a->X;
    tlp->var_28 = (dX << 16) / dY;
    dX = opt_b->X - opt_a->X;
    tlp->var_2C = (dX << 16) / dY;

    ret = 0;
    switch (vec_mode) /* swars-final @ 0x122142, genewars-beta @ 0xEFE72 */
    {
    case RendVec_mode00:
    case RendVec_mode14:
    case RendVec_mode15:
        ret = trig_fb_md00(tlp, tlr, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode01:
    case RendVec_mode04:
    case RendVec_mode16:
    case RendVec_mode17:
        ret = trig_fb_md01(tlp, tlr, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode02:
    case RendVec_mode03:
    case RendVec_mode07:
    case RendVec_mode08:
    case RendVec_mode09:
    case RendVec_mode10:
    case RendVec_mode11:
    case RendVec_mode12:
    case RendVec_mode13:
    case RendVec_mode18:
    case RendVec_mode19:
    case RendVec_mode22:
    case RendVec_mode23:
        ret = trig_fb_md02(tlp, tlr, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode05:
    case RendVec_mode06:
    case RendVec_mode20:
    case RendVec_mode21:
    case RendVec_mode24:
    case RendVec_mode25:
    case RendVec_mode26:
        ret = trig_fb_md05(tlp, tlr, opt_a, opt_b, opt_c);
        break;
    }

    return ret;
}

static inline int trig_ft_md00(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pY;
    struct PolyPoint *pp;

    pX = opt_a->X << 16;
    pY = opt_b->X << 16;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        tlp->var_34 += tlp->var_78;
        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        pX += tlp->var_28 * (-tlp->var_78);
        pY += (-tlp->var_78) * tlp->var_2C;
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height;
            tlp->var_34 = vec_window_height;
        }
    }
    else
    {
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height - tlp->var_78;
            tlp->var_34 = vec_window_height - tlp->var_78;
        }
    }
    pp = polyscans;
    for (; tlp->var_34; tlp->var_34--)
    {
        pp->X = pX;
        pX += tlp->var_28;
        pp->Y = pY;
        pY += tlp->var_2C;
        ++pp;
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

static inline int trig_ft_md01(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pY;
    long pS;
    struct PolyPoint *pp;

    {
        long dX;
        dX = opt_b->X - opt_a->X;
        tlr->var_60 = (opt_b->S - opt_a->S) / dX;
        tlp->var_64 = (opt_c->S - opt_a->S) / tlr->var_44;
    }
    pX = opt_a->X << 16;
    pY = opt_b->X << 16;
    pS = opt_a->S;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        tlp->var_34 += tlp->var_78;
        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        pX += tlp->var_28 * (-tlp->var_78);
        pY += (-tlp->var_78) * tlp->var_2C;
        pS += (-tlp->var_78) * tlp->var_64;
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height;
            tlp->var_34 = vec_window_height;
        }
    }
    else
    {
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height - tlp->var_78;
            tlp->var_34 = vec_window_height - tlp->var_78;
        }
    }
    pp = polyscans;
    for (; tlp->var_34; tlp->var_34--)
    {
        pp->X = pX;
        pX += tlp->var_28;
        pp->Y = pY;
        pY += tlp->var_2C;
        pp->S = pS;
        pS += tlp->var_64;
        ++pp;
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

static inline int trig_ft_md02(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pY;
    long pU, pV;
    struct PolyPoint *pp;
    {
        long dX;
        dX = opt_b->X - opt_a->X;
        tlr->var_48 = (opt_b->U - opt_a->U) / dX;
        tlr->var_54 = (opt_b->V - opt_a->V) / dX;
        tlp->var_4C = (opt_c->U - opt_a->U) / tlr->var_44;
        tlp->var_58 = (opt_c->V - opt_a->V) / tlr->var_44;
    }
    pX = opt_a->X << 16;
    pY = opt_b->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        tlp->var_34 += tlp->var_78;
        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        pX += tlp->var_28 * (-tlp->var_78);
        pY += (-tlp->var_78) * tlp->var_2C;
        pU += (-tlp->var_78) * tlp->var_4C;
        pV += (-tlp->var_78) * tlp->var_58;
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height;
            tlp->var_34 = vec_window_height;
        }
    }
    else
    {
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height - tlp->var_78;
            tlp->var_34 = vec_window_height - tlp->var_78;
        }
    }
    pp = polyscans;
    for (; tlp->var_34; tlp->var_34--)
    {
        pp->X = pX;
        pX += tlp->var_28;
        pp->Y = pY;
        pY += tlp->var_2C;
        pp->U = pU;
        pU += tlp->var_4C;
        pp->V = pV;
        pV += tlp->var_58;
        ++pp;
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

static inline int trig_ft_md05(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pY;
    long pU, pV, pS;
    struct PolyPoint *pp;

    {
        long dX;
        dX = opt_b->X - opt_a->X;
        tlr->var_48 = (opt_b->U - opt_a->U) / dX;
        tlr->var_54 = (opt_b->V - opt_a->V) / dX;
        tlr->var_60 = (opt_b->S - opt_a->S) / dX;
        tlp->var_4C = (opt_c->U - opt_a->U) / tlr->var_44;
        tlp->var_58 = (opt_c->V - opt_a->V) / tlr->var_44;
        tlp->var_64 = (opt_c->S - opt_a->S) / tlr->var_44;
    }
    pX = opt_a->X << 16;
    pY = opt_b->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    pS = opt_a->S;
    if (tlp->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        tlp->var_34 += tlp->var_78;
        eH_overflow = __OFSUBL__(tlr->var_44, -tlp->var_78);
        eH = tlr->var_44 + tlp->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)tlr->var_44, (long)tlp->var_78);
            return 0;
        }
        tlr->var_44 = eH;
        tlp->var_6C = -tlp->var_78;
        pX += tlp->var_28 * (-tlp->var_78);
        pY += (-tlp->var_78) * tlp->var_2C;
        pU += (-tlp->var_78) * tlp->var_4C;
        pV += (-tlp->var_78) * tlp->var_58;
        pS += (-tlp->var_78) * tlp->var_64;
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height;
            tlp->var_34 = vec_window_height;
        }
    }
    else
    {
        if (tlp->var_8B) {
            tlr->var_44 = vec_window_height - tlp->var_78;
            tlp->var_34 = vec_window_height - tlp->var_78;
        }
    }
    pp = polyscans;
    for (; tlp->var_34; tlp->var_34--)
    {
        pp->X = pX;
        pX += tlp->var_28;
        pp->Y = pY;
        pY += tlp->var_2C;
        pp->U = pU;
        pU += tlp->var_4C;
        pp->V = pV;
        pV += tlp->var_58;
        pp->S = pS;
        pS += tlp->var_64;
        ++pp;
    }
    assert(pp < &polyscans[POLY_SCANS_COUNT]);
    return 1;
}

int trig_ft_start(struct TrigLocalPrep *tlp, struct TrigLocalRend *tlr, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    long dX, dY;

    tlp->var_78 = opt_a->Y;
    if (opt_a->Y < 0) {
        tlr->var_24 = poly_screen;
        tlp->var_8A = 1;
    } else if (opt_a->Y < vec_window_height) {
        tlr->var_24 = poly_screen + vec_screen_width * opt_a->Y;
        tlp->var_8A = 0;
    } else {
        LOGNO("height %ld exceeded by opt_a Y %ld", (long)vec_window_height, (long)opt_a->Y);
        return 0;
    }

    tlp->var_8B = opt_c->Y > vec_window_height;
    dY = opt_c->Y - opt_a->Y;
    tlp->var_34 = dY;
    tlr->var_44 = dY;
    dX = opt_c->X - opt_a->X;
    tlp->var_28 = (dX << 16) / dY;
    dX = opt_c->X - opt_b->X;
    tlp->var_2C = (dX << 16) / dY;

    ret = 0;
    switch (vec_mode) /* swars-final @ 0x1225c1, genewars-beta @ 0xF02F1 */
    {
    case RendVec_mode00:
    case RendVec_mode14:
    case RendVec_mode15:
        ret = trig_ft_md00(tlp, tlr, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode01:
    case RendVec_mode04:
    case RendVec_mode16:
    case RendVec_mode17:
        ret = trig_ft_md01(tlp, tlr, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode02:
    case RendVec_mode03:
    case RendVec_mode07:
    case RendVec_mode08:
    case RendVec_mode09:
    case RendVec_mode10:
    case RendVec_mode11:
    case RendVec_mode12:
    case RendVec_mode13:
    case RendVec_mode18:
    case RendVec_mode19:
    case RendVec_mode22:
    case RendVec_mode23:
        ret = trig_ft_md02(tlp, tlr, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode05:
    case RendVec_mode06:
    case RendVec_mode20:
    case RendVec_mode21:
    case RendVec_mode24:
    case RendVec_mode25:
    case RendVec_mode26:
        ret = trig_ft_md05(tlp, tlr, opt_a, opt_b, opt_c);
        break;
    }

    return ret;
}

/******************************************************************************/
