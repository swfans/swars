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

static inline int trig_ll_md00(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pYa, pYb;
    struct PolyPoint *pp;

    pX = opt_a->X << 16;
    pYa = opt_a->X << 16;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        // whether the addition (lvu->var_44 + lvu->var_78) would overflow
        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        if (lvu->var_6C - lvu->var_38 >= 0)
        {
            lvu->var_3C -= lvu->var_6C - lvu->var_38;
            lvu->var_6C -= lvu->var_38;
            pX += lvu->var_28 * lvu->var_6C + lvu->var_38 * lvu->var_28;
            pYb = lvu->var_30 * lvu->var_6C + lvu->var_40;
            if (lvu->var_8C)
            {
                lvu->var_3C = vec_window_height;
                lvu->var_44 = vec_window_height;
            }
            lvu->var_38 = 0;
        }
        else
        {
            lvu->var_38 -= lvu->var_6C;
            pX += lvu->var_28 * lvu->var_6C;
            pYa += lvu->var_6C * lvu->var_2C;
            if (lvu->var_8C)
            {
                lvu->var_44 = vec_window_height;
                if (lvu->var_8B) {
                    lvu->var_38 = vec_window_height;
                } else {
                    lvu->var_8B = vec_window_height <= lvu->var_38;
                    lvu->var_3C = vec_window_height - lvu->var_38;
                }
            }
            pYb = lvu->var_40;
        }
    }
    else
    {
        if (lvu->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - lvu->var_78;
            lvu->var_44 = dH;
            if (lvu->var_8B) {
                lvu->var_38 = dH;
            } else {
                // whether the subtraction (dH - lvu->var_38) would overflow
                eH_overflow = __OFSUBL__(dH, lvu->var_38);
                eH = dH - lvu->var_38;
                lvu->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                lvu->var_3C = eH;
            }
        }
        pYb = lvu->var_40;
    }
    pp = polyscans;
    for (; lvu->var_38; lvu->var_38--)
    {
        pp->X = pX;
        pX += lvu->var_28;
        pp->Y = pYa;
        pYa += lvu->var_2C;
        ++pp;
    }
    if (!lvu->var_8B)
    {
        for (; lvu->var_3C; lvu->var_3C--)
        {
            pp->X = pX;
            pX += lvu->var_28;
            pp->Y = pYb;
            pYb += lvu->var_30;
            ++pp;
        }
    }
    return 1;
}

static inline int trig_ll_md01(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    struct PolyPoint *pp;
    long pX, pYa, pYb;
    long pS;

    lvu->var_80 = (lvu->var_38 << 16) / lvu->var_34;
    {
        long dX, wX;
        long eX;
        TbBool eX_overflow;

        dX = opt_a->X - opt_c->X;
        wX = (lvu->var_80 * dX) >> 16;
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
            wS = (lvu->var_80 * dS) >> 16;
            lvu->var_60 = (opt_b->S + wS - opt_a->S) / (eX + 1);
        }
    }
    lvu->var_64 = (opt_c->S - opt_a->S) / lvu->var_34;
    pX = opt_a->X << 16;
    pYa = opt_a->X << 16;
    pS = opt_a->S;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        if (lvu->var_6C - lvu->var_38 >= 0)
        {
            lvu->var_3C -= lvu->var_6C - lvu->var_38;
            lvu->var_6C -= lvu->var_38;
            pX += lvu->var_28 * lvu->var_6C + lvu->var_38 * lvu->var_28;
            pYb = lvu->var_30 * lvu->var_6C + lvu->var_40;
            pS += lvu->var_6C * lvu->var_64 + lvu->var_38 * lvu->var_64;
            if (lvu->var_8C)
            {
              lvu->var_3C = vec_window_height;
              lvu->var_44 = vec_window_height;
            }
            lvu->var_38 = 0;
        }
        else
        {
            lvu->var_38 -= lvu->var_6C;
            pX += lvu->var_28 * lvu->var_6C;
            pYa += lvu->var_6C * lvu->var_2C;
            pS += lvu->var_6C * lvu->var_64;
            if (lvu->var_8C)
            {
                lvu->var_44 = vec_window_height;
                if (lvu->var_8B) {
                    lvu->var_38 = vec_window_height;
                } else {
                    lvu->var_8B = vec_window_height <= lvu->var_38;
                    lvu->var_3C = vec_window_height - lvu->var_38;
                }
            }
            pYb = lvu->var_40;
        }
    }
    else
    {
        if (lvu->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - lvu->var_78;
            lvu->var_44 = dH;
            if (lvu->var_8B) {
                lvu->var_38 = dH;
            } else {
                eH_overflow = __OFSUBL__(dH, lvu->var_38);
                eH = dH - lvu->var_38;
                lvu->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                lvu->var_3C = eH;
            }
        }
        pYb = lvu->var_40;
    }
    pp = polyscans;
    for (; lvu->var_38; lvu->var_38--)
    {
        pp->X = pX;
        pX += lvu->var_28;
        pp->Y = pYa;
        pYa += lvu->var_2C;
        pp->S = pS;
        pS += lvu->var_64;
        ++pp;
    }
    if (!lvu->var_8B)
    {
      for (; lvu->var_3C; lvu->var_3C--)
      {
          pp->X = pX;
          pX += lvu->var_28;
          pp->Y = pYb;
          pYb += lvu->var_30;
          pp->S = pS;
          pS += lvu->var_64;
          ++pp;
      }
    }
    return 1;
}

