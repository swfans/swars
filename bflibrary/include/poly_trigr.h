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

struct TrigLocalRend;

#pragma pack()

void trig_render_md00(struct TrigLocalRend *tlr);
void trig_render_md01(struct TrigLocalRend *tlr);
void trig_render_md02(struct TrigLocalRend *tlr);
void trig_render_md03(struct TrigLocalRend *tlr);
void trig_render_md04(struct TrigLocalRend *tlr);
void trig_render_md05(struct TrigLocalRend *tlr);
void trig_render_md06(struct TrigLocalRend *tlr);
void trig_render_md07(struct TrigLocalRend *tlr);
void trig_render_md08(struct TrigLocalRend *tlr);
void trig_render_md09(struct TrigLocalRend *tlr);
void trig_render_md10(struct TrigLocalRend *tlr);
void trig_render_md12(struct TrigLocalRend *tlr);
void trig_render_md13(struct TrigLocalRend *tlr);
void trig_render_md14(struct TrigLocalRend *tlr);
void trig_render_md15(struct TrigLocalRend *tlr);
void trig_render_md16(struct TrigLocalRend *tlr);
void trig_render_md17(struct TrigLocalRend *tlr);
void trig_render_md18(struct TrigLocalRend *tlr);
void trig_render_md19(struct TrigLocalRend *tlr);
void trig_render_md20(struct TrigLocalRend *tlr);
void trig_render_md21(struct TrigLocalRend *tlr);
void trig_render_md22(struct TrigLocalRend *tlr);
void trig_render_md23(struct TrigLocalRend *tlr);
void trig_render_md24(struct TrigLocalRend *tlr);
void trig_render_md25(struct TrigLocalRend *tlr);
void trig_render_md26(struct TrigLocalRend *tlr);

#ifdef __cplusplus
};
#endif

#endif // POLY_TRIGR_H_
/******************************************************************************/
