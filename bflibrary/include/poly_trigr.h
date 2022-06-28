/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file poly_trigr.h
 *     Header file for poly_trigr.c.
 * @par Purpose:
 *     Textured 2D triangle rendring, final rasterization modes.
 * @par Comment:
 *     To be used internally; to draw a triangle, use API from `poly.h`.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef POLY_TRIGR_H_
#define POLY_TRIGR_H_

#include "poly.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

struct TrigLocals;

#pragma pack()

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

#ifdef __cplusplus
};
#endif

#endif // POLY_TRIGR_H_
/******************************************************************************/