static inline int trig_ll_md02(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pYa, pYb;
    long pU, pV;
    struct PolyPoint *pp;

    lvu->var_80 = (lvu->var_38 << 16) / lvu->var_34;
    {
        long dX, wX;
        long eX;
        TbBool eX_overflow;

        dX = opt_a->X - opt_c->X;
        wX = lvu->var_80 * dX >> 16;
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
            wS = (lvu->var_80 * dS) >> 16;
            lvu->var_48 = (opt_b->U + wS - opt_a->U) / (eX + 1);
            dS = opt_a->V - opt_c->V;
            wS = (lvu->var_80 * dS) >> 16;
            lvu->var_54 = (opt_b->V + wS - opt_a->V) / (eX + 1);
        }
    }
    lvu->var_4C = (opt_c->U - opt_a->U) / lvu->var_34;
    lvu->var_58 = (opt_c->V - opt_a->V) / lvu->var_34;
    pX = opt_a->X << 16;
    pYa = opt_a->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        if (lvu->var_6C - lvu->var_38 >= 0 )
        {
            lvu->var_3C -= lvu->var_6C - lvu->var_38;
            lvu->var_6C -= lvu->var_38;
            pX += lvu->var_28 * lvu->var_6C + lvu->var_38 * lvu->var_28;
            pYb = lvu->var_30 * lvu->var_6C + lvu->var_40;
            pU += lvu->var_6C * lvu->var_4C + lvu->var_38 * lvu->var_4C;
            pV += lvu->var_6C * lvu->var_58 + lvu->var_38 * lvu->var_58;
            if ( lvu->var_8C )
            {
                lvu->var_3C = vec_window_height;
                lvu->var_44 = vec_window_height;
            }
            lvu->var_38 = 0;
        }
        else
        {
            lvu->var_38 -= lvu->var_6C;
            pX += lvu->var_28 * lvu->var_6C;
            pYa += lvu->var_6C * lvu->var_2C;
            pU += lvu->var_6C * lvu->var_4C;
            pV += lvu->var_6C * lvu->var_58;
            if ( lvu->var_8C )
            {
                lvu->var_44 = vec_window_height;
                if (lvu->var_8B) {
                  lvu->var_38 = vec_window_height;
                } else {
                  lvu->var_8B = vec_window_height <= lvu->var_38;
                  lvu->var_3C = vec_window_height - lvu->var_38;
                }
            }
            pYb = lvu->var_40;
        }
    }
    else
    {
        if (lvu->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - lvu->var_78;
            lvu->var_44 = dH;
            if (lvu->var_8B) {
                lvu->var_38 = dH;
            } else {
                eH_overflow = __OFSUBL__(dH, lvu->var_38);
                eH = dH - lvu->var_38;
                lvu->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                lvu->var_3C = eH;
            }
        }
        pYb = lvu->var_40;
    }
    pp = polyscans;
    for (; lvu->var_38; lvu->var_38--)
    {
        pp->X = pX;
        pX += lvu->var_28;
        pp->Y = pYa;
        pYa += lvu->var_2C;
        pp->U = pU;
        pU += lvu->var_4C;
        pp->V = pV;
        pV += lvu->var_58;
        ++pp;
    }
    if (!lvu->var_8B)
    {
        for (; lvu->var_3C; lvu->var_3C--)
        {
            pp->X = pX;
            pX += lvu->var_28;
            pp->Y = pYb;
            pYb += lvu->var_30;
            pp->U = pU;
            pU += lvu->var_4C;
            pp->V = pV;
            pV += lvu->var_58;
            ++pp;
        }
    }
    return 1;
}

static inline int trig_ll_md05(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pYa, pYb;
    long pU, pV, pS;
    struct PolyPoint *pp;

    lvu->var_80 = (lvu->var_38 << 16) / lvu->var_34;
    {
        long dX, wX;
        long eX;
        TbBool eX_overflow;

        dX = opt_a->X - opt_c->X;
        wX = lvu->var_80 * dX >> 16;
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
            wS = (lvu->var_80 * dS) >> 16;
            lvu->var_48 = (opt_b->U + wS - opt_a->U) / (eX + 1);
            dS = opt_a->V - opt_c->V;
            wS = (lvu->var_80 * dS) >> 16;
            lvu->var_54 = (opt_b->V + wS - opt_a->V) / (eX + 1);
            dS = opt_a->S - opt_c->S;
            wS = (lvu->var_80 * dS) >> 16;
            lvu->var_60 = (opt_b->S + wS - opt_a->S) / (eX + 1);
        }
    }
    lvu->var_4C = (opt_c->U - opt_a->U) / lvu->var_34;
    lvu->var_58 = (opt_c->V - opt_a->V) / lvu->var_34;
    lvu->var_64 = (opt_c->S - opt_a->S) / lvu->var_34;

    pX = opt_a->X << 16;
    pYa = opt_a->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    pS = opt_a->S;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        if (lvu->var_6C - lvu->var_38 >= 0)
        {
            lvu->var_3C -= lvu->var_6C - lvu->var_38;
            lvu->var_6C -= lvu->var_38;
            pX += lvu->var_28 * lvu->var_6C + lvu->var_38 * lvu->var_28;
            pYb = lvu->var_30 * lvu->var_6C + lvu->var_40;
            pU += lvu->var_6C * lvu->var_4C + lvu->var_38 * lvu->var_4C;
            pV += lvu->var_6C * lvu->var_58 + lvu->var_38 * lvu->var_58;
            pS += lvu->var_6C * lvu->var_64 + lvu->var_38 * lvu->var_64;
            if (lvu->var_8C) {
              lvu->var_3C = vec_window_height;
              lvu->var_44 = vec_window_height;
            }
            lvu->var_38 = 0;
        }
        else
        {
            lvu->var_38 -= lvu->var_6C;
            pX += lvu->var_28 * lvu->var_6C;
            pYa += lvu->var_6C * lvu->var_2C;
            pU += lvu->var_6C * lvu->var_4C;
            pV += lvu->var_6C * lvu->var_58;
            pS += lvu->var_6C * lvu->var_64;
            if (lvu->var_8C)
            {
                lvu->var_44 = vec_window_height;
                if (lvu->var_8B) {
                    lvu->var_38 = vec_window_height;
                } else {
                    lvu->var_8B = vec_window_height <= lvu->var_38;
                    lvu->var_3C = vec_window_height - lvu->var_38;
                }
            }
            pYb = lvu->var_40;
        }
    }
    else
    {
        if (lvu->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - lvu->var_78;
            lvu->var_44 = vec_window_height - lvu->var_78;
            if (lvu->var_8B) {
                lvu->var_38 = vec_window_height - lvu->var_78;
            } else {
                eH_overflow = __OFSUBL__(dH, lvu->var_38);
                eH = dH - lvu->var_38;
                lvu->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                lvu->var_3C = eH;
            }
        }
        pYb = lvu->var_40;
    }
    pp = polyscans;
    for (; lvu->var_38; lvu->var_38--)
    {
        pp->X = pX;
        pX += lvu->var_28;
        pp->Y = pYa;
        pYa += lvu->var_2C;
        pp->U = pU;
        pU += lvu->var_4C;
        pp->V = pV;
        pV += lvu->var_58;
        pp->S = pS;
        pS += lvu->var_64;
        ++pp;
    }
    if ( !lvu->var_8B )
    {
        for (; lvu->var_3C; lvu->var_3C--)
        {
          pp->X = pX;
          pX += lvu->var_28;
          pp->Y = pYb;
          pYb += lvu->var_30;
          pp->U = pU;
          pU += lvu->var_4C;
          pp->V = pV;
          pV += lvu->var_58;
          pp->S = pS;
          pS += lvu->var_64;
          ++pp;
        }
    }
    return 1;
}

