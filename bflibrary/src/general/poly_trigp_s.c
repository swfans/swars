/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file poly_trigp.c
 *     Implementation of related functions.
 * @par Purpose:
 *     Textured 2D triangle rendring, clipping and preparation.
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
#include "poly_trigp.h"
#include "poly.h"

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

    asm volatile (" \
            mov    %%eax,%%esi\n \
            mov    %%edx,%%edi\n \
            mov    %%ebx,%%ecx\n \
            mov    0x4(%%esi),%%eax\n \
            mov    0x4(%%edi),%%ebx\n \
            mov    0x4(%%ecx),%%edx\n \
            cmp    %%ebx,%%eax\n \
            je     jump_tprep_Aff\n \
            jg     jump_tprep_Ac5\n \
            cmp    %%edx,%%eax\n \
            je     jump_tprep_Aec\n \
            jl     jump_tprep_B3a\n \
            xchg   %%esi,%%ecx\n \
            xchg   %%edi,%%ecx\n \
            jmp    trig_ll_mark\n \
        jump_tprep_Ac5:\n \
            cmp    %%edx,%%eax\n \
            je     jump_tprep_Sb2\n \
            jl     jump_tprep_K4d\n \
            cmp    %%edx,%%ebx\n \
            je     jump_tprep_Q33\n \
            jl     jump_tprep_Ae6\n \
            xchg   %%esi,%%ecx\n \
            xchg   %%edi,%%ecx\n \
            jmp    trig_rl_mark\n \
        jump_tprep_Q33:\n \
            mov    (%%ecx),%%eax\n \
            cmp    (%%edi),%%eax\n \
            jle    trig_tprep_bailout\n \
            xchg   %%esi,%%edi\n \
            xchg   %%edi,%%ecx\n \
            jmp    trig_ft_mark\n \
        jump_tprep_Sb2:\n \
            mov    (%%ecx),%%eax\n \
            cmp    (%%esi),%%eax\n \
            jle    trig_tprep_bailout\n \
            xchg   %%esi,%%edi\n \
            xchg   %%edi,%%ecx\n \
            jmp    trig_fb_mark\n \
        jump_tprep_K4d:\n \
            xchg   %%esi,%%edi\n \
            xchg   %%edi,%%ecx\n \
            jmp    trig_rl_mark\n \
        jump_tprep_Ae6:\n \
            xchg   %%esi,%%edi\n \
            xchg   %%edi,%%ecx\n \
            jmp    trig_ll_mark\n \
        jump_tprep_Aec:\n \
            mov    (%%esi),%%eax\n \
            cmp    (%%ecx),%%eax\n \
            jle    trig_tprep_bailout\n \
            xchg   %%esi,%%ecx\n \
            xchg   %%edi,%%ecx\n \
            jmp    trig_ft_mark\n \
        jump_tprep_Aff:\n \
            cmp    %%edx,%%eax\n \
            je     trig_tprep_bailout\n \
            jl     jump_tprep_B1c\n \
            mov    (%%esi),%%eax\n \
            cmp    (%%edi),%%eax\n \
            jle    trig_tprep_bailout\n \
            xchg   %%esi,%%ecx\n \
            xchg   %%edi,%%ecx\n \
            jmp    trig_fb_mark\n \
        jump_tprep_B1c:\n \
            mov    (%%edi),%%eax\n \
            cmp    (%%esi),%%eax\n \
            jle    trig_tprep_bailout\n \
            jmp    trig_ft_mark\n \
        jump_tprep_B2b:\n \
            mov    (%%edi),%%eax\n \
            cmp    (%%ecx),%%eax\n \
            jle    trig_tprep_bailout\n \
            jmp    trig_fb_mark\n \
        jump_tprep_B3a:\n \
            cmp    %%edx,%%ebx\n \
            je     jump_tprep_B2b\n \
            jg     trig_rl_mark\n \
            jmp    trig_ll_mark\n \
\n \
trig_ll_mark:\n \
            movb   $0x1,%[start_type]\n \
            jmp    jump_tprep_end\n \
trig_rl_mark:\n \
            movb   $0x2,%[start_type]\n \
            jmp    jump_tprep_end\n \
trig_fb_mark:\n \
            movb   $0x3,%[start_type]\n \
            jmp    jump_tprep_end\n \
trig_ft_mark:\n \
            movb   $0x4,%[start_type]\n \
            jmp    jump_tprep_end\n \
trig_tprep_bailout:\n \
            movb   $0x0,%[start_type]\n \
jump_tprep_end:\n \
    "
                 : "=S" (ordpt_a), "=D" (ordpt_b), "=c" (ordpt_c), [start_type] "=m" (start_type)
                 : "a" (*opt_a), "d" (*opt_b), "b" (*opt_c)
                 : "memory", "cc");

    *opt_a = ordpt_a;
    *opt_b = ordpt_b;
    *opt_c = ordpt_c;
    return start_type;
}

