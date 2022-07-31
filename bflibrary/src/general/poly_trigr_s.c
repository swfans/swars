/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file poly_trigr.c
 *     Textured 2D triangle rendring, final rasterization modes.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     Old assembly implementation, to be used for finding regressions.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "poly_trigr.h"
#include "poly.h"

extern const long add_to_edi[];

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
}

void trig_render_md01(struct TrigLocals *lvu)
{
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
}

void trig_render_md02(struct TrigLocals *lvu)
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
}

/**
 * Verified in swars - uses:
 * - zealot car antennas
 * - tank lower chassis
 * - Large red and white rocket building - red stipes
 */
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
}

/**
 * Verified in swars - uses:
 * - wire fences
 * - cross bars
 */
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
}

/******************************************************************************/