int trig_ll_start(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    long dX, dY;

    lvu->var_78 = opt_a->Y;
    if (opt_a->Y < 0) {
      lvu->var_24 = poly_screen;
      lvu->var_8A = 1;
    } else if (opt_a->Y < vec_window_height) {
      lvu->var_24 = poly_screen + vec_screen_width * opt_a->Y;
      lvu->var_8A = 0;
    } else {
        LOGNO("height %ld exceeded by opt_a Y %ld", (long)vec_window_height, (long)opt_a->Y);
        return 0;
    }

    lvu->var_8C = opt_c->Y > vec_window_height;
    dY = opt_c->Y - opt_a->Y;
    lvu->var_34 = dY;
    lvu->var_44 = dY;

    lvu->var_8B = opt_b->Y > vec_window_height;
    dY = opt_b->Y - opt_a->Y;
    lvu->var_38 = dY;
    dX = opt_c->X - opt_a->X;
    lvu->var_28 = (dX << 16) / lvu->var_34;
    dX = opt_b->X - opt_a->X;
    if ((dX << 16) / dY <= lvu->var_28) {
        LOGNO("value (%ld << 16) / %ld below min %ld", (long)dX, (long)dY, (long)lvu->var_28);
        return 0;
    }
    lvu->var_2C = (dX << 16) / dY;

    dY = opt_c->Y - opt_b->Y;
    dX = opt_c->X - opt_b->X;
    lvu->var_30 = (dX << 16) / dY;
    lvu->var_3C = dY;
    lvu->var_40 = opt_b->X << 16;

    ret = 0;
    switch (vec_mode) /* swars-final @ 0x120F07 */
    {
    case RendVec_mode00:
    case RendVec_mode14:
    case RendVec_mode15:
        ret = trig_ll_md00(lvu, opt_a, opt_b, opt_c);
        break;

    case RendVec_mode01:
    case RendVec_mode04:
    case RendVec_mode16:
    case RendVec_mode17:
        ret = trig_ll_md01(lvu, opt_a, opt_b, opt_c);
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
        ret = trig_ll_md02(lvu, opt_a, opt_b, opt_c);
        break;

    case RendVec_mode05:
    case RendVec_mode06:
    case RendVec_mode20:
    case RendVec_mode21:
    case RendVec_mode24:
    case RendVec_mode25:
    case RendVec_mode26:
        ret = trig_ll_md05(lvu, opt_a, opt_b, opt_c);
        break;
    }

    return ret;
}

static inline int trig_rl_md00(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    ulong pXa, pXb, pY;
    struct PolyPoint *pp;

    pXa = opt_a->X << 16;
    pY = opt_a->X << 16;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        if (lvu->var_6C - lvu->var_34 >= 0)
        {
            lvu->var_6C -= lvu->var_34;
            lvu->var_3C -= lvu->var_6C;
            pXb = lvu->var_30 * lvu->var_6C + lvu->var_40;
            pY += lvu->var_6C * lvu->var_2C + lvu->var_34 * lvu->var_2C;
            if (lvu->var_8C) {
              lvu->var_3C = vec_window_height;
              lvu->var_44 = vec_window_height;
            }
            lvu->var_34 = 0;
        }
        else
        {
            lvu->var_34 -= lvu->var_6C;
            pXa += lvu->var_28 * lvu->var_6C;
            pY += lvu->var_6C * lvu->var_2C;
            if (lvu->var_8C)
            {
                lvu->var_44 = vec_window_height;
                if (lvu->var_8B) {
                    lvu->var_34 = vec_window_height;
                } else {
                    lvu->var_8B = vec_window_height <= lvu->var_34;
                    lvu->var_3C = vec_window_height - lvu->var_34;
                }
            }
            pXb = lvu->var_40;
        }
    }
    else
    {
        if (lvu->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - lvu->var_78;
            lvu->var_44 = dH;
            if (lvu->var_8B) {
                lvu->var_34 = dH;
            } else {
                eH_overflow = __OFSUBL__(dH, lvu->var_34);
                eH = dH - lvu->var_34;
                lvu->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                lvu->var_3C = eH;
            }
        }
        pXb = lvu->var_40;
    }
    pp = polyscans;
    for (; lvu->var_34; lvu->var_34--)
    {
        pp->X = pXa;
        pXa += lvu->var_28;
        pp->Y = pY;
        pY += lvu->var_2C;
        ++pp;
    }
    if (!lvu->var_8B)
    {
        for (; lvu->var_3C; lvu->var_3C--)
        {
            pp->X = pXb;
            pXb += lvu->var_30;
            pp->Y = pY;
            pY += lvu->var_2C;
            ++pp;
        }
    }
    return 1;
}

