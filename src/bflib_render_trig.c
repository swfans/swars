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
            mov    %%ah,0x1(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            mov    %%ah,0x2(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            mov    %%ah,0x3(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            mov    %%ah,0x4(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            mov    %%ah,0x5(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            mov    %%ah,0x6(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            mov    %%ah,0x7(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            mov    %%ah,0x8(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            mov    %%ah,0x9(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            mov    %%ah,0xa(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            mov    %%ah,0xb(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            mov    %%ah,0xc(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            mov    %%ah,0xd(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            mov    %%ah,0xe(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            mov    %%ah,0xf(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_rend_md01_c41\n \
            add    $0x10,%%edi\n \
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
    short pX, pY;
    short pS;
    TbBool pS_carry;
    struct PolyPoint *pp;
    ubyte *o;
    ushort colS;

    pp = polyscans;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pX = pp->X >> 16;
        pY = pp->Y >> 16;
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pX  < 0)
        {
            long mX;
            short colH, colL;

            if (pY <= 0)
                continue;
            mX = lv.var_60 * (ushort)(-pX);
            pS_carry = __CFADDS__(pp->S, mX);
            pS = pp->S + mX;
            colL = (mX >> 8);
            colH = (pp->S >> 16) + pS_carry;
            colS = (colH << 8) + colL;
            if (pY > vec_window_width)
                pY = vec_window_width;
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
            colH = (pp->S >> 16) & 0xFF;
            colS = (colH << 8) + vec_colour;
            pS = pp->S;
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
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x1(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x2(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x3(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x4(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x5(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x6(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x7(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x8(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x9(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0xa(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0xb(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0xc(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0xd(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0xe(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0xf(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md02_ed9\n \
            add    $0x10,%%edi\n \
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
        short pU;

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
            colH = __ROL4__(pp->V + mX, 16);
            mX = lv.var_48 * (-pX);
            pU = pp->U + mX;
            colL = pU >> 16;
            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
            if (pY > vec_window_width)
                pY = vec_window_width;
            pX = pU >> 8;
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
            colH = __ROL4__(pp->V, 16);
            pU = pp->U;
            colL = pU >> 16;
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
            pU_carry = __CFADDS__(lv.var_70, pU);
            pU = lv.var_70 + pU;
            colH = (lv.var_54 >> 16) + pU_carry + (colS >> 8);

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
#endif
}

void trig_render_md03(struct TrigLocals *lvu)
{
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
            je     jump_rend_md03_f9b\n \
            mov    %%al,(%%edi)\n \
        jump_rend_md03_f9b:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_fbe\n \
            mov    %%al,0x1(%%edi)\n \
        jump_rend_md03_fbe:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_fe1\n \
            mov    %%al,0x2(%%edi)\n \
        jump_rend_md03_fe1:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_004\n \
            mov    %%al,0x3(%%edi)\n \
        jump_rend_md03_004:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_027\n \
            mov    %%al,0x4(%%edi)\n \
        jump_rend_md03_027:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_04a\n \
            mov    %%al,0x5(%%edi)\n \
        jump_rend_md03_04a:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_06d\n \
            mov    %%al,0x6(%%edi)\n \
        jump_rend_md03_06d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_090\n \
            mov    %%al,0x7(%%edi)\n \
        jump_rend_md03_090:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_0b3\n \
            mov    %%al,0x8(%%edi)\n \
        jump_rend_md03_0b3:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_0d6\n \
            mov    %%al,0x9(%%edi)\n \
        jump_rend_md03_0d6:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_0f9\n \
            mov    %%al,0xa(%%edi)\n \
        jump_rend_md03_0f9:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_11c\n \
            mov    %%al,0xb(%%edi)\n \
        jump_rend_md03_11c:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_13f\n \
            mov    %%al,0xc(%%edi)\n \
        jump_rend_md03_13f:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_15e\n \
            mov    %%al,0xd(%%edi)\n \
        jump_rend_md03_15e:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_17d\n \
            mov    %%al,0xe(%%edi)\n \
        jump_rend_md03_17d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md03_19c\n \
            mov    %%al,0xf(%%edi)\n \
        jump_rend_md03_19c:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md03_1b9\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md04(struct TrigLocals *lvu)
{
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
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x1(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x2(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x3(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x4(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x5(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x6(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x7(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x8(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x9(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0xa(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0xb(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0xc(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0xd(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0xe(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0xf(%%edi)\n \
            je     jump_rend_md04_3e1\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md05(struct TrigLocals *lvu)
{
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
            mov    %%ebp,%%eax\n \
            and    $0xf,%%eax\n \
            add    "EXPORT_SYMBOL(add_to_edi)"(,%%eax,4),%%edi\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
            jmp    *gt_jt(,%%eax,4)\n \
\n \
gt_jt: /* swars-final @ 0x123530 */\n \
            .long   gt_md00\n \
            .long   gt_md01\n \
            .long   gt_md02\n \
            .long   gt_md03\n \
            .long   gt_md04\n \
            .long   gt_md05\n \
            .long   gt_md06\n \
            .long   gt_md07\n \
            .long   gt_md08\n \
            .long   gt_md09\n \
            .long   gt_md10\n \
            .long   gt_md11\n \
            .long   gt_md12\n \
            .long   gt_md13\n \
            .long   gt_md14\n \
            .long   gt_md15\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md00:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md15:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0x1(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md14:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0x2(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md13:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0x3(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md12:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0x4(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md11:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0x5(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md10:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0x6(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md09:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0x7(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md08:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0x8(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md07:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0x9(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md06:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0xa(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md05:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0xb(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md04:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0xc(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md03:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0xd(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md02:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0xe(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
gt_md01:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            adc    0x64+%[lv],%%bh\n \
            mov    %%al,0xf(%%edi)\n \
            add    $0x10,%%edi\n \
            sub    $0x10,%%ebp\n \
            jg     gt_md00\n \
        jump_rend_gt_73b:\n \
            decl   0x20+%[lv]\n \
            jne    jump_rend_md05_46d\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
}

void trig_render_md06(struct TrigLocals *lvu)
{
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
            add    "EXPORT_SYMBOL(add_to_edi)"(,%%esi,4),%%edi\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%ebp\n \
            jmp    *mgt_jt(,%%esi,4)\n \
\n \
mgt_jt: /* swars-final @ 0x12383a, genewars-beta @ 0xF156A */\n \
            .long   mgt_md00\n \
            .long   mgt_md01\n \
            .long   mgt_md02\n \
            .long   mgt_md03\n \
            .long   mgt_md04\n \
            .long   mgt_md05\n \
            .long   mgt_md06\n \
            .long   mgt_md07\n \
            .long   mgt_md08\n \
            .long   mgt_md09\n \
            .long   mgt_md10\n \
            .long   mgt_md11\n \
            .long   mgt_md12\n \
            .long   mgt_md13\n \
            .long   mgt_md14\n \
            .long   mgt_md15\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md00:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12388f\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_12388f:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md15:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1238b8\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x1(%%edi)\n \
        jump_1238b8:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md14:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1238e1\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x2(%%edi)\n \
        jump_1238e1:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md13:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12390a\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x3(%%edi)\n \
        jump_12390a:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md12:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_123933\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x4(%%edi)\n \
        jump_123933:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md11:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12395c\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x5(%%edi)\n \
        jump_12395c:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md10:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_123985\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x6(%%edi)\n \
        jump_123985:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md09:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1239ae\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x7(%%edi)\n \
        jump_1239ae:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md08:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1239d7\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x8(%%edi)\n \
        jump_1239d7:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md07:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_123a00\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x9(%%edi)\n \
        jump_123a00:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md06:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_123a29\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xa(%%edi)\n \
        jump_123a29:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md05:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_123a52\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xb(%%edi)\n \
        jump_123a52:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md04:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_123a7b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xc(%%edi)\n \
        jump_123a7b:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md03:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_123aa4\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xd(%%edi)\n \
        jump_123aa4:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md02:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_123acd\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xe(%%edi)\n \
        jump_123acd:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
mgt_md01:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%ebx,%%ebp,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_123af6\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
        jump_123af6:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%ah\n \
            add    $0x10,%%edi\n \
            sub    $0x10,%%cx\n \
            jg     mgt_md00\n \
        jump_123b1c:\n \
            decl   0x20+%[lv]\n \
            jne    jump_123770\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
}

void trig_render_md07(struct TrigLocals *lvu)
{
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
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x1(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x2(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x3(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x4(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x5(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x6(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x7(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x8(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x9(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xa(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xb(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xc(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xd(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xe(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xf(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_123e28\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md08(struct TrigLocals *lvu)
{
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
            je     jump_123eff\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_123eff:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_123f28\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x1(%%edi)\n \
        jump_123f28:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_123f51\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x2(%%edi)\n \
        jump_123f51:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_123f7a\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x3(%%edi)\n \
        jump_123f7a:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_123fa3\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x4(%%edi)\n \
        jump_123fa3:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_123fcc\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x5(%%edi)\n \
        jump_123fcc:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_123ff5\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x6(%%edi)\n \
        jump_123ff5:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_12401e\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x7(%%edi)\n \
        jump_12401e:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_124047\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x8(%%edi)\n \
        jump_124047:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_124070\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x9(%%edi)\n \
        jump_124070:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_124099\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xa(%%edi)\n \
        jump_124099:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_1240c2\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xb(%%edi)\n \
        jump_1240c2:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_1240eb\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xc(%%edi)\n \
        jump_1240eb:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_124110\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xd(%%edi)\n \
        jump_124110:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_124135\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xe(%%edi)\n \
        jump_124135:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%al,%%al\n \
            je     jump_12415a\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
        jump_12415a:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12416e\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md09(struct TrigLocals *lvu)
{
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
            je     jump_124241\n \
            mov    (%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_124241:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_12426d\n \
            mov    0x1(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x1(%%edi)\n \
        jump_12426d:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_124299\n \
            mov    0x2(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x2(%%edi)\n \
        jump_124299:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_1242c5\n \
            mov    0x3(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x3(%%edi)\n \
        jump_1242c5:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_1242f1\n \
            mov    0x4(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x4(%%edi)\n \
        jump_1242f1:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_12431d\n \
            mov    0x5(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x5(%%edi)\n \
        jump_12431d:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_124349\n \
            mov    0x6(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x6(%%edi)\n \
        jump_124349:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_124375\n \
            mov    0x7(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x7(%%edi)\n \
        jump_124375:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_1243a1\n \
            mov    0x8(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x8(%%edi)\n \
        jump_1243a1:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_1243cd\n \
            mov    0x9(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x9(%%edi)\n \
        jump_1243cd:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_1243f9\n \
            mov    0xa(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xa(%%edi)\n \
        jump_1243f9:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_124425\n \
            mov    0xb(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xb(%%edi)\n \
        jump_124425:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_124451\n \
            mov    0xc(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xc(%%edi)\n \
        jump_124451:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_12447d\n \
            mov    0xd(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xd(%%edi)\n \
        jump_12447d:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_1244a5\n \
            mov    0xe(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xe(%%edi)\n \
        jump_1244a5:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            or     %%ah,%%ah\n \
            je     jump_1244cd\n \
            mov    0xf(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
        jump_1244cd:\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1244e1\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md10(struct TrigLocals *lvu)
{
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
            je     jump_1245b1\n \
            mov    (%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_1245b1:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1245dd\n \
            mov    0x1(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x1(%%edi)\n \
        jump_1245dd:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_124609\n \
            mov    0x2(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x2(%%edi)\n \
        jump_124609:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_124635\n \
            mov    0x3(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x3(%%edi)\n \
        jump_124635:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_124661\n \
            mov    0x4(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x4(%%edi)\n \
        jump_124661:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12468d\n \
            mov    0x5(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x5(%%edi)\n \
        jump_12468d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1246b9\n \
            mov    0x6(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x6(%%edi)\n \
        jump_1246b9:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1246e5\n \
            mov    0x7(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x7(%%edi)\n \
        jump_1246e5:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_124711\n \
            mov    0x8(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x8(%%edi)\n \
        jump_124711:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12473d\n \
            mov    0x9(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x9(%%edi)\n \
        jump_12473d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_124769\n \
            mov    0xa(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xa(%%edi)\n \
        jump_124769:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_124795\n \
            mov    0xb(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xb(%%edi)\n \
        jump_124795:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1247c1\n \
            mov    0xc(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xc(%%edi)\n \
        jump_1247c1:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1247ed\n \
            mov    0xd(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xd(%%edi)\n \
        jump_1247ed:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_124815\n \
            mov    0xe(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xe(%%edi)\n \
        jump_124815:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12483d\n \
            mov    0xf(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
        jump_12483d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_12485a\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md12(struct TrigLocals *lvu)
{
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
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0x1(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0x2(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0x3(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0x4(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0x5(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0x6(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0x7(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0x8(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0x9(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0xa(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0xb(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0xc(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0xd(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0xe(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%ah,0xf(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124b5f\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md13(struct TrigLocals *lvu)
{
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
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x1(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x2(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x3(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x4(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x5(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x6(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x7(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x8(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x9(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xa(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xb(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xc(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xd(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xe(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xf(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_124e65\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md14(struct TrigLocals *lvu)
{
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
            mov    0x1(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x1(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            mov    0x2(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x2(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            mov    0x3(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x3(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            mov    0x4(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x4(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            mov    0x5(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x5(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            mov    0x6(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x6(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            mov    0x7(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x7(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            mov    0x8(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x8(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            mov    0x9(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x9(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            mov    0xa(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xa(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            mov    0xb(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xb(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            mov    0xc(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xc(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            mov    0xd(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xd(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            mov    0xe(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xe(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            mov    0xf(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
            dec    %%cx\n \
            je     jump_124ffe\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md15(struct TrigLocals *lvu)
{
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
            mov    0x1(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0x1(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            mov    0x2(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0x2(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            mov    0x3(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0x3(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            mov    0x4(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0x4(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            mov    0x5(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0x5(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            mov    0x6(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0x6(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            mov    0x7(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0x7(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            mov    0x8(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0x8(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            mov    0x9(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0x9(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            mov    0xa(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0xa(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            mov    0xb(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0xb(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            mov    0xc(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0xc(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            mov    0xd(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0xd(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            mov    0xe(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0xe(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            mov    0xf(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%ah\n \
            mov    %%ah,0xf(%%edi)\n \
            dec    %%cx\n \
            je     jump_125192\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md16(struct TrigLocals *lvu)
{
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
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0x1(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x1(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0x2(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x2(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0x3(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x3(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0x4(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x4(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0x5(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x5(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0x6(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x6(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0x7(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x7(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0x8(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x8(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0x9(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x9(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0xa(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0xa(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0xb(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0xb(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0xc(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0xc(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0xd(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0xd(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0xe(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0xe(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dh\n \
            mov    0xf(%%edi),%%dl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0xf(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_12544b\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md17(struct TrigLocals *lvu)
{
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
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0x1(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x1(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0x2(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x2(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0x3(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x3(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0x4(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x4(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0x5(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x5(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0x6(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x6(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0x7(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x7(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0x8(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x8(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0x9(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0x9(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0xa(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0xa(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0xb(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0xb(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0xc(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0xc(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0xd(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0xd(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0xe(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0xe(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            mov    0xf(%%edi),%%dh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%edx),%%dl\n \
            mov    %%dl,0xf(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_125704\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md18(struct TrigLocals *lvu)
{
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
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x1(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x1(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x2(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x2(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x3(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x3(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x4(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x4(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x5(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x5(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x6(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x6(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x7(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x7(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x8(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x8(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x9(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x9(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0xa(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xa(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0xb(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xb(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0xc(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xc(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0xd(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xd(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0xe(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xe(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            add    0x24+%[lv],%%dx\n \
            mov    0xf(%%edi),%%al\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xf(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125a2f\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md19(struct TrigLocals *lvu)
{
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
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x1(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x1(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x2(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x2(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x3(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x3(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x4(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x4(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x5(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x5(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x6(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x6(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x7(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x7(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x8(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x8(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0x9(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0x9(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0xa(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xa(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0xb(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xb(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0xc(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xc(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0xd(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xd(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0xe(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xe(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    0xf(%%edi),%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    %%al,0xf(%%edi)\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_125d5e\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md20(struct TrigLocals *lvu)
{
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
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x1(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x1(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x2(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x2(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x3(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x3(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x4(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x4(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x5(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x5(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x6(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x6(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x7(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x7(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x8(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x8(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x9(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x9(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0xa(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0xa(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0xb(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0xb(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0xc(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0xc(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0xd(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0xd(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0xe(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0xe(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0xf(%%edi),%%al\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0xf(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1261d7\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md21(struct TrigLocals *lvu)
{
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
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x1(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x1(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x2(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x2(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x3(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x3(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x4(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x4(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x5(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x5(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x6(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x6(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x7(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x7(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x8(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x8(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0x9(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0x9(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0xa(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0xa(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0xb(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0xb(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0xc(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0xc(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0xd(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0xd(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0xe(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0xe(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            mov    %%cl,%%ah\n \
            adc    0x26+%[lv],%%bl\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            add    0x4c+%[lv],%%edx\n \
            mov    0xf(%%edi),%%ah\n \
            adc    0x32+%[lv],%%bh\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            add    0x50+%[lv],%%ecx\n \
            mov    %%al,0xf(%%edi)\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_126650\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md22(struct TrigLocals *lvu)
{
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
            je     jump_12671a\n \
            mov    (%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_12671a:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_126746\n \
            mov    0x1(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x1(%%edi)\n \
        jump_126746:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_126772\n \
            mov    0x2(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x2(%%edi)\n \
        jump_126772:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_12679e\n \
            mov    0x3(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x3(%%edi)\n \
        jump_12679e:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_1267ca\n \
            mov    0x4(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x4(%%edi)\n \
        jump_1267ca:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_1267f6\n \
            mov    0x5(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x5(%%edi)\n \
        jump_1267f6:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_126822\n \
            mov    0x6(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x6(%%edi)\n \
        jump_126822:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_12684e\n \
            mov    0x7(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x7(%%edi)\n \
        jump_12684e:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_12687a\n \
            mov    0x8(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x8(%%edi)\n \
        jump_12687a:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_1268a6\n \
            mov    0x9(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x9(%%edi)\n \
        jump_1268a6:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_1268d2\n \
            mov    0xa(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xa(%%edi)\n \
        jump_1268d2:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_1268fe\n \
            mov    0xb(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xb(%%edi)\n \
        jump_1268fe:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_12692a\n \
            mov    0xc(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xc(%%edi)\n \
        jump_12692a:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_126956\n \
            mov    0xd(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xd(%%edi)\n \
        jump_126956:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_12697e\n \
            mov    0xe(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xe(%%edi)\n \
        jump_12697e:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            mov    (%%ebx,%%esi,1),%%ah\n \
            or     %%ah,%%ah\n \
            je     jump_1269a6\n \
            mov    0xf(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
        jump_1269a6:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1269c3\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md23(struct TrigLocals *lvu)
{
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
            je     jump_rend_md23_B8d\n \
            mov    (%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_rend_md23_B8d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_Bb9\n \
            mov    0x1(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x1(%%edi)\n \
        jump_rend_md23_Bb9:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_Be5\n \
            mov    0x2(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x2(%%edi)\n \
        jump_rend_md23_Be5:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_C11\n \
            mov    0x3(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x3(%%edi)\n \
        jump_rend_md23_C11:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_C3d\n \
            mov    0x4(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x4(%%edi)\n \
        jump_rend_md23_C3d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_C69\n \
            mov    0x5(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x5(%%edi)\n \
        jump_rend_md23_C69:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_C95\n \
            mov    0x6(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x6(%%edi)\n \
        jump_rend_md23_C95:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_Cc1\n \
            mov    0x7(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x7(%%edi)\n \
        jump_rend_md23_Cc1:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_Ced\n \
            mov    0x8(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x8(%%edi)\n \
        jump_rend_md23_Ced:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_D19\n \
            mov    0x9(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x9(%%edi)\n \
        jump_rend_md23_D19:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_D45\n \
            mov    0xa(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xa(%%edi)\n \
        jump_rend_md23_D45:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_D71\n \
            mov    0xb(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xb(%%edi)\n \
        jump_rend_md23_D71:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_D9d\n \
            mov    0xc(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xc(%%edi)\n \
        jump_rend_md23_D9d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_Dc9\n \
            mov    0xd(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xd(%%edi)\n \
        jump_rend_md23_Dc9:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_Df1\n \
            mov    0xe(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xe(%%edi)\n \
        jump_rend_md23_Df1:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md23_E19\n \
            mov    0xf(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
        jump_rend_md23_E19:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_rend_md23_E36\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md24(struct TrigLocals *lvu)
{
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
            je     jump_rend_md24_B2e\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    (%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_rend_md24_B2e:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_B6c\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x1(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x1(%%edi)\n \
        jump_rend_md24_B6c:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_Baa\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x2(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x2(%%edi)\n \
        jump_rend_md24_Baa:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_Be8\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x3(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x3(%%edi)\n \
        jump_rend_md24_Be8:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_f26\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x4(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x4(%%edi)\n \
        jump_rend_md24_f26:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_f64\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x5(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x5(%%edi)\n \
        jump_rend_md24_f64:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_fa2\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x6(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x6(%%edi)\n \
        jump_rend_md24_fa2:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_fe0\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x7(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x7(%%edi)\n \
        jump_rend_md24_fe0:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_C1e\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x8(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x8(%%edi)\n \
        jump_rend_md24_C1e:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_C5c\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x9(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x9(%%edi)\n \
        jump_rend_md24_C5c:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_C9a\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0xa(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xa(%%edi)\n \
        jump_rend_md24_C9a:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_Cd8\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0xb(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xb(%%edi)\n \
        jump_rend_md24_Cd8:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_D16\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0xc(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xc(%%edi)\n \
        jump_rend_md24_D16:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_D54\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0xd(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xd(%%edi)\n \
        jump_rend_md24_D54:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_D8e\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0xe(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xe(%%edi)\n \
        jump_rend_md24_D8e:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md24_Dc8\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0xf(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
        jump_rend_md24_Dc8:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md24_Def\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md25(struct TrigLocals *lvu)
{
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
            je     jump_rend_md25_2e7\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    (%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_rend_md25_2e7:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_325\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x1(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x1(%%edi)\n \
        jump_rend_md25_325:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_363\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x2(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x2(%%edi)\n \
        jump_rend_md25_363:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_3a1\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x3(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x3(%%edi)\n \
        jump_rend_md25_3a1:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_3df\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x4(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x4(%%edi)\n \
        jump_rend_md25_3df:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_41d\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x5(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x5(%%edi)\n \
        jump_rend_md25_41d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_45b\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x6(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x6(%%edi)\n \
        jump_rend_md25_45b:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_499\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x7(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x7(%%edi)\n \
        jump_rend_md25_499:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_4d7\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x8(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x8(%%edi)\n \
        jump_rend_md25_4d7:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_515\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x9(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x9(%%edi)\n \
        jump_rend_md25_515:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_553\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xa(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xa(%%edi)\n \
        jump_rend_md25_553:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_591\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xb(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xb(%%edi)\n \
        jump_rend_md25_591:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_5cf\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xc(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xc(%%edi)\n \
        jump_rend_md25_5cf:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_60d\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xd(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xd(%%edi)\n \
        jump_rend_md25_60d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_647\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xe(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xe(%%edi)\n \
        jump_rend_md25_647:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_rend_md25_681\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xf(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
        jump_rend_md25_681:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_rend_md25_6a8\n \
            add    $0x10,%%edi\n \
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
}

void trig_render_md26(struct TrigLocals *lvu)
{
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
            mov    %%ebp,%%eax\n \
            and    $0xf,%%eax\n \
            add    "EXPORT_SYMBOL(add_to_edi)"(,%%eax,4),%%edi\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
            jmp    *t12_jt(,%%eax,4)\n \
\n \
t12_jt: /* swars-final @ 0x1277c0, genewars-beta @ 0xF54F0 */\n \
            .long   t12_md00\n \
            .long   t12_md01\n \
            .long   t12_md02\n \
            .long   t12_md03\n \
            .long   t12_md04\n \
            .long   t12_md05\n \
            .long   t12_md06\n \
            .long   t12_md07\n \
            .long   t12_md08\n \
            .long   t12_md09\n \
            .long   t12_md10\n \
            .long   t12_md11\n \
            .long   t12_md12\n \
            .long   t12_md13\n \
            .long   t12_md14\n \
            .long   t12_md15\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md00:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_E75\n \
        jump_t12_C1b:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md15:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_Ea0\n \
        jump_t12_C3e:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x1(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md14:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_Ecd\n \
        jump_t12_C62:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x2(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md13:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_Efa\n \
        jump_t12_C86:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x3(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md12:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_F27\n \
        jump_t12_Caa:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x4(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md11:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_F54\n \
        jump_t12_Cce:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x5(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md10:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_F81\n \
        jump_t12_Cf2:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x6(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md09:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_Fae\n \
        jump_t12_D16:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x7(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md08:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_Fdb\n \
        jump_t12_D3a:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x8(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md07:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_G08\n \
        jump_t12_D5e:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0x9(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md06:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_G35\n \
        jump_t12_D82:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xa(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md05:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_G62\n \
        jump_t12_Da6:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xb(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md04:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_G8f\n \
        jump_t12_Dca:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xc(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md03:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_Gbc\n \
        jump_t12_Dee:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xd(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md02:\n \
/*----------------------------------------------------------------*/\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            jbe    jump_t12_Ge9\n \
        jump_t12_E12:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xe(%%edi)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
t12_md01:\n \
/*----------------------------------------------------------------*/\n \
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
            mov    %%al,0xf(%%edi)\n \
            add    $0x10,%%edi\n \
            sub    $0x10,%%ebp\n \
            jg     t12_md00\n \
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
            ja     jump_t12_C1b\n \
        jump_t12_E75:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    (%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_C3e\n \
        jump_t12_Ea0:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x1(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x1(%%edi)\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_C62\n \
        jump_t12_Ecd:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x2(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x2(%%edi)\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_C86\n \
        jump_t12_Efa:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x3(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x3(%%edi)\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_Caa\n \
        jump_t12_F27:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x4(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x4(%%edi)\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_Cce\n \
        jump_t12_F54:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x5(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x5(%%edi)\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_Cf2\n \
        jump_t12_F81:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x6(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x6(%%edi)\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_D16\n \
        jump_t12_Fae:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x7(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x7(%%edi)\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_D3a\n \
        jump_t12_Fdb:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x8(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x8(%%edi)\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_D5e\n \
        jump_t12_G08:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x9(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x9(%%edi)\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_D82\n \
        jump_t12_G35:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xa(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xa(%%edi)\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_Da6\n \
        jump_t12_G62:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xb(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xb(%%edi)\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_Dca\n \
        jump_t12_G8f:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xc(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xc(%%edi)\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_Dee\n \
        jump_t12_Gbc:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xd(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xd(%%edi)\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_t12_E12\n \
        jump_t12_Ge9:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xe(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xe(%%edi)\n \
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
            mov    0xf(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
            add    $0x10,%%edi\n \
            sub    $0x10,%%ebp\n \
            jg     jump_t12_E5a\n \
            decl   0x20+%[lv]\n \
            jne    jump_t12_Af4\n \
        jump_t12_end:\n \
            popal\n \
        "
                 : [lv] "+o" (lv)
                 : "o0" (lv)
                 : "memory", "cc");
}
/******************************************************************************/
