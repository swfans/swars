/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_render_trig.c
 *     Rendering function trig() for drawing 3D view elements.
 * @par Purpose:
 *     Function for rendering 3D elements.
 * @par Comment:
 *     Go away from here, you bad optimizer! Do not compile this with optimizations.
 * @author   Tomasz Lis
 * @date     20 Mar 2009 - 30 Mar 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_render.h"
#include "bfgentab.h"
#include "poly.h"
#include "poly_trigp.h"
#include "poly_trigr.h"
#include "swlog.h"

#include "globals.h"

/******************************************************************************/
// Allow enabling non-remade versions of trig(), to check for regressions
#define USE_ASM_TRIG_DIVIDED 0

#define STRINGIFY(x) #x
#ifdef NEED_UNDERSCORE
# define EXPORT_SYMBOL(sym) STRINGIFY(_ ## sym)
#else
# define EXPORT_SYMBOL(sym) STRINGIFY(sym)
#endif

extern const long add_to_edi[];

/******************************************************************************/
extern GameTurn gameturn;

/* trig() wrapper for debug (comparing remade trig() with original assembly */
void trig_dbg(struct PolyPoint *point_a, struct PolyPoint *point_b,
  struct PolyPoint *point_c)
{
    if (gameturn & 0x04) {
        asm volatile ("call ASM_trig\n"
            :  : "a" (point_a), "d" (point_b), "b" (point_c));
    } else {
        trig(point_a, point_b, point_c);
    }
}
/******************************************************************************/

/**
 * whether the subtraction (x-y) of two short ints would overflow
 */
static inline ubyte __OFSUBS__(short x, short y)
{
    return ((x < 0) ^ (y < 0)) & ((x < 0) ^ (x-y < 0));
}

/**
 * whether the addition (x+y) of two short ints would use carry
 */
static inline ubyte __CFADDS__(short x, short y)
{
    return (ushort)(x) > (ushort)(x+y);
}

/**
 * whether the addition (x+y) of two long ints would use carry
 */
static inline ubyte __CFADDL__(long x, long y)
{
    return (ulong)(x) > (ulong)(x+y);
}

/**
 * rotate left unsigned long
 */
static inline ulong __ROL4__(ulong value, int count)
{
    const uint nbits = 4 * 8;

    if (count > 0) {
        count %= nbits;
        ulong high = value >> (nbits - count);
        value <<= count;
        value |= high;
    } else {
        count = -count % nbits;
        ulong low = value << (nbits - count);
        value >>= count;
        value |= low;
    }
    return value;
}

void trig_render_md00(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x0+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(vec_colour)",%%al\n \
            mov    %%al,%%ah\n \
            mov    %%ax,%%bx\n \
            shl    $0x10,%%eax\n \
            mov    %%bx,%%ax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_rend_md00_a3c:\n \
            mov    0x2(%%esi),%%bx\n \
            movzwl 0x6(%%esi),%%ecx\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edx\n \
            or     %%bx,%%bx\n \
            jns    jump_rend_md00_a66\n \
            or     %%cx,%%cx\n \
            jle    jump_rend_md00_a89\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md00_a62\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md00_a62:\n \
            mov    %%edx,%%edi\n \
            jmp    jump_rend_md00_a7c\n \
        jump_rend_md00_a66:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md00_a74\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md00_a74:\n \
            sub    %%bx,%%cx\n \
            jle    jump_rend_md00_a89\n \
            lea    (%%ebx,%%edx,1),%%edi\n \
        jump_rend_md00_a7c:\n \
            shr    %%ecx\n \
            jae    jump_rend_md00_a81\n \
            stos   %%al,%%es:(%%edi)\n \
        jump_rend_md00_a81:\n \
            shr    %%ecx\n \
            jae    jump_rend_md00_a87\n \
            stos   %%ax,%%es:(%%edi)\n \
        jump_rend_md00_a87:\n \
            rep stos %%eax,%%es:(%%edi)\n \
        jump_rend_md00_a89:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_rend_md00_a3c\n \
        jump_rend_md00_a92:\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *o_ln;
    ubyte col;
    short pX, pY;
    ubyte *o;

    pp = polyscans;
    o_ln = lv.var_24;
    col = vec_colour;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pX = pp->X >> 16;
        pY = pp->Y >> 16;
        o_ln += vec_screen_width;
        if (pX < 0)
        {
            if (pY <= 0)
                continue;
            if (pY > vec_window_width)
                pY = vec_window_width;
            o = &o_ln[0];
        }
        else
        {
            TbBool pY_overflow;
            if (pY > vec_window_width)
                pY = vec_window_width;
            pY_overflow = __OFSUBS__(pY, pX);
            pY = pY - pX;
            if (((pY < 0) ^ pY_overflow) | (pY == 0))
                continue;
            o = &o_ln[pX];
        }
        memset(o, col, pY);
    }
#endif
}

