/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file helpers_screen.c
 *     Implementation of screen-related helper functions for tests.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     05 Nov 2021 - 29 Jul 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <string.h>
#include "helpers_screen.h"
#include "bfpalette.h"
#include "bfutility.h"
#include "bftstlog.h"

void generate_xor_textute(ubyte *Tex, ushort Width, ushort Height, ushort FillW,
  ushort FillH, const ubyte *Pal, short FactorsX[], short FactorsY[])
{
    short x, y;
    ubyte *t;

    t = Tex;
    for (y = 0; y < FillH; y++)
    {
        for (x = 0; x < FillW; x++)
        {
            ubyte r, g, b;
            r = ((x * FactorsX[0]) >> 10) ^ ((y * FactorsY[0]) >> 10);
            g = ((x * FactorsX[1]) >> 10) ^ ((y * FactorsY[1]) >> 10);
            b = ((x * FactorsX[2]) >> 10) ^ ((y * FactorsY[2]) >> 10);
            t[x] = LbPaletteFindColour(Pal, r, g, b);
        }
        t += Width;
    }
}

void generate_example_texture_map_xor_based(const ubyte *pal, ubyte *texmap)
{
    ubyte *tmap;
    short factors0[] = {256, 256, 256};
    tmap = texmap;
    generate_xor_textute(tmap, 256, 256, 256,  64, pal, factors0, factors0);
    short factors1[] = {512,   0,   0};
    tmap = texmap + 256 *  64 + 0;
    generate_xor_textute(tmap, 256, 256, 128,  64, pal, factors1, factors1);
    short factors2[] = {  0, 512,   0};
    tmap = texmap + 256 *  64 + 128;
    generate_xor_textute(tmap, 256, 256, 128,  64, pal, factors2, factors2);
    short factors3[] = {  0,   0, 512};
    tmap = texmap + 256 * 128 + 0;
    generate_xor_textute(tmap, 256, 256, 128,  64, pal, factors3, factors3);
    short factors4[] = {512, 512,   0};
    tmap = texmap + 256 * 128 + 128;
    generate_xor_textute(tmap, 256, 256, 128,  64, pal, factors4, factors4);
    short factors5[] = {512,   0, 512};
    tmap = texmap + 256 * 192 + 0;
    generate_xor_textute(tmap, 256, 256, 128,  64, pal, factors5, factors5);
    short factors6[] = {  0, 512, 512};
    tmap = texmap + 256 * 192 + 128;
    generate_xor_textute(tmap, 256, 256, 128,  64, pal, factors6, factors6);
}

void raw_to_wscreen(short X, short Y, ushort Width, ushort Height, ubyte *Raw)
{
    short y;
    ushort w, h;
    ubyte *r;
    ubyte *o;

    r = Raw;

    if (X >= 0) {
        w = Width;
    } else if (X > -Width) {
        w = Width - X;
        Raw += (-X);
    } else
        return;
    if (X + w > lbDisplay.GraphicsScreenWidth)
        w = lbDisplay.GraphicsScreenWidth - X;

    if (Y >= 0) {
        h = Height;
    } else if (Y > -Height) {
        h = Height - Y;
        Raw += lbDisplay.GraphicsScreenWidth * (-Y);
    } else
        return;
    if (Y + h > lbDisplay.GraphicsScreenHeight)
        h = lbDisplay.GraphicsScreenHeight - Y;

    o = lbDisplay.WScreen + max(Y,0) * lbDisplay.GraphicsScreenWidth + max(X,0);

    for (y = 0; y < h; y++)
    {
        memcpy(o, r, w);
        r += Width;
        o += lbDisplay.GraphicsScreenWidth;
    }
}

/******************************************************************************/