static inline int trig_rl_md01(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pXa, pXb, pY;
    long pS;
    struct PolyPoint *pp;

    lvu->var_80 = (lvu->var_34 << 16) / lvu->var_38;
    {
        long dXa, wXb;
        long eX;
        TbBool eX_overflow;

        wXb = lvu->var_80 * (opt_b->X - opt_a->X) >> 16;
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
            wS = (lvu->var_80 * dS) >> 16;
            lvu->var_60 = (opt_a->S + wS - opt_c->S) / (eX + 1);
        }
    }
    lvu->var_64 = (opt_c->S - opt_a->S) / lvu->var_34;
    lvu->var_68 = (opt_b->S - opt_c->S) / lvu->var_3C;
    pXa = opt_a->X << 16;
    pY = opt_a->X << 16;
    pS = opt_a->S;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        if (lvu->var_6C - lvu->var_34 >= 0)
        {
            lvu->var_6C -= lvu->var_34;
            lvu->var_3C -= lvu->var_6C;
            pXb = lvu->var_30 * lvu->var_6C + lvu->var_40;
            pY += lvu->var_6C * lvu->var_2C + lvu->var_34 * lvu->var_2C;
            pS += lvu->var_6C * lvu->var_68 + lvu->var_34 * lvu->var_64;
            if (lvu->var_8C) {
                lvu->var_3C = vec_window_height;
                lvu->var_44 = vec_window_height;
            }
            lvu->var_34 = 0;
        }
        else
        {
            lvu->var_34 -= lvu->var_6C;
            pXa += lvu->var_28 * lvu->var_6C;
            pY += lvu->var_6C * lvu->var_2C;
            pS += lvu->var_6C * lvu->var_64;
            if ( lvu->var_8C )
            {
                lvu->var_44 = vec_window_height;
                if ( lvu->var_8B )
                {
                  lvu->var_34 = vec_window_height;
                }
                else
                {
                  lvu->var_8B = vec_window_height <= lvu->var_34;
                  lvu->var_3C = vec_window_height - lvu->var_34;
                }
            }
            pXb = lvu->var_40;
        }
    }
    else
    {
        if (lvu->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - lvu->var_78;
            lvu->var_44 = vec_window_height - lvu->var_78;
            if (lvu->var_8B) {
                lvu->var_34 = vec_window_height - lvu->var_78;
            } else {
                eH_overflow = __OFSUBL__(dH, lvu->var_34);
                eH = dH - lvu->var_34;
                lvu->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                lvu->var_3C = eH;
            }
        }
        pXb = lvu->var_40;
    }
    pp = polyscans;
    for (; lvu->var_34; lvu->var_34--)
    {
        pp->X = pXa;
        pXa += lvu->var_28;
        pp->Y = pY;
        pY += lvu->var_2C;
        pp->S = pS;
        pS += lvu->var_64;
        ++pp;
    }
    if (!lvu->var_8B)
    {
        for (; lvu->var_3C; lvu->var_3C--)
        {
            pp->X = pXb;
            pXb += lvu->var_30;
            pp->Y = pY;
            pY += lvu->var_2C;
            pp->S = pS;
            pS += lvu->var_68;
            ++pp;
        }
    }
    return 1;
}

static inline int trig_rl_md02(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pXa, pXb, pY;
    long pU, pV;
    struct PolyPoint *pp;

    lvu->var_80 = (lvu->var_34 << 16) / lvu->var_38;
    {
        long dXa, wXb;
        long eX;
        TbBool eX_overflow;

        wXb = lvu->var_80 * (opt_b->X - opt_a->X) >> 16;
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
            wS = (lvu->var_80 * dS) >> 16;
            lvu->var_48 = (opt_a->U + wS - opt_c->U) / (eX + 1);
            dS = opt_b->V - opt_a->V;
            wS = (lvu->var_80 * dS) >> 16;
            lvu->var_54 = (opt_a->V + wS - opt_c->V) / (eX + 1);
        }
    }
    lvu->var_4C = (opt_c->U - opt_a->U) / lvu->var_34;
    lvu->var_58 = (opt_c->V - opt_a->V) / lvu->var_34;
    lvu->var_50 = (opt_b->U - opt_c->U) / lvu->var_3C;
    lvu->var_5C = (opt_b->V - opt_c->V) / lvu->var_3C;
    pXa = opt_a->X << 16;
    pY = opt_a->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        if (lvu->var_6C - lvu->var_34 >= 0)
        {
            lvu->var_6C -= lvu->var_34;
            lvu->var_3C -= lvu->var_6C;
            pXb = lvu->var_30 * lvu->var_6C + lvu->var_40;
            pY += lvu->var_6C * lvu->var_2C + lvu->var_34 * lvu->var_2C;
            pU += lvu->var_6C * lvu->var_50 + lvu->var_34 * lvu->var_4C;
            pV += lvu->var_6C * lvu->var_5C + lvu->var_34 * lvu->var_58;
            if (lvu->var_8C) {
                lvu->var_3C = vec_window_height;
                lvu->var_44 = vec_window_height;
            }
            lvu->var_34 = 0;
        }
        else
        {
            lvu->var_34 -= lvu->var_6C;
            pXa += lvu->var_28 * lvu->var_6C;
            pY += lvu->var_6C * lvu->var_2C;
            pU += lvu->var_6C * lvu->var_4C;
            pV += lvu->var_6C * lvu->var_58;
            if ( lvu->var_8C )
            {
                lvu->var_44 = vec_window_height;
                if (lvu->var_8B) {
                    lvu->var_34 = vec_window_height;
                } else {
                    lvu->var_8B = vec_window_height <= lvu->var_34;
                    lvu->var_3C = vec_window_height - lvu->var_34;
                }
            }
            pXb = lvu->var_40;
        }
    }
    else
    {
        if (lvu->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - lvu->var_78;
            lvu->var_44 = dH;
            if (lvu->var_8B) {
                lvu->var_34 = dH;
            } else {
                eH_overflow = __OFSUBL__(dH, lvu->var_34);
                eH = dH - lvu->var_34;
                lvu->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                lvu->var_3C = eH;
            }
        }
        pXb = lvu->var_40;
    }
    pp = polyscans;
    for (; lvu->var_34; lvu->var_34--)
    {
        pp->X = pXa;
        pXa += lvu->var_28;
        pp->Y = pY;
        pY += lvu->var_2C;
        pp->U = pU;
        pU += lvu->var_4C;
        pp->V = pV;
        pV += lvu->var_58;
        ++pp;
    }
    if (!lvu->var_8B)
    {
        for (; lvu->var_3C; lvu->var_3C--)
        {
            pp->X = pXb;
            pXb += lvu->var_30;
            pp->Y = pY;
            pY += lvu->var_2C;
            pp->U = pU;
            pU += lvu->var_50;
            pp->V = pV;
            pV += lvu->var_5C;
            ++pp;
        }
    }
    return 1;
}

