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

/** Triangle rendering function.
 *
 * @param point_a
 * @param point_b
 * @param point_c
 */
void trig(struct PolyPoint *point_a, struct PolyPoint *point_b, struct PolyPoint *point_c)
{
#if 0
    asm volatile ("call ASM_trig\n"
        :  : "a" (point_a), "d" (point_b), "b" (point_c));
    return;
#endif
    static struct TrigLocals lv;

    asm volatile (" \
            mov    %%eax,%%esi\n \
            mov    %%edx,%%edi\n \
            mov    %%ebx,%%ecx\n \
            mov    0x4(%%esi),%%eax\n \
            mov    0x4(%%edi),%%ebx\n \
            mov    0x4(%%ecx),%%edx\n \
            cmp    %%ebx,%%eax\n \
            je     jump_120dff\n \
            jg     jump_120dc5\n \
            cmp    %%edx,%%eax\n \
            je     jump_120dec\n \
            jl     jump_120e3a\n \
            xchg   %%esi,%%ecx\n \
            xchg   %%edi,%%ecx\n \
            jmp    jump_120e44\n \
        jump_120dc5:\n \
            cmp    %%edx,%%eax\n \
            je     jump_1220b4\n \
            jl     jump_12174d\n \
            cmp    %%edx,%%ebx\n \
            je     jump_122533\n \
            jl     jump_120de6\n \
            xchg   %%esi,%%ecx\n \
            xchg   %%edi,%%ecx\n \
            jmp    jump_121751\n \
        jump_120de6:\n \
            xchg   %%esi,%%edi\n \
            xchg   %%edi,%%ecx\n \
            jmp    jump_120e44\n \
        jump_120dec:\n \
            mov    (%%esi),%%eax\n \
            cmp    (%%ecx),%%eax\n \
            jle    jump_122a92\n \
            xchg   %%esi,%%ecx\n \
            xchg   %%edi,%%ecx\n \
            jmp    jump_122541\n \
        jump_120dff:\n \
            cmp    %%edx,%%eax\n \
            je     jump_122a92\n \
            jl     jump_120e1c\n \
            mov    (%%esi),%%eax\n \
            cmp    (%%edi),%%eax\n \
            jle    jump_122a92\n \
            xchg   %%esi,%%ecx\n \
            xchg   %%edi,%%ecx\n \
            jmp    jump_1220c2\n \
        jump_120e1c:\n \
            mov    (%%edi),%%eax\n \
            cmp    (%%esi),%%eax\n \
            jle    jump_122a92\n \
            jmp    jump_122541\n \
        jump_120e2b:\n \
            mov    (%%edi),%%eax\n \
            cmp    (%%ecx),%%eax\n \
            jle    jump_122a92\n \
            jmp    jump_1220c2\n \
        jump_120e3a:\n \
            cmp    %%edx,%%ebx\n \
            je     jump_120e2b\n \
            jg     jump_121751\n \
        jump_120e44:\n \
            mov    0x4(%%esi),%%eax\n \
            mov    %%eax,0x54+%[lv]\n \
            or     %%eax,%%eax\n \
            jns    jump_120e5f\n \
            mov    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x1,0x66+%[lv]\n \
            jmp    jump_120e82\n \
        jump_120e5f:\n \
            cmp    "EXPORT_SYMBOL(vec_window_height)",%%eax\n \
            jge    jump_122a92\n \
            mov    %%eax,%%ebx\n \
            imul   "EXPORT_SYMBOL(vec_screen_width)",%%ebx\n \
            add    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x0,0x66+%[lv]\n \
        jump_120e82:\n \
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
            jle    jump_122a92\n \
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
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *ll_jt(,%%eax,4)\n \
\n \
ll_jt: /* swars-final @ 0x120F07 */\n \
            .long   ll_md00\n \
            .long   ll_md01\n \
            .long   ll_md02\n \
            .long   ll_md02\n \
            .long   ll_md01\n \
            .long   ll_md05\n \
            .long   ll_md05\n \
            .long   ll_md02\n \
            .long   ll_md02\n \
            .long   ll_md02\n \
            .long   ll_md02\n \
            .long   ll_md02\n \
            .long   ll_md02\n \
            .long   ll_md02\n \
            .long   ll_md00\n \
            .long   ll_md00\n \
            .long   ll_md01\n \
            .long   ll_md01\n \
            .long   ll_md02\n \
            .long   ll_md02\n \
            .long   ll_md05\n \
            .long   ll_md05\n \
            .long   ll_md02\n \
            .long   ll_md02\n \
            .long   ll_md05\n \
            .long   ll_md05\n \
            .long   ll_md05\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
ll_md05:\n \
/*----------------------------------------------------------------*/\n \
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
            jl     jump_122a92\n \
            je     jump_120fee\n \
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
        jump_120fee:\n \
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
            je     jump_12114f\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_122a92\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x14+%[lv],%%edi\n \
            js     jump_1210e7\n \
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
            je     jump_1210dc\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_1210dc:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_1211b3\n \
        jump_1210e7:\n \
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
            je     jump_12117e\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_121140\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_12114d\n \
        jump_121140:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_12114d:\n \
            jmp    jump_12117e\n \
        jump_12114f:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_12117e\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_121171\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_12117e\n \
        jump_121171:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_12117e:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_121184:\n \
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
            jne    jump_121184\n \
            mov    0x1c+%[lv],%%ebx\n \
        jump_1211b3:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_1211c8\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
        jump_1211c8:\n \
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
            jne    jump_1211c8\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
ll_md02:\n \
/*----------------------------------------------------------------*/\n \
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
            jl     jump_122a92\n \
            je     jump_121261\n \
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
        jump_121261:\n \
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
            je     jump_12138b\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_122a92\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x14+%[lv],%%edi\n \
            js     jump_12132e\n \
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
            je     jump_121323\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_121323:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_1213e8\n \
        jump_12132e:\n \
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
            je     jump_1213ba\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_12137c\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_121389\n \
        jump_12137c:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_121389:\n \
            jmp    jump_1213ba\n \
        jump_12138b:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_1213ba\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_1213ad\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_1213ba\n \
        jump_1213ad:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_1213ba:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_1213c0:\n \
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
            jne    jump_1213c0\n \
            mov    0x1c+%[lv],%%ebx\n \
        jump_1213e8:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_1213fd\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
        jump_1213fd:\n \
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
            jne    jump_1213fd\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
ll_md01:\n \
/*----------------------------------------------------------------*/\n \
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
            jl     jump_122a92\n \
            je     jump_121474\n \
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
        jump_121474:\n \
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
            je     jump_12156b\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_122a92\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x14+%[lv],%%edi\n \
            js     jump_121519\n \
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
            je     jump_12150e\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_12150e:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_1215c1\n \
        jump_121519:\n \
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
            je     jump_12159a\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_12155c\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_121569\n \
        jump_12155c:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_121569:\n \
            jmp    jump_12159a\n \
        jump_12156b:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_12159a\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_12158d\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_12159a\n \
        jump_12158d:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_12159a:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_1215a0:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x40+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x14+%[lv]\n \
            jne    jump_1215a0\n \
            mov    0x1c+%[lv],%%ebx\n \
        jump_1215c1:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_1215d6\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
        jump_1215d6:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0xc+%[lv],%%ebx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x40+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x18+%[lv]\n \
            jne    jump_1215d6\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
ll_md00:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_1216c5\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_122a92\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x14+%[lv],%%edi\n \
            js     jump_12167e\n \
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
            je     jump_121673\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_121673:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_121714\n \
        jump_12167e:\n \
            mov    0x48+%[lv],%%edi\n \
            sub    %%edi,0x14+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_1216f4\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_1216b6\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_1216c3\n \
        jump_1216b6:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_1216c3:\n \
            jmp    jump_1216f4\n \
        jump_1216c5:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_1216f4\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_1216e7\n \
            mov    %%edi,0x14+%[lv]\n \
            jmp    jump_1216f4\n \
        jump_1216e7:\n \
            sub    0x14+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_1216f4:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_1216fa:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            add    $0x14,%%edi\n \
            decl   0x14+%[lv]\n \
            jne    jump_1216fa\n \
            mov    0x1c+%[lv],%%ebx\n \
        jump_121714:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_121729\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
        jump_121729:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0xc+%[lv],%%ebx\n \
            add    $0x14,%%edi\n \
            decl   0x18+%[lv]\n \
            jne    jump_121729\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
        jump_12174d:\n \
            xchg   %%esi,%%edi\n \
            xchg   %%edi,%%ecx\n \
        jump_121751:\n \
            mov    0x4(%%esi),%%eax\n \
            mov    %%eax,0x54+%[lv]\n \
            or     %%eax,%%eax\n \
            jns    jump_12176c\n \
            mov    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x1,0x66+%[lv]\n \
            jmp    jump_12178f\n \
        jump_12176c:\n \
            cmp    "EXPORT_SYMBOL(vec_window_height)",%%eax\n \
            jge    jump_122a92\n \
            mov    %%eax,%%ebx\n \
            imul   "EXPORT_SYMBOL(vec_screen_width)",%%ebx\n \
            add    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x0,0x66+%[lv]\n \
        jump_12178f:\n \
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
            jle    jump_122a92\n \
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
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *rl_jt(,%%eax,4)\n \
\n \
rl_jt: /* swars-final @ 0x121814 */\n \
            .long   rl_md00\n \
            .long   rl_md01\n \
            .long   rl_md02\n \
            .long   rl_md02\n \
            .long   rl_md01\n \
            .long   rl_md05\n \
            .long   rl_md05\n \
            .long   rl_md02\n \
            .long   rl_md02\n \
            .long   rl_md02\n \
            .long   rl_md02\n \
            .long   rl_md02\n \
            .long   rl_md02\n \
            .long   rl_md02\n \
            .long   rl_md00\n \
            .long   rl_md00\n \
            .long   rl_md01\n \
            .long   rl_md01\n \
            .long   rl_md02\n \
            .long   rl_md02\n \
            .long   rl_md05\n \
            .long   rl_md05\n \
            .long   rl_md02\n \
            .long   rl_md02\n \
            .long   rl_md05\n \
            .long   rl_md05\n \
            .long   rl_md05\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
rl_md05:\n \
/*----------------------------------------------------------------*/\n \
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
            jl     jump_122a92\n \
            je     jump_1218f7\n \
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
        jump_1218f7:\n \
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
            je     jump_121a89\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_122a92\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x10+%[lv],%%edi\n \
            js     jump_121a21\n \
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
            je     jump_121a16\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_121a16:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_121aed\n \
        jump_121a21:\n \
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
            je     jump_121ab8\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_121a7a\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_121a87\n \
        jump_121a7a:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_121a87:\n \
            jmp    jump_121ab8\n \
        jump_121a89:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_121ab8\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_121aab\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_121ab8\n \
        jump_121aab:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_121ab8:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_121abe:\n \
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
            jne    jump_121abe\n \
            mov    0x1c+%[lv],%%eax\n \
        jump_121aed:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_121b02\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
        jump_121b02:\n \
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
            jne    jump_121b02\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
rl_md02:\n \
/*----------------------------------------------------------------*/\n \
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
            jl     jump_122a92\n \
            je     jump_121b9b\n \
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
        jump_121b9b:\n \
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
            je     jump_121ce3\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_122a92\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x10+%[lv],%%edi\n \
            js     jump_121c86\n \
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
            je     jump_121c7b\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_121c7b:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_121d40\n \
        jump_121c86:\n \
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
            je     jump_121d12\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_121cd4\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_121ce1\n \
        jump_121cd4:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_121ce1:\n \
            jmp    jump_121d12\n \
        jump_121ce3:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_121d12\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_121d05\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_121d12\n \
        jump_121d05:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_121d12:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_121d18:\n \
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
            jne    jump_121d18\n \
            mov    0x1c+%[lv],%%eax\n \
        jump_121d40:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_121d55\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
        jump_121d55:\n \
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
            jne    jump_121d55\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
rl_md01:\n \
/*----------------------------------------------------------------*/\n \
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
            jl     jump_122a92\n \
            je     jump_121dcc\n \
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
        jump_121dcc:\n \
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
            je     jump_121ed2\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_122a92\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x10+%[lv],%%edi\n \
            js     jump_121e80\n \
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
            je     jump_121e75\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_121e75:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_121f28\n \
        jump_121e80:\n \
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
            je     jump_121f01\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_121ec3\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_121ed0\n \
        jump_121ec3:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_121ed0:\n \
            jmp    jump_121f01\n \
        jump_121ed2:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_121f01\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_121ef4\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_121f01\n \
        jump_121ef4:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_121f01:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_121f07:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x40+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x10+%[lv]\n \
            jne    jump_121f07\n \
            mov    0x1c+%[lv],%%eax\n \
        jump_121f28:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_121f3d\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
        jump_121f3d:\n \
            mov    %%eax,(%%edi)\n \
            add    0xc+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            mov    %%esi,0x10(%%edi)\n \
            add    0x44+%[lv],%%esi\n \
            add    $0x14,%%edi\n \
            decl   0x18+%[lv]\n \
            jne    jump_121f3d\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
rl_md00:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_12202c\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_122a92\n \
            mov    %%edi,0x48+%[lv]\n \
            cmp    0x10+%[lv],%%edi\n \
            js     jump_121fe5\n \
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
            je     jump_121fda\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x18+%[lv]\n \
            mov    %%edi,0x20+%[lv]\n \
        jump_121fda:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
            jmp    jump_12207b\n \
        jump_121fe5:\n \
            mov    0x48+%[lv],%%edi\n \
            sub    %%edi,0x10+%[lv]\n \
            imul   0x4+%[lv],%%edi\n \
            add    %%edi,%%eax\n \
            mov    0x8+%[lv],%%edi\n \
            imul   0x48+%[lv],%%edi\n \
            add    %%edi,%%ebx\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_12205b\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_12201d\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_12202a\n \
        jump_12201d:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_12202a:\n \
            jmp    jump_12205b\n \
        jump_12202c:\n \
            cmpb   $0x0,0x68+%[lv]\n \
            je     jump_12205b\n \
            mov    "EXPORT_SYMBOL(vec_window_height)",%%edi\n \
            sub    0x54+%[lv],%%edi\n \
            mov    %%edi,0x20+%[lv]\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_12204e\n \
            mov    %%edi,0x10+%[lv]\n \
            jmp    jump_12205b\n \
        jump_12204e:\n \
            sub    0x10+%[lv],%%edi\n \
            setle  0x67+%[lv]\n \
            mov    %%edi,0x18+%[lv]\n \
        jump_12205b:\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%edi\n \
        jump_122061:\n \
            mov    %%eax,(%%edi)\n \
            add    0x4+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            add    $0x14,%%edi\n \
            decl   0x10+%[lv]\n \
            jne    jump_122061\n \
            mov    0x1c+%[lv],%%eax\n \
        jump_12207b:\n \
            cmpb   $0x0,0x67+%[lv]\n \
            je     jump_122090\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
        jump_122090:\n \
            mov    %%eax,(%%edi)\n \
            add    0xc+%[lv],%%eax\n \
            mov    %%ebx,0x4(%%edi)\n \
            add    0x8+%[lv],%%ebx\n \
            add    $0x14,%%edi\n \
            decl   0x18+%[lv]\n \
            jne    jump_122090\n \
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
        jump_1220b4:\n \
            mov    (%%ecx),%%eax\n \
            cmp    (%%esi),%%eax\n \
            jle    jump_122a92\n \
            xchg   %%esi,%%edi\n \
            xchg   %%edi,%%ecx\n \
        jump_1220c2:\n \
            mov    0x4(%%esi),%%eax\n \
            mov    %%eax,0x54+%[lv]\n \
            or     %%eax,%%eax\n \
            jns    jump_1220dd\n \
            mov    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x1,0x66+%[lv]\n \
            jmp    jump_122100\n \
        jump_1220dd:\n \
            cmp    "EXPORT_SYMBOL(vec_window_height)",%%eax\n \
            jge    jump_122a92\n \
            mov    %%eax,%%ebx\n \
            imul   "EXPORT_SYMBOL(vec_screen_width)",%%ebx\n \
            add    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x0,0x66+%[lv]\n \
        jump_122100:\n \
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
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *fb_jt(,%%eax,4)\n \
\n \
fb_jt: /* swars-final @ 0x122142, genewars-beta @ 0xEFE72 */\n \
            .long   fb_md00\n \
            .long   fb_md01\n \
            .long   fb_md02\n \
            .long   fb_md02\n \
            .long   fb_md01\n \
            .long   fb_md03\n \
            .long   fb_md03\n \
            .long   fb_md02\n \
            .long   fb_md02\n \
            .long   fb_md02\n \
            .long   fb_md02\n \
            .long   fb_md02\n \
            .long   fb_md02\n \
            .long   fb_md02\n \
            .long   fb_md00\n \
            .long   fb_md00\n \
            .long   fb_md01\n \
            .long   fb_md01\n \
            .long   fb_md02\n \
            .long   fb_md02\n \
            .long   fb_md03\n \
            .long   fb_md03\n \
            .long   fb_md02\n \
            .long   fb_md02\n \
            .long   fb_md03\n \
            .long   fb_md03\n \
            .long   fb_md03\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
fb_md03:\n \
/*----------------------------------------------------------------*/\n \
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
            jle    jump_122a92\n \
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
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
fb_md02:\n \
/*----------------------------------------------------------------*/\n \
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
            jle    jump_122a92\n \
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
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
fb_md01:\n \
/*----------------------------------------------------------------*/\n \
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
            jle    jump_122a92\n \
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
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
fb_md00:\n \
/*----------------------------------------------------------------*/\n \
            mov    (%%esi),%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,%%ebx\n \
            cmpb   $0x0,0x66+%[lv]\n \
            je     jump_1224f0\n \
            mov    0x54+%[lv],%%edi\n \
            neg    %%edi\n \
            sub    %%edi,0x10+%[lv]\n \
            sub    %%edi,0x20+%[lv]\n \
            jle    jump_122a92\n \
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
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
        jump_122533:\n \
            mov    (%%ecx),%%eax\n \
            cmp    (%%edi),%%eax\n \
            jle    jump_122a92\n \
            xchg   %%esi,%%edi\n \
            xchg   %%edi,%%ecx\n \
        jump_122541:\n \
            mov    0x4(%%esi),%%eax\n \
            mov    %%eax,0x54+%[lv]\n \
            or     %%eax,%%eax\n \
            jns    jump_12255c\n \
            mov    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x1,0x66+%[lv]\n \
            jmp    jump_12257f\n \
        jump_12255c:\n \
            cmp    "EXPORT_SYMBOL(vec_window_height)",%%eax\n \
            jge    jump_122a92\n \
            mov    %%eax,%%ebx\n \
            imul   "EXPORT_SYMBOL(vec_screen_width)",%%ebx\n \
            add    "EXPORT_SYMBOL(poly_screen)",%%ebx\n \
            mov    %%ebx,0x0+%[lv]\n \
            movb   $0x0,0x66+%[lv]\n \
        jump_12257f:\n \
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
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *ft_jt(,%%eax,4)\n \
\n \
ft_jt: /* swars-final @ 0x1225c1, genewars-beta @ 0xF02F1 */\n \
            .long   ft_md00\n \
            .long   ft_md01\n \
            .long   ft_md02\n \
            .long   ft_md02\n \
            .long   ft_md01\n \
            .long   ft_md05\n \
            .long   ft_md05\n \
            .long   ft_md02\n \
            .long   ft_md02\n \
            .long   ft_md02\n \
            .long   ft_md02\n \
            .long   ft_md02\n \
            .long   ft_md02\n \
            .long   ft_md02\n \
            .long   ft_md00\n \
            .long   ft_md00\n \
            .long   ft_md01\n \
            .long   ft_md01\n \
            .long   ft_md02\n \
            .long   ft_md02\n \
            .long   ft_md05\n \
            .long   ft_md05\n \
            .long   ft_md02\n \
            .long   ft_md02\n \
            .long   ft_md05\n \
            .long   ft_md05\n \
            .long   ft_md05\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
ft_md05:\n \
/*----------------------------------------------------------------*/\n \
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
            jle    jump_122a92\n \
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
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
ft_md02:\n \
/*----------------------------------------------------------------*/\n \
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
            jle    jump_122a92\n \
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
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
ft_md01:\n \
/*----------------------------------------------------------------*/\n \
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
            jle    jump_122a92\n \
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
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
ft_md00:\n \
/*----------------------------------------------------------------*/\n \
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
            jle    jump_122a92\n \
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
            movzbl "EXPORT_SYMBOL(vec_mode)",%%eax\n \
            jmp    *render_table(,%%eax,4)\n \
\n \
render_table: /* swars-final @ 0x1229b3, genewars-beta @ 0xF06E3 */\n \
            .long   render_md00\n \
            .long   render_md01\n \
            .long   render_md02\n \
            .long   render_md03\n \
            .long   render_md04\n \
            .long   render_md05\n \
            .long   render_md06\n \
            .long   render_md07\n \
            .long   render_md08\n \
            .long   render_md09\n \
            .long   render_md10\n \
            .long   render_md07\n \
            .long   render_md12\n \
            .long   render_md13\n \
            .long   render_md14\n \
            .long   render_md15\n \
            .long   render_md16\n \
            .long   render_md17\n \
            .long   render_md18\n \
            .long   render_md19\n \
            .long   render_md20\n \
            .long   render_md21\n \
            .long   render_md22\n \
            .long   render_md23\n \
            .long   render_md24\n \
            .long   render_md25\n \
            .long   render_md26\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md00:\n \
/*----------------------------------------------------------------*/\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x0+%[lv],%%edx\n \
            mov    "EXPORT_SYMBOL(vec_colour)",%%al\n \
            mov    %%al,%%ah\n \
            mov    %%ax,%%bx\n \
            shl    $0x10,%%eax\n \
            mov    %%bx,%%ax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_122a3c:\n \
            mov    0x2(%%esi),%%bx\n \
            movzwl 0x6(%%esi),%%ecx\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edx\n \
            or     %%bx,%%bx\n \
            jns    jump_122a66\n \
            or     %%cx,%%cx\n \
            jle    jump_122a89\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_122a62\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_122a62:\n \
            mov    %%edx,%%edi\n \
            jmp    jump_122a7c\n \
        jump_122a66:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_122a74\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_122a74:\n \
            sub    %%bx,%%cx\n \
            jle    jump_122a89\n \
            lea    (%%ebx,%%edx,1),%%edi\n \
        jump_122a7c:\n \
            shr    %%ecx\n \
            jae    jump_122a81\n \
            stos   %%al,%%es:(%%edi)\n \
        jump_122a81:\n \
            shr    %%ecx\n \
            jae    jump_122a87\n \
            stos   %%ax,%%es:(%%edi)\n \
        jump_122a87:\n \
            rep stos %%eax,%%es:(%%edi)\n \
        jump_122a89:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_122a3c\n \
        jump_122a92:\n \
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md01:\n \
/*----------------------------------------------------------------*/\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_122aa1:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_122af3\n \
            or     %%cx,%%cx\n \
            jle    jump_122c41\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            imul   0x3c+%[lv],%%eax\n \
            mov    %%ax,%%bx\n \
            shr    $0x8,%%eax\n \
            add    0x10(%%esi),%%bx\n \
            adc    0x12(%%esi),%%ah\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_122ae9\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_122ae9:\n \
            movzwl %%ax,%%eax\n \
            mov    "EXPORT_SYMBOL(vec_colour)",%%al\n \
            jmp    jump_122b1a\n \
        jump_122af3:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_122b01\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_122b01:\n \
            sub    %%ax,%%cx\n \
            jle    jump_122c41\n \
            add    %%eax,%%edi\n \
            movzbl "EXPORT_SYMBOL(vec_colour)",%%eax\n \
            mov    0x10(%%esi),%%bx\n \
            mov    0x12(%%esi),%%ah\n \
        jump_122b1a:\n \
            mov    %%ah,(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0x1(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0x2(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0x3(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0x4(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0x5(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0x6(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0x7(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0x8(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0x9(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0xa(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0xb(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0xc(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0xd(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0xe(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            mov    %%ah,0xf(%%edi)\n \
            add    0x3c+%[lv],%%bx\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            je     jump_122c41\n \
            add    $0x10,%%edi\n \
            jmp    jump_122b1a\n \
        jump_122c41:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_122aa1\n \
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md02:\n \
/*----------------------------------------------------------------*/\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_122c6a:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_122cc4\n \
            or     %%cx,%%cx\n \
            jle    jump_122edd\n \
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
            jle    jump_122cbf\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_122cbf:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_122cec\n \
        jump_122cc4:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_122cd2\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_122cd2:\n \
            sub    %%ax,%%cx\n \
            jle    jump_122edd\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_122cec:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_122cf6:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x1(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x2(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x3(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x4(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x5(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x6(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x7(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x8(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0x9(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0xa(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0xb(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0xc(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0xd(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0xe(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            mov    %%al,0xf(%%edi)\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_122ed9\n \
            add    $0x10,%%edi\n \
            jmp    jump_122cf6\n \
        jump_122ed9:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_122edd:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_122c6a\n \
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md03:\n \
/*----------------------------------------------------------------*/\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_122f06:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_122f60\n \
            or     %%cx,%%cx\n \
            jle    jump_1231bd\n \
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
            jle    jump_122f5b\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_122f5b:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_122f88\n \
        jump_122f60:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_122f6e\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_122f6e:\n \
            sub    %%ax,%%cx\n \
            jle    jump_1231bd\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_122f88:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_122f92:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_122f9b\n \
            mov    %%al,(%%edi)\n \
        jump_122f9b:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_122fbe\n \
            mov    %%al,0x1(%%edi)\n \
        jump_122fbe:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_122fe1\n \
            mov    %%al,0x2(%%edi)\n \
        jump_122fe1:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_123004\n \
            mov    %%al,0x3(%%edi)\n \
        jump_123004:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_123027\n \
            mov    %%al,0x4(%%edi)\n \
        jump_123027:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12304a\n \
            mov    %%al,0x5(%%edi)\n \
        jump_12304a:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12306d\n \
            mov    %%al,0x6(%%edi)\n \
        jump_12306d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_123090\n \
            mov    %%al,0x7(%%edi)\n \
        jump_123090:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1230b3\n \
            mov    %%al,0x8(%%edi)\n \
        jump_1230b3:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1230d6\n \
            mov    %%al,0x9(%%edi)\n \
        jump_1230d6:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1230f9\n \
            mov    %%al,0xa(%%edi)\n \
        jump_1230f9:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12311c\n \
            mov    %%al,0xb(%%edi)\n \
        jump_12311c:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12313f\n \
            mov    %%al,0xc(%%edi)\n \
        jump_12313f:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12315e\n \
            mov    %%al,0xd(%%edi)\n \
        jump_12315e:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12317d\n \
            mov    %%al,0xe(%%edi)\n \
        jump_12317d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12319c\n \
            mov    %%al,0xf(%%edi)\n \
        jump_12319c:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_1231b9\n \
            add    $0x10,%%edi\n \
            jmp    jump_122f92\n \
        jump_1231b9:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_1231bd:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_122f06\n \
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md04:\n \
/*----------------------------------------------------------------*/\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_1231d9:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_12322b\n \
            or     %%cx,%%cx\n \
            jle    jump_1233e1\n \
            neg    %%ax\n \
            movzwl %%ax,%%eax\n \
            imul   0x3c+%[lv],%%eax\n \
            mov    %%ax,%%bx\n \
            shr    $0x8,%%eax\n \
            add    0x10(%%esi),%%bx\n \
            adc    0x12(%%esi),%%ah\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_123221\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_123221:\n \
            movzwl %%ax,%%eax\n \
            mov    "EXPORT_SYMBOL(vec_colour)",%%al\n \
            jmp    jump_123252\n \
        jump_12322b:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_123239\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_123239:\n \
            sub    %%ax,%%cx\n \
            jle    jump_1233e1\n \
            add    %%eax,%%edi\n \
            movzbl "EXPORT_SYMBOL(vec_colour)",%%eax\n \
            mov    0x10(%%esi),%%bx\n \
            mov    0x12(%%esi),%%ah\n \
        jump_123252:\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x1(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x2(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x3(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x4(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x5(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x6(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x7(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x8(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0x9(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0xa(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0xb(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0xc(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0xd(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0xe(%%edi)\n \
            je     jump_1233e1\n \
            add    0x3c+%[lv],%%bx\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%dl\n \
            adc    0x3e+%[lv],%%ah\n \
            dec    %%cx\n \
            mov    %%dl,0xf(%%edi)\n \
            je     jump_1233e1\n \
            add    $0x10,%%edi\n \
            jmp    jump_123252\n \
        jump_1233e1:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_1231d9\n \
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md05:\n \
/*----------------------------------------------------------------*/\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    %%esi,0x5c+%[lv]\n \
            xor    %%ebx,%%ebx\n \
            mov    0x24+%[lv],%%ecx\n \
            mov    0x30+%[lv],%%edx\n \
            mov    0x3c+%[lv],%%ebp\n \
            cmp    $0x0,%%ebp\n \
            jae    jump_123451\n \
            dec    %%ecx\n \
        jump_123451:\n \
            rol    $0x10,%%ecx\n \
            rol    $0x10,%%edx\n \
            shr    $0x8,%%ebp\n \
            mov    %%dl,%%bl\n \
            mov    %%cl,%%dl\n \
            mov    %%bp,%%cx\n \
            mov    %%ecx,0x4c+%[lv]\n \
            mov    %%edx,0x50+%[lv]\n \
            mov    %%bl,0x64+%[lv]\n \
        jump_12346d:\n \
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
            jns    jump_1234db\n \
            or     %%ebp,%%ebp\n \
            jle    jump_12373b\n \
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
            jle    jump_1234d9\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ebp\n \
        jump_1234d9:\n \
            jmp    jump_12350e\n \
        jump_1234db:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ebp\n \
            jle    jump_1234e9\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ebp\n \
        jump_1234e9:\n \
            sub    %%eax,%%ebp\n \
            jle    jump_12373b\n \
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
        jump_12350e:\n \
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
        jump_12373b:\n \
            decl   0x20+%[lv]\n \
            jne    jump_12346d\n \
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md06:\n \
/*----------------------------------------------------------------*/\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md07:\n \
/*----------------------------------------------------------------*/\n \
            cmpb   $0x20,"EXPORT_SYMBOL(vec_colour)"\n \
            je     render_md02\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md08:\n \
/*----------------------------------------------------------------*/\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md09:\n \
/*----------------------------------------------------------------*/\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md10:\n \
/*----------------------------------------------------------------*/\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md12:\n \
/*----------------------------------------------------------------*/\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md13:\n \
/*----------------------------------------------------------------*/\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md14:\n \
/*----------------------------------------------------------------*/\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md15:\n \
/*----------------------------------------------------------------*/\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md16:\n \
/*----------------------------------------------------------------*/\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md17:\n \
/*----------------------------------------------------------------*/\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md18:\n \
/*----------------------------------------------------------------*/\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md19:\n \
/*----------------------------------------------------------------*/\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md20:\n \
/*----------------------------------------------------------------*/\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md21:\n \
/*----------------------------------------------------------------*/\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md22:\n \
/*----------------------------------------------------------------*/\n \
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
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md23:\n \
/*----------------------------------------------------------------*/\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    0x30+%[lv],%%eax\n \
            shl    $0x10,%%eax\n \
            mov    %%eax,0x4c+%[lv]\n \
            xor    %%eax,%%eax\n \
            xor    %%ebx,%%ebx\n \
            xor    %%ecx,%%ecx\n \
        jump_1269f0:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_126a4a\n \
            or     %%cx,%%cx\n \
            jle    jump_126d3a\n \
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
            jle    jump_126a45\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_126a45:\n \
            movzwl %%ax,%%eax\n \
            jmp    jump_126a72\n \
        jump_126a4a:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_126a58\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_126a58:\n \
            sub    %%ax,%%cx\n \
            jle    jump_126d3a\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
        jump_126a72:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_126a7c:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126a8d\n \
            mov    (%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_126a8d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126ab9\n \
            mov    0x1(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x1(%%edi)\n \
        jump_126ab9:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126ae5\n \
            mov    0x2(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x2(%%edi)\n \
        jump_126ae5:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126b11\n \
            mov    0x3(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x3(%%edi)\n \
        jump_126b11:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126b3d\n \
            mov    0x4(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x4(%%edi)\n \
        jump_126b3d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126b69\n \
            mov    0x5(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x5(%%edi)\n \
        jump_126b69:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126b95\n \
            mov    0x6(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x6(%%edi)\n \
        jump_126b95:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126bc1\n \
            mov    0x7(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x7(%%edi)\n \
        jump_126bc1:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126bed\n \
            mov    0x8(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x8(%%edi)\n \
        jump_126bed:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126c19\n \
            mov    0x9(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x9(%%edi)\n \
        jump_126c19:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126c45\n \
            mov    0xa(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xa(%%edi)\n \
        jump_126c45:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126c71\n \
            mov    0xb(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xb(%%edi)\n \
        jump_126c71:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126c9d\n \
            mov    0xc(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xc(%%edi)\n \
        jump_126c9d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126cc9\n \
            mov    0xd(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xd(%%edi)\n \
        jump_126cc9:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126cf1\n \
            mov    0xe(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xe(%%edi)\n \
        jump_126cf1:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126d19\n \
            mov    0xf(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
        jump_126d19:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            dec    %%cx\n \
            je     jump_126d36\n \
            add    $0x10,%%edi\n \
            jmp    jump_126a7c\n \
        jump_126d36:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_126d3a:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_1269f0\n \
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md24:\n \
/*----------------------------------------------------------------*/\n \
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
        jump_126d6e:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_126dd9\n \
            or     %%cx,%%cx\n \
            jle    jump_1271f3\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_126d9e\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_126d9e:\n \
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
            jmp    jump_126e0b\n \
        jump_126dd9:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_126de7\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_126de7:\n \
            sub    %%ax,%%cx\n \
            jle    jump_1271f3\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
            mov    %%ecx,0x58+%[lv]\n \
            mov    0x10(%%esi),%%ecx\n \
            rol    $0x10,%%ecx\n \
        jump_126e0b:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_126e15:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126e2e\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    (%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_126e2e:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126e6c\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x1(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x1(%%edi)\n \
        jump_126e6c:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126eaa\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x2(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x2(%%edi)\n \
        jump_126eaa:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126ee8\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x3(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x3(%%edi)\n \
        jump_126ee8:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126f26\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x4(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x4(%%edi)\n \
        jump_126f26:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126f64\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x5(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x5(%%edi)\n \
        jump_126f64:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126fa2\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x6(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x6(%%edi)\n \
        jump_126fa2:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_126fe0\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x7(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x7(%%edi)\n \
        jump_126fe0:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12701e\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x8(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x8(%%edi)\n \
        jump_12701e:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12705c\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0x9(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x9(%%edi)\n \
        jump_12705c:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12709a\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0xa(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xa(%%edi)\n \
        jump_12709a:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1270d8\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0xb(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xb(%%edi)\n \
        jump_1270d8:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_127116\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0xc(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xc(%%edi)\n \
        jump_127116:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_127154\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0xd(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xd(%%edi)\n \
        jump_127154:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12718e\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0xe(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xe(%%edi)\n \
        jump_12718e:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1271c8\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%ah\n \
            mov    0xf(%%edi),%%al\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
        jump_1271c8:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1271ef\n \
            add    $0x10,%%edi\n \
            jmp    jump_126e15\n \
        jump_1271ef:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_1271f3:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_126d6e\n \
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md25:\n \
/*----------------------------------------------------------------*/\n \
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
        jump_127227:\n \
            mov    0x2(%%esi),%%ax\n \
            movzwl 0x6(%%esi),%%ecx\n \
            mov    0x0+%[lv],%%edi\n \
            add    "EXPORT_SYMBOL(vec_screen_width)",%%edi\n \
            mov    %%edi,0x0+%[lv]\n \
            or     %%ax,%%ax\n \
            jns    jump_127292\n \
            or     %%cx,%%cx\n \
            jle    jump_1276ac\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_127257\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_127257:\n \
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
            jmp    jump_1272c4\n \
        jump_127292:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
            jle    jump_1272a0\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ecx\n \
        jump_1272a0:\n \
            sub    %%ax,%%cx\n \
            jle    jump_1276ac\n \
            add    %%eax,%%edi\n \
            mov    0xc(%%esi),%%edx\n \
            rol    $0x10,%%edx\n \
            mov    %%dl,%%bh\n \
            mov    0x8(%%esi),%%dx\n \
            mov    0xa(%%esi),%%bl\n \
            mov    %%ecx,0x58+%[lv]\n \
            mov    0x10(%%esi),%%ecx\n \
            rol    $0x10,%%ecx\n \
        jump_1272c4:\n \
            mov    %%esi,0x5c+%[lv]\n \
            mov    "EXPORT_SYMBOL(vec_map)",%%esi\n \
        jump_1272ce:\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1272e7\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    (%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,(%%edi)\n \
        jump_1272e7:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_127325\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x1(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x1(%%edi)\n \
        jump_127325:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_127363\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x2(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x2(%%edi)\n \
        jump_127363:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1273a1\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x3(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x3(%%edi)\n \
        jump_1273a1:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1273df\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x4(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x4(%%edi)\n \
        jump_1273df:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12741d\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x5(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x5(%%edi)\n \
        jump_12741d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12745b\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x6(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x6(%%edi)\n \
        jump_12745b:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_127499\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x7(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x7(%%edi)\n \
        jump_127499:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1274d7\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x8(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x8(%%edi)\n \
        jump_1274d7:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_127515\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0x9(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0x9(%%edi)\n \
        jump_127515:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_127553\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xa(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xa(%%edi)\n \
        jump_127553:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_127591\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xb(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xb(%%edi)\n \
        jump_127591:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_1275cf\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xc(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xc(%%edi)\n \
        jump_1275cf:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_12760d\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xd(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xd(%%edi)\n \
        jump_12760d:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_127647\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xe(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xe(%%edi)\n \
        jump_127647:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            or     %%al,%%al\n \
            je     jump_127681\n \
            mov    %%cl,%%ah\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xf(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
        jump_127681:\n \
            add    0x24+%[lv],%%dx\n \
            adc    0x26+%[lv],%%bl\n \
            add    0x4c+%[lv],%%edx\n \
            adc    0x32+%[lv],%%bh\n \
            add    0x50+%[lv],%%ecx\n \
            adc    0x3e+%[lv],%%cl\n \
            decl   0x58+%[lv]\n \
            je     jump_1276a8\n \
            add    $0x10,%%edi\n \
            jmp    jump_1272ce\n \
        jump_1276a8:\n \
            mov    0x5c+%[lv],%%esi\n \
        jump_1276ac:\n \
            add    $0x14,%%esi\n \
            decl   0x20+%[lv]\n \
            jne    jump_127227\n \
            jmp    jump_return\n \
\n \
\n \
/*----------------------------------------------------------------*/\n \
render_md26:\n \
/*----------------------------------------------------------------*/\n \
            lea    "EXPORT_SYMBOL(polyscans)",%%esi\n \
            mov    %%esi,0x5c+%[lv]\n \
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
        jump_1276f4:\n \
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
            jns    jump_127762\n \
            or     %%ebp,%%ebp\n \
            jle    jump_127a4b\n \
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
            jle    jump_127760\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ebp\n \
        jump_127760:\n \
            jmp    jump_127795\n \
        jump_127762:\n \
            cmp    "EXPORT_SYMBOL(vec_window_width)",%%ebp\n \
            jle    jump_127770\n \
            mov    "EXPORT_SYMBOL(vec_window_width)",%%ebp\n \
        jump_127770:\n \
            sub    %%eax,%%ebp\n \
            jle    jump_127a4b\n \
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
        jump_127795:\n \
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
            jbe    jump_127a75\n \
        jump_12781b:\n \
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
            jbe    jump_127aa0\n \
        jump_12783e:\n \
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
            jbe    jump_127acd\n \
        jump_127862:\n \
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
            jbe    jump_127afa\n \
        jump_127886:\n \
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
            jbe    jump_127b27\n \
        jump_1278aa:\n \
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
            jbe    jump_127b54\n \
        jump_1278ce:\n \
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
            jbe    jump_127b81\n \
        jump_1278f2:\n \
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
            jbe    jump_127bae\n \
        jump_127916:\n \
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
            jbe    jump_127bdb\n \
        jump_12793a:\n \
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
            jbe    jump_127c08\n \
        jump_12795e:\n \
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
            jbe    jump_127c35\n \
        jump_127982:\n \
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
            jbe    jump_127c62\n \
        jump_1279a6:\n \
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
            jbe    jump_127c8f\n \
        jump_1279ca:\n \
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
            jbe    jump_127cbc\n \
        jump_1279ee:\n \
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
            jbe    jump_127ce9\n \
        jump_127a12:\n \
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
            jbe    jump_127d16\n \
        jump_127a36:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
            add    $0x10,%%edi\n \
            sub    $0x10,%%ebp\n \
            jg     t12_md00\n \
        jump_127a4b:\n \
            decl   0x20+%[lv]\n \
            jne    jump_1276f4\n \
            jmp    jump_return\n \
        jump_127a5a:\n \
            mov    %%ch,%%ah\n \
            mov    %%dl,%%bl\n \
            add    0x4c+%[lv],%%ecx\n \
            mov    (%%ebx,%%esi,1),%%al\n \
            adc    0x50+%[lv],%%edx\n \
            adc    0x64+%[lv],%%bh\n \
            cmp    $0xc,%%al\n \
            ja     jump_12781b\n \
        jump_127a75:\n \
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
            ja     jump_12783e\n \
        jump_127aa0:\n \
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
            ja     jump_127862\n \
        jump_127acd:\n \
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
            ja     jump_127886\n \
        jump_127afa:\n \
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
            ja     jump_1278aa\n \
        jump_127b27:\n \
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
            ja     jump_1278ce\n \
        jump_127b54:\n \
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
            ja     jump_1278f2\n \
        jump_127b81:\n \
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
            ja     jump_127916\n \
        jump_127bae:\n \
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
            ja     jump_12793a\n \
        jump_127bdb:\n \
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
            ja     jump_12795e\n \
        jump_127c08:\n \
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
            ja     jump_127982\n \
        jump_127c35:\n \
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
            ja     jump_1279a6\n \
        jump_127c62:\n \
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
            ja     jump_1279ca\n \
        jump_127c8f:\n \
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
            ja     jump_1279ee\n \
        jump_127cbc:\n \
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
            ja     jump_127a12\n \
        jump_127ce9:\n \
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
            ja     jump_127a36\n \
        jump_127d16:\n \
            mov    "EXPORT_SYMBOL(fade_table)"(%%eax),%%al\n \
            mov    0xf(%%edi),%%ah\n \
            mov    "EXPORT_SYMBOL(ghost_table)"(%%eax),%%al\n \
            mov    %%al,0xf(%%edi)\n \
            add    $0x10,%%edi\n \
            sub    $0x10,%%ebp\n \
            jg     jump_127a5a\n \
            decl   0x20+%[lv]\n \
            jne    jump_1276f4\n \
        jump_return:\n \
    "
                 : [lv] "+o" (lv)
                 : "a" (point_a), "d" (point_b), "b" (point_c), "o0" (lv)
                 : "memory", "cc", "%ecx", "%edi", "%esi");// "%eax", "%edx", "%ebx",
    LOGNO("end");
}
/******************************************************************************/
