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
#include "swlog.h"

/******************************************************************************/
#pragma pack(1)

enum RenderingStartType {
    RendStart_NO = 0,
    RendStart_LL,
    RendStart_RL,
    RendStart_FB,
    RendStart_FT,
};

enum RenderingVectorMode {
    RendVec_mode00 = 0,
    RendVec_mode01,
    RendVec_mode02,
    RendVec_mode03,
    RendVec_mode04,
    RendVec_mode05,
    RendVec_mode06,
    RendVec_mode07,
    RendVec_mode08,
    RendVec_mode09,
    RendVec_mode10,
    RendVec_mode11,
    RendVec_mode12,
    RendVec_mode13,
    RendVec_mode14,
    RendVec_mode15,
    RendVec_mode16,
    RendVec_mode17,
    RendVec_mode18,
    RendVec_mode19,
    RendVec_mode20,
    RendVec_mode21,
    RendVec_mode22,
    RendVec_mode23,
    RendVec_mode24,
    RendVec_mode25,
    RendVec_mode26,
};

struct TrigLocals {
    unsigned char v[0x6C];
};

#pragma pack()

#define STRINGIFY(x) #x
#ifdef NEED_UNDERSCORE
# define EXPORT_SYMBOL(sym) STRINGIFY(_ ## sym)
#else
# define EXPORT_SYMBOL(sym) STRINGIFY(sym)
#endif

extern const long add_to_edi[];

/******************************************************************************/

// TODO Switch to local var when remade - this is global because assembly alters EBP
struct TrigLocals lv;

ubyte trig_reorder_input_points(struct PolyPoint **opt_a,
  struct PolyPoint **opt_b, struct PolyPoint **opt_c);
int trig_ll_start(struct TrigLocals *lv, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c);
int trig_rl_start(struct TrigLocals *lv, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c);
int trig_fb_start(struct TrigLocals *lv, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c);
int trig_ft_start(struct TrigLocals *lv, const struct PolyPoint *opt_a,
  const struct PolyPoint *opt_b, const struct PolyPoint *opt_c);
void trig_render_md00(struct TrigLocals *lv);
void trig_render_md01(struct TrigLocals *lv);
void trig_render_md02(struct TrigLocals *lv);
void trig_render_md03(struct TrigLocals *lv);
void trig_render_md04(struct TrigLocals *lv);
void trig_render_md05(struct TrigLocals *lv);
void trig_render_md06(struct TrigLocals *lv);
void trig_render_md07(struct TrigLocals *lv);
void trig_render_md08(struct TrigLocals *lv);
void trig_render_md09(struct TrigLocals *lv);
void trig_render_md10(struct TrigLocals *lv);
void trig_render_md12(struct TrigLocals *lv);
void trig_render_md13(struct TrigLocals *lv);
void trig_render_md14(struct TrigLocals *lv);
void trig_render_md15(struct TrigLocals *lv);
void trig_render_md16(struct TrigLocals *lv);
void trig_render_md17(struct TrigLocals *lv);
void trig_render_md18(struct TrigLocals *lv);
void trig_render_md19(struct TrigLocals *lv);
void trig_render_md20(struct TrigLocals *lv);
void trig_render_md21(struct TrigLocals *lv);
void trig_render_md22(struct TrigLocals *lv);
void trig_render_md23(struct TrigLocals *lv);
void trig_render_md24(struct TrigLocals *lv);
void trig_render_md25(struct TrigLocals *lv);
void trig_render_md26(struct TrigLocals *lv);

/** Triangle rendering function.
 *
 * @param point_a Coordinates and texture mapping of first point.
 * @param point_b Coordinates and texture mapping of second point.
 * @param point_c Coordinates and texture mapping of third point.
 */