static inline int trig_rl_md05(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pXa, pXb, pY;
    long pU, pV, pS;
    struct PolyPoint *pp;

    lvu->var_80 = (lvu->var_34 << 16) / lvu->var_38;
    {
        long dXa, wXb;
        long eX;
        TbBool eX_overflow;

        wXb = lvu->var_80 * (opt_b->X - opt_a->X) >> 16;
        dXa = opt_a->X - opt_c->X;
        eX_overflow = __OFSUBL__(wXb, -dXa);
        eX = wXb + dXa;
        if ((eX < 0) ^ eX_overflow) {
            LOGNO("skip due to sum %ld %ld", (long)wXb, (long)dXa);
            return 0;
        }
        lvu->var_60 = wXb;
        if (eX != 0) {
            long long dS, wS;

            dS = opt_b->U - opt_a->U;
            wS = (lvu->var_80 * dS) >> 16;
            lvu->var_48 = (opt_a->U + wS - opt_c->U) / (eX + 1);
            dS = opt_b->V - opt_a->V;
            wS = (lvu->var_80 * dS) >> 16;
            lvu->var_54 = (opt_a->V + wS - opt_c->V) / (eX + 1);
            dS = opt_b->S - opt_a->S;
            wS = (lvu->var_80 * dS) >> 16;
            lvu->var_60 = (opt_a->S + wS - opt_c->S) / (eX + 1);
        }
    }
    lvu->var_4C = (opt_c->U - opt_a->U) / lvu->var_34;
    lvu->var_58 = (opt_c->V - opt_a->V) / lvu->var_34;
    lvu->var_64 = (opt_c->S - opt_a->S) / lvu->var_34;
    lvu->var_50 = (opt_b->U - opt_c->U) / lvu->var_3C;
    lvu->var_5C = (opt_b->V - opt_c->V) / lvu->var_3C;
    lvu->var_68 = (opt_b->S - opt_c->S) / lvu->var_3C;
    pXa = opt_a->X << 16;
    pY = opt_a->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    pS = opt_a->S;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        if (lvu->var_6C - lvu->var_34 >= 0)
        {
            lvu->var_6C -= lvu->var_34;
            lvu->var_3C -= lvu->var_6C;
            pXb = lvu->var_30 * lvu->var_6C + lvu->var_40;
            pY += lvu->var_6C * lvu->var_2C + lvu->var_34 * lvu->var_2C;
            pU += lvu->var_6C * lvu->var_50 + lvu->var_34 * lvu->var_4C;
            pV += lvu->var_6C * lvu->var_5C + lvu->var_34 * lvu->var_58;
            pS += lvu->var_6C * lvu->var_68 + lvu->var_34 * lvu->var_64;
            if (lvu->var_8C) {
                lvu->var_3C = vec_window_height;
                lvu->var_44 = vec_window_height;
            }
            lvu->var_34 = 0;
        }
        else
        {
            lvu->var_34 -= lvu->var_6C;
            pXa += lvu->var_28 * lvu->var_6C;
            pY += lvu->var_6C * lvu->var_2C;
            pU += lvu->var_6C * lvu->var_4C;
            pV += lvu->var_6C * lvu->var_58;
            pS += lvu->var_6C * lvu->var_64;
            if (lvu->var_8C) {
                lvu->var_44 = vec_window_height;
                if (lvu->var_8B) {
                    lvu->var_34 = vec_window_height;
                } else {
                    lvu->var_8B = vec_window_height <= lvu->var_34;
                    lvu->var_3C = vec_window_height - lvu->var_34;
                }
            }
            pXb = lvu->var_40;
        }
    }
    else
    {
        if (lvu->var_8C)
        {
            long dH, eH;
            TbBool eH_overflow;

            dH = vec_window_height - lvu->var_78;
            lvu->var_44 = dH;
            if (lvu->var_8B) {
                lvu->var_34 = dH;
            } else {
                eH_overflow = __OFSUBL__(dH, lvu->var_34);
                eH = dH - lvu->var_34;
                lvu->var_8B = ((eH < 0) ^ eH_overflow) | (eH == 0);
                lvu->var_3C = eH;
            }
        }
        pXb = lvu->var_40;
    }
    pp = polyscans;
    for (; lvu->var_34; lvu->var_34--)
    {
        pp->X = pXa;
        pXa += lvu->var_28;
        pp->Y = pY;
        pY += lvu->var_2C;
        pp->U = pU;
        pU += lvu->var_4C;
        pp->V = pV;
        pV += lvu->var_58;
        pp->S = pS;
        pS += lvu->var_64;
        ++pp;
    }
    if (!lvu->var_8B)
    {
        for (; lvu->var_3C; lvu->var_3C--)
        {
          pp->X = pXb;
          pXb += lvu->var_30;
          pp->Y = pY;
          pY += lvu->var_2C;
          pp->U = pU;
          pU += lvu->var_50;
          pp->V = pV;
          pV += lvu->var_5C;
          pp->S = pS;
          pS += lvu->var_68;
          ++pp;
        }
    }
    return 1;
}