void trig_render_md01(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_rend_md01_aa1:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_rend_md01_af3\n \
            or     %%cx,%%cx\n \
            jle    jump_rend_md01_c41\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            imul   0x3c+%[lv],%%eax\n \
            mov    %%ax,%%bx\n \
            shr    $0x8,%%eax\n \
            add    0x10(%%esi),%%bx\n \
            adc    0x12(%%esi),%%ah\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md01_ae9\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md01_ae9:\n \
            movzwl %%ax,%%eax\n \
            mov    "EXPORT_SYMBOL(vec_colour)",%%al\n \
            jmp    jump_rend_md01_b1a\n \
        jump_rend_md01_af3:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md01_b01\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md01_b01:\n \
            sub    %%ax,%%cx\n \
            jle    jump_rend_md01_c41\n \
            add    %%eax,%%edi\n \
            movzbl "EXPORT_SYMBOL(vec_colour)",%%eax\n \
            mov    0x10(%%esi),%%bx\n \
            mov    0x12(%%esi),%%ah\n \
        jump_rend_md01_b1a:\n \
            mov    %%ah,(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            add    $0x1,%%edi\n \
            jmp    jump_rend_md01_b1a\n \
        jump_rend_md01_c41:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_rend_md01_aa1\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    short pS;
    TbBool pS_carry;
    struct PolyPoint *pp;
    ushort colS;

    pp = polyscans;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pX, pY;
        ubyte *o;

        pX = pp->X >> 16;
        pY = pp->Y >> 16;
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;

        if (pX  < 0)
        {
            long mX;
            short colH;

            if (pY <= 0)
                continue;
            mX = lv.var_60 * (ushort)(-pX);
            pS_carry = __CFADDS__(pp->S, mX);
            pS = pp->S + mX;
            // Delcate code - if we add before shifting, the result is different
            colH = (mX >> 16) + (pp->S >> 16) + pS_carry;
            if (pY > vec_window_width)
                pY = vec_window_width;

            colS = ((colH & 0xFF) << 8) + vec_colour;
        }
        else
        {
            TbBool pY_overflow;
            short colH;

            if (pY > vec_window_width)
              pY = vec_window_width;
            pY_overflow = __OFSUBS__(pY, pX);
            pY = pY - pX;
            if (((pY < 0) ^ pY_overflow) | (pY == 0))
                continue;
            o += pX;
            colH = pp->S >> 16;
            pS = pp->S;

            colS = ((colH & 0xFF) << 8) + vec_colour;
        }

        for (;pY > 0; pY--, o++)
        {
            short colH, colL;

            *o = colS >> 8;

            colL = colS;
            pS_carry = __CFADDS__(lv.var_60, pS);
            pS = lv.var_60 + pS;
            colH = (lv.var_60 >> 16) + pS_carry + (colS >> 8);

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md02(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_rend_md02_c6a:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_rend_md02_cc4\n \
            or     %%cx,%%cx\n \
            jle    jump_rend_md02_edd\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md02_cbf\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md02_cbf:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_rend_md02_cec\n \
        jump_rend_md02_cc4:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md02_cd2\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md02_cd2:\n \
            sub    %%ax,%%cx\n \
            jle    jump_rend_md02_edd\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_rend_md02_cec:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_rend_md02_cf6:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            add    $0x1,%%edi\n \
            jmp    jump_rend_md02_cf6\n \
        jump_rend_md02_ed9:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_rend_md02_edd:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_rend_md02_c6a\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *o;
    ushort colS;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pX, pY;
        long pU;

        pX = pp->X >> 16;
        pY = pp->Y >> 16;
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;

        if (pX < 0)
        {
            ushort colL, colH;
            ulong factorA;
            long mX;

            if (pY <= 0)
                continue;
            mX = lv.var_54 * (-pX);
            factorA = __ROL4__(pp->V + mX, 16);
            colH = factorA;
            mX = lv.var_48 * (-pX);
            pU = (factorA & 0xFFFF0000) | ((pp->U + mX) & 0xFFFF);
            colL = (pp->U + mX) >> 16;
            if (pY > vec_window_width)
                pY = vec_window_width;
            pX = (pp->U + mX) >> 8;

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            short colL, colH;
            TbBool pY_overflow;

            if (pY > vec_window_width)
                pY = vec_window_width;
            pY_overflow = __OFSUBS__(pY, pX);
            pY = pY - pX;
            if (((pY < 0) ^ pY_overflow) | (pY == 0))
                continue;
            o += pX;
            pU = __ROL4__(pp->V, 16);
            colH = pU;
            pU = (pU & 0xFFFF0000) | (pp->U & 0xFFFF);
            colL = pp->U >> 16;

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;

        for (; pY > 0; pY--, o++)
        {
            short colL, colH;
            TbBool pU_carry;

            *o = m[colS];

            pU_carry = __CFADDS__(lv.var_48, pU);
            pU = lv.var_48 + pU;
            colL = (lv.var_48 >> 16) + pU_carry + colS;

            pU_carry = __CFADDL__(lv.var_70, pU);
            pU = lv.var_70 + pU;
            colH = (lv.var_54 >> 16) + pU_carry + (colS >> 8);

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md03(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_rend_md03_f06:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_rend_md03_f60\n \
            or     %%cx,%%cx\n \
            jle    jump_rend_md03_1bd\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md03_f5b\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md03_f5b:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_rend_md03_f88\n \
        jump_rend_md03_f60:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md03_f6e\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md03_f6e:\n \
            sub    %%ax,%%cx\n \
            jle    jump_rend_md03_1bd\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_rend_md03_f88:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_rend_md03_f92:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_19c\n \
            mov    %%al,(%%edi)\n \
        jump_rend_md03_19c:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            add    $0x1,%%edi\n \
            jmp    jump_rend_md03_f92\n \
        jump_rend_md03_1b9:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_rend_md03_1bd:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_rend_md03_f06\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *o;
    ushort colS;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pX, pY;
        ulong factorA;
        long pU;

        pX = pp->X >> 16;
        pY = pp->Y >> 16;
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pX < 0)
        {
            short colL, colH;
            long mX;

            if (pY <= 0)
                continue;
            mX = lv.var_54 * (-pX);
            factorA = __ROL4__(pp->V + mX, 16);
            colH = factorA;
            mX = lv.var_48 * (-pX);
            pU = (factorA & 0xFFFF0000) | ((pp->U + mX) & 0xFFFF);
            colL = (pp->U + mX) >> 16;
            if (pY > vec_window_width)
                pY = vec_window_width;

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            short colL, colH;
            TbBool pY_overflow;

            if (pY > vec_window_width)
                pY = vec_window_width;
            pY_overflow = __OFSUBS__(pY, pX);
            pY = pY - pX;
            if (((pY < 0) ^ pY_overflow) | (pY == 0))
                continue;
            o += pX;
            pU = __ROL4__(pp->V, 16);
            colH = pU;
            pU = (pU & 0xFFFF0000) | ((pp->U) & 0xFFFF);
            colL = (pp->U) >> 16;

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;

        for (; pY > 0; pY--, o++)
        {
            short colL, colH;
            TbBool pU_carry;

            if (m[colS] != 0)
                *o = m[colS];
            pU_carry = __CFADDS__(lv.var_48, pU);
            pU = lv.var_48 + pU;
            colL = (lv.var_48 >> 16) + pU_carry + colS;
            // Why are we adding value for which only high 16 bits are important?
            pU_carry = __CFADDL__(lv.var_70, pU);
            pU = lv.var_70 + pU;
            colH = (lv.var_54 >> 16) + pU_carry + (colS >> 8);

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

/**
 * Verified in swars - uses:
 * - zealot car antennas
 * - tank lower chassis
 * - Large red and white rocket building - red stipes
 */
void trig_render_md04(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_rend_md04_1d9:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_rend_md04_22b\n \
            or     %%cx,%%cx\n \
            jle    jump_rend_md04_3e1\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            imul   0x3c+%[lv],%%eax\n \
            mov    %%ax,%%bx\n \
            shr    $0x8,%%eax\n \
            add    0x10(%%esi),%%bx\n \
            adc    0x12(%%esi),%%ah\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md04_221\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md04_221:\n \
            movzwl %%ax,%%eax\n \
            mov    "EXPORT_SYMBOL(vec_colour)",%%al\n \
            jmp    jump_rend_md04_252\n \
        jump_rend_md04_22b:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md04_239\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md04_239:\n \
            sub    %%ax,%%cx\n \
            jle    jump_rend_md04_3e1\n \
            add    %%eax,%%edi\n \
            movzbl "EXPORT_SYMBOL(vec_colour)",%%eax\n \
            mov    0x10(%%esi),%%bx\n \
            mov    0x12(%%esi),%%ah\n \
        jump_rend_md04_252:\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    $0x1,%%edi\n \
            jmp    jump_rend_md04_252\n \
        jump_rend_md04_3e1:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_rend_md04_1d9\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *f;
    ubyte *o;
    ushort colS;

    pp = polyscans;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pX, pY;
        short pU;

        pX = pp->X >> 16;
        pY = pp->Y >> 16;
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pX < 0)
        {
            ushort colL, colH;
            TbBool pU_carry;
            long mX;

            if (pY <= 0)
                continue;
            mX = lv.var_60 * (-pX);
            pU_carry = __CFADDS__(pp->S, mX);
            pU = pp->S + mX;
            colH = (pp->S >> 16) + pU_carry + (mX >> 16);
            if (pY > vec_window_width)
                pY = vec_window_width;
            colL = vec_colour;

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            TbBool pY_overflow;

            if (pY > vec_window_width)
                pY = vec_window_width;
            pY_overflow = __OFSUBS__(pY, pX);
            pY = pY - pX;
            if (((pY < 0) ^ pY_overflow) | (pY == 0))
                continue;
            o += pX;
            colL = vec_colour;
            pU = pp->S;
            colH = pp->S >> 16;

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        f = pixmap.fade_table;

        for (;pY > 0; pY--, o++)
        {
            ushort colL, colH;
            TbBool pU_carry;

            pU_carry = __CFADDS__(lv.var_60, pU);
            pU = lv.var_60 + pU;
            colL = colS;
            colH = (lv.var_60 >> 16) + pU_carry + (colS >> 8);
            *o = f[colS];

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md05(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    %%esi,0x5c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            mov    0x24+%[lv],%%ecx\n \
            mov    0x30+%[lv],%%edx\n \
            mov    0x3c+%[lv],%%ebp\n \
            cmp    $0x0,%%ebp\n \
            jae    jump_rend_md05_451\n \
            dec    %%ecx\n \
        jump_rend_md05_451:\n \
            rol    $0x10,%%ecx\n \
            rol    $0x10,%%edx\n \
            shr    $0x8,%%ebp\n \
            mov    %%dl,%%bl\n \
            mov    %%cl,%%dl\n \
            mov    %%bp,%%cx\n \
            mov    %%ecx,0x4c+%[lv]\n \
            mov    %%edx,0x50+%[lv]\n \
            mov    %%bl,0x64+%[lv]\n \
        jump_rend_md05_46d:\n \
            mov    0x5c+%[lv],%%esi\n \
            addl   $0x14,0x5c+%[lv]\n \
            mov    (%%esi),%%eax\n \
            mov    0x4(%%esi),%%ebp\n \
            sar    $0x10,%%eax\n \
            sar    $0x10,%%ebp\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%eax,%%eax\n \
            jns    jump_rend_md05_4db\n \
            or     %%ebp,%%ebp\n \
            jle    jump_rend_gt_73b\n \
            neg    %%eax\n \
            mov    0x24+%[lv],%%ecx\n \
            imul   %%eax,%%ecx\n \
            add    0x8(%%esi),%%ecx\n \
            mov    0x30+%[lv],%%edx\n \
            imul   %%eax,%%edx\n \
            add    0xc(%%esi),%%edx\n \
            mov    0x3c+%[lv],%%ebx\n \
            imul   %%eax,%%ebx\n \
            add    0x10(%%esi),%%ebx\n \
            rol    $0x10,%%ecx\n \
            rol    $0x10,%%edx\n \
            shr    $0x8,%%ebx\n \
            mov    %%dl,%%al\n \
            mov    %%cl,%%dl\n \
            mov    %%bx,%%cx\n \
            mov    %%al,%%bh\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ebp\n \
            jle    jump_rend_md05_4d9\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ebp\n \
        jump_rend_md05_4d9:\n \
            jmp    jump_rend_md05_50e\n \
        jump_rend_md05_4db:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ebp\n \
            jle    jump_rend_md05_4e9\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ebp\n \
        jump_rend_md05_4e9:\n \
            sub    %%eax,%%ebp\n \
            jle    jump_rend_gt_73b\n \
            add    %%eax,%%edi\n \
            mov    0x8(%%esi),%%ecx\n \
            mov    0xc(%%esi),%%edx\n \
            mov    0x10(%%esi),%%ebx\n \
            rol    $0x10,%%ecx\n \
            rol    $0x10,%%edx\n \
            shr    $0x8,%%ebx\n \
            mov    %%dl,%%al\n \
            mov    %%cl,%%dl\n \
            mov    %%bx,%%cx\n \
            mov    %%al,%%bh\n \
        jump_rend_md05_50e:\n \
            xor    %%dh,%%dh\n \
            and    $0xffff,%%ebx\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
\n \
gt_md01:\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,(%%edi)\n \
            add    $0x1,%%edi\n \
            sub    $0x1,%%ebp\n \
            jg     gt_md01\n \
        jump_rend_gt_73b:\n \
            decl   0x20+%[lv]\n \
            jne    jump_rend_md05_46d\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *f;
    ubyte *o;
    ubyte *o_ln;
    long rfactA, rfactB;

    pp = polyscans;
    {
        ulong factorA, factorB, factorC;
        factorC = lv.var_48;
        if (lv.var_60 < 0) factorC--;
        factorC = __ROL4__(factorC, 16);
        factorA = __ROL4__(lv.var_54, 16);
        factorB = ((ulong)lv.var_60) >> 8;
        lv.var_88 = (factorA & 0xFF);
        lv.var_74 = (factorA & 0xFFFFFF00) | (factorC & 0xFF);
        lv.var_70 = (factorC & 0xFFFF0000) | (factorB & 0xFFFF);
    }
    for (; lv.var_44; lv.var_44--, pp++)
    {
        long pX, pY;
        ushort colM;

        pX = pp->X >> 16;
        pY = pp->Y >> 16;
        o_ln = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;

        if (pX < 0)
        {
            ulong factorA, factorB;
            ushort colL, colH;
            long mX;

            if (pY <= 0)
                continue;
            mX = lv.var_48 * (-pX);
            factorA = __ROL4__(pp->U + mX, 16);
            mX = lv.var_54 * (-pX);
            factorB = __ROL4__(pp->V + mX, 16);
            mX = lv.var_60 * (-pX);
            colL = (pp->S + mX) >> 8;
            colH = factorB;
            rfactB = (factorA & 0xFF);
            rfactA = (factorA & 0xFFFF0000) | (colL & 0xFFFF);
            if (pY > vec_window_width)
                pY = vec_window_width;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ulong factorA, factorB;
            ushort colL, colH;
            TbBool pY_overflow;

            if (pY > vec_window_width)
                pY = vec_window_width;
            pY_overflow = __OFSUBS__(pY, pX);
            pY = pY - pX;
            if (((pY < 0) ^ pY_overflow) | (pY == 0))
                continue;
            o_ln += pX;
            factorA = __ROL4__(pp->U, 16);
            factorB = __ROL4__(pp->V, 16);
            colL = pp->S >> 8;
            colH = factorB;
            // Should the high part really be preserved?
            rfactB = (factorB & 0xFFFF0000) | (factorA & 0xFF);
            rfactA = (factorA & 0xFFFF0000) | (colL & 0xFFFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        o = o_ln;
        m = vec_map;
        f = pixmap.fade_table;

        for (; pY > 0; pY--, o++)
        {
            ushort colL, colH;
            ushort colS;
            TbBool rfactA_carry;
            TbBool rfactB_carry;

            colM = (colM & 0xFF00) + (rfactB & 0xFF);
            colS = (((rfactA >> 8) & 0xFF) << 8) + m[colM];

            rfactA_carry = __CFADDL__(rfactA, lv.var_70);
            rfactA = rfactA + lv.var_70;

            rfactB_carry = __CFADDL__(rfactB + rfactA_carry, lv.var_74);
            rfactB = rfactB + lv.var_74 + rfactA_carry;

            colH = lv.var_88 + rfactB_carry + (colM >> 8);
            colL = colM;
            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);

            *o = f[colS];
        }
    }
#endif
}

/**
 * Verified in swars - uses:
 * - wire fences
 * - cross bars
 */
void trig_render_md06(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            mov    0x3c+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x50+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_123770:\n \
            mov    0x5c+%[lv],%%esi\n \
            addl   $0x14,0x5c+%[lv]\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_1237ea\n \
            or     %%cx,%%cx\n \
            jle    jump_123b1c\n \
            mov    %%cx,%%bp\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            mov    %%eax,%%ecx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            imul   0x3c+%[lv],%%ecx\n \
            add    0x10(%%esi),%%ecx\n \
            rol    $0x10,%%ecx\n \
            mov    %%cl,%%ah\n \
            mov    %%bp,%%cx\n \
            movzwl %%ax,%%eax\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%cx\n \
            jle    jump_1237e8\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%cx\n \
        jump_1237e8:\n \
            jmp    jump_123820\n \
        jump_1237ea:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_1237f8\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_1237f8:\n \
            sub    %%ax,%%cx\n \
            jle    jump_123b1c\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            mov    0xa(%%esi),%%bl\n \
            rol    $0x10,%%edx\n \
            mov    %%cx,%%bp\n \
            mov    %%dl,%%bh\n \
            mov    0x10(%%esi),%%ecx\n \
            mov    0x8(%%esi),%%dx\n \
            rol    $0x10,%%ecx\n \
            mov    %%cl,%%ah\n \
            mov    %%bp,%%cx\n \
        jump_123820:\n \
            mov    %%cx,%%si\n \
            and    $0xf,%%esi\n \
\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%ebp\n \
\n \
mgt_md01:\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_123af6\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_123af6:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
            add    $0x1,%%edi\n \
            sub    $0x1,%%cx\n \
            jg     mgt_md01\n \
        jump_123b1c:\n \
            decl   0x20+%[lv]\n \
            jne    jump_123770\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte pY_overflow;
    ushort colM;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    lv.var_74 = lv.var_60 << 16;
    colM = 0;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        ubyte *m;
        ubyte *f;
        ubyte *o;
        ushort pXa;
        int pYa;
        short pYb;
        int factorA;
        long pY;
        long factorB;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            int pXMa;
            int pXMb;
            ulong mX;

            if ( (short)pYa <= 0 )
                continue;
            pYb = pYa;
            pXMa = (ushort)-(short)pXa;
            pXMb = pXMa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXMa, 16);
            colH = factorA;
            mX = pp->U + lv.var_48 * pXMa;
            factorA = (factorA & 0xFFFF0000) | (mX & 0xFFFF);
            pXa = mX >> 8;
            colL = (pXa >> 8);
            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
            factorB = __ROL4__(pp->S + lv.var_60 * pXMb, 16);
            pXa = (pXa & 0xFFFF00FF) | ((factorB & 0xFF) << 8);
            pY = (factorB & 0xFFFF0000) | (pYb & 0xFFFF);
            pXa = (ushort)pXa;
            if (pYb > vec_window_width)
                pY = vec_window_width;
        }
        else
        {
            ushort colL, colH;
            ubyte pLa_overflow;
            short pLa;
            short pLb;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pLa_overflow = __OFSUBS__(pYa, pXa);
            pLa = pYa - pXa;
            if ( ((ubyte)((pLa < 0) ^ pLa_overflow) | (pLa == 0)) )
                continue;

            o += pXa;
            colL = (pp->U >> 16);
            factorA = __ROL4__(pp->V, 16);
            pLb = pLa;
            colH = factorA;
            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
            factorA = (factorA & 0xFFFF0000) | (pp->U & 0xFFFF);
            factorB = __ROL4__(pp->S, 16);
            pXa = (pXa & 0xFFFF00FF) | ((factorB & 0xFF) << 8);
            pY = (factorB & 0xFFFF0000) | (pLb & 0xFFFF);
        }

        m = vec_map;
        f = pixmap.fade_table;
        do
        {
            ushort colL, colH;
            ubyte fct_carry;

            pXa = (pXa & 0xFF00) | (m[colM] & 0xFF);
            if (pXa & 0xFF)
                *o = f[pXa];
            fct_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) | ((lv.var_48 + factorA) & 0xFFFF);
            colL = (lv.var_48 >> 16) + fct_carry + colM;
            fct_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (lv.var_54 >> 16) + fct_carry + (colM >> 8);
            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
            fct_carry = __CFADDL__(lv.var_74, pY);
            pY += lv.var_74;
            pXa = (((pXa >> 8) + (lv.var_60 >> 16) + fct_carry) << 8) | (pXa & 0xFF);
            ++o;
            pY_overflow = __OFSUBS__(pY, 1);
            pY--;
        }
        while ( !((ubyte)(((pY & 0x8000u) != 0) ^ pY_overflow) | ((ushort)pY == 0)) );
    }
#endif
}

void trig_render_md07(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_123b4f:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_123ba9\n \
            or     %%cx,%%cx\n \
            jle    jump_123e2c\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_123ba4\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_123ba4:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_123bd1\n \
        jump_123ba9:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_123bb7\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_123bb7:\n \
            sub    %%ax,%%cx\n \
            jle    jump_123e2c\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_123bd1:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
            mov    "EXPORT_SYMBOL(vec_colour)",%%ah\n \
        jump_123be1:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            add    $0x1,%%edi\n \
            jmp    jump_123be1\n \
        jump_123e28:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_123e2c:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_123b4f\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    short pXa;
    ushort colM;
    long pYa;
    ubyte *o;
    long pXm;
    long factorA;
    ubyte *m;
    ubyte *f;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    colM = 0;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            ulong factorB, factorC;

            if ( (short)pYa <= 0 )
                continue;
            pXm = (ushort)-(short)pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorC = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) | (factorC & 0xFFFF);
            factorB = factorC >> 8;
            colL = ((factorB >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorB;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( (ubyte)(((pYa & 0x8000u) != 0) ^ pY_overflow) | ((ushort)pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) | (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        f = pixmap.fade_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            colS = (vec_colour << 8) + m[colM];
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) | ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            *o = f[colS];
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md08(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_123e55:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_123eaf\n \
            or     %%cx,%%cx\n \
            jle    jump_124172\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_123eaa\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_123eaa:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_123ed7\n \
        jump_123eaf:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_123ebd\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_123ebd:\n \
            sub    %%ax,%%cx\n \
            jle    jump_124172\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_123ed7:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
            mov    "EXPORT_SYMBOL(vec_colour)",%%ah\n \
        jump_123ee7:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_12415a\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_12415a:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            add    $0x1,%%edi\n \
            jmp    jump_123ee7\n \
        jump_12416e:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_124172:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_123e55\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    short pXa;
    ushort colM;
    long pYa;
    ubyte *o;
    long factorA;
    ubyte *m;
    ubyte *f;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    colM = 0;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            ulong factorB, factorC;
            long pXm;

            if ( (short)pYa <= 0 )
                continue;
            pXm = (ushort)-(short)pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = ((factorC >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorC;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( (ubyte)(((pYa & 0x8000u) != 0) ^ pY_overflow) | ((ushort)pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        f = pixmap.fade_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            colS = (vec_colour << 8) + m[colM];
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            if (colS & 0xFF)
                *o = f[colS];
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md09(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_12419b:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_1241f5\n \
            or     %%cx,%%cx\n \
            jle    jump_1244e5\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_1241f0\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_1241f0:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_12421d\n \
        jump_1241f5:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_124203\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_124203:\n \
            sub    %%ax,%%cx\n \
            jle    jump_1244e5\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_12421d:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_124227:\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_1244cd\n \
            mov    (%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_1244cd:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            add    $0x1,%%edi\n \
            jmp    jump_124227\n \
        jump_1244e1:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_1244e5:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_12419b\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    short pXa;
    ushort colM;
    long factorA;
    short pYa;
    ubyte *o;
    long pXm;
    ubyte *m;
    ubyte *f;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    colM = 0;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            ulong factorB, factorC;

            if ( (short)pYa <= 0 )
                continue;
            pXm = (ushort)-(short)pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = ((factorC >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorC;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( (ubyte)(((pYa & 0x8000u) != 0) ^ pY_overflow) | ((ushort)pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        f = pixmap.fade_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            colS = m[colM] << 8;
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            if ((colS >> 8) & 0xFF) {
                colS = (colS & 0xFF00) | (*o);
                *o = f[colS];
            }
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md10(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_12450e:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_124568\n \
            or     %%cx,%%cx\n \
            jle    jump_12485e\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_124563\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_124563:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_124590\n \
        jump_124568:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_124576\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_124576:\n \
            sub    %%ax,%%cx\n \
            jle    jump_12485e\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_124590:\n \
            mov    "EXPORT_SYMBOL(vec_colour)",%%ah\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_1245a0:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12483d\n \
            mov    (%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_12483d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            add    $0x1,%%edi\n \
            jmp    jump_1245a0\n \
        jump_12485a:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_12485e:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_12450e\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    short pXa;
    ushort colM;
    short pYa;
    ubyte *o;
    ulong factorB;
    long factorA;
    ulong factorC;
    ubyte *m;
    ubyte *f;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    colM = 0;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            long pXm;

            if ( (short)pYa <= 0 )
                continue;
            pXm = (ushort)-(short)pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = ((factorC >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorC;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( (ubyte)(((pYa & 0x8000u) != 0) ^ pY_overflow) | ((ushort)pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        f = pixmap.fade_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            if (m[colM]) {
                colS = (vec_colour << 8) | (*o);
                *o = f[colS];
            }
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md12(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_124887:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_1248e1\n \
            or     %%cx,%%cx\n \
            jle    jump_124b63\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_1248dc\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_1248dc:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_124909\n \
        jump_1248e1:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_1248ef\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_1248ef:\n \
            sub    %%ax,%%cx\n \
            jle    jump_124b63\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_124909:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
            mov    "EXPORT_SYMBOL(vec_colour)",%%al\n \
        jump_124918:\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            add    $0x1,%%edi\n \
            jmp    jump_124918\n \
        jump_124b5f:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_124b63:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_124887\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    long pXa;
    ushort colM;
    short pYa;
    ubyte *o;
    long pXm;
    long factorA;
    ubyte *m;
    ubyte *g;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    colM = 0;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            ulong factorB, factorC;

            if ( (short)pYa <= 0 )
                continue;
            pXm = (ushort)-(short)pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorC = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorC & 0xFFFF);
            factorB = factorC >> 8;
            colL = ((factorB >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorB;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( (ubyte)(((pYa & 0x8000u) != 0) ^ pY_overflow) | ((ushort)pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            colS = (m[colM] << 8) | vec_colour;
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            *o = g[colS];
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md13(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_124b8c:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_124be6\n \
            or     %%cx,%%cx\n \
            jle    jump_124e69\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_124be1\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_124be1:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_124c0e\n \
        jump_124be6:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_124bf4\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_124bf4:\n \
            sub    %%ax,%%cx\n \
            jle    jump_124e69\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_124c0e:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
            mov    "EXPORT_SYMBOL(vec_colour)",%%ah\n \
        jump_124c1e:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            add    $0x1,%%edi\n \
            jmp    jump_124c1e\n \
        jump_124e65:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_124e69:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_124b8c\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    short pXa;
    ushort colM;
    short pYa;
    ubyte *o;
    long pXm;
    long factorA;
    ubyte *m;
    ubyte *g;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    colM = 0;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            ulong factorB, factorC;

            if ( (short)pYa <= 0 )
                continue;
            pXm = (ushort)-(short)pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = ((factorC >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorC;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( (ubyte)(((pYa & 0x8000u) != 0) ^ pY_overflow) | ((ushort)pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            colS = m[colM] | (vec_colour << 8);
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            *o = g[colS];
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md14(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x0+%[lv],%%edx\n \
            xor    %%eax,%%eax\n \
            mov    "EXPORT_SYMBOL(vec_colour)",%%ah\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_124e90:\n \
            mov    0x2(%%esi),%%bx\n \
            movzwl 0x6(%%esi),%%ecx\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edx\n \
            or     %%bx,%%bx\n \
            jns    jump_124ebe\n \
            or     %%cx,%%cx\n \
            jle    jump_124ffe\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_124eba\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_124eba:\n \
            mov    %%edx,%%edi\n \
            jmp    jump_124ed8\n \
        jump_124ebe:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_124ecc\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_124ecc:\n \
            sub    %%bx,%%cx\n \
            jle    jump_124ffe\n \
            lea    (%%ebx,%%edx,1),%%edi\n \
        jump_124ed8:\n \
            mov    (%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            add    $0x1,%%edi\n \
            jmp    jump_124ed8\n \
        jump_124ffe:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_124e90\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *o_ln;
    ushort colM;
    short pXa;
    short pYa;
    ubyte *o;
    ubyte *g;

    pp = polyscans;
    o_ln = lv.var_24;
    colM = (vec_colour << 8);

    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o_ln += vec_screen_width;

        if (pXa < 0)
        {
            if (pYa <= 0)
                continue;
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            o = o_ln;
        }
        else
        {
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o = &o_ln[pXa];
        }

        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
              colM = (colM & 0xFF00) | *o;
              *o = g[colM];
        }
    }
#endif
}

void trig_render_md15(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x0+%[lv],%%edx\n \
            movzbl "EXPORT_SYMBOL(vec_colour)",%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_125024:\n \
            mov    0x2(%%esi),%%bx\n \
            movzwl 0x6(%%esi),%%ecx\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edx\n \
            or     %%bx,%%bx\n \
            jns    jump_125052\n \
            or     %%cx,%%cx\n \
            jle    jump_125192\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_12504e\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_12504e:\n \
            mov    %%edx,%%edi\n \
            jmp    jump_12506c\n \
        jump_125052:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_125060\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_125060:\n \
            sub    %%bx,%%cx\n \
            jle    jump_125192\n \
            lea    (%%ebx,%%edx,1),%%edi\n \
        jump_12506c:\n \
            mov    (%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            add    $0x1,%%edi\n \
            jmp    jump_12506c\n \
        jump_125192:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_125024\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *o_ln;
    ushort colM;
    short pXa;
    short pYa;
    ubyte *o;
    ubyte *g;

    pp = polyscans;
    o_ln = lv.var_24;
    colM = vec_colour;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o_ln += vec_screen_width;
        if (pXa < 0)
        {
            if (pYa <= 0)
                continue;
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            o = o_ln;
        }
        else
        {
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o = &o_ln[pXa];
        }

        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
              colM = (*o << 8) | (colM & 0xFF);
              *o = g[colM];
        }
    }
#endif
}

void trig_render_md16(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            xor    %%eax,%%eax\n \
            xor    %%edx,%%edx\n \
        jump_1251ac:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_1251fe\n \
            or     %%cx,%%cx\n \
            jle    jump_12544b\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            imul   0x3c+%[lv],%%eax\n \
            mov    %%ax,%%bx\n \
            shr    $0x8,%%eax\n \
            add    0x10(%%esi),%%bx\n \
            adc    0x12(%%esi),%%ah\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_1251f4\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_1251f4:\n \
            movzwl %%ax,%%eax\n \
            mov    "EXPORT_SYMBOL(vec_colour)",%%al\n \
            jmp    jump_125225\n \
        jump_1251fe:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_12520c\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_12520c:\n \
            sub    %%ax,%%cx\n \
            jle    jump_12544b\n \
            add    %%eax,%%edi\n \
            movzbl "EXPORT_SYMBOL(vec_colour)",%%eax\n \
            mov    0x10(%%esi),%%bx\n \
            mov    0x12(%%esi),%%ah\n \
        jump_125225:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    (%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            add    $0x1,%%edi\n \
            jmp    jump_125225\n \
        jump_12544b:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_1251ac\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    short pXa;
    short pYa;
    ushort colM;
    ubyte *o;
    ubyte *g;
    ubyte *f;
    short factorA;

    pp = polyscans;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;

        if (pXa < 0)
        {
            ushort colL, colH;
            ubyte factorA_carry;
            ulong pXMa;
            short pXMb;

            if (pYa <= 0)
                continue;
            pXMa = lv.var_60 * (ushort)-pXa;
            pXMb = pXMa;
            pXa = pXMa >> 8;
            factorA_carry = __CFADDS__(pp->S, pXMb);
            factorA = (pp->S) + pXMb;
            colH = (pXa >> 8) + (pp->S >> 16) + factorA_carry;
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            colL = vec_colour;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            colL = vec_colour;
            factorA = pp->S;
            colH = (pp->S >> 16);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        g = pixmap.ghost_table;
        f = pixmap.fade_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            colS = (f[colM] << 8) | *o;
            *o = g[colS];
            factorA_carry = __CFADDS__(lv.var_60, factorA);
            factorA += (lv.var_60 & 0xFFFF);
            colH = (colM >> 8) + (lv.var_60 >> 16) + factorA_carry;
            colL = colM;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md17(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            xor    %%eax,%%eax\n \
            xor    %%edx,%%edx\n \
        jump_125465:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_1254b7\n \
            or     %%cx,%%cx\n \
            jle    jump_125704\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            imul   0x3c+%[lv],%%eax\n \
            mov    %%ax,%%bx\n \
            shr    $0x8,%%eax\n \
            add    0x10(%%esi),%%bx\n \
            adc    0x12(%%esi),%%ah\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_1254ad\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_1254ad:\n \
            movzwl %%ax,%%eax\n \
            mov    "EXPORT_SYMBOL(vec_colour)",%%al\n \
            jmp    jump_1254de\n \
        jump_1254b7:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_1254c5\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_1254c5:\n \
            sub    %%ax,%%cx\n \
            jle    jump_125704\n \
            add    %%eax,%%edi\n \
            movzbl "EXPORT_SYMBOL(vec_colour)",%%eax\n \
            mov    0x10(%%esi),%%bx\n \
            mov    0x12(%%esi),%%ah\n \
        jump_1254de:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    (%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            add    $0x1,%%edi\n \
            jmp    jump_1254de\n \
        jump_125704:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_125465\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    short pXa;
    long colM;
    short pYa;
    ubyte *o;
    ubyte *g;
    ubyte *f;
    ubyte factorA_carry;
    short factorA;

    pp = polyscans;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            ulong pXMa;
            short pXMb;

            if ( (short)pYa <= 0 )
                continue;
            pXMa = lv.var_60 * (ushort)-pXa;
            pXMb = pXMa;
            pXa = pXMa >> 8;
            factorA_carry = __CFADDS__(pp->S, pXMb);
            factorA = pp->S + pXMb;
            colH = (pXa >> 8) + (pp->S >> 16) + factorA_carry;
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            colL = vec_colour;

            pXa = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( (ubyte)(((pYa & 0x8000u) != 0) ^ pY_overflow) | ((ushort)pYa == 0) )
                continue;

            o += pXa;
            colL = vec_colour;
            factorA = pp->S;
            colH = (pp->S >> 16);

            pXa = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        g = pixmap.ghost_table;
        f = pixmap.fade_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;

            colM = ((*o) << 8) + f[pXa];
            *o = g[colM];
            factorA_carry = __CFADDS__(lv.var_60, factorA);
            factorA += (lv.var_60 & 0xFFFF);
            colH = (pXa >> 8) + ((lv.var_60 >> 16) & 0xFF) + factorA_carry;
            colL = pXa;

            pXa = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md18(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_12572d:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_125787\n \
            or     %%cx,%%cx\n \
            jle    jump_125a33\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_125782\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_125782:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_1257af\n \
        jump_125787:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_125795\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_125795:\n \
            sub    %%ax,%%cx\n \
            jle    jump_125a33\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_1257af:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_1257b9:\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    (%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            add    $0x1,%%edi\n \
            jmp    jump_1257b9\n \
        jump_125a2f:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_125a33:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_12572d\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    short pXa;
    ushort colM;
    short pYa;
    ubyte *o;
    long factorA;
    ubyte *m;
    ubyte *g;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        long pXm;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            ushort colL, colH;
            ulong factorB, factorC;

            if (pYa <= 0)
                continue;
            pXm = (ushort)-pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = (factorC >> 8);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorC;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_carry;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_carry = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_carry) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = (pp->U >> 16);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            colH = m[colM];
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) | ((lv.var_48 + factorA) & 0xFFFF);
            colL = *o;
            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            *o = g[colS];
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md19(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_125a5c:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_125ab6\n \
            or     %%cx,%%cx\n \
            jle    jump_125d62\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_125ab1\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_125ab1:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_125ade\n \
        jump_125ab6:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_125ac4\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_125ac4:\n \
            sub    %%ax,%%cx\n \
            jle    jump_125d62\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_125ade:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_125ae8:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    (%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            add    $0x1,%%edi\n \
            jmp    jump_125ae8\n \
        jump_125d5e:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_125d62:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_125a5c\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *g;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa;
        ushort colM;
        short pYa;
        ubyte *o;
        long factorA;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            ushort colL, colH;
            long pXm;
            ulong factorB, factorC;

            if (pYa <= 0)
                continue;
            pXm = (ushort)-pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = ((factorC >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colS = ((*o) << 8) + m[colM];
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            *o = g[colS];
            colH = (colM >> 8) + (lv.var_54 >> 16) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md20(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            mov    0x3c+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x50+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_125d96:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_125e01\n \
            or     %%cx,%%cx\n \
            jle    jump_1261db\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_125dc6\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_125dc6:\n \
            mov    %%ecx,0x58+%[lv]\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            mov    %%eax,%%ecx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            imul   0x3c+%[lv],%%ecx\n \
            add    0x10(%%esi),%%ecx\n \
            rol    $0x10,%%ecx\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_125e33\n \
        jump_125e01:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_125e0f\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_125e0f:\n \
            sub    %%ax,%%cx\n \
            jle    jump_1261db\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
            mov    %%ecx,0x58+%[lv]\n \
            mov    0x10(%%esi),%%ecx\n \
            rol    $0x10,%%ecx\n \
        jump_125e33:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_125e3d:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    (%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            add    $0x1,%%edi\n \
            jmp    jump_125e3d\n \
        jump_1261d7:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_1261db:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_125d96\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *g;
    ubyte *f;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    lv.var_74 = lv.var_60 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa;
        ushort colM;
        short pYa;
        ubyte *o;
        long pXMa;
        long pXMb;
        long factorA;
        long factorC;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            ushort colL, colH;
            ulong factorB;

            if (pYa <= 0)
                continue;
            if (pYa > vec_window_width)
                pYa = vec_window_width;
            lv.var_7C = pYa;
            pXMa = (ushort)-pXa;
            pXMb = pXMa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXMa, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXMa;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            pXa = factorB >> 8;
            colL = ((pXa >> 8) & 0xFF);
            factorC = __ROL4__(pp->S + lv.var_60 * pXMb, 16);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);
            lv.var_7C = pYa;
            factorC = __ROL4__(pp->S, 16);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;
        f = pixmap.fade_table;

        for (; lv.var_7C > 0; lv.var_7C--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colS = ((factorC & 0xFF) << 8) + m[colM];
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colS = ((f[colS] & 0xFF) << 8) + *o;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;
            factorA_carry = __CFADDL__(lv.var_74, factorC);
            factorC += lv.var_74;
            *o = g[colS];
            factorC = (factorC & 0xFFFFFF00) | (((lv.var_60 >> 16) + factorA_carry + factorC) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md21(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            mov    0x3c+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x50+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_12620f:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_12627a\n \
            or     %%cx,%%cx\n \
            jle    jump_126654\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_12623f\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_12623f:\n \
            mov    %%ecx,0x58+%[lv]\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            mov    %%eax,%%ecx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            imul   0x3c+%[lv],%%ecx\n \
            add    0x10(%%esi),%%ecx\n \
            rol    $0x10,%%ecx\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_1262ac\n \
        jump_12627a:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_126288\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_126288:\n \
            sub    %%ax,%%cx\n \
            jle    jump_126654\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
            mov    %%ecx,0x58+%[lv]\n \
            mov    0x10(%%esi),%%ecx\n \
            rol    $0x10,%%ecx\n \
        jump_1262ac:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_1262b6:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    (%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            add    $0x1,%%edi\n \
            jmp    jump_1262b6\n \
        jump_126650:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_126654:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_12620f\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *m;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    lv.var_74 = lv.var_60 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa;
        ushort colM;
        short pYa;
        ubyte *o;
        ubyte *g;
        ubyte *f;
        long factorA, factorC;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            ushort colL, colH;
            long pXMa;
            long pXMb;
            ulong factorB;

            if (pYa <= 0)
                continue;
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            lv.var_7C = pYa;
            pXMa = (ushort)-pXa;
            pXMb = pXMa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXMa, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXMa;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            pXa = factorB >> 8;
            colL = ((pXa >> 8) & 0xFF);
            factorC = __ROL4__(pp->S + lv.var_60 * pXMb, 16);
            pXa = (pXa & 0xFFFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);
            lv.var_7C = pYa;
            factorC = __ROL4__(pp->S, 16);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;
        f = pixmap.fade_table;

        for (; lv.var_7C > 0; lv.var_7C--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            colS = ((factorC & 0xFF) << 8) + (m[colM] & 0xFF);
            colS = (((*o) & 0xFF) << 8) + (f[colS] & 0xFF);
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;
            factorA_carry = __CFADDL__(lv.var_74, factorC);
            factorC += lv.var_74;
            *o = g[colS];
            factorC = (factorC & 0xFFFFFF00) | (((lv.var_60 >> 16) + factorA_carry + factorC) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md22(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_12667d:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_1266d7\n \
            or     %%cx,%%cx\n \
            jle    jump_1269c7\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_1266d2\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_1266d2:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_1266ff\n \
        jump_1266d7:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_1266e5\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_1266e5:\n \
            sub    %%ax,%%cx\n \
            jle    jump_1269c7\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_1266ff:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_126709:\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_1269a6\n \
            mov    (%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_1269a6:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            add    $0x1,%%edi\n \
            jmp    jump_126709\n \
        jump_1269c3:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_1269c7:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_12667d\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *g;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa;
        ushort colM;
        short pYa;
        ubyte *o;
        long pXm;
        long factorA;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            ushort colL, colH;
            ulong factorB, factorC;

            if (pYa <= 0)
                continue;
            pXm = (ushort)-pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = ((factorC >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = factorC & 0xFFFF;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            if (m[colM]) {
                colS = ((m[colM] & 0xFF) << 8) + *o;
                *o = g[colS];
            }
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md23(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_rend_md23_Af0:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_rend_md23_B4a\n \
            or     %%cx,%%cx\n \
            jle    jump_rend_md23_E3a\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md23_B45\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md23_B45:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_rend_md23_B72\n \
        jump_rend_md23_B4a:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md23_B58\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md23_B58:\n \
            sub    %%ax,%%cx\n \
            jle    jump_rend_md23_E3a\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_rend_md23_B72:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_rend_md23_B7c:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_E19\n \
            mov    (%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_rend_md23_E19:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            add    $0x1,%%edi\n \
            jmp    jump_rend_md23_B7c\n \
        jump_rend_md23_E36:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_rend_md23_E3a:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_rend_md23_Af0\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *g;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa;
        ushort colM;
        short pYa;
        ubyte *o;
        long pXm;
        long factorA;
        ubyte factorA_carry;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            ulong factorB, factorC;

            if (pYa <= 0)
                continue;
            pXm = (ushort)-pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = ((factorC >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorC;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if (((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;

            if (m[colM]) {
                colS = (((*o) & 0xFF) << 8) + m[colM];
                *o = g[colS];
            }
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md24(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            mov    0x3c+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x50+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_rend_md24_A6e:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_rend_md24_Ad9\n \
            or     %%cx,%%cx\n \
            jle    jump_rend_md24_Df3\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md24_A9e\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md24_A9e:\n \
            mov    %%ecx,0x58+%[lv]\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            mov    %%eax,%%ecx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            imul   0x3c+%[lv],%%ecx\n \
            add    0x10(%%esi),%%ecx\n \
            rol    $0x10,%%ecx\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_rend_md24_B0b\n \
        jump_rend_md24_Ad9:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md24_Ae7\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md24_Ae7:\n \
            sub    %%ax,%%cx\n \
            jle    jump_rend_md24_Df3\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
            mov    %%ecx,0x58+%[lv]\n \
            mov    0x10(%%esi),%%ecx\n \
            rol    $0x10,%%ecx\n \
        jump_rend_md24_B0b:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_rend_md24_B15:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_Dc8\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    (%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_rend_md24_Dc8:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            add    $0x1,%%edi\n \
            jmp    jump_rend_md24_B15\n \
        jump_rend_md24_Def:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_rend_md24_Df3:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_rend_md24_A6e\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *g;
    ubyte *f;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    lv.var_74 = lv.var_60 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa;
        ushort colM;
        short pYa;
        ubyte *o;
        long pXMa;
        long pXMb;
        long factorA;
        long factorC;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            ushort colL, colH;
            ulong factorB;

            if (pYa <= 0)
                continue;
            if (pYa > vec_window_width)
                pYa = vec_window_width;
            lv.var_7C = pYa;
            pXMa = (ushort)-pXa;
            pXMb = pXMa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXMa, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXMa;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            pXa = factorB >> 8;
            colL = ((pXa >> 8) & 0xFF);
            factorC = __ROL4__(pp->S + lv.var_60 * pXMb, 16);
            pXa = (ushort)pXa;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if (((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);
            lv.var_7C = pYa;
            factorC = __ROL4__(pp->S, 16);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;
        f = pixmap.fade_table;

        for (; lv.var_7C > 0; lv.var_7C--, o++)
        {
            ushort colL, colH;
            ubyte factorA_carry;

            if (m[colM]) {
                ushort colS;

                colS = ((factorC & 0xFF) << 8) + m[colM];
                colS = (f[colS] << 8) + *o;
                *o = g[colS];
            }
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;
            factorA_carry = __CFADDL__(lv.var_74, factorC);
            factorC += lv.var_74;
            factorC = (factorC & 0xFFFFFF00) + (((lv.var_60 >> 16) + factorA_carry + factorC) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md25(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            mov    0x3c+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x50+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_rend_md25_227:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_rend_md25_292\n \
            or     %%cx,%%cx\n \
            jle    jump_rend_md25_6ac\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md25_257\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md25_257:\n \
            mov    %%ecx,0x58+%[lv]\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            mov    %%eax,%%edx\n \
            mov    %%eax,%%ecx\n \
            imul   0x30+%[lv],%%edx\n \
            add    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            imul   0x24+%[lv],%%eax\n \
            add    0x8(%%esi),%%eax\n \
            mov    %%ax,%%dx\n \
            shr    $0x8,%%eax\n \
            mov    %%ah,%%bl\n \
            imul   0x3c+%[lv],%%ecx\n \
            add    0x10(%%esi),%%ecx\n \
            rol    $0x10,%%ecx\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_rend_md25_2c4\n \
        jump_rend_md25_292:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_rend_md25_2a0\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_rend_md25_2a0:\n \
            sub    %%ax,%%cx\n \
            jle    jump_rend_md25_6ac\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
            mov    %%ecx,0x58+%[lv]\n \
            mov    0x10(%%esi),%%ecx\n \
            rol    $0x10,%%ecx\n \
        jump_rend_md25_2c4:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_rend_md25_2ce:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_681\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    (%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_rend_md25_681:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            add    $0x1,%%edi\n \
            jmp    jump_rend_md25_2ce\n \
        jump_rend_md25_6a8:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_rend_md25_6ac:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_rend_md25_227\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *g;
    ubyte *f;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    lv.var_74 = lv.var_60 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa;
        ushort colM;
        short pYa;
        ubyte *o;
        long pXMa;
        long pXMb;
        long factorA;
        long factorC;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            ushort colL, colH;
            ulong factorB;

            if (pYa <= 0)
                continue;
            if (pYa > vec_window_width)
                pYa = vec_window_width;
            lv.var_7C = pYa;
            pXMa = (ushort)-pXa;
            pXMb = pXMa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXMa, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXMa;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            pXa = factorB >> 8;
            colL = ((pXa >> 8) & 0xFF);
            factorC = __ROL4__(pp->S + lv.var_60 * pXMb, 16);
            pXa = (ushort)pXa;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if (((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);
            lv.var_7C = pYa;
            factorC = __ROL4__(pp->S, 16);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;
        f = pixmap.fade_table;

        for (; lv.var_7C > 0; lv.var_7C--, o++)
        {
            ushort colL, colH;
            ubyte factorA_carry;

            if (m[colM]) {
                ushort colS;

                colS = ((factorC & 0xFF) << 8) + m[colM];
                colS = (((*o) & 0xFF) << 8) + f[colS];
                *o = g[colS];
            }
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;
            factorA_carry = __CFADDL__(lv.var_74, factorC);
            factorC += lv.var_74;
            factorC = (factorC & 0xFFFFFF00) | (((lv.var_60 >> 16) + factorA_carry + factorC) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md26(struct TrigLocals *lvu)
{
#if USE_ASM_TRIG_DIVIDED
        asm volatile (" \
            pushal\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    %%esi,0x5c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            mov    0x24+%[lv],%%ecx\n \
            mov    0x30+%[lv],%%edx\n \
            mov    0x3c+%[lv],%%ebp\n \
            rol    $0x10,%%ecx\n \
            rol    $0x10,%%edx\n \
            shr    $0x8,%%ebp\n \
            mov    %%dl,%%bl\n \
            mov    %%cl,%%dl\n \
            mov    %%bp,%%cx\n \
            xor    %%dh,%%dh\n \
            mov    %%ecx,0x4c+%[lv]\n \
            mov    %%edx,0x50+%[lv]\n \
            mov    %%bl,0x64+%[lv]\n \
        jump_t12_Af4:\n \
            mov    0x5c+%[lv],%%esi\n \
            addl   $0x14,0x5c+%[lv]\n \
            mov    (%%esi),%%eax\n \
            mov    0x4(%%esi),%%ebp\n \
            sar    $0x10,%%eax\n \
            sar    $0x10,%%ebp\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%eax,%%eax\n \
            jns    jump_t12_B62\n \
            or     %%ebp,%%ebp\n \
            jle    jump_t12_E4b\n \
            neg    %%eax\n \
            mov    0x24+%[lv],%%ecx\n \
            imul   %%eax,%%ecx\n \
            add    0x8(%%esi),%%ecx\n \
            mov    0x30+%[lv],%%edx\n \
            imul   %%eax,%%edx\n \
            add    0xc(%%esi),%%edx\n \
            mov    0x3c+%[lv],%%ebx\n \
            imul   %%eax,%%ebx\n \
            add    0x10(%%esi),%%ebx\n \
            rol    $0x10,%%ecx\n \
            rol    $0x10,%%edx\n \
            shr    $0x8,%%ebx\n \
            mov    %%dl,%%al\n \
            mov    %%cl,%%dl\n \
            mov    %%bx,%%cx\n \
            mov    %%al,%%bh\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ebp\n \
            jle    jump_t12_B60\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ebp\n \
        jump_t12_B60:\n \
            jmp    jump_t12_B95\n \
        jump_t12_B62:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ebp\n \
            jle    jump_t12_B70\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ebp\n \
        jump_t12_B70:\n \
            sub    %%eax,%%ebp\n \
            jle    jump_t12_E4b\n \
            add    %%eax,%%edi\n \
            mov    0x8(%%esi),%%ecx\n \
            mov    0xc(%%esi),%%edx\n \
            mov    0x10(%%esi),%%ebx\n \
            rol    $0x10,%%ecx\n \
            rol    $0x10,%%edx\n \
            shr    $0x8,%%ebx\n \
            mov    %%dl,%%al\n \
            mov    %%cl,%%dl\n \
            mov    %%bx,%%cx\n \
            mov    %%al,%%bh\n \
        jump_t12_B95:\n \
            xor    %%dh,%%dh\n \
            and    $0xffff,%%ebx\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
\n \
t12_md01:\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_H16\n \
        jump_t12_E36:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
\n \
            add    $0x1,%%edi\n \
            sub    $0x1,%%ebp\n \
            jg     t12_md01\n \
        jump_t12_E4b:\n \
            decl   0x20+%[lv]\n \
            jne    jump_t12_Af4\n \
            jmp    jump_t12_end\n \
        jump_t12_E5a:\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_E36\n \
        jump_t12_H16:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    (%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
            add    $0x1,%%edi\n \
            sub    $0x1,%%ebp\n \
            jg     jump_t12_E5a\n \
            decl   0x20+%[lv]\n \
            jne    jump_t12_Af4\n \
        jump_t12_end:\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
#else
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *g;
    ubyte *f;

    pp = polyscans;
    lv.var_80 = pp;
    {
        ulong v1;
        ulong v2;
        ubyte v3;

        v1 = __ROL4__(lv.var_48, 16);
        v2 = __ROL4__(lv.var_54, 16);
        v3 = v2;
        v2 = (v2 & 0xFFFFFF00) + (v1 & 0xFF);
        v1 = (v1 & 0xFFFF0000) + (((ulong)lv.var_60 >> 8) & 0xFFFF);
        v2 = (v2 & 0xFFFF0000) + (v2 & 0xFF);
        lv.var_70 = v1;
        lv.var_74 = v2;
        lv.var_88 = v3;
    }
    for (; lv.var_44; lv.var_44--)
    {
        long pXa;
        long pYa;
        ubyte *o;
        ulong factorB, factorD;
        long factorA;
        ulong factorC;
        ubyte pY_overflow;
        ushort colM;

        pp = lv.var_80;
        lv.var_80 += 20;
        pXa = pp->X >> 16;
        pYa = pp->Y >> 16;
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            if (pYa <= 0)
                continue;
            pXa = -pXa;
            factorA = __ROL4__(pp->U + pXa * lv.var_48, 16);
            factorB = __ROL4__(pp->V + pXa * lv.var_54, 16);
            factorC = (ulong)(pp->S + pXa * lv.var_60) >> 8;
            factorB = (factorB & 0xFFFFFF00) | (factorA & 0xFF);
            factorA = (factorA & 0xFFFF0000) | (factorC & 0xFFFF);
            factorD = __ROL4__(pp->V + pXa * lv.var_54, 16);
            if (pYa > vec_window_width)
                pYa = vec_window_width;

            colM = (factorC & 0xFF) + ((factorD & 0xFF) << 8);
        }
        else
        {
            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa -= pXa;
            if (((pYa < 0) ^ pY_overflow) | (pYa == 0))
                continue;
            o += pXa;
            factorA = __ROL4__(pp->U, 16);
            factorB = __ROL4__(pp->V, 16);
            factorC = (ulong)pp->S >> 8;
            factorB = (factorB & 0xFFFFFF00) | (factorA & 0xFF);
            factorA = (factorA & 0xFFFF0000) | (factorC & 0xFFFF);
            factorD = __ROL4__(pp->V, 16);

            colM = (factorC & 0xFF) + ((factorD & 0xFF) << 8);
        }

        factorB = (factorB & 0xFFFF00FF);
        m = vec_map;
        g = pixmap.ghost_table;
        f = pixmap.fade_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colS;
            ubyte factorA_carry, factorB_carry;

            colS = (factorA & 0xFF00) | (factorB & 0xFF);
            //colS = (factorA  & 0xFF00) | m[colM];  //TODO fix/enable

            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA = lv.var_70 + factorA;
            factorB_carry = __CFADDS__(lv.var_74, factorB + factorA_carry);
            factorB = lv.var_74 + factorB + factorA_carry;
            colM = (colM & 0xFF) + ((((colM >> 8) + lv.var_88 + factorA_carry) << 8) & 0xFF00);

            if ((colS & 0xFF) <= 0xCu) {
                colS = ((*o) << 8) | f[colS];
                *o = g[colS];
            } else {
                *o = f[colS];
            }
        }
    }
#endif
}
/******************************************************************************/