void trig(struct PolyPoint *point_a, struct PolyPoint *point_b,
  struct PolyPoint *point_c)
{
#if 0
    asm volatile ("call ASM_trig\n"
        :  : "a" (point_a), "d" (point_b), "b" (point_c));
    return;
#endif
    struct PolyPoint *opt_a;
    struct PolyPoint *opt_b;
    struct PolyPoint *opt_c;
    ubyte start_type;

    LOGNO("Pa(%ld,%ld,%ld)", point_a->X, point_a->Y, point_a->S);
    LOGNO("Pb(%ld,%ld,%ld)", point_b->X, point_b->Y, point_b->S);
    LOGNO("Pc(%ld,%ld,%ld)", point_c->X, point_c->Y, point_c->S);

    opt_a = point_a;
    opt_b = point_b;
    opt_c = point_c;
    start_type = trig_reorder_input_points(&opt_a, &opt_b, &opt_c);

    switch (start_type)
    {
    case RendStart_LL:
        if (!trig_ll_start(&lv, opt_a, opt_b, opt_c)) {
            return;
        }
        break;
    case RendStart_RL:
        if (!trig_rl_start(&lv, opt_a, opt_b, opt_c)) {
            return;
        }
        break;
    case RendStart_FB:
        if (!trig_fb_start(&lv, opt_a, opt_b, opt_c)) {
            return;
        }
        break;
    case RendStart_FT:
        if (!trig_ft_start(&lv, opt_a, opt_b, opt_c)) {
            return;
        }
        break;
    case RendStart_NO:
        return;
    }

    LOGNO("render mode %d",(int)vec_mode);

    switch (vec_mode)
    {
    case RendVec_mode00:
        trig_render_md00(&lv);
        break;

    case RendVec_mode01:
        trig_render_md01(&lv);
        break;

    case RendVec_mode02:
        trig_render_md02(&lv);
        break;

    case RendVec_mode03:
        trig_render_md03(&lv);
        break;

    case RendVec_mode04:
        trig_render_md04(&lv);
        break;

    case RendVec_mode05:
        trig_render_md05(&lv);
        break;

    case RendVec_mode06:
        trig_render_md06(&lv);
        break;

    case RendVec_mode07:
    case RendVec_mode11:
        if (vec_colour == 0x20)
            trig_render_md02(&lv);
        else
            trig_render_md07(&lv);
        break;

    case RendVec_mode08:
        trig_render_md08(&lv);
        break;

    case RendVec_mode09:
        trig_render_md09(&lv);
        break;

    case RendVec_mode10:
        trig_render_md10(&lv);
        break;

    case RendVec_mode12:
        trig_render_md12(&lv);
        break;

    case RendVec_mode13:
        trig_render_md13(&lv);
        break;

    case RendVec_mode14:
        trig_render_md14(&lv);
        break;

    case RendVec_mode15:
        trig_render_md15(&lv);
        break;

    case RendVec_mode16:
        trig_render_md16(&lv);
        break;

    case RendVec_mode17:
        trig_render_md17(&lv);
        break;

    case RendVec_mode18:
        trig_render_md18(&lv);
        break;

    case RendVec_mode19:
        trig_render_md19(&lv);
        break;

    case RendVec_mode20:
        trig_render_md20(&lv);
        break;

    case RendVec_mode21:
        trig_render_md21(&lv);
        break;

    case RendVec_mode22:
        trig_render_md22(&lv);
        break;

    case RendVec_mode23:
        trig_render_md23(&lv);
        break;

    case RendVec_mode24:
        trig_render_md24(&lv);
        break;

    case RendVec_mode25:
        trig_render_md25(&lv);
        break;

    case RendVec_mode26:
        trig_render_md26(&lv);
        break;
    }

    LOGNO("end");
}

ubyte trig_reorder_input_points(struct PolyPoint **opt_a,
  struct PolyPoint **opt_b, struct PolyPoint **opt_c)
{
    struct PolyPoint *ordpt_a;
    struct PolyPoint *ordpt_b;
    struct PolyPoint *ordpt_c;
    ubyte start_type;

#if 0
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
#else
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
#endif
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