int trig_rl_start(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    long dX, dY;

    lvu->var_78 = opt_a->Y;
    if (opt_a->Y < 0) {
      lvu->var_24 = poly_screen;
      lvu->var_8A = 1;
    } else if (opt_a->Y < vec_window_height) {
      lvu->var_24 = poly_screen + vec_screen_width * opt_a->Y;
      lvu->var_8A = 0;
    } else  {
        LOGNO("height %ld exceeded by opt_a Y %ld", (long)vec_window_height, (long)opt_a->Y);
        return 0;
    }

    lvu->var_8B = opt_c->Y > vec_window_height;
    dY = opt_c->Y - opt_a->Y;
    lvu->var_34 = dY;

    lvu->var_8C = opt_b->Y > vec_window_height;
    dY = opt_b->Y - opt_a->Y;
    lvu->var_38 = dY;
    lvu->var_44 = dY;
    dX = opt_c->X - opt_a->X;
    lvu->var_28 = (dX << 16) / lvu->var_34;
    dX = opt_b->X - opt_a->X;
    if ((dX << 16) / dY <= lvu->var_28) {
        LOGNO("value (%ld << 16) / %ld below min %ld", (long)dX, (long)dY, (long)lvu->var_28);
        return 0;
    }
    lvu->var_2C = (dX << 16) / dY;

    dY = opt_b->Y - opt_c->Y;
    dX = opt_b->X - opt_c->X;
    lvu->var_30 = (dX << 16) / dY;
    lvu->var_3C = dY;
    lvu->var_40 = opt_c->X << 16;

    ret = 0;
    switch (vec_mode) /* swars-final @ 0x121814 */
    {
    case RendVec_mode00:
    case RendVec_mode14:
    case RendVec_mode15:
        ret = trig_rl_md00(lvu, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode01:
    case RendVec_mode04:
    case RendVec_mode16:
    case RendVec_mode17:
        ret = trig_rl_md01(lvu, opt_a, opt_b, opt_c);
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
        ret = trig_rl_md02(lvu, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode05:
    case RendVec_mode06:
    case RendVec_mode20:
    case RendVec_mode21:
    case RendVec_mode24:
    case RendVec_mode25:
    case RendVec_mode26:
        ret = trig_rl_md05(lvu, opt_a, opt_b, opt_c);
        break;
    }

    return ret;
}

static inline int trig_fb_md00(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pY;
    struct PolyPoint *pp;

    pX = opt_a->X << 16;
    pY = opt_a->X << 16;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        lvu->var_34 += lvu->var_78;
        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        pX += lvu->var_28 * (-lvu->var_78);
        pY += (-lvu->var_78) * lvu->var_2C;
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height;
            lvu->var_34 = vec_window_height;
        }
    }
    else
    {
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height - lvu->var_78;
            lvu->var_34 = vec_window_height - lvu->var_78;
        }
    }
    pp = polyscans;
    for (; lvu->var_34; lvu->var_34--)
    {
        pp->X = pX;
        pX += lvu->var_28;
        pp->Y = pY;
        pY += lvu->var_2C;
        ++pp;
    }
    return 1;
}

static inline int trig_fb_md01(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int pX, pY;
    int pS;
    struct PolyPoint *pp;

    {
        long dX;
        dX = opt_b->X - opt_c->X;
        lvu->var_60 = (opt_b->S - opt_c->S) / dX;
        lvu->var_64 = (opt_c->S - opt_a->S) / lvu->var_44;
    }
    pX = opt_a->X << 16;
    pY = opt_a->X << 16;
    pS = opt_a->S;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        lvu->var_34 += lvu->var_78;
        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        pX += lvu->var_28 * (-lvu->var_78);
        pY += (-lvu->var_78) * lvu->var_2C;
        pS += (-lvu->var_78) * lvu->var_64;
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height;
            lvu->var_34 = vec_window_height;
        }
    }
    else
    {
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height - lvu->var_78;
            lvu->var_34 = vec_window_height - lvu->var_78;
        }
    }
    pp = polyscans;
    for (; lvu->var_34; lvu->var_34--)
    {
        pp->X = pX;
        pX += lvu->var_28;
        pp->Y = pY;
        pY += lvu->var_2C;
        pp->S = pS;
        pS += lvu->var_64;
        ++pp;
    }
    return 1;
}

static inline int trig_fb_md02(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pY;
    long pU, pV;
    struct PolyPoint *pp;

    {
        long dX;
        dX = opt_b->X - opt_c->X;
        lvu->var_48 = (opt_b->U - opt_c->U) / dX;
        lvu->var_54 = (opt_b->V - opt_c->V) / dX;
        lvu->var_4C = (opt_c->U - opt_a->U) / lvu->var_44;
        lvu->var_58 = (opt_c->V - opt_a->V) / lvu->var_44;
    }
    pX = opt_a->X << 16;
    pY = opt_a->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        lvu->var_34 += lvu->var_78;
        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        pX += lvu->var_28 * (-lvu->var_78);
        pY += (-lvu->var_78) * lvu->var_2C;
        pU += (-lvu->var_78) * lvu->var_4C;
        pV += (-lvu->var_78) * lvu->var_58;
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height;
            lvu->var_34 = vec_window_height;
        }
    }
    else
    {
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height - lvu->var_78;
            lvu->var_34 = vec_window_height - lvu->var_78;
        }
    }
    pp = polyscans;
    for (; lvu->var_34; lvu->var_34--)
    {
        pp->X = pX;
        pX += lvu->var_28;
        pp->Y = pY;
        pY += lvu->var_2C;
        pp->U = pU;
        pU += lvu->var_4C;
        pp->V = pV;
        pV += lvu->var_58;
        ++pp;
    }
    return 1;
}