int trig_ll_md00(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_pr_ll_md00_Jc5\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_ll_md00_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x14+%[lv],%%edi\n \
            js     jump_pr_ll_md00_J7e\n \
            mov    0x4+%[lv],%%edi\n \
            imul   0x14+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x1c+%[lv],%%ebx\n \
            mov    0x48+%[lv],%%edi\n \
            sub    0x14+%[lv],%%edi\n \
            sub    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x48+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x48+%[lv],%%edi\n \
            imul   0xc+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_ll_md00_J73\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_pr_ll_md00_J73:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_pr_ll_md00_K14\n \
        jump_pr_ll_md00_J7e:\n \
            mov    0x48+%[lv],%%edi\n \
            sub    %%edi,0x14+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_ll_md00_Jf4\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_ll_md00_Jb6\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_pr_ll_md00_Jc3\n \
        jump_pr_ll_md00_Jb6:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_ll_md00_Jc3:\n \
            jmp    jump_pr_ll_md00_Jf4\n \
        jump_pr_ll_md00_Jc5:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_ll_md00_Jf4\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_ll_md00_Je7\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_pr_ll_md00_Jf4\n \
        jump_pr_ll_md00_Je7:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_ll_md00_Jf4:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_pr_ll_md00_Jfa:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            add    $0x14,%%edi\n \
            decl   0x14+%[lv]\n \
            jne    jump_pr_ll_md00_Jfa\n \
            mov    0x1c+%[lv],%%ebx\n \
        jump_pr_ll_md00_K14:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_ll_md00_K29\n \
            jmp    jump_pr_ll_md00_render\n \
        jump_pr_ll_md00_K29:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0xc+%[lv],%%ebx\n \
            add    $0x14,%%edi\n \
            decl   0x18+%[lv]\n \
            jne    jump_pr_ll_md00_K29\n \
            jmp    jump_pr_ll_md00_render\n \
\n \
jump_pr_ll_md00_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_ll_md00_end\n \
jump_pr_ll_md00_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_ll_md00_end\n \
\n \
jump_pr_ll_md00_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_ll_md01(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    0x14+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x5c+%[lv]\n \
            mov    (%%esi),%%eax\n \
            sub    (%%ecx),%%eax\n \
            imull  0x5c+%[lv]\n \
            sar    $0x10,%%eax\n \
            mov    (%%edi),%%ebx\n \
            sub    (%%esi),%%ebx\n \
            add    %%eax,%%ebx\n \
            jl     jump_pr_ll_md01_bailout\n \
            je     jump_pr_ll_md01_H74\n \
            inc    %%ebx\n \
            mov    0x10(%%esi),%%eax\n \
            sub    0x10(%%ecx),%%eax\n \
            imull  0x5c+%[lv]\n \
            shrd   $0x10,%%edx,%%eax\n \
            add    0x10(%%edi),%%eax\n \
            sub    0x10(%%esi),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x3c+%[lv]\n \
        jump_pr_ll_md01_H74:\n \
            mov    0x10(%%ecx),%%eax\n \
            sub    0x10(%%esi),%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x40+%[lv]\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            mov    0x10(%%esi),%%esi\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_pr_ll_md01_I6b\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_ll_md01_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x14+%[lv],%%edi\n \
            js     jump_pr_ll_md01_I19\n \
            mov    0x4+%[lv],%%edi\n \
            imul   0x14+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x14+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            mov    0x1c+%[lv],%%ebx\n \
            mov    0x48+%[lv],%%edi\n \
            sub    0x14+%[lv],%%edi\n \
            sub    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x48+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x48+%[lv],%%edi\n \
            imul   0xc+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_ll_md01_I0e\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_pr_ll_md01_I0e:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_pr_ll_md01_Ic1\n \
        jump_pr_ll_md01_I19:\n \
            mov    0x48+%[lv],%%edi\n \
            sub    %%edi,0x14+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_ll_md01_I9a\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_ll_md01_I5c\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_pr_ll_md01_I69\n \
        jump_pr_ll_md01_I5c:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_ll_md01_I69:\n \
            jmp    jump_pr_ll_md01_I9a\n \
        jump_pr_ll_md01_I6b:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_ll_md01_I9a\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_ll_md01_I8d\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_pr_ll_md01_I9a\n \
        jump_pr_ll_md01_I8d:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_ll_md01_I9a:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_pr_ll_md01_Ia0:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x40+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x14+%[lv]\n \
            jne    jump_pr_ll_md01_Ia0\n \
            mov    0x1c+%[lv],%%ebx\n \
        jump_pr_ll_md01_Ic1:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_ll_md01_Id6\n \
            jmp    jump_pr_ll_md01_render\n \
        jump_pr_ll_md01_Id6:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0xc+%[lv],%%ebx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x40+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x18+%[lv]\n \
            jne    jump_pr_ll_md01_Id6\n \
            jmp    jump_pr_ll_md01_render\n \
\n \
jump_pr_ll_md01_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_ll_md01_end\n \
jump_pr_ll_md01_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_ll_md01_end\n \
\n \
jump_pr_ll_md01_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_ll_md02(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    0x14+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x5c+%[lv]\n \
            mov    (%%esi),%%eax\n \
            sub    (%%ecx),%%eax\n \
            imull  0x5c+%[lv]\n \
            sar    $0x10,%%eax\n \
            mov    (%%edi),%%ebx\n \
            sub    (%%esi),%%ebx\n \
            add    %%eax,%%ebx\n \
            jl     jump_pr_ll_md02_bailout\n \
            je     jump_pr_ll_md02_F61\n \
            inc    %%ebx\n \
            mov    0x8(%%esi),%%eax\n \
            sub    0x8(%%ecx),%%eax\n \
            imull  0x5c+%[lv]\n \
            shrd   $0x10,%%edx,%%eax\n \
            add    0x8(%%edi),%%eax\n \
            sub    0x8(%%esi),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x24+%[lv]\n \
            mov    0xc(%%esi),%%eax\n \
            sub    0xc(%%ecx),%%eax\n \
            imull  0x5c+%[lv]\n \
            shrd   $0x10,%%edx,%%eax\n \
            add    0xc(%%edi),%%eax\n \
            sub    0xc(%%esi),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x30+%[lv]\n \
        jump_pr_ll_md02_F61:\n \
            mov    0x8(%%ecx),%%eax\n \
            sub    0x8(%%esi),%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x28+%[lv]\n \
            mov    0xc(%%ecx),%%eax\n \
            sub    0xc(%%esi),%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x34+%[lv]\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            mov    0x8(%%esi),%%ecx\n \
            mov    0xc(%%esi),%%edx\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_pr_ll_md02_G8b\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_ll_md02_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x14+%[lv],%%edi\n \
            js     jump_pr_ll_md02_G2e\n \
            mov    0x4+%[lv],%%edi\n \
            imul   0x14+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x28+%[lv],%%edi\n \
            imul   0x14+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x34+%[lv],%%edi\n \
            imul   0x14+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            mov    0x1c+%[lv],%%ebx\n \
            mov    0x48+%[lv],%%edi\n \
            sub    0x14+%[lv],%%edi\n \
            sub    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x48+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x48+%[lv],%%edi\n \
            imul   0xc+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x28+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x34+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_ll_md02_G23\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_pr_ll_md02_G23:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_pr_ll_md02_Ge8\n \
        jump_pr_ll_md02_G2e:\n \
            mov    0x48+%[lv],%%edi\n \
            sub    %%edi,0x14+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x28+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x34+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_ll_md02_Gba\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_ll_md02_G7c\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_pr_ll_md02_G89\n \
        jump_pr_ll_md02_G7c:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_ll_md02_G89:\n \
            jmp    jump_pr_ll_md02_Gba\n \
        jump_pr_ll_md02_G8b:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_ll_md02_Gba\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_ll_md02_Gad\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_pr_ll_md02_Gba\n \
        jump_pr_ll_md02_Gad:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_ll_md02_Gba:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_pr_ll_md02_Gc0:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%ecx,0x8(%%edi)\n \
            add    0x28+%[lv],%%ecx\n \
            mov    %%edx,0xc(%%edi)\n \
            add    0x34+%[lv],%%edx\n \
            add    $0x14,%%edi\n \
            decl   0x14+%[lv]\n \
            jne    jump_pr_ll_md02_Gc0\n \
            mov    0x1c+%[lv],%%ebx\n \
        jump_pr_ll_md02_Ge8:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_ll_md02_Gfd\n \
            jmp    jump_pr_ll_md02_render\n \
        jump_pr_ll_md02_Gfd:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0xc+%[lv],%%ebx\n \
            mov    %%ecx,0x8(%%edi)\n \
            add    0x28+%[lv],%%ecx\n \
            mov    %%edx,0xc(%%edi)\n \
            add    0x34+%[lv],%%edx\n \
            add    $0x14,%%edi\n \
            decl   0x18+%[lv]\n \
            jne    jump_pr_ll_md02_Gfd\n \
            jmp    jump_pr_ll_md02_render\n \
\n \
jump_pr_ll_md02_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_ll_md02_end\n \
jump_pr_ll_md02_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_ll_md02_end\n \
\n \
jump_pr_ll_md02_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_ll_md05(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    0x14+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x5c+%[lv]\n \
            mov    (%%esi),%%eax\n \
            sub    (%%ecx),%%eax\n \
            imull  0x5c+%[lv]\n \
            sar    $0x10,%%eax\n \
            mov    (%%edi),%%ebx\n \
            sub    (%%esi),%%ebx\n \
            add    %%eax,%%ebx\n \
            jl     jump_pr_ll_md05_bailout\n \
            je     jump_pr_ll_md05_Cee\n \
            inc    %%ebx\n \
            mov    0x8(%%esi),%%eax\n \
            sub    0x8(%%ecx),%%eax\n \
            imull  0x5c+%[lv]\n \
            shrd   $0x10,%%edx,%%eax\n \
            add    0x8(%%edi),%%eax\n \
            sub    0x8(%%esi),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x24+%[lv]\n \
            mov    0xc(%%esi),%%eax\n \
            sub    0xc(%%ecx),%%eax\n \
            imull  0x5c+%[lv]\n \
            shrd   $0x10,%%edx,%%eax\n \
            add    0xc(%%edi),%%eax\n \
            sub    0xc(%%esi),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x30+%[lv]\n \
            mov    0x10(%%esi),%%eax\n \
            sub    0x10(%%ecx),%%eax\n \
            imull  0x5c+%[lv]\n \
            shrd   $0x10,%%edx,%%eax\n \
            add    0x10(%%edi),%%eax\n \
            sub    0x10(%%esi),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x3c+%[lv]\n \
        jump_pr_ll_md05_Cee:\n \
            mov    0x8(%%ecx),%%eax\n \
            sub    0x8(%%esi),%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x28+%[lv]\n \
            mov    0xc(%%ecx),%%eax\n \
            sub    0xc(%%esi),%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x34+%[lv]\n \
            mov    0x10(%%ecx),%%eax\n \
            sub    0x10(%%esi),%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x40+%[lv]\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            mov    0x8(%%esi),%%ecx\n \
            mov    0xc(%%esi),%%edx\n \
            mov    0x10(%%esi),%%esi\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_pr_ll_md05_E4f\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_ll_md05_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x14+%[lv],%%edi\n \
            js     jump_pr_ll_md05_De7\n \
            mov    0x4+%[lv],%%edi\n \
            imul   0x14+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x28+%[lv],%%edi\n \
            imul   0x14+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x34+%[lv],%%edi\n \
            imul   0x14+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x14+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            mov    0x1c+%[lv],%%ebx\n \
            mov    0x48+%[lv],%%edi\n \
            sub    0x14+%[lv],%%edi\n \
            sub    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x48+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x48+%[lv],%%edi\n \
            imul   0xc+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x28+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x34+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_ll_md05_Ddc\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_pr_ll_md05_Ddc:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_pr_ll_md05_Eb3\n \
        jump_pr_ll_md05_De7:\n \
            mov    0x48+%[lv],%%edi\n \
            sub    %%edi,0x14+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x28+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x34+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_ll_md05_E7e\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_ll_md05_E40\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_pr_ll_md05_E4d\n \
        jump_pr_ll_md05_E40:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_ll_md05_E4d:\n \
            jmp    jump_pr_ll_md05_E7e\n \
        jump_pr_ll_md05_E4f:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_ll_md05_E7e\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_ll_md05_E71\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_pr_ll_md05_E7e\n \
        jump_pr_ll_md05_E71:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_ll_md05_E7e:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_pr_ll_md05_E84:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%ecx,0x8(%%edi)\n \
            add    0x28+%[lv],%%ecx\n \
            mov    %%edx,0xc(%%edi)\n \
            add    0x34+%[lv],%%edx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x40+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x14+%[lv]\n \
            jne    jump_pr_ll_md05_E84\n \
            mov    0x1c+%[lv],%%ebx\n \
        jump_pr_ll_md05_Eb3:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_ll_md05_Ec8\n \
            jmp    jump_pr_ll_md05_render\n \
        jump_pr_ll_md05_Ec8:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0xc+%[lv],%%ebx\n \
            mov    %%ecx,0x8(%%edi)\n \
            add    0x28+%[lv],%%ecx\n \
            mov    %%edx,0xc(%%edi)\n \
            add    0x34+%[lv],%%edx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x40+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x18+%[lv]\n \
            jne    jump_pr_ll_md05_Ec8\n \
            jmp    jump_pr_ll_md05_render\n \
\n \
jump_pr_ll_md05_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_ll_md05_end\n \
jump_pr_ll_md05_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_ll_md05_end\n \
\n \
jump_pr_ll_md05_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_ll_start(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
        jump_pr_ll_B44:\n \
            mov    0x4(%%esi),%%eax\n \
            mov    %%eax,0x54+%[lv]\n \
            or     %%eax,%%eax\n \
            jns    jump_pr_ll_B5f\n \
            mov    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x1,0x66+%[lv]\n \
            jmp    jump_pr_ll_B82\n \
        jump_pr_ll_pre_bailout:\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_ll_pre_end\n \
        jump_pr_ll_B5f:\n \
            cmp    "EXPORT_SYMBOL(vec_window_height)",%%eax\n \
            jge    jump_pr_ll_pre_bailout\n \
            mov    %%eax,%%ebx\n \
            imul   "EXPORT_SYMBOL(vec_screen_width)",%%ebx\n \
            add    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x0,0x66+%[lv]\n \
        jump_pr_ll_B82:\n \
            mov    0x4(%%ecx),%%ebx\n \
            cmp    "EXPORT_SYMBOL(vec_window_height)",%%ebx\n \
            setg   0x68+%[lv]\n \
            sub    %%eax,%%ebx\n \
            mov    %%ebx,0x10+%[lv]\n \
            mov    %%ebx,0x20+%[lv]\n \
            mov    0x4(%%edi),%%ebx\n \
            cmp    "EXPORT_SYMBOL(vec_window_height)",%%ebx\n \
            setg   0x67+%[lv]\n \
            sub    %%eax,%%ebx\n \
            mov    %%ebx,0x14+%[lv]\n \
            mov    (%%ecx),%%eax\n \
            sub    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x4+%[lv]\n \
            mov    (%%edi),%%eax\n \
            sub    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idivl  0x14+%[lv]\n \
            cmp    0x4+%[lv],%%eax\n \
            jle    jump_pr_ll_pre_bailout\n \
            mov    %%eax,0x8+%[lv]\n \
            mov    0x4(%%ecx),%%ebx\n \
            sub    0x4(%%edi),%%ebx\n \
            mov    (%%ecx),%%eax\n \
            sub    (%%edi),%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0xc+%[lv]\n \
            mov    %%ebx,0x18+%[lv]\n \
            mov    (%%edi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x1c+%[lv]\n \
            movl   $0x1,%%eax\n \
        jump_pr_ll_pre_end:\n \
    "
                 : [lv] "=o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c)
                 : "memory", "cc", "%ebx");

    if (!ret)
        return 0;

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

int trig_rl_md00(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_pr_rl_S2c\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_rl_md00_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x10+%[lv],%%edi\n \
            js     jump_pr_rl_Re5\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x10+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x1c+%[lv],%%eax\n \
            mov    0x48+%[lv],%%edi\n \
            sub    0x10+%[lv],%%edi\n \
            mov    %%edi,0x48+%[lv]\n \
            sub    %%edi,0x18+%[lv]\n \
            imul   0xc+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_rl_Rda\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_pr_rl_Rda:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_pr_rl_S7b\n \
        jump_pr_rl_Re5:\n \
            mov    0x48+%[lv],%%edi\n \
            sub    %%edi,0x10+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_rl_S5b\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_rl_S1d\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_pr_rl_S2a\n \
        jump_pr_rl_S1d:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_rl_S2a:\n \
            jmp    jump_pr_rl_S5b\n \
        jump_pr_rl_S2c:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_rl_S5b\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_rl_S4e\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_pr_rl_S5b\n \
        jump_pr_rl_S4e:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_rl_S5b:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_pr_rl_S61:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            add    $0x14,%%edi\n \
            decl   0x10+%[lv]\n \
            jne    jump_pr_rl_S61\n \
            mov    0x1c+%[lv],%%eax\n \
        jump_pr_rl_S7b:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_rl_S90\n \
            jmp    jump_pr_rl_md00_render\n \
        jump_pr_rl_S90:\n \
            mov    %%eax,(%%edi)\n \
            add    0xc+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            add    $0x14,%%edi\n \
            decl   0x18+%[lv]\n \
            jne    jump_pr_rl_S90\n \
            jmp    jump_pr_rl_md00_render\n \
\n \
jump_pr_rl_md00_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_rl_md00_end\n \
jump_pr_rl_md00_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_rl_md00_end\n \
\n \
jump_pr_rl_md00_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_rl_md01(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    0x10+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idivl  0x14+%[lv]\n \
            mov    %%eax,0x5c+%[lv]\n \
            mov    (%%edi),%%eax\n \
            sub    (%%esi),%%eax\n \
            imull  0x5c+%[lv]\n \
            sar    $0x10,%%eax\n \
            mov    (%%esi),%%ebx\n \
            sub    (%%ecx),%%ebx\n \
            add    %%eax,%%ebx\n \
            jl     jump_pr_rl_md01_bailout\n \
            je     jump_pr_rl_Pcc\n \
            inc    %%ebx\n \
            mov    0x10(%%edi),%%eax\n \
            sub    0x10(%%esi),%%eax\n \
            imull  0x5c+%[lv]\n \
            shrd   $0x10,%%edx,%%eax\n \
            add    0x10(%%esi),%%eax\n \
            sub    0x10(%%ecx),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x3c+%[lv]\n \
        jump_pr_rl_Pcc:\n \
            mov    0x10(%%ecx),%%eax\n \
            sub    0x10(%%esi),%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x40+%[lv]\n \
            mov    0x10(%%edi),%%eax\n \
            sub    0x10(%%ecx),%%eax\n \
            cltd\n \
            idivl  0x18+%[lv]\n \
            mov    %%eax,0x44+%[lv]\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            mov    0x10(%%esi),%%esi\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_pr_rl_Qd2\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_rl_md01_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x10+%[lv],%%edi\n \
            js     jump_pr_rl_Q80\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x10+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x10+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            mov    0x1c+%[lv],%%eax\n \
            mov    0x48+%[lv],%%edi\n \
            sub    0x10+%[lv],%%edi\n \
            mov    %%edi,0x48+%[lv]\n \
            sub    %%edi,0x18+%[lv]\n \
            imul   0xc+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x44+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_rl_Q75\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_pr_rl_Q75:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_pr_rl_R28\n \
        jump_pr_rl_Q80:\n \
            mov    0x48+%[lv],%%edi\n \
            sub    %%edi,0x10+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_rl_R01\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_rl_Qc3\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_pr_rl_Qd0\n \
        jump_pr_rl_Qc3:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_rl_Qd0:\n \
            jmp    jump_pr_rl_R01\n \
        jump_pr_rl_Qd2:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_rl_R01\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_rl_Qf4\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_pr_rl_R01\n \
        jump_pr_rl_Qf4:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_rl_R01:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_pr_rl_R07:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x40+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x10+%[lv]\n \
            jne    jump_pr_rl_R07\n \
            mov    0x1c+%[lv],%%eax\n \
        jump_pr_rl_R28:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_rl_R3d\n \
            jmp    jump_pr_rl_md01_render\n \
        jump_pr_rl_R3d:\n \
            mov    %%eax,(%%edi)\n \
            add    0xc+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x44+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x18+%[lv]\n \
            jne    jump_pr_rl_R3d\n \
            jmp    jump_pr_rl_md01_render\n \
\n \
jump_pr_rl_md01_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_rl_md01_end\n \
jump_pr_rl_md01_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_rl_md01_end\n \
\n \
jump_pr_rl_md01_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_rl_md02(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    0x10+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idivl  0x14+%[lv]\n \
            mov    %%eax,0x5c+%[lv]\n \
            mov    (%%edi),%%eax\n \
            sub    (%%esi),%%eax\n \
            imull  0x5c+%[lv]\n \
            sar    $0x10,%%eax\n \
            mov    (%%esi),%%ebx\n \
            sub    (%%ecx),%%ebx\n \
            add    %%eax,%%ebx\n \
            jl     jump_pr_rl_md02_bailout\n \
            je     jump_pr_rl_N9b\n \
            inc    %%ebx\n \
            mov    0x8(%%edi),%%eax\n \
            sub    0x8(%%esi),%%eax\n \
            imull  0x5c+%[lv]\n \
            shrd   $0x10,%%edx,%%eax\n \
            add    0x8(%%esi),%%eax\n \
            sub    0x8(%%ecx),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x24+%[lv]\n \
            mov    0xc(%%edi),%%eax\n \
            sub    0xc(%%esi),%%eax\n \
            imull  0x5c+%[lv]\n \
            shrd   $0x10,%%edx,%%eax\n \
            add    0xc(%%esi),%%eax\n \
            sub    0xc(%%ecx),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x30+%[lv]\n \
        jump_pr_rl_N9b:\n \
            mov    0x8(%%ecx),%%eax\n \
            sub    0x8(%%esi),%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x28+%[lv]\n \
            mov    0xc(%%ecx),%%eax\n \
            sub    0xc(%%esi),%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x34+%[lv]\n \
            mov    0x8(%%edi),%%eax\n \
            sub    0x8(%%ecx),%%eax\n \
            cltd\n \
            idivl  0x18+%[lv]\n \
            mov    %%eax,0x2c+%[lv]\n \
            mov    0xc(%%edi),%%eax\n \
            sub    0xc(%%ecx),%%eax\n \
            cltd\n \
            idivl  0x18+%[lv]\n \
            mov    %%eax,0x38+%[lv]\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            mov    0x8(%%esi),%%ecx\n \
            mov    0xc(%%esi),%%edx\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_pr_rl_Oe3\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_rl_md02_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x10+%[lv],%%edi\n \
            js     jump_pr_rl_O86\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x10+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x28+%[lv],%%edi\n \
            imul   0x10+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x34+%[lv],%%edi\n \
            imul   0x10+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            mov    0x1c+%[lv],%%eax\n \
            mov    0x48+%[lv],%%edi\n \
            sub    0x10+%[lv],%%edi\n \
            mov    %%edi,0x48+%[lv]\n \
            sub    %%edi,0x18+%[lv]\n \
            imul   0xc+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x2c+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x38+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_rl_O7b\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_pr_rl_O7b:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_pr_rl_P40\n \
        jump_pr_rl_O86:\n \
            mov    0x48+%[lv],%%edi\n \
            sub    %%edi,0x10+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x28+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x34+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_rl_P12\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_rl_Od4\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_pr_rl_Oe1\n \
        jump_pr_rl_Od4:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_rl_Oe1:\n \
            jmp    jump_pr_rl_P12\n \
        jump_pr_rl_Oe3:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_rl_P12\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_rl_P05\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_pr_rl_P12\n \
        jump_pr_rl_P05:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_rl_P12:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_pr_rl_P18:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%ecx,0x8(%%edi)\n \
            add    0x28+%[lv],%%ecx\n \
            mov    %%edx,0xc(%%edi)\n \
            add    0x34+%[lv],%%edx\n \
            add    $0x14,%%edi\n \
            decl   0x10+%[lv]\n \
            jne    jump_pr_rl_P18\n \
            mov    0x1c+%[lv],%%eax\n \
        jump_pr_rl_P40:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_rl_P55\n \
            jmp    jump_pr_rl_md02_render\n \
        jump_pr_rl_P55:\n \
            mov    %%eax,(%%edi)\n \
            add    0xc+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%ecx,0x8(%%edi)\n \
            add    0x2c+%[lv],%%ecx\n \
            mov    %%edx,0xc(%%edi)\n \
            add    0x38+%[lv],%%edx\n \
            add    $0x14,%%edi\n \
            decl   0x18+%[lv]\n \
            jne    jump_pr_rl_P55\n \
            jmp    jump_pr_rl_md02_render\n \
\n \
jump_pr_rl_md02_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_rl_md02_end\n \
jump_pr_rl_md02_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_rl_md02_end\n \
\n \
jump_pr_rl_md02_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_rl_md05(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    0x10+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idivl  0x14+%[lv]\n \
            mov    %%eax,0x5c+%[lv]\n \
            mov    (%%edi),%%eax\n \
            sub    (%%esi),%%eax\n \
            imull  0x5c+%[lv]\n \
            sar    $0x10,%%eax\n \
            mov    (%%esi),%%ebx\n \
            sub    (%%ecx),%%ebx\n \
            add    %%eax,%%ebx\n \
            jl     jump_pr_rl_md05_bailout\n \
            je     jump_pr_rl_Lf7\n \
            inc    %%ebx\n \
            mov    0x8(%%edi),%%eax\n \
            sub    0x8(%%esi),%%eax\n \
            imull  0x5c+%[lv]\n \
            shrd   $0x10,%%edx,%%eax\n \
            add    0x8(%%esi),%%eax\n \
            sub    0x8(%%ecx),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x24+%[lv]\n \
            mov    0xc(%%edi),%%eax\n \
            sub    0xc(%%esi),%%eax\n \
            imull  0x5c+%[lv]\n \
            shrd   $0x10,%%edx,%%eax\n \
            add    0xc(%%esi),%%eax\n \
            sub    0xc(%%ecx),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x30+%[lv]\n \
            mov    0x10(%%edi),%%eax\n \
            sub    0x10(%%esi),%%eax\n \
            imull  0x5c+%[lv]\n \
            shrd   $0x10,%%edx,%%eax\n \
            add    0x10(%%esi),%%eax\n \
            sub    0x10(%%ecx),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
        jump_pr_rl_Lf7:\n \
            mov    %%eax,0x3c+%[lv]\n \
            mov    0x8(%%ecx),%%eax\n \
            sub    0x8(%%esi),%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x28+%[lv]\n \
            mov    0xc(%%ecx),%%eax\n \
            sub    0xc(%%esi),%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x34+%[lv]\n \
            mov    0x10(%%ecx),%%eax\n \
            sub    0x10(%%esi),%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x40+%[lv]\n \
            mov    0x8(%%edi),%%eax\n \
            sub    0x8(%%ecx),%%eax\n \
            cltd\n \
            idivl  0x18+%[lv]\n \
            mov    %%eax,0x2c+%[lv]\n \
            mov    0xc(%%edi),%%eax\n \
            sub    0xc(%%ecx),%%eax\n \
            cltd\n \
            idivl  0x18+%[lv]\n \
            mov    %%eax,0x38+%[lv]\n \
            mov    0x10(%%edi),%%eax\n \
            sub    0x10(%%ecx),%%eax\n \
            cltd\n \
            idivl  0x18+%[lv]\n \
            mov    %%eax,0x44+%[lv]\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            mov    0x8(%%esi),%%ecx\n \
            mov    0xc(%%esi),%%edx\n \
            mov    0x10(%%esi),%%esi\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_pr_rl_M89\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_rl_md05_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x10+%[lv],%%edi\n \
            js     jump_pr_rl_M21\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x10+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x28+%[lv],%%edi\n \
            imul   0x10+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x34+%[lv],%%edi\n \
            imul   0x10+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x10+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            mov    0x1c+%[lv],%%eax\n \
            mov    0x48+%[lv],%%edi\n \
            sub    0x10+%[lv],%%edi\n \
            mov    %%edi,0x48+%[lv]\n \
            sub    %%edi,0x18+%[lv]\n \
            imul   0xc+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x2c+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x38+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            mov    0x44+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_rl_M16\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_pr_rl_M16:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_pr_rl_Med\n \
        jump_pr_rl_M21:\n \
            mov    0x48+%[lv],%%edi\n \
            sub    %%edi,0x10+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x28+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x34+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_rl_Mb8\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_rl_M7a\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_pr_rl_M87\n \
        jump_pr_rl_M7a:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_rl_M87:\n \
            jmp    jump_pr_rl_Mb8\n \
        jump_pr_rl_M89:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_pr_rl_Mb8\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_rl_Mab\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_pr_rl_Mb8\n \
        jump_pr_rl_Mab:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_pr_rl_Mb8:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_pr_rl_Mbe:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%ecx,0x8(%%edi)\n \
            add    0x28+%[lv],%%ecx\n \
            mov    %%edx,0xc(%%edi)\n \
            add    0x34+%[lv],%%edx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x40+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x10+%[lv]\n \
            jne    jump_pr_rl_Mbe\n \
            mov    0x1c+%[lv],%%eax\n \
        jump_pr_rl_Med:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_pr_rl_N02\n \
            jmp    jump_pr_rl_md05_render\n \
        jump_pr_rl_N02:\n \
            mov    %%eax,(%%edi)\n \
            add    0xc+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%ecx,0x8(%%edi)\n \
            add    0x2c+%[lv],%%ecx\n \
            mov    %%edx,0xc(%%edi)\n \
            add    0x38+%[lv],%%edx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x44+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x18+%[lv]\n \
            jne    jump_pr_rl_N02\n \
            jmp    jump_pr_rl_md05_render\n \
\n \
jump_pr_rl_md05_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_rl_md05_end\n \
jump_pr_rl_md05_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_rl_md05_end\n \
\n \
jump_pr_rl_md05_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_rl_start(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
        jump_pr_rl_K51:\n \
            mov    0x4(%%esi),%%eax\n \
            mov    %%eax,0x54+%[lv]\n \
            or     %%eax,%%eax\n \
            jns    jump_pr_rl_K6c\n \
            mov    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x1,0x66+%[lv]\n \
            jmp    jump_pr_rl_K8f\n \
        jump_pr_rl_pre_bailout:\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_rl_pre_end\n \
        jump_pr_rl_K6c:\n \
            cmp    "EXPORT_SYMBOL(vec_window_height)",%%eax\n \
            jge    jump_pr_rl_pre_bailout\n \
            mov    %%eax,%%ebx\n \
            imul   "EXPORT_SYMBOL(vec_screen_width)",%%ebx\n \
            add    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x0,0x66+%[lv]\n \
        jump_pr_rl_K8f:\n \
            mov    0x4(%%ecx),%%ebx\n \
            cmp    "EXPORT_SYMBOL(vec_window_height)",%%ebx\n \
            setg   0x67+%[lv]\n \
            sub    %%eax,%%ebx\n \
            mov    %%ebx,0x10+%[lv]\n \
            mov    0x4(%%edi),%%ebx\n \
            cmp    "EXPORT_SYMBOL(vec_window_height)",%%ebx\n \
            setg   0x68+%[lv]\n \
            sub    %%eax,%%ebx\n \
            mov    %%ebx,0x14+%[lv]\n \
            mov    %%ebx,0x20+%[lv]\n \
            mov    (%%ecx),%%eax\n \
            sub    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idivl  0x10+%[lv]\n \
            mov    %%eax,0x4+%[lv]\n \
            mov    (%%edi),%%eax\n \
            sub    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idivl  0x14+%[lv]\n \
            cmp    0x4+%[lv],%%eax\n \
            jle    jump_pr_rl_pre_bailout\n \
            mov    %%eax,0x8+%[lv]\n \
            mov    0x4(%%edi),%%ebx\n \
            sub    0x4(%%ecx),%%ebx\n \
            mov    (%%edi),%%eax\n \
            sub    (%%ecx),%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0xc+%[lv]\n \
            mov    %%ebx,0x18+%[lv]\n \
            mov    (%%ecx),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x1c+%[lv]\n \
            movl   $0x1,%%eax\n \
        jump_pr_rl_pre_end:\n \
    "
                 : [lv] "=o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c)
                 : "memory", "cc", "%ebx");

    if (!ret)
        return 0;

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

int trig_fb_md00(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_1224f0\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x10+%[lv]\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_fb_md00_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_122509\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_122509\n \
        jump_1224f0:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_122509\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
        jump_122509:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_12250f:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            add    $0x14,%%edi\n \
            decl   0x10+%[lv]\n \
            jne    jump_12250f\n \
            jmp    jump_pr_fb_md00_render\n \
\n \
jump_pr_fb_md00_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_fb_md00_end\n \
jump_pr_fb_md00_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_fb_md00_end\n \
\n \
jump_pr_fb_md00_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_fb_md01(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    (%%edi),%%ebx\n \
            sub    (%%ecx),%%ebx\n \
            mov    0x10(%%edi),%%eax\n \
            sub    0x10(%%ecx),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x3c+%[lv]\n \
            mov    0x10(%%ecx),%%eax\n \
            sub    0x10(%%esi),%%eax\n \
            cltd\n \
            idivl  0x20+%[lv]\n \
            mov    %%eax,0x40+%[lv]\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            mov    0x10(%%esi),%%esi\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_122457\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x10+%[lv]\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_fb_md01_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_122470\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_122470\n \
        jump_122457:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_122470\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
        jump_122470:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_122476:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x40+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x10+%[lv]\n \
            jne    jump_122476\n \
            jmp    jump_pr_fb_md01_render\n \
\n \
jump_pr_fb_md01_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_fb_md01_end\n \
jump_pr_fb_md01_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_fb_md01_end\n \
\n \
jump_pr_fb_md01_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_fb_md02(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    (%%edi),%%ebx\n \
            sub    (%%ecx),%%ebx\n \
            mov    0x8(%%edi),%%eax\n \
            sub    0x8(%%ecx),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x24+%[lv]\n \
            mov    0xc(%%edi),%%eax\n \
            sub    0xc(%%ecx),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x30+%[lv]\n \
            mov    0x8(%%ecx),%%eax\n \
            sub    0x8(%%esi),%%eax\n \
            cltd\n \
            idivl  0x20+%[lv]\n \
            mov    %%eax,0x28+%[lv]\n \
            mov    0xc(%%ecx),%%eax\n \
            sub    0xc(%%esi),%%eax\n \
            cltd\n \
            idivl  0x20+%[lv]\n \
            mov    %%eax,0x34+%[lv]\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            mov    0x8(%%esi),%%ecx\n \
            mov    0xc(%%esi),%%edx\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_122389\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x10+%[lv]\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_fb_md02_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x28+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x34+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_1223a2\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_1223a2\n \
        jump_122389:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_1223a2\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
        jump_1223a2:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_1223a8:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%ecx,0x8(%%edi)\n \
            add    0x28+%[lv],%%ecx\n \
            mov    %%edx,0xc(%%edi)\n \
            add    0x34+%[lv],%%edx\n \
            add    $0x14,%%edi\n \
            decl   0x10+%[lv]\n \
            jne    jump_1223a8\n \
            jmp    jump_pr_fb_md02_render\n \
\n \
jump_pr_fb_md02_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_fb_md02_end\n \
jump_pr_fb_md02_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_fb_md02_end\n \
\n \
jump_pr_fb_md02_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_fb_md05(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    (%%edi),%%ebx\n \
            sub    (%%ecx),%%ebx\n \
            mov    0x8(%%edi),%%eax\n \
            sub    0x8(%%ecx),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x24+%[lv]\n \
            mov    0xc(%%edi),%%eax\n \
            sub    0xc(%%ecx),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x30+%[lv]\n \
            mov    0x10(%%edi),%%eax\n \
            sub    0x10(%%ecx),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x3c+%[lv]\n \
            mov    0x8(%%ecx),%%eax\n \
            sub    0x8(%%esi),%%eax\n \
            cltd\n \
            idivl  0x20+%[lv]\n \
            mov    %%eax,0x28+%[lv]\n \
            mov    0xc(%%ecx),%%eax\n \
            sub    0xc(%%esi),%%eax\n \
            cltd\n \
            idivl  0x20+%[lv]\n \
            mov    %%eax,0x34+%[lv]\n \
            mov    0x10(%%ecx),%%eax\n \
            sub    0x10(%%esi),%%eax\n \
            cltd\n \
            idivl  0x20+%[lv]\n \
            mov    %%eax,0x40+%[lv]\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            mov    0x8(%%esi),%%ecx\n \
            mov    0xc(%%esi),%%edx\n \
            mov    0x10(%%esi),%%esi\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_12227f\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x10+%[lv]\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_fb_md05_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x28+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x34+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_122298\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_122298\n \
        jump_12227f:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_122298\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
        jump_122298:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_12229e:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%ecx,0x8(%%edi)\n \
            add    0x28+%[lv],%%ecx\n \
            mov    %%edx,0xc(%%edi)\n \
            add    0x34+%[lv],%%edx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x40+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x10+%[lv]\n \
            jne    jump_12229e\n \
            jmp    jump_pr_fb_md05_render\n \
\n \
jump_pr_fb_md05_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_fb_md05_end\n \
jump_pr_fb_md05_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_fb_md05_end\n \
\n \
jump_pr_fb_md05_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_fb_start(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
        jump_pr_fb_Sc2:\n \
            mov    0x4(%%esi),%%eax\n \
            mov    %%eax,0x54+%[lv]\n \
            or     %%eax,%%eax\n \
            jns    jump_pr_fb_Sdd\n \
            mov    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x1,0x66+%[lv]\n \
            jmp    jump_pr_fb_T00\n \
        jump_pr_fb_pre_bailout:\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_fb_pre_end\n \
        jump_pr_fb_Sdd:\n \
            cmp    "EXPORT_SYMBOL(vec_window_height)",%%eax\n \
            jge    jump_pr_fb_pre_bailout\n \
            mov    %%eax,%%ebx\n \
            imul   "EXPORT_SYMBOL(vec_screen_width)",%%ebx\n \
            add    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x0,0x66+%[lv]\n \
        jump_pr_fb_T00:\n \
            mov    0x4(%%ecx),%%ebx\n \
            cmp    "EXPORT_SYMBOL(vec_window_height)",%%ebx\n \
            setg   0x67+%[lv]\n \
            sub    %%eax,%%ebx\n \
            mov    %%ebx,0x10+%[lv]\n \
            mov    %%ebx,0x20+%[lv]\n \
            mov    (%%ecx),%%eax\n \
            sub    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x4+%[lv]\n \
            mov    (%%edi),%%eax\n \
            sub    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x8+%[lv]\n \
            movl   $0x1,%%eax\n \
        jump_pr_fb_pre_end:\n \
    "
                 : [lv] "=o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c)
                 : "memory", "cc", "%ebx");

    if (!ret)
          return 0;

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

int trig_ft_md00(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    (%%edi),%%ebx\n \
            shl    $0x10,%%ebx\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_122970\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x10+%[lv]\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_ft_md00_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_122989\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_122989\n \
        jump_122970:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_122989\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
        jump_122989:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_12298f:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            add    $0x14,%%edi\n \
            decl   0x10+%[lv]\n \
            jne    jump_12298f\n \
            jmp    jump_pr_ft_md00_render\n \
\n \
jump_pr_ft_md00_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_ft_md00_end\n \
jump_pr_ft_md00_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_ft_md00_end\n \
\n \
jump_pr_ft_md00_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_ft_md01(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    (%%edi),%%ebx\n \
            sub    (%%esi),%%ebx\n \
            mov    0x10(%%edi),%%eax\n \
            sub    0x10(%%esi),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x3c+%[lv]\n \
            mov    0x10(%%ecx),%%eax\n \
            sub    0x10(%%esi),%%eax\n \
            cltd\n \
            idivl  0x20+%[lv]\n \
            mov    %%eax,0x40+%[lv]\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    (%%edi),%%ebx\n \
            shl    $0x10,%%ebx\n \
            mov    0x10(%%esi),%%esi\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_1228d4\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x10+%[lv]\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_ft_md01_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_1228ed\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_1228ed\n \
        jump_1228d4:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_1228ed\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
        jump_1228ed:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_1228f3:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x40+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x10+%[lv]\n \
            jne    jump_1228f3\n \
            jmp    jump_pr_ft_md01_render\n \
\n \
jump_pr_ft_md01_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_ft_md01_end\n \
jump_pr_ft_md01_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_ft_md01_end\n \
\n \
jump_pr_ft_md01_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_ft_md02(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    (%%edi),%%ebx\n \
            sub    (%%esi),%%ebx\n \
            mov    0x8(%%edi),%%eax\n \
            sub    0x8(%%esi),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x24+%[lv]\n \
            mov    0xc(%%edi),%%eax\n \
            sub    0xc(%%esi),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x30+%[lv]\n \
            mov    0x8(%%ecx),%%eax\n \
            sub    0x8(%%esi),%%eax\n \
            cltd\n \
            idivl  0x20+%[lv]\n \
            mov    %%eax,0x28+%[lv]\n \
            mov    0xc(%%ecx),%%eax\n \
            sub    0xc(%%esi),%%eax\n \
            cltd\n \
            idivl  0x20+%[lv]\n \
            mov    %%eax,0x34+%[lv]\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    (%%edi),%%ebx\n \
            shl    $0x10,%%ebx\n \
            mov    0x8(%%esi),%%ecx\n \
            mov    0xc(%%esi),%%edx\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_122803\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x10+%[lv]\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_ft_md02_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x28+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x34+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_12281c\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_12281c\n \
        jump_122803:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_12281c\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
        jump_12281c:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_122822:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%ecx,0x8(%%edi)\n \
            add    0x28+%[lv],%%ecx\n \
            mov    %%edx,0xc(%%edi)\n \
            add    0x34+%[lv],%%edx\n \
            add    $0x14,%%edi\n \
            decl   0x10+%[lv]\n \
            jne    jump_122822\n \
            jmp    jump_pr_ft_md02_render\n \
\n \
jump_pr_ft_md02_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_ft_md02_end\n \
jump_pr_ft_md02_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_ft_md02_end\n \
\n \
jump_pr_ft_md02_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_ft_md05(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
            pushal\n \
            mov    (%%edi),%%ebx\n \
            sub    (%%esi),%%ebx\n \
            mov    0x8(%%edi),%%eax\n \
            sub    0x8(%%esi),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x24+%[lv]\n \
            mov    0xc(%%edi),%%eax\n \
            sub    0xc(%%esi),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x30+%[lv]\n \
            mov    0x10(%%edi),%%eax\n \
            sub    0x10(%%esi),%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x3c+%[lv]\n \
            mov    0x8(%%ecx),%%eax\n \
            sub    0x8(%%esi),%%eax\n \
            cltd\n \
            idivl  0x20+%[lv]\n \
            mov    %%eax,0x28+%[lv]\n \
            mov    0xc(%%ecx),%%eax\n \
            sub    0xc(%%esi),%%eax\n \
            cltd\n \
            idivl  0x20+%[lv]\n \
            mov    %%eax,0x34+%[lv]\n \
            mov    0x10(%%ecx),%%eax\n \
            sub    0x10(%%esi),%%eax\n \
            cltd\n \
            idivl  0x20+%[lv]\n \
            mov    %%eax,0x40+%[lv]\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    (%%edi),%%ebx\n \
            shl    $0x10,%%ebx\n \
            mov    0x8(%%esi),%%ecx\n \
            mov    0xc(%%esi),%%edx\n \
            mov    0x10(%%esi),%%esi\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_122701\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x10+%[lv]\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_pr_ft_md05_bailout\n \
            mov    %%edi,0x48+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            mov    0x28+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ecx\n \
            mov    0x34+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%edx\n \
            mov    0x40+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%esi\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_12271a\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_12271a\n \
        jump_122701:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_12271a\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            mov    %%edi,0x10+%[lv]\n \
        jump_12271a:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_122720:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%ecx,0x8(%%edi)\n \
            add    0x28+%[lv],%%ecx\n \
            mov    %%edx,0xc(%%edi)\n \
            add    0x34+%[lv],%%edx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x40+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x10+%[lv]\n \
            jne    jump_122720\n \
            jmp    jump_pr_ft_md05_render\n \
\n \
jump_pr_ft_md05_bailout:\n \
            popal\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_ft_md05_end\n \
jump_pr_ft_md05_render:\n \
            popal\n \
            movl   $0x1,%%eax\n \
            jmp    jump_pr_ft_md05_end\n \
\n \
jump_pr_ft_md05_end:\n \
    "
                 : [lv] "+o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c), "o0" (lv)
                 : "memory", "cc");
    return ret;
}

int trig_ft_start(struct TrigLocals *lvu, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c)
{
    int ret;
    asm volatile (" \
        jump_pr_ft_A41:\n \
            mov    0x4(%%esi),%%eax\n \
            mov    %%eax,0x54+%[lv]\n \
            or     %%eax,%%eax\n \
            jns    jump_pr_ft_A5c\n \
            mov    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x1,0x66+%[lv]\n \
            jmp    jump_pr_ft_A7f\n \
        jump_pr_ft_pre_bailout:\n \
            movl   $0x0,%%eax\n \
            jmp    jump_pr_ft_pre_end\n \
        jump_pr_ft_A5c:\n \
            cmp    "EXPORT_SYMBOL(vec_window_height)",%%eax\n \
            jge    jump_pr_ft_pre_bailout\n \
            mov    %%eax,%%ebx\n \
            imul   "EXPORT_SYMBOL(vec_screen_width)",%%ebx\n \
            add    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x0,0x66+%[lv]\n \
        jump_pr_ft_A7f:\n \
            mov    0x4(%%ecx),%%ebx\n \
            cmp    "EXPORT_SYMBOL(vec_window_height)",%%ebx\n \
            setg   0x67+%[lv]\n \
            sub    %%eax,%%ebx\n \
            mov    %%ebx,0x10+%[lv]\n \
            mov    %%ebx,0x20+%[lv]\n \
            mov    (%%ecx),%%eax\n \
            sub    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x4+%[lv]\n \
            mov    (%%ecx),%%eax\n \
            sub    (%%edi),%%eax\n \
            shl    $0x10,%%eax\n \
            cltd\n \
            idiv   %%ebx\n \
            mov    %%eax,0x8+%[lv]\n \
            movl   $0x1,%%eax\n \
        jump_pr_ft_pre_end:\n \
    "
                 : [lv] "=o" (lv), "=a" (ret)
                 : "S" (opt_a), "D" (opt_b), "c" (opt_c)
                 : "memory", "cc", "%ebx");

    if (!ret)
        return 0;

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