static inline int trig_fb_md05(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pY;
    long pU, pV, pS;
    struct PolyPoint *pp;

    {
        long dX;
        dX = opt_b->X - opt_c->X;
        lvu->var_48 = (opt_b->U - opt_c->U) / dX;
        lvu->var_54 = (opt_b->V - opt_c->V) / dX;
        lvu->var_60 = (opt_b->S - opt_c->S) / dX;
        lvu->var_4C = (opt_c->U - opt_a->U) / lvu->var_44;
        lvu->var_58 = (opt_c->V - opt_a->V) / lvu->var_44;
        lvu->var_64 = (opt_c->S - opt_a->S) / lvu->var_44;
    }
    pX = opt_a->X << 16;
    pY = opt_a->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    pS = opt_a->S;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        lvu->var_34 += lvu->var_78;
        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        pX += lvu->var_28 * (-lvu->var_78);
        pY += (-lvu->var_78) * lvu->var_2C;
        pU += (-lvu->var_78) * lvu->var_4C;
        pV += (-lvu->var_78) * lvu->var_58;
        pS += (-lvu->var_78) * lvu->var_64;
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height;
            lvu->var_34 = vec_window_height;
        }
    }
    else
    {
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height - lvu->var_78;
            lvu->var_34 = vec_window_height - lvu->var_78;
        }
    }
    pp = polyscans;
    for (; lvu->var_34; lvu->var_34--)
    {
        pp->X = pX;
        pX += lvu->var_28;
        pp->Y = pY;
        pY += lvu->var_2C;
        pp->U = pU;
        pU += lvu->var_4C;
        pp->V = pV;
        pV += lvu->var_58;
        pp->S = pS;
        pS += lvu->var_64;
        ++pp;
    }
    return 1;
}

int trig_fb_start(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    long dX, dY;

    lvu->var_78 = opt_a->Y;
    if (opt_a->Y < 0) {
        lvu->var_24 = poly_screen;
        lvu->var_8A = 1;
    } else if (opt_a->Y < vec_window_height) {
        lvu->var_24 = poly_screen + vec_screen_width * opt_a->Y;
        lvu->var_8A = 0;
    } else {
        LOGNO("height %ld exceeded by opt_a Y %ld", (long)vec_window_height, (long)opt_a->Y);
        return 0;
    }
    lvu->var_8B = opt_c->Y > vec_window_height;
    dY = opt_c->Y - opt_a->Y;
    lvu->var_34 = dY;
    lvu->var_44 = dY;
    dX = opt_c->X - opt_a->X;
    lvu->var_28 = (dX << 16) / dY;
    dX = opt_b->X - opt_a->X;
    lvu->var_2C = (dX << 16) / dY;

    ret = 0;
    switch (vec_mode) /* swars-final @ 0x122142, genewars-beta @ 0xEFE72 */
    {
    case RendVec_mode00:
    case RendVec_mode14:
    case RendVec_mode15:
        ret = trig_fb_md00(lvu, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode01:
    case RendVec_mode04:
    case RendVec_mode16:
    case RendVec_mode17:
        ret = trig_fb_md01(lvu, opt_a, opt_b, opt_c);
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
        ret = trig_fb_md02(lvu, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode05:
    case RendVec_mode06:
    case RendVec_mode20:
    case RendVec_mode21:
    case RendVec_mode24:
    case RendVec_mode25:
    case RendVec_mode26:
        ret = trig_fb_md05(lvu, opt_a, opt_b, opt_c);
        break;
    }

    return ret;
}

static inline int trig_ft_md00(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pY;
    struct PolyPoint *pp;

    pX = opt_a->X << 16;
    pY = opt_b->X << 16;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        lvu->var_34 += lvu->var_78;
        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        pX += lvu->var_28 * (-lvu->var_78);
        pY += (-lvu->var_78) * lvu->var_2C;
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height;
            lvu->var_34 = vec_window_height;
        }
    }
    else
    {
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height - lvu->var_78;
            lvu->var_34 = vec_window_height - lvu->var_78;
        }
    }
    pp = polyscans;
    for (; lvu->var_34; lvu->var_34--)
    {
        pp->X = pX;
        pX += lvu->var_28;
        pp->Y = pY;
        pY += lvu->var_2C;
        ++pp;
    }
    return 1;
}

static inline int trig_ft_md01(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pY;
    long pS;
    struct PolyPoint *pp;

    {
        long dX;
        dX = opt_b->X - opt_a->X;
        lvu->var_60 = (opt_b->S - opt_a->S) / dX;
        lvu->var_64 = (opt_c->S - opt_a->S) / lvu->var_44;
    }
    pX = opt_a->X << 16;
    pY = opt_b->X << 16;
    pS = opt_a->S;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        lvu->var_34 += lvu->var_78;
        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        pX += lvu->var_28 * (-lvu->var_78);
        pY += (-lvu->var_78) * lvu->var_2C;
        pS += (-lvu->var_78) * lvu->var_64;
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height;
            lvu->var_34 = vec_window_height;
        }
    }
    else
    {
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height - lvu->var_78;
            lvu->var_34 = vec_window_height - lvu->var_78;
        }
    }
    pp = polyscans;
    for (; lvu->var_34; lvu->var_34--)
    {
        pp->X = pX;
        pX += lvu->var_28;
        pp->Y = pY;
        pY += lvu->var_2C;
        pp->S = pS;
        pS += lvu->var_64;
        ++pp;
    }
    return 1;
}

static inline int trig_ft_md02(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pY;
    long pU, pV;
    struct PolyPoint *pp;
    {
        long dX;
        dX = opt_b->X - opt_a->X;
        lvu->var_48 = (opt_b->U - opt_a->U) / dX;
        lvu->var_54 = (opt_b->V - opt_a->V) / dX;
        lvu->var_4C = (opt_c->U - opt_a->U) / lvu->var_44;
        lvu->var_58 = (opt_c->V - opt_a->V) / lvu->var_44;
    }
    pX = opt_a->X << 16;
    pY = opt_b->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        lvu->var_34 += lvu->var_78;
        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        pX += lvu->var_28 * (-lvu->var_78);
        pY += (-lvu->var_78) * lvu->var_2C;
        pU += (-lvu->var_78) * lvu->var_4C;
        pV += (-lvu->var_78) * lvu->var_58;
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height;
            lvu->var_34 = vec_window_height;
        }
    }
    else
    {
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height - lvu->var_78;
            lvu->var_34 = vec_window_height - lvu->var_78;
        }
    }
    pp = polyscans;
    for (; lvu->var_34; lvu->var_34--)
    {
        pp->X = pX;
        pX += lvu->var_28;
        pp->Y = pY;
        pY += lvu->var_2C;
        pp->U = pU;
        pU += lvu->var_4C;
        pp->V = pV;
        pV += lvu->var_58;
        ++pp;
    }
    return 1;
}

static inline int trig_ft_md05(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    long pX, pY;
    long pU, pV, pS;
    struct PolyPoint *pp;

    {
        long dX;
        dX = opt_b->X - opt_a->X;
        lvu->var_48 = (opt_b->U - opt_a->U) / dX;
        lvu->var_54 = (opt_b->V - opt_a->V) / dX;
        lvu->var_60 = (opt_b->S - opt_a->S) / dX;
        lvu->var_4C = (opt_c->U - opt_a->U) / lvu->var_44;
        lvu->var_58 = (opt_c->V - opt_a->V) / lvu->var_44;
        lvu->var_64 = (opt_c->S - opt_a->S) / lvu->var_44;
    }
    pX = opt_a->X << 16;
    pY = opt_b->X << 16;
    pU = opt_a->U;
    pV = opt_a->V;
    pS = opt_a->S;
    if (lvu->var_8A)
    {
        long eH;
        TbBool eH_overflow;

        lvu->var_34 += lvu->var_78;
        eH_overflow = __OFSUBL__(lvu->var_44, -lvu->var_78);
        eH = lvu->var_44 + lvu->var_78;
        if (((eH < 0) ^ eH_overflow) | (eH == 0)) {
            LOGNO("skip due to sum %ld %ld", (long)lvu->var_44, (long)lvu->var_78);
            return 0;
        }
        lvu->var_44 = eH;
        lvu->var_6C = -lvu->var_78;
        pX += lvu->var_28 * (-lvu->var_78);
        pY += (-lvu->var_78) * lvu->var_2C;
        pU += (-lvu->var_78) * lvu->var_4C;
        pV += (-lvu->var_78) * lvu->var_58;
        pS += (-lvu->var_78) * lvu->var_64;
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height;
            lvu->var_34 = vec_window_height;
        }
    }
    else
    {
        if (lvu->var_8B) {
            lvu->var_44 = vec_window_height - lvu->var_78;
            lvu->var_34 = vec_window_height - lvu->var_78;
        }
    }
    pp = polyscans;
    for (; lvu->var_34; lvu->var_34--)
    {
        pp->X = pX;
        pX += lvu->var_28;
        pp->Y = pY;
        pY += lvu->var_2C;
        pp->U = pU;
        pU += lvu->var_4C;
        pp->V = pV;
        pV += lvu->var_58;
        pp->S = pS;
        pS += lvu->var_64;
        ++pp;
    }
    return 1;
}

int trig_ft_start(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    long dX, dY;

    lvu->var_78 = opt_a->Y;
    if (opt_a->Y < 0) {
      lvu->var_24 = poly_screen;
      lvu->var_8A = 1;
    } else if (opt_a->Y < vec_window_height) {
      lvu->var_24 = poly_screen + vec_screen_width * opt_a->Y;
      lvu->var_8A = 0;
    } else {
        LOGNO("height %ld exceeded by opt_a Y %ld", (long)vec_window_height, (long)opt_a->Y);
        return 0;
    }
    lvu->var_8B = opt_c->Y > vec_window_height;
    dY = opt_c->Y - opt_a->Y;
    lvu->var_34 = dY;
    lvu->var_44 = dY;
    dX = opt_c->X - opt_a->X;
    lvu->var_28 = (dX << 16) / dY;
    dX = opt_c->X - opt_b->X;
    lvu->var_2C = (dX << 16) / dY;

    ret = 0;
    switch (vec_mode) /* swars-final @ 0x1225c1, genewars-beta @ 0xF02F1 */
    {
    case RendVec_mode00:
    case RendVec_mode14:
    case RendVec_mode15:
        ret = trig_ft_md00(lvu, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode01:
    case RendVec_mode04:
    case RendVec_mode16:
    case RendVec_mode17:
        ret = trig_ft_md01(lvu, opt_a, opt_b, opt_c);
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
        ret = trig_ft_md02(lvu, opt_a, opt_b, opt_c);
        break;
    case RendVec_mode05:
    case RendVec_mode06:
    case RendVec_mode20:
    case RendVec_mode21:
    case RendVec_mode24:
    case RendVec_mode25:
    case RendVec_mode26:
        ret = trig_ft_md05(lvu, opt_a, opt_b, opt_c);
        break;
    }

    return ret;
}

/******************************************************************************/
