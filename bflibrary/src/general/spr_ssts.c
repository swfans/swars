/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file spr_ssta.c
 *     Sprites drawing with scaling, using their standard colors, but forcing alpha.
 * @par Purpose:
 *     Drawing scaled sprites, with original colors and optional transparency.
 * @par Comment:
 *     Part of 8-bit graphics canvas drawing library.
 *     Used for drawing sprites on screen.
 * @author   Tomasz Lis
 * @date     12 Feb 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfsprite.h"

#include <stdlib.h>
#include <assert.h>
#include "bftypes.h"
#include "insspr.h"
#include "bfscreen.h"
#include "bfgentab.h"
#include "bflog.h"

void LbSpriteDrawRemapUsingScalingSub1(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap, long v339)
{
  int h;
  ubyte *v8;
  ubyte v257;
  ubyte *v258;
  ubyte *v259;
  ubyte *v260;
  long *v261;
  int v262;
  int v263;
  ubyte *v264;
  int v265;
  int v266;
  char v267;
  char v268;
  ubyte v269;
  int i;
  int v271;
  int v272;
  int v273;
  int v274;
  int v275;
  int v276;
  int v277;
  int v278;
  int v279;
  int v280;
  int v281;
  int v282;
  int v283;
  int v284;
  int v285;
  int v286;
  ubyte v287;
  int v288;
  ubyte v289;
  int v290;
  int v291;
  int v292;
  int v293;
  int v294;
  int v295;
  int v296;
  int v297;
  int v298;
  int v299;
  long *v304;
  ubyte *v316;
  int v328;
  int v336;
  ubyte *v344;
  ubyte *v345;
  ubyte *v349;
  int v353;
  int v357;
  int v361;
  char v365;

  h = sprite->SHeight;
  v8 = sprite->Data;
        do
        {
          v328 = h;
          v353 = *((ulong *)ystep + 1);
          if ( v353 )
          {
            v316 = outbuf;
            v336 = *((ulong *)ystep + 1);
            v304 = ystep;
            v261 = xstep;
            while ( 1 )
            {
              v286 = 0;
              v287 = *v8++;
              if ( !v287 )
                break;
              if ( (v287 & 0x80u) != 0 )
              {
                v287 = -(char)v287;
                v298 = &outbuf[-v261[0] - v261[1]];
                v261 -= 2 * v287;
                outbuf = (ubyte *)(v261[0] + v261[1] + v298);
              }
              else
              {
                v345 = outbuf;
                do
                {
                  v288 = v261[1];
                  if ( v288 )
                  {
                    v286 = *v8;
                    v289 = cmap[v286];
                    *outbuf = v289;
                    v290 = v288 - 1;
                    if ( !v290 )
                      goto LABEL_559;
                    *(outbuf - 1) = v289;
                    v291 = v290 - 1;
                    if ( !v291 )
                      goto LABEL_559;
                    *(outbuf - 2) = v289;
                    v292 = v291 - 1;
                    if ( !v292 )
                      goto LABEL_559;
                    *(outbuf - 3) = v289;
                    v293 = v292 - 1;
                    if ( !v293 )
                      goto LABEL_559;
                    *(outbuf - 4) = v289;
                    v294 = v293 - 1;
                    if ( !v294 )
                      goto LABEL_559;
                    *(outbuf - 5) = v289;
                    v295 = v294 - 1;
                    if ( v295
                      && (*(outbuf - 6) = v289, (v296 = v295 - 1) != 0)
                      && (*(outbuf - 7) = v289, (v297 = v296 - 1) != 0) )
                    {
                      outbuf -= 8;
                      do
                      {
                        *outbuf-- = v289;
                        --v297;
                      }
                      while ( v297 );
                    }
                    else
                    {
LABEL_559:
                      outbuf -= v261[1];
                    }
                  }
                  ++v8;
                  v261 -= 2;
                  --v287;
                }
                while ( v287 );
                v262 = v353 - 1;
                if ( v353 != 1 && v353 - 1 >= 0 )
                {
                  v349 = outbuf;
                  v263 = v345 - outbuf;
                  v264 = outbuf;
                  v344 = outbuf;
                  v265 = v263 + 1;
                  v266 = &outbuf[scanline];
                  v357 = v265;
                  do
                  {
                    v361 = v266;
                    if ( v265 )
                    {
                      if ( !(v266 & 3) )
                        goto LABEL_560;
                      v267 = *v264++;
                      *(ubyte *)v266++ = v267;
                      if ( --v265 )
                      {
                        if ( !(v266 & 3) )
                          goto LABEL_560;
                        v268 = *v264++;
                        *(ubyte *)v266++ = v268;
                        if ( --v265 )
                        {
                          if ( !(v266 & 3) || (v269 = *v264, ++v264, *(ubyte *)v266 = v269, ++v266, --v265, v265) )
                          {
LABEL_560:
                            v365 = v265;
                            for ( i = v265 >> 2; i; i = v284 - 1 )
                            {
                              v271 = *(ulong *)v264;
                              v264 += 4;
                              *(ulong *)v266 = v271;
                              v266 += 4;
                              v272 = i - 1;
                              if ( !v272 )
                                break;
                              v273 = *(ulong *)v264;
                              v264 += 4;
                              *(ulong *)v266 = v273;
                              v266 += 4;
                              v274 = v272 - 1;
                              if ( !v274 )
                                break;
                              v275 = *(ulong *)v264;
                              v264 += 4;
                              *(ulong *)v266 = v275;
                              v266 += 4;
                              v276 = v274 - 1;
                              if ( !v276 )
                                break;
                              v277 = *(ulong *)v264;
                              v264 += 4;
                              *(ulong *)v266 = v277;
                              v266 += 4;
                              v278 = v276 - 1;
                              if ( !v278 )
                                break;
                              v279 = *(ulong *)v264;
                              v264 += 4;
                              *(ulong *)v266 = v279;
                              v266 += 4;
                              v280 = v278 - 1;
                              if ( !v280 )
                                break;
                              v281 = *(ulong *)v264;
                              v264 += 4;
                              *(ulong *)v266 = v281;
                              v266 += 4;
                              v282 = v280 - 1;
                              if ( !v282 )
                                break;
                              v283 = *(ulong *)v264;
                              v264 += 4;
                              *(ulong *)v266 = v283;
                              v266 += 4;
                              v284 = v282 - 1;
                              if ( !v284 )
                                break;
                              v285 = *(ulong *)v264;
                              v264 += 4;
                              *(ulong *)v266 = v285;
                              v266 += 4;
                            }
                            if ( v365 & 3 )
                            {
                              *(ubyte *)v266 = *v264;
                              if ( (v365 & 3) != 1 )
                              {
                                *(ubyte *)(v266 + 1) = v264[1];
                                if ( (v365 & 3) != 2 )
                                  *(ubyte *)(v266 + 2) = v264[2];
                              }
                            }
                          }
                        }
                      }
                    }
                    v264 = v344;
                    v265 = v357;
                    v266 = scanline + v361;
                    --v262;
                  }
                  while ( v262 );
                  outbuf = v349;
                }
              }
            }
            ystep = v304;
            outbuf = &v316[scanline];
            v299 = v336 - 1;
            if ( v336 != 1 )
            {
              do
              {
                outbuf += scanline;
                --v299;
              }
              while ( v299 );
            }
          }
          else
          {
            v257 = 0;
            while ( 1 )
            {
              v257 = *v8++;
              if ( !v257 )
                break;
              if ( (v257 & 0x80u) == 0 )
              {
                v258 = &v8[v257];
                v257 = *v258;
                v8 = v258 + 1;
                if ( !v257 )
                  break;
                if ( (v257 & 0x80u) == 0 )
                {
                  v259 = &v8[v257];
                  v257 = *v259;
                  v8 = v259 + 1;
                  if ( !v257 )
                    break;
                  if ( (v257 & 0x80u) == 0 )
                  {
                    v260 = &v8[v257];
                    v257 = *v260;
                    v8 = v260 + 1;
                    if ( !v257 )
                      break;
                    if ( (v257 & 0x80u) == 0 )
                      v8 += v257;
                  }
                }
              }
            }
          }
          ystep += v339;
          h = v328 - 1;
        }
        while ( v328 != 1 );
}

void LbSpriteDrawRemapUsingScalingSub2(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap, long v339)
{
  int h;
  ubyte *v8;
  ubyte v215;
  ubyte *v216;
  ubyte *v217;
  ubyte *v218;
  long *v219;
  int v220;
  char *v221;
  int v222;
  int v223;
  char v224;
  char v225;
  char v226;
  int j;
  int v228;
  int v229;
  int v230;
  int v231;
  int v232;
  int v233;
  int v234;
  int v235;
  int v236;
  int v237;
  int v238;
  int v239;
  int v240;
  int v241;
  int v242;
  ubyte v243;
  ubyte v244;
  int v245;
  ubyte v246;
  int v247;
  int v248;
  int v249;
  int v250;
  int v251;
  int v252;
  int v253;
  int v254;
  int v255;
  int v256;
  long *v303;
  ubyte *v315;
  int v327;
  int v335;
  ubyte *v343;
  ubyte *v348;
  int v352;
  int v356;
  int v360;
  char v364;

  h = sprite->SHeight;
  v8 = sprite->Data;
        do
        {
          v327 = h;
          v352 = *((ulong *)ystep + 1);
          if ( v352 )
          {
            v315 = outbuf;
            v335 = *((ulong *)ystep + 1);
            v303 = ystep;
            v219 = xstep;
            while ( 1 )
            {
              v243 = 0;
              v244 = *v8++;
              if ( !v244 )
                break;
              if ( (v244 & 0x80u) != 0 )
              {
                v244 = -(char)v244;
                v255 = &outbuf[-v219[0]];
                v219 += 2 * v244;
                outbuf = (ubyte *)(v219[0] + v255);
              }
              else
              {
                v343 = outbuf;
                do
                {
                  v245 = v219[1];
                  if ( v245 )
                  {
                    v243 = *v8;
                    v246 = cmap[v243];
                    *outbuf = v246;
                    v247 = v245 - 1;
                    if ( !v247 )
                      goto LABEL_561;
                    outbuf[1] = v246;
                    v248 = v247 - 1;
                    if ( !v248 )
                      goto LABEL_561;
                    outbuf[2] = v246;
                    v249 = v248 - 1;
                    if ( !v249 )
                      goto LABEL_561;
                    outbuf[3] = v246;
                    v250 = v249 - 1;
                    if ( !v250 )
                      goto LABEL_561;
                    outbuf[4] = v246;
                    v251 = v250 - 1;
                    if ( !v251 )
                      goto LABEL_561;
                    outbuf[5] = v246;
                    v252 = v251 - 1;
                    if ( v252 && (outbuf[6] = v246, (v253 = v252 - 1) != 0) && (outbuf[7] = v246, (v254 = v253 - 1) != 0) )
                    {
                      outbuf += 8;
                      do
                      {
                        *outbuf++ = v246;
                        --v254;
                      }
                      while ( v254 );
                    }
                    else
                    {
LABEL_561:
                      outbuf += v219[1];
                    }
                  }
                  ++v8;
                  v219 += 2;
                  --v244;
                }
                while ( v244 );
                v220 = v352 - 1;
                if ( v352 != 1 && v352 - 1 >= 0 )
                {
                  v221 = (char *)v343;
                  v348 = outbuf;
                  v222 = outbuf - v343;
                  v223 = &v343[scanline];
                  v356 = v222;
                  do
                  {
                    v360 = v223;
                    if ( v222 )
                    {
                      if ( !(v223 & 3) )
                        goto LABEL_562;
                      v224 = *v221++;
                      *(ubyte *)v223++ = v224;
                      if ( --v222 )
                      {
                        if ( !(v223 & 3) )
                          goto LABEL_562;
                        v225 = *v221++;
                        *(ubyte *)v223++ = v225;
                        if ( --v222 )
                        {
                          if ( !(v223 & 3) || (v226 = *v221, ++v221, *(ubyte *)v223 = v226, ++v223, --v222, v222) )
                          {
LABEL_562:
                            v364 = v222;
                            for ( j = v222 >> 2; j; j = v241 - 1 )
                            {
                              v228 = *(ulong *)v221;
                              v221 += 4;
                              *(ulong *)v223 = v228;
                              v223 += 4;
                              v229 = j - 1;
                              if ( !v229 )
                                break;
                              v230 = *(ulong *)v221;
                              v221 += 4;
                              *(ulong *)v223 = v230;
                              v223 += 4;
                              v231 = v229 - 1;
                              if ( !v231 )
                                break;
                              v232 = *(ulong *)v221;
                              v221 += 4;
                              *(ulong *)v223 = v232;
                              v223 += 4;
                              v233 = v231 - 1;
                              if ( !v233 )
                                break;
                              v234 = *(ulong *)v221;
                              v221 += 4;
                              *(ulong *)v223 = v234;
                              v223 += 4;
                              v235 = v233 - 1;
                              if ( !v235 )
                                break;
                              v236 = *(ulong *)v221;
                              v221 += 4;
                              *(ulong *)v223 = v236;
                              v223 += 4;
                              v237 = v235 - 1;
                              if ( !v237 )
                                break;
                              v238 = *(ulong *)v221;
                              v221 += 4;
                              *(ulong *)v223 = v238;
                              v223 += 4;
                              v239 = v237 - 1;
                              if ( !v239 )
                                break;
                              v240 = *(ulong *)v221;
                              v221 += 4;
                              *(ulong *)v223 = v240;
                              v223 += 4;
                              v241 = v239 - 1;
                              if ( !v241 )
                                break;
                              v242 = *(ulong *)v221;
                              v221 += 4;
                              *(ulong *)v223 = v242;
                              v223 += 4;
                            }
                            if ( v364 & 3 )
                            {
                              *(ubyte *)v223 = *v221;
                              if ( (v364 & 3) != 1 )
                              {
                                *(ubyte *)(v223 + 1) = v221[1];
                                if ( (v364 & 3) != 2 )
                                  *(ubyte *)(v223 + 2) = v221[2];
                              }
                            }
                          }
                        }
                      }
                    }
                    v221 = (char *)v343;
                    v222 = v356;
                    v223 = scanline + v360;
                    --v220;
                  }
                  while ( v220 );
                  outbuf = v348;
                }
              }
            }
            ystep = v303;
            outbuf = &v315[scanline];
            v256 = v335 - 1;
            if ( v335 != 1 )
            {
              do
              {
                outbuf += scanline;
                --v256;
              }
              while ( v256 );
            }
          }
          else
          {
            v215 = 0;
            while ( 1 )
            {
              v215 = *v8++;
              if ( !v215 )
                break;
              if ( (v215 & 0x80u) == 0 )
              {
                v216 = &v8[v215];
                v215 = *v216;
                v8 = v216 + 1;
                if ( !v215 )
                  break;
                if ( (v215 & 0x80u) == 0 )
                {
                  v217 = &v8[v215];
                  v215 = *v217;
                  v8 = v217 + 1;
                  if ( !v215 )
                    break;
                  if ( (v215 & 0x80u) == 0 )
                  {
                    v218 = &v8[v215];
                    v215 = *v218;
                    v8 = v218 + 1;
                    if ( !v215 )
                      break;
                    if ( (v215 & 0x80u) == 0 )
                      v8 += v215;
                  }
                }
              }
            }
          }
          ystep += v339;
          h = v327 - 1;
        }
        while ( v327 != 1 );
}

void LbSpriteDrawRemapUsingScalingSub3(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap, long v339)
{
  int h;
  ubyte *v8;
  int v11;
  ubyte *v200;
  ubyte *v201;
  ubyte *v202;
  long *v204;
  ubyte v205;
  int v206;
  int v207;
  int v208;
  int v209;
  int v210;
  int v211;
  int v212;
  int v213;
  int v214;
  ubyte *v314;
  ubyte *v330;
  int v334;

  v8 = sprite->Data;

    for (h=sprite->SHeight; h > 0; h--)
        {
          v334 = ystep[1];
          v330 = v8;
          if ( v334 )
          {
            do
            {
              v8 = v330;
              v314 = outbuf;
              v204 = xstep;
              while ( 1 )
              {
                v205 = *v8++;
                if ( !v205 )
                  break;
                if ( (v205 & 0x80u) != 0 )
                {
                  v205 = -(char)v205;
                  v214 = &outbuf[-v204[0] - v204[1]];
                  v204 -= 2 * v205;
                  outbuf = (ubyte *)(v204[0] + v204[1] + v214);
                }
                else
                {
                  do
                  {
                    v206 = v204[1];
                    if ( v206 )
                    {
                      v11 = ((*v8) << 8) & 0xff00;
                      do
                      {
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf-- = ghost_table[v11];
                        v207 = v206 - 1;
                        if ( !v207 )
                          break;
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf-- = ghost_table[v11];
                        v208 = v207 - 1;
                        if ( !v208 )
                          break;
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf-- = ghost_table[v11];
                        v209 = v208 - 1;
                        if ( !v209 )
                          break;
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf-- = ghost_table[v11];
                        v210 = v209 - 1;
                        if ( !v210 )
                          break;
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf-- = ghost_table[v11];
                        v211 = v210 - 1;
                        if ( !v211 )
                          break;
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf-- = ghost_table[v11];
                        v212 = v211 - 1;
                        if ( !v212 )
                          break;
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf-- = ghost_table[v11];
                        v213 = v212 - 1;
                        if ( !v213 )
                          break;
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf-- = ghost_table[v11];
                        v206 = v213 - 1;
                      }
                      while ( v206 );
                    }
                    ++v8;
                    v204 -= 2;
                    --v205;
                  }
                  while ( v205 );
                }
              }
              outbuf = &v314[scanline];
              v334--;
            }
            while ( v334 != 0 );
          }
          else
          {
            v11 = 0;
            while ( 1 )
            {
              v11 = (v11 & 0xff00) + *v8;
              v8++;
              if ( !(ubyte)v11 )
                break;
              if ( (v11 & 0x80u) == 0 )
              {
                v200 = &v8[v11];
                v11 = (v11 & 0xff00) + *v200;
                v8 = v200 + 1;
                if ( !(ubyte)v11 )
                  break;
                if ( (v11 & 0x80u) == 0 )
                {
                  v201 = &v8[v11];
                  v11 = (v11 & 0xff00) + *v201;
                  v8 = v201 + 1;
                  if ( !(ubyte)v11 )
                    break;
                  if ( (v11 & 0x80u) == 0 )
                  {
                    v202 = &v8[v11];
                    v11 = (v11 & 0xff00) + *v202;
                    v8 = v202 + 1;
                    if ( !(ubyte)v11 )
                      break;
                    if ( (v11 & 0x80u) == 0 )
                      v8 += v11;
                  }
                }
              }
            }
          }
          ystep += v339;
        }
}

void LbSpriteDrawRemapUsingScalingSub4(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap, long v339)
{
  int h;
  ubyte *v8;
  int v11;
  ubyte *v185;
  ubyte *v186;
  ubyte *v187;
  int v188;
  long *v189;
  ubyte v190;
  int v191;
  int v192;
  int v193;
  int v194;
  int v195;
  int v196;
  int v197;
  int v198;
  int v199;
  ubyte *v313;
  int v325;
  ubyte *v329;
  int v333;
    long *ycurstep;

  v8 = sprite->Data;
    ycurstep = ystep;

    for (h=sprite->SHeight; h > 0; h--)
    {
          v329 = v8;
          if ( ycurstep[1] )
          {
            v333 = ycurstep[1];
            do
            {
              v8 = v329;
              v313 = outbuf;
              v189 = xstep;
              while ( 1 )
              {
                v190 = *v8++;
                if ( !v190 )
                  break;
                if ( (v190 & 0x80u) != 0 )
                {
                  v190 = -(char)v190;
                  v199 = &outbuf[-v189[0]];
                  v189 += 2 * v190;
                  outbuf = (ubyte *)(v189[0] + v199);
                }
                else
                {
                  do
                  {
                    v191 = v189[1];
                    if ( v191 )
                    {
                      v11 = ((*v8) << 8) & 0xff00;
                      do
                      {
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf++ = ghost_table[v11];
                        v192 = v191 - 1;
                        if ( !v192 )
                          break;
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf++ = ghost_table[v11];
                        v193 = v192 - 1;
                        if ( !v193 )
                          break;
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf++ = ghost_table[v11];
                        v194 = v193 - 1;
                        if ( !v194 )
                          break;
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf++ = ghost_table[v11];
                        v195 = v194 - 1;
                        if ( !v195 )
                          break;
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf++ = ghost_table[v11];
                        v196 = v195 - 1;
                        if ( !v196 )
                          break;
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf++ = ghost_table[v11];
                        v197 = v196 - 1;
                        if ( !v197 )
                          break;
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf++ = ghost_table[v11];
                        v198 = v197 - 1;
                        if ( !v198 )
                          break;
                        v11 = (v11 & 0xff00) + *outbuf;
                        *outbuf++ = ghost_table[v11];
                        v191 = v198 - 1;
                      }
                      while ( v191 );
                    }
                    ++v8;
                    v189 += 2;
                    --v190;
                  }
                  while ( v190 );
                }
              }
              outbuf = &v313[scanline];
              v333--;
            }
            while ( v333 != 0 );
          }
          else
          {
            v11 = 0;
            while ( 1 )
            {
              v11 = (v11 & 0xff00) + *v8;
              v8++;
              if ( !(ubyte)v11 )
                break;
              if ( (v11 & 0x80u) == 0 )
              {
                v185 = &v8[v11];
                v11 = (v11 & 0xff00) + *v185;
                v8 = v185 + 1;
                if ( !(ubyte)v11 )
                  break;
                if ( (v11 & 0x80u) == 0 )
                {
                  v186 = &v8[v11];
                  v11 = (v11 & 0xff00) + *v186;
                  v8 = v186 + 1;
                  if ( !(ubyte)v11 )
                    break;
                  if ( (v11 & 0x80u) == 0 )
                  {
                    v187 = &v8[v11];
                    v11 = (v11 & 0xff00) + *v187;
                    v8 = v187 + 1;
                    if ( !(ubyte)v11 )
                      break;
                    if ( (v11 & 0x80u) == 0 )
                      v8 += v11;
                  }
                }
              }
            }
          }
          ycurstep += v339;
    }
}

void LbSpriteDrawRemapUsingScalingSub5(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap, long v339)
{
  int h;
  ubyte *v8;
  int v102;
  ubyte *v103;
  ubyte *v104;
  ubyte *v105;
  char *v106;
  int v107;
  char *v108;
  int v109;
  int v110;
  char v111;
  char v112;
  char v113;
  int l;
  int v115;
  int v116;
  int v117;
  int v118;
  int v119;
  int v120;
  int v121;
  int v122;
  int v123;
  int v124;
  int v125;
  int v126;
  int v127;
  int v128;
  int v129;
  int v130;
  int v131;
  ubyte v132;
  int v133;
  int v134;
  int v135;
  int v136;
  int v137;
  int v138;
  int v139;
  int v140;
  int v141;
  int v142;
  ubyte v143;
  ubyte *v144;
  ubyte *v145;
  ubyte *v146;
  char *v147;
  int v148;
  int v149;
  ubyte *v150;
  int v151;
  int v152;
  char v153;
  char v154;
  ubyte v155;
  int k;
  int v157;
  int v158;
  int v159;
  int v160;
  int v161;
  int v162;
  int v163;
  int v164;
  int v165;
  int v166;
  int v167;
  int v168;
  int v169;
  int v170;
  int v171;
  ubyte v172;
  int v173;
  ubyte v174;
  int v175;
  int v176;
  int v177;
  int v178;
  int v179;
  int v180;
  int v181;
  int v182;
  int v183;
  int v184;
  char *v302;
  ubyte *v312;
  int v324;
  int v332;
  ubyte *v341;
  ubyte *v342;
  ubyte *v347;
  int v351;
  int v355;
  int v359;
  char v363;

  h = sprite->SHeight;
  v8 = sprite->Data;
      do
      {
        v324 = h;
        v351 = *((ulong *)ystep + 1);
        if ( v351 )
        {
          v312 = outbuf;
          v332 = *((ulong *)ystep + 1);
          v302 = ystep;
          v147 = xstep;
          while ( 1 )
          {
            v172 = *v8++;
            if ( !v172 )
              break;
            if ( (v172 & 0x80u) != 0 )
            {
              v172 = -(char)v172;
              v183 = &outbuf[-*(ulong *)v147 - *((ulong *)v147 + 1)];
              v147 -= 8 * v172;
              outbuf = (ubyte *)(*(ulong *)v147 + *((ulong *)v147 + 1) + v183);
            }
            else
            {
              v342 = outbuf;
              do
              {
                v173 = *((ulong *)v147 + 1);
                if ( v173 )
                {
                  v174 = *v8;
                  *outbuf = *v8;
                  v175 = v173 - 1;
                  if ( !v175 )
                    goto LABEL_563;
                  *(outbuf - 1) = v174;
                  v176 = v175 - 1;
                  if ( !v176 )
                    goto LABEL_563;
                  *(outbuf - 2) = v174;
                  v177 = v176 - 1;
                  if ( !v177 )
                    goto LABEL_563;
                  *(outbuf - 3) = v174;
                  v178 = v177 - 1;
                  if ( !v178 )
                    goto LABEL_563;
                  *(outbuf - 4) = v174;
                  v179 = v178 - 1;
                  if ( !v179 )
                    goto LABEL_563;
                  *(outbuf - 5) = v174;
                  v180 = v179 - 1;
                  if ( v180
                    && (*(outbuf - 6) = v174, (v181 = v180 - 1) != 0)
                    && (*(outbuf - 7) = v174, (v182 = v181 - 1) != 0) )
                  {
                    outbuf -= 8;
                    do
                    {
                      *outbuf-- = v174;
                      --v182;
                    }
                    while ( v182 );
                  }
                  else
                  {
LABEL_563:
                    outbuf -= *((ulong *)v147 + 1);
                  }
                }
                ++v8;
                v147 -= 8;
                --v172;
              }
              while ( v172 );
              v148 = v351 - 1;
              if ( v351 != 1 && v351 - 1 >= 0 )
              {
                v347 = outbuf;
                v149 = v342 - outbuf;
                v150 = outbuf;
                v341 = outbuf;
                v151 = v149 + 1;
                v152 = &outbuf[scanline];
                v355 = v151;
                do
                {
                  v359 = v152;
                  if ( v151 )
                  {
                    if ( !(v152 & 3) )
                      goto LABEL_564;
                    v153 = *v150++;
                    *(ubyte *)v152++ = v153;
                    if ( --v151 )
                    {
                      if ( !(v152 & 3) )
                        goto LABEL_564;
                      v154 = *v150++;
                      *(ubyte *)v152++ = v154;
                      if ( --v151 )
                      {
                        if ( !(v152 & 3) || (v155 = *v150, ++v150, *(ubyte *)v152 = v155, ++v152, --v151, v151) )
                        {
LABEL_564:
                          v363 = v151;
                          for ( k = v151 >> 2; k; k = v170 - 1 )
                          {
                            v157 = *(ulong *)v150;
                            v150 += 4;
                            *(ulong *)v152 = v157;
                            v152 += 4;
                            v158 = k - 1;
                            if ( !v158 )
                              break;
                            v159 = *(ulong *)v150;
                            v150 += 4;
                            *(ulong *)v152 = v159;
                            v152 += 4;
                            v160 = v158 - 1;
                            if ( !v160 )
                              break;
                            v161 = *(ulong *)v150;
                            v150 += 4;
                            *(ulong *)v152 = v161;
                            v152 += 4;
                            v162 = v160 - 1;
                            if ( !v162 )
                              break;
                            v163 = *(ulong *)v150;
                            v150 += 4;
                            *(ulong *)v152 = v163;
                            v152 += 4;
                            v164 = v162 - 1;
                            if ( !v164 )
                              break;
                            v165 = *(ulong *)v150;
                            v150 += 4;
                            *(ulong *)v152 = v165;
                            v152 += 4;
                            v166 = v164 - 1;
                            if ( !v166 )
                              break;
                            v167 = *(ulong *)v150;
                            v150 += 4;
                            *(ulong *)v152 = v167;
                            v152 += 4;
                            v168 = v166 - 1;
                            if ( !v168 )
                              break;
                            v169 = *(ulong *)v150;
                            v150 += 4;
                            *(ulong *)v152 = v169;
                            v152 += 4;
                            v170 = v168 - 1;
                            if ( !v170 )
                              break;
                            v171 = *(ulong *)v150;
                            v150 += 4;
                            *(ulong *)v152 = v171;
                            v152 += 4;
                          }
                          if ( v363 & 3 )
                          {
                            *(ubyte *)v152 = *v150;
                            if ( (v363 & 3) != 1 )
                            {
                              *(ubyte *)(v152 + 1) = v150[1];
                              if ( (v363 & 3) != 2 )
                                *(ubyte *)(v152 + 2) = v150[2];
                            }
                          }
                        }
                      }
                    }
                  }
                  v150 = v341;
                  v151 = v355;
                  v152 = scanline + v359;
                  --v148;
                }
                while ( v148 );
                outbuf = v347;
              }
            }
          }
          ystep = v302;
          outbuf = &v312[scanline];
          v184 = v332 - 1;
          if ( v332 != 1 )
          {
            do
            {
              outbuf += scanline;
              --v184;
            }
            while ( v184 );
          }
        }
        else
        {
          v143 = 0;
          while ( 1 )
          {
            v143 = *v8++;
            if ( !v143 )
              break;
            if ( (v143 & 0x80u) == 0 )
            {
              v144 = &v8[v143];
              v143 = *v144;
              v8 = v144 + 1;
              if ( !v143 )
                break;
              if ( (v143 & 0x80u) == 0 )
              {
                v145 = &v8[v143];
                v143 = *v145;
                v8 = v145 + 1;
                if ( !v143 )
                  break;
                if ( (v143 & 0x80u) == 0 )
                {
                  v146 = &v8[v143];
                  v143 = *v146;
                  v8 = v146 + 1;
                  if ( !v143 )
                    break;
                  if ( (v143 & 0x80u) == 0 )
                    v8 += v143;
                }
              }
            }
          }
        }
        ystep += v339;
        h = v324 - 1;
      }
      while ( v324 != 1 );
}

void LbSpriteDrawRemapUsingScalingSub6(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap, long v339)
{
  int v3;
  signed int v4;
  char *v5;
  char *v6;
  int h;
  ubyte *v8;
  int v9;
  ubyte *v10;
  int v11;
  int v12;
  ubyte *v13;
  ubyte *v14;
  ubyte *v15;
  int v17;
  ubyte v18;
  ubyte v19;
  ubyte v20;
  ubyte v21;
  ubyte v22;
  ubyte v23;
  ubyte v24;
  ubyte v25;
  int v26;
  int v27;
  ubyte *v28;
  ubyte *v29;
  ubyte *v30;
  char *v31;
  int v32;
  ubyte v33;
  ubyte v34;
  ubyte v35;
  ubyte v36;
  ubyte v37;
  ubyte v38;
  ubyte v39;
  ubyte v40;
  int v41;
  int v42;
  ubyte *v43;
  ubyte *v44;
  ubyte *v45;
  char *v46;
  int v47;
  int v48;
  int v49;
  int v50;
  int v51;
  int v52;
  int v53;
  int v54;
  int v55;
  int v56;
  int v57;
  ubyte *v58;
  ubyte *v59;
  ubyte *v60;
  char *v61;
  int v62;
  int v63;
  int v64;
  int v65;
  int v66;
  int v67;
  int v68;
  int v69;
  int v70;
  int v71;
  int v72;
  ubyte *v73;
  ubyte *v74;
  ubyte *v75;
  char *v76;
  int v77;
  int v78;
  int v79;
  int v80;
  int v81;
  int v82;
  int v83;
  int v84;
  int v85;
  int v86;
  int v87;
  ubyte *v88;
  ubyte *v89;
  ubyte *v90;
  char *v91;
  int v92;
  int v93;
  int v94;
  int v95;
  int v96;
  int v97;
  int v98;
  int v99;
  int v100;
  int v101;
  ubyte v102;
  ubyte *v103;
  ubyte *v104;
  ubyte *v105;
  char *v106;
  int v107;
  char *v108;
  int v109;
  int v110;
  char v111;
  char v112;
  char v113;
  int l;
  int v115;
  int v116;
  int v117;
  int v118;
  int v119;
  int v120;
  int v121;
  int v122;
  int v123;
  int v124;
  int v125;
  int v126;
  int v127;
  int v128;
  int v129;
  ubyte v130;
  int v131;
  ubyte v132;
  int v133;
  int v134;
  int v135;
  int v136;
  int v137;
  int v138;
  int v139;
  int v140;
  int v141;
  int v142;
  char *v301;
  ubyte *v311;
  int v323;
  int v331;
  ubyte *v340;
  ubyte *v346;
  int v350;
  int v354;
  int v358;
  char v362;

  h = sprite->SHeight;
  v8 = sprite->Data;
      do
      {
        v323 = h;
        v350 = *((ulong *)ystep + 1);
        if ( v350 )
        {
          v311 = outbuf;
          v331 = *((ulong *)ystep + 1);
          v301 = ystep;
          v106 = xstep;
          while ( 1 )
          {
            v130 = *v8++;
            if ( !v130 )
              break;
            if ( (v130 & 0x80u) != 0 )
            {
              v130 = -(char)v130;
              v141 = &outbuf[-*(ulong *)v106];
              v106 += 8 * v130;
              outbuf = (ubyte *)(*(ulong *)v106 + v141);
            }
            else
            {
              v340 = outbuf;
              do
              {
                v131 = *((ulong *)v106 + 1);
                if ( v131 )
                {
                  v132 = *v8;
                  *outbuf = *v8;
                  v133 = v131 - 1;
                  if ( !v133 )
                    goto LABEL_565;
                  outbuf[1] = v132;
                  v134 = v133 - 1;
                  if ( !v134 )
                    goto LABEL_565;
                  outbuf[2] = v132;
                  v135 = v134 - 1;
                  if ( !v135 )
                    goto LABEL_565;
                  outbuf[3] = v132;
                  v136 = v135 - 1;
                  if ( !v136 )
                    goto LABEL_565;
                  outbuf[4] = v132;
                  v137 = v136 - 1;
                  if ( !v137 )
                    goto LABEL_565;
                  outbuf[5] = v132;
                  v138 = v137 - 1;
                  if ( v138 && (outbuf[6] = v132, (v139 = v138 - 1) != 0) && (outbuf[7] = v132, (v140 = v139 - 1) != 0) )
                  {
                    outbuf += 8;
                    do
                    {
                      *outbuf++ = v132;
                      --v140;
                    }
                    while ( v140 );
                  }
                  else
                  {
LABEL_565:
                    outbuf += *((ulong *)v106 + 1);
                  }
                }
                ++v8;
                v106 += 8;
                --v130;
              }
              while ( v130 );
              v107 = v350 - 1;
              if ( v350 != 1 && v350 - 1 >= 0 )
              {
                v108 = (char *)v340;
                v346 = outbuf;
                v109 = outbuf - v340;
                v110 = &v340[scanline];
                v354 = v109;
                do
                {
                  v358 = v110;
                  if ( v109 )
                  {
                    if ( !(v110 & 3) )
                      goto LABEL_566;
                    v111 = *v108++;
                    *(ubyte *)v110++ = v111;
                    if ( --v109 )
                    {
                      if ( !(v110 & 3) )
                        goto LABEL_566;
                      v112 = *v108++;
                      *(ubyte *)v110++ = v112;
                      if ( --v109 )
                      {
                        if ( !(v110 & 3) || (v113 = *v108, ++v108, *(ubyte *)v110 = v113, ++v110, --v109, v109) )
                        {
LABEL_566:
                          v362 = v109;
                          for ( l = v109 >> 2; l; l = v128 - 1 )
                          {
                            v115 = *(ulong *)v108;
                            v108 += 4;
                            *(ulong *)v110 = v115;
                            v110 += 4;
                            v116 = l - 1;
                            if ( !v116 )
                              break;
                            v117 = *(ulong *)v108;
                            v108 += 4;
                            *(ulong *)v110 = v117;
                            v110 += 4;
                            v118 = v116 - 1;
                            if ( !v118 )
                              break;
                            v119 = *(ulong *)v108;
                            v108 += 4;
                            *(ulong *)v110 = v119;
                            v110 += 4;
                            v120 = v118 - 1;
                            if ( !v120 )
                              break;
                            v121 = *(ulong *)v108;
                            v108 += 4;
                            *(ulong *)v110 = v121;
                            v110 += 4;
                            v122 = v120 - 1;
                            if ( !v122 )
                              break;
                            v123 = *(ulong *)v108;
                            v108 += 4;
                            *(ulong *)v110 = v123;
                            v110 += 4;
                            v124 = v122 - 1;
                            if ( !v124 )
                              break;
                            v125 = *(ulong *)v108;
                            v108 += 4;
                            *(ulong *)v110 = v125;
                            v110 += 4;
                            v126 = v124 - 1;
                            if ( !v126 )
                              break;
                            v127 = *(ulong *)v108;
                            v108 += 4;
                            *(ulong *)v110 = v127;
                            v110 += 4;
                            v128 = v126 - 1;
                            if ( !v128 )
                              break;
                            v129 = *(ulong *)v108;
                            v108 += 4;
                            *(ulong *)v110 = v129;
                            v110 += 4;
                          }
                          if ( v362 & 3 )
                          {
                            *(ubyte *)v110 = *v108;
                            if ( (v362 & 3) != 1 )
                            {
                              *(ubyte *)(v110 + 1) = v108[1];
                              if ( (v362 & 3) != 2 )
                                *(ubyte *)(v110 + 2) = v108[2];
                            }
                          }
                        }
                      }
                    }
                  }
                  v108 = (char *)v340;
                  v109 = v354;
                  v110 = scanline + v358;
                  --v107;
                }
                while ( v107 );
                outbuf = v346;
              }
            }
          }
          ystep = v301;
          outbuf = &v311[scanline];
          v142 = v331 - 1;
          if ( v331 != 1 )
          {
            do
            {
              outbuf += scanline;
              --v142;
            }
            while ( v142 );
          }
        }
        else
        {
          v102 = 0;
          while ( 1 )
          {
            v102 = *v8++;
            if ( !v102 )
              break;
            if ( (v102 & 0x80u) == 0 )
            {
              v103 = &v8[v102];
              v102 = *v103;
              v8 = v103 + 1;
              if ( !v102 )
                break;
              if ( (v102 & 0x80u) == 0 )
              {
                v104 = &v8[v102];
                v102 = *v104;
                v8 = v104 + 1;
                if ( !v102 )
                  break;
                if ( (v102 & 0x80u) == 0 )
                {
                  v105 = &v8[v102];
                  v102 = *v105;
                  v8 = v105 + 1;
                  if ( !v102 )
                    break;
                  if ( (v102 & 0x80u) == 0 )
                    v8 += v102;
                }
              }
            }
          }
        }
        ystep += v339;
        h = v323 - 1;
      }
      while ( v323 != 1 );
}

void LbSpriteDrawRemapUsingScalingSub7(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap, long v339)
{
  int h;
  ubyte *v8;
  int gspos_y2;
  int gspos_y3;
  int gspos_y4;
  int gspos_y5;
  int gspos_y6;
  int gspos_y7;
  int gspos_y8;
  int gspos_y9;
  int v87;
  ubyte *v88;
  ubyte *v89;
  ubyte *v90;
  char *v91;
  ubyte v92;
  int v93;
  int v94;
  int v95;
  int v96;
  int v97;
  int v98;
  int v99;
  int v100;
  int v101;
  ubyte *v310;
  int v322;

  h = sprite->SHeight;
  v8 = sprite->Data;
      do
      {
        if ( *((ulong *)ystep + 1) )
        {
          v310 = outbuf;
          v322 = h;
          v91 = xstep;
LABEL_219:
          while ( 1 )
          {
            v92 = *v8++;
            if ( !v92 )
              break;
            if ( (v92 & 0x80u) != 0 )
            {
              v92 = -(char)v92;
              v101 = &outbuf[-*(ulong *)v91 - *((ulong *)v91 + 1)];
              v91 -= 8 * v92;
              outbuf = (ubyte *)(*(ulong *)v91 + *((ulong *)v91 + 1) + v101);
            }
            else
            {
              do
              {
                while ( 1 )
                {
                  v93 = *((ulong *)v91 + 1);
                  if ( v93 )
                  {
                    v93 = (v93 & 0xff00) + *v8;
                    v91 -= 8;
                    ++v8;
                    *outbuf-- = cmap[v93];
                    if ( !--v92 )
                      goto LABEL_219;
                    v94 = *((ulong *)v91 + 1);
                    if ( v94 )
                    {
                      v94 = (v94 & 0xff00) + *v8;
                      v91 -= 8;
                      ++v8;
                      *outbuf-- = cmap[v94];
                      if ( !--v92 )
                        goto LABEL_219;
                      v95 = *((ulong *)v91 + 1);
                      if ( v95 )
                      {
                        v95 = (v95 & 0xff00) + *v8;
                        v91 -= 8;
                        ++v8;
                        *outbuf-- = cmap[v95];
                        if ( !--v92 )
                          goto LABEL_219;
                        v96 = *((ulong *)v91 + 1);
                        if ( v96 )
                        {
                          v96 = (v96 & 0xff00) + *v8;
                          v91 -= 8;
                          ++v8;
                          *outbuf-- = cmap[v96];
                          if ( !--v92 )
                            goto LABEL_219;
                          v97 = *((ulong *)v91 + 1);
                          if ( v97 )
                          {
                            v97 = (v97 & 0xff00) + *v8;
                            v91 -= 8;
                            ++v8;
                            *outbuf-- = cmap[v97];
                            if ( !--v92 )
                              goto LABEL_219;
                            v98 = *((ulong *)v91 + 1);
                            if ( v98 )
                            {
                              v98 = (v98 & 0xff00) + *v8;
                              v91 -= 8;
                              ++v8;
                              *outbuf-- = cmap[v98];
                              if ( !--v92 )
                                goto LABEL_219;
                              v99 = *((ulong *)v91 + 1);
                              if ( v99 )
                              {
                                v99 = (v99 & 0xff00) + *v8;
                                v91 -= 8;
                                ++v8;
                                *outbuf-- = cmap[v99];
                                if ( !--v92 )
                                  goto LABEL_219;
                                v100 = *((ulong *)v91 + 1);
                                if ( v100 )
                                  break;
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                  ++v8;
                  v91 -= 8;
                  if ( !--v92 )
                    goto LABEL_219;
                }
                v100 = (v100 & 0xff00) + *v8;
                v91 -= 8;
                ++v8;
                *outbuf-- = cmap[v100];
                --v92;
              }
              while ( v92 );
            }
          }
          h = v322;
          outbuf = &v310[scanline];
        }
        else
        {
          v87 = 0;
          while ( 1 )
          {
            v87 = (v87 & 0xff00) + *v8;
            v8++;
            if ( !(ubyte)v87 )
              break;
            if ( (v87 & 0x80u) == 0 )
            {
              v88 = &v8[v87];
              v87 = (v87 & 0xff00) + *v88;
              v8 = v88 + 1;
              if ( !(ubyte)v87 )
                break;
              if ( (v87 & 0x80u) == 0 )
              {
                v89 = &v8[v87];
                v87 = (v87 & 0xff00) + *v89;
                v8 = v89 + 1;
                if ( !(ubyte)v87 )
                  break;
                if ( (v87 & 0x80u) == 0 )
                {
                  v90 = &v8[v87];
                  v87 = (v87 & 0xff00) + *v90;
                  v8 = v90 + 1;
                  if ( !(ubyte)v87 )
                    break;
                  if ( (v87 & 0x80u) == 0 )
                    v8 += v87;
                }
              }
            }
          }
        }
        ystep += v339;
        --h;
      }
      while ( h );
}

void LbSpriteDrawRemapUsingScalingSub08(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap, long v339)
{
  int h;
  ubyte *v8;
  int v72;
  ubyte *v73;
  ubyte *v74;
  ubyte *v75;
  char *v76;
  ubyte v77;
  int v78;
  int v79;
  int v80;
  int v81;
  int v82;
  int v83;
  int v84;
  int v85;
  int v86;
  ubyte *v309;
  int v321;

  h = sprite->SHeight;
  v8 = sprite->Data;
      do
      {
        if ( *((ulong *)ystep + 1) )
        {
          v309 = outbuf;
          v321 = h;
          v76 = xstep;
LABEL_181:
          while ( 1 )
          {
            v77 = *v8++;
            if ( !v77 )
              break;
            if ( (v77 & 0x80u) != 0 )
            {
              v77 = -(char)v77;
              v86 = &outbuf[-*(ulong *)v76];
              v76 += 8 * v77;
              outbuf = (ubyte *)(*(ulong *)v76 + v86);
            }
            else
            {
              do
              {
                while ( 1 )
                {
                  v78 = *((ulong *)v76 + 1);
                  if ( v78 )
                  {
                    v78 = (v78 & 0xff00) + *v8;
                    v76 += 8;
                    ++v8;
                    *outbuf++ = *(ubyte *)(cmap + v78);
                    if ( !--v77 )
                      goto LABEL_181;
                    v79 = *((ulong *)v76 + 1);
                    if ( v79 )
                    {
                      v79 = (v79 & 0xff00) + *v8;
                      v76 += 8;
                      ++v8;
                      *outbuf++ = *(ubyte *)(cmap + v79);
                      if ( !--v77 )
                        goto LABEL_181;
                      v80 = *((ulong *)v76 + 1);
                      if ( v80 )
                      {
                        v80 = (v80 & 0xff00) + *v8;
                        v76 += 8;
                        ++v8;
                        *outbuf++ = *(ubyte *)(cmap + v80);
                        if ( !--v77 )
                          goto LABEL_181;
                        v81 = *((ulong *)v76 + 1);
                        if ( v81 )
                        {
                          v81 = (v81 & 0xff00) + *v8;
                          v76 += 8;
                          ++v8;
                          *outbuf++ = *(ubyte *)(cmap + v81);
                          if ( !--v77 )
                            goto LABEL_181;
                          v82 = *((ulong *)v76 + 1);
                          if ( v82 )
                          {
                            v82 = (v82 & 0xff00) + *v8;
                            v76 += 8;
                            ++v8;
                            *outbuf++ = *(ubyte *)(cmap + v82);
                            if ( !--v77 )
                              goto LABEL_181;
                            v83 = *((ulong *)v76 + 1);
                            if ( v83 )
                            {
                              v83 = (v83 & 0xff00) + *v8;
                              v76 += 8;
                              ++v8;
                              *outbuf++ = *(ubyte *)(cmap + v83);
                              if ( !--v77 )
                                goto LABEL_181;
                              v84 = *((ulong *)v76 + 1);
                              if ( v84 )
                              {
                                v84 = (v84 & 0xff00) + *v8;
                                v76 += 8;
                                ++v8;
                                *outbuf++ = *(ubyte *)(cmap + v84);
                                if ( !--v77 )
                                  goto LABEL_181;
                                v85 = *((ulong *)v76 + 1);
                                if ( v85 )
                                  break;
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                  ++v8;
                  v76 += 8;
                  if ( !--v77 )
                    goto LABEL_181;
                }
                v85 = (v85 & 0xff00) + *v8;
                v76 += 8;
                ++v8;
                *outbuf++ = *(ubyte *)(cmap + v85);
                --v77;
              }
              while ( v77 );
            }
          }
          h = v321;
          outbuf = &v309[scanline];
        }
        else
        {
          v72 = 0;
          while ( 1 )
          {
            v72 = (v72 & 0xff00) + *v8;
            v8++;
            if ( !(ubyte)v72 )
              break;
            if ( (v72 & 0x80u) == 0 )
            {
              v73 = &v8[v72];
              v72 = (v72 & 0xff00) + *v73;
              v8 = v73 + 1;
              if ( !(ubyte)v72 )
                break;
              if ( (v72 & 0x80u) == 0 )
              {
                v74 = &v8[v72];
                v72 = (v72 & 0xff00) + *v74;
                v8 = v74 + 1;
                if ( !(ubyte)v72 )
                  break;
                if ( (v72 & 0x80u) == 0 )
                {
                  v75 = &v8[v72];
                  v72 = (v72 & 0xff00) + *v75;
                  v8 = v75 + 1;
                  if ( !(ubyte)v72 )
                    break;
                  if ( (v72 & 0x80u) == 0 )
                    v8 += v72;
                }
              }
            }
          }
        }
        ystep += v339;
        --h;
      }
      while ( h );
}

void LbSpriteDrawRemapUsingScalingSub09(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap, long v339)
{
  int h;
  ubyte *v8;
  int v57;
  ubyte *v58;
  ubyte *v59;
  ubyte *v60;
  char *v61;
  ubyte v62;
  int v63;
  int v64;
  int v65;
  int v66;
  int v67;
  int v68;
  int v69;
  int v70;
  int v71;
  ubyte *v308;
  int v320;

  h = sprite->SHeight;
  v8 = sprite->Data;
      do
      {
        if ( *((ulong *)ystep + 1) )
        {
          v308 = outbuf;
          v320 = h;
          v61 = xstep;
LABEL_142:
          while ( 1 )
          {
            v62 = *v8++;
            if ( !v62 )
              break;
            if ( (v62 & 0x80u) != 0 )
            {
              v62 = -(char)v62;
              v71 = &outbuf[-*(ulong *)v61 - *((ulong *)v61 + 1)];
              v61 -= 8 * v62;
              outbuf = (ubyte *)(*(ulong *)v61 + *((ulong *)v61 + 1) + v71);
            }
            else
            {
              do
              {
                while ( 1 )
                {
                  v63 = *((ulong *)v61 + 1);
                  if ( v63 )
                  {
                    v63 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                    v61 -= 8;
                    ++v8;
                    *outbuf-- = ghost_table[v63];
                    if ( !--v62 )
                      goto LABEL_142;
                    v64 = *((ulong *)v61 + 1);
                    if ( v64 )
                    {
                      v64 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                      v61 -= 8;
                      ++v8;
                      *outbuf-- = ghost_table[v64];
                      if ( !--v62 )
                        goto LABEL_142;
                      v65 = *((ulong *)v61 + 1);
                      if ( v65 )
                      {
                        v65 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                        v61 -= 8;
                        ++v8;
                        *outbuf-- = ghost_table[v65];
                        if ( !--v62 )
                          goto LABEL_142;
                        v66 = *((ulong *)v61 + 1);
                        if ( v66 )
                        {
                          v66 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                          v61 -= 8;
                          ++v8;
                          *outbuf-- = ghost_table[v66];
                          if ( !--v62 )
                            goto LABEL_142;
                          v67 = *((ulong *)v61 + 1);
                          if ( v67 )
                          {
                            v67 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                            v61 -= 8;
                            ++v8;
                            *outbuf-- = ghost_table[v67];
                            if ( !--v62 )
                              goto LABEL_142;
                            v68 = *((ulong *)v61 + 1);
                            if ( v68 )
                            {
                              v68 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                              v61 -= 8;
                              ++v8;
                              *outbuf-- = ghost_table[v68];
                              if ( !--v62 )
                                goto LABEL_142;
                              v69 = *((ulong *)v61 + 1);
                              if ( v69 )
                              {
                                v69 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                                v61 -= 8;
                                ++v8;
                                *outbuf-- = ghost_table[v69];
                                if ( !--v62 )
                                  goto LABEL_142;
                                v70 = *((ulong *)v61 + 1);
                                if ( v70 )
                                  break;
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                  ++v8;
                  v61 -= 8;
                  if ( !--v62 )
                    goto LABEL_142;
                }
                v70 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                v61 -= 8;
                ++v8;
                *outbuf-- = ghost_table[v70];
                --v62;
              }
              while ( v62 );
            }
          }
          h = v320;
          outbuf = &v308[scanline];
        }
        else
        {
          v57 = 0;
          while ( 1 )
          {
            v57 = (v57 & 0xff00) + *v8;
            v8++;
            if ( !(ubyte)v57 )
              break;
            if ( (v57 & 0x80u) == 0 )
            {
              v58 = &v8[v57];
              v57 = (v57 & 0xff00) + *v58;
              v8 = v58 + 1;
              if ( !(ubyte)v57 )
                break;
              if ( (v57 & 0x80u) == 0 )
              {
                v59 = &v8[v57];
                v57 = (v57 & 0xff00) + *v59;
                v8 = v59 + 1;
                if ( !(ubyte)v57 )
                  break;
                if ( (v57 & 0x80u) == 0 )
                {
                  v60 = &v8[v57];
                  v57 = (v57 & 0xff00) + *v60;
                  v8 = v60 + 1;
                  if ( !(ubyte)v57 )
                    break;
                  if ( (v57 & 0x80u) == 0 )
                    v8 += v57;
                }
              }
            }
          }
        }
        ystep += v339;
        --h;
      }
      while ( h );
}

void LbSpriteDrawRemapUsingScalingSub10(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap, long v339)
{
  int h;
  ubyte *v8;
  ubyte v42;
  ubyte *v43;
  ubyte *v44;
  ubyte *v45;
  long *v46;
  ubyte v47;
  int v48;
  int v49;
  int v50;
  int v51;
  int v52;
  int v53;
  int v54;
  int v55;
  int v56;
  ubyte *v307;
  int v319;

  h = sprite->SHeight;
  v8 = sprite->Data;
      do
      {
        if ( ystep[1] )
        {
          v307 = outbuf;
          v319 = h;
          v46 = xstep;
LABEL_104:
          while ( 1 )
          {
            v47 = *v8++;
            if ( !v47 )
              break;
            if ( (v47 & 0x80u) != 0 )
            {
              v47 = -(char)v47;
              v56 = &outbuf[-v46[0]];
              v46 += 2 * v47;
              outbuf = (ubyte *)(v46[0] + v56);
            }
            else
            {
              do
              {
                while ( 1 )
                {
                  v48 = v46[1];
                  if ( v48 )
                  {
                    v48 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                    v46 += 2;
                    ++v8;
                    *outbuf++ = ghost_table[v48];
                    if ( !--v47 )
                      goto LABEL_104;
                    v49 = v46[1];
                    if ( v49 )
                    {
                     v49 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                      v46 += 2;
                      ++v8;
                      *outbuf++ = ghost_table[v49];
                      if ( !--v47 )
                        goto LABEL_104;
                      v50 = v46[1];
                      if ( v50 )
                      {
                       v50 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                        v46 += 2;
                        ++v8;
                        *outbuf++ = ghost_table[v50];
                        if ( !--v47 )
                          goto LABEL_104;
                        v51 = v46[1];
                        if ( v51 )
                        {
                         v51 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                          v46 += 2;
                          ++v8;
                          *outbuf++ = ghost_table[v51];
                          if ( !--v47 )
                            goto LABEL_104;
                          v52 = v46[1];
                          if ( v52 )
                          {
                            v52 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                            v46 += 2;
                            ++v8;
                            *outbuf++ = ghost_table[v52];
                            if ( !--v47 )
                              goto LABEL_104;
                            v53 = v46[1];
                            if ( v53 )
                            {
                              v53 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                              v46 += 2;
                              ++v8;
                              *outbuf++ = ghost_table[v53];
                              if ( !--v47 )
                                goto LABEL_104;
                              v54 = v46[1];
                              if ( v54 )
                              {
                                v54 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                                v46 += 2;
                                ++v8;
                                *outbuf++ = ghost_table[v54];
                                if ( !--v47 )
                                  goto LABEL_104;
                                v55 = v46[1];
                                if ( v55 )
                                  break;
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                  ++v8;
                  v46 += 2;
                  if ( !--v47 )
                    goto LABEL_104;
                }
                v55 = (((*v8) << 8) & 0xff00) + (*outbuf & 0x00ff);
                v46 += 2;
                ++v8;
                *outbuf++ = ghost_table[v55];
                --v47;
              }
              while ( v47 );
            }
          }
          h = v319;
          outbuf = &v307[scanline];
        }
        else
        {
          v42 = 0;
          while ( 1 )
          {
            v42 = *v8++;
            if ( !v42 )
              break;
            if ( (v42 & 0x80u) == 0 )
            {
              v43 = &v8[v42];
              v42 = *v43;
              v8 = v43 + 1;
              if ( !v42 )
                break;
              if ( (v42 & 0x80u) == 0 )
              {
                v44 = &v8[v42];
                v42 = *v44;
                v8 = v44 + 1;
                if ( !v42 )
                  break;
                if ( (v42 & 0x80u) == 0 )
                {
                  v45 = &v8[v42];
                  v42 = *v45;
                  v8 = v45 + 1;
                  if ( !v42 )
                    break;
                  if ( (v42 & 0x80u) == 0 )
                    v8 += v42;
                }
              }
            }
          }
        }
        ystep += v339;
        --h;
      }
      while ( h );
}

void LbSpriteDrawRemapUsingScalingSub11(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap, long v339)
{
  int h;
  ubyte *v8;
  ubyte v27;
  ubyte *v28;
  ubyte *v29;
  ubyte *v30;
  char *v31;
  ubyte v32;
  ubyte v33;
  ubyte v34;
  ubyte v35;
  ubyte v36;
  ubyte v37;
  ubyte v38;
  ubyte v39;
  ubyte v40;
  int v41;
  ubyte *v306;
  int v318;

  h = sprite->SHeight;
  v8 = sprite->Data;
    do
    {
      if ( *((ulong *)ystep + 1) )
      {
        v306 = outbuf;
        v318 = h;
        v31 = xstep;
LABEL_65:
        while ( 1 )
        {
          v32 = *v8++;
          if ( !v32 )
            break;
          if ( (v32 & 0x80u) != 0 )
          {
            v32 = -(char)v32;
            v41 = &outbuf[-*(ulong *)v31 - *((ulong *)v31 + 1)];
            v31 -= 8 * v32;
            outbuf = (ubyte *)(*(ulong *)v31 + *((ulong *)v31 + 1) + v41);
          }
          else
          {
            do
            {
              while ( 1 )
              {
                if ( *((ulong *)v31 + 1) )
                {
                  v33 = *v8++;
                  v31 -= 8;
                  *outbuf-- = v33;
                  if ( !--v32 )
                    goto LABEL_65;
                  if ( *((ulong *)v31 + 1) )
                  {
                    v34 = *v8++;
                    v31 -= 8;
                    *outbuf-- = v34;
                    if ( !--v32 )
                      goto LABEL_65;
                    if ( *((ulong *)v31 + 1) )
                    {
                      v35 = *v8++;
                      v31 -= 8;
                      *outbuf-- = v35;
                      if ( !--v32 )
                        goto LABEL_65;
                      if ( *((ulong *)v31 + 1) )
                      {
                        v36 = *v8++;
                        v31 -= 8;
                        *outbuf-- = v36;
                        if ( !--v32 )
                          goto LABEL_65;
                        if ( *((ulong *)v31 + 1) )
                        {
                          v37 = *v8++;
                          v31 -= 8;
                          *outbuf-- = v37;
                          if ( !--v32 )
                            goto LABEL_65;
                          if ( *((ulong *)v31 + 1) )
                          {
                            v38 = *v8++;
                            v31 -= 8;
                            *outbuf-- = v38;
                            if ( !--v32 )
                              goto LABEL_65;
                            if ( *((ulong *)v31 + 1) )
                            {
                              v39 = *v8++;
                              v31 -= 8;
                              *outbuf-- = v39;
                              if ( !--v32 )
                                goto LABEL_65;
                              if ( *((ulong *)v31 + 1) )
                                break;
                            }
                          }
                        }
                      }
                    }
                  }
                }
                ++v8;
                v31 -= 8;
                if ( !--v32 )
                  goto LABEL_65;
              }
              v40 = *v8++;
              v31 -= 8;
              *outbuf-- = v40;
              --v32;
            }
            while ( v32 );
          }
        }
        h = v318;
        outbuf = &v306[scanline];
      }
      else
      {
        v27 = 0;
        while ( 1 )
        {
          v27 = *v8++;
          if ( !v27 )
            break;
          if ( (v27 & 0x80u) == 0 )
          {
            v28 = &v8[v27];
            v27 = *v28;
            v8 = v28 + 1;
            if ( !v27 )
              break;
            if ( (v27 & 0x80u) == 0 )
            {
              v29 = &v8[v27];
              v27 = *v29;
              v8 = v29 + 1;
              if ( !v27 )
                break;
              if ( (v27 & 0x80u) == 0 )
              {
                v30 = &v8[v27];
                v27 = *v30;
                v8 = v30 + 1;
                if ( !v27 )
                  break;
                if ( (v27 & 0x80u) == 0 )
                  v8 += v27;
              }
            }
          }
        }
      }
      ystep += v339;
      --h;
    }
    while ( h );
}

void LbSpriteDrawRemapUsingScalingSub12(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap, long v339)
{
  int h;
  ubyte *v8;
  ubyte v12;
  ubyte *v13;
  ubyte *v14;
  ubyte *v15;
  long *v16;
  ubyte v17;
  ubyte v18;
  ubyte v19;
  ubyte v20;
  ubyte v21;
  ubyte v22;
  ubyte v23;
  ubyte v24;
  ubyte v25;
  int v26;
  ubyte *v305;
  int v317;

  h = sprite->SHeight;
  v8 = sprite->Data;
    do
    {
      if ( *((ulong *)ystep + 1) )
      {
        v305 = outbuf;
        v317 = h;
        v16 = xstep;
LABEL_27:
        while ( 1 )
        {
          v17 = *v8++;
          if ( !v17 )
            break;
          if ( (v17 & 0x80u) != 0 )
          {
            v17 = -(char)v17;
            v26 = &outbuf[-v16[0]];
            v16 += 2 * v17;
            outbuf = (ubyte *)(v16[0] + v26);
          }
          else
          {
            do
            {
              while ( 1 )
              {
                if ( v16[1] )
                {
                  v18 = *v8++;
                  v16 += 2;
                  *outbuf++ = v18;
                  if ( !--v17 )
                    goto LABEL_27;
                  if ( v16[1] )
                  {
                    v19 = *v8++;
                    v16 += 2;
                    *outbuf++ = v19;
                    if ( !--v17 )
                      goto LABEL_27;
                    if ( v16[1] )
                    {
                      v20 = *v8++;
                      v16 += 2;
                      *outbuf++ = v20;
                      if ( !--v17 )
                        goto LABEL_27;
                      if ( v16[1] )
                      {
                        v21 = *v8++;
                        v16 += 2;
                        *outbuf++ = v21;
                        if ( !--v17 )
                          goto LABEL_27;
                        if ( v16[1] )
                        {
                          v22 = *v8++;
                          v16 += 2;
                          *outbuf++ = v22;
                          if ( !--v17 )
                            goto LABEL_27;
                          if ( v16[1] )
                          {
                            v23 = *v8++;
                            v16 += 2;
                            *outbuf++ = v23;
                            if ( !--v17 )
                              goto LABEL_27;
                            if ( v16[1] )
                            {
                              v24 = *v8++;
                              v16 += 2;
                              *outbuf++ = v24;
                              if ( !--v17 )
                                goto LABEL_27;
                              if ( v16[1] )
                                break;
                            }
                          }
                        }
                      }
                    }
                  }
                }
                ++v8;
                v16 += 2;
                if ( !--v17 )
                  goto LABEL_27;
              }
              v25 = *v8++;
              v16 += 2;
              *outbuf++ = v25;
              --v17;
            }
            while ( v17 );
          }
        }
        h = v317;
        outbuf = &v305[scanline];
      }
      else
      {
        v12 = 0;
        while ( 1 )
        {
          v12 = *v8++;
          if ( !v12 )
            break;
          if ( (v12 & 0x80u) == 0 )
          {
            v13 = &v8[v12];
            v12 = *v13;
            v8 = v13 + 1;
            if ( !v12 )
              break;
            if ( (v12 & 0x80u) == 0 )
            {
              v14 = &v8[v12];
              v12 = *v14;
              v8 = v14 + 1;
              if ( !v12 )
                break;
              if ( (v12 & 0x80u) == 0 )
              {
                v15 = &v8[v12];
                v12 = *v15;
                v8 = v15 + 1;
                if ( !v12 )
                  break;
                if ( (v12 & 0x80u) == 0 )
                  v8 += v12;
              }
            }
          }
        }
      }
      ystep += v339;
      --h;
    }
    while ( h );
}

/**
 * Draws a scaled sprite on current graphics window at given position, ignoring shadows.
 * Requires SetAlphaScalingData() to be called before.
 * This is a modification of LbSpriteDrawUsingScalingData() which changes meaning
 * of Lb_TEXT_UNDERLNSHADOW and Lb_SPRITE_TRANSPAR8. No idea why, but it's used in Syndicate Wars.
 *
 * @param posx The X coord within current graphics window.
 * @param posy The Y coord within current graphics window.
 * @param sprite The source sprite.
 * @return Gives 0 on success.
 * @see LbSpriteSetScalingData()
 */
TbResult DrawSpriteWthShadowUsingScalingData(long posx, long posy, const TbSprite *sprite)
{
  int scanline;
  long *ystep;
  long *xstep;
  long v339;

    //TODO set this in higher level function instead, when possible
    render_alpha = lbSpriteReMapPtr;
    assert(render_alpha != NULL);
    LIBLOG("Drawing at (%ld,%ld)",posx,posy);

  {
    scanline = lbDisplay.GraphicsScreenWidth;
    v339 = 2;
    if ( lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ )
      posx = sprite->SWidth + posx - 1;
    if ( lbDisplay.DrawFlags & Lb_SPRITE_FLIP_VERTIC )
    {
      posy = sprite->SHeight + posy - 1;
      scanline = -lbDisplay.GraphicsScreenWidth;
      v339 = -2;
    }
    xstep = &alpha_xsteps_array[2 * posx];
    ystep = &alpha_ysteps_array[2 * posy];
  }
    int outheight;
  ubyte *outbuf;
  {
    int gspos_x,gspos_y;
    gspos_y = ystep[0];
    if ( lbDisplay.DrawFlags & Lb_SPRITE_FLIP_VERTIC )
      gspos_y += ystep[1] - 1;
    gspos_x = xstep[0];
    if ( lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ )
      gspos_x += xstep[1] - 1;
    outbuf = &lbDisplay.GraphicsWindowPtr[gspos_x + lbDisplay.GraphicsScreenWidth * gspos_y];
    outheight = lbDisplay.GraphicsScreenHeight;
  }

  if ( alpha_scale_up )
  {
    if ( lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8 )
    {
      if ( lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ )
      {
          LbSpriteDrawRemapUsingScalingSub1(outbuf, scanline,
                outheight, xstep, ystep, sprite, render_alpha, v339);
      }
      else
      {
          LbSpriteDrawRemapUsingScalingSub2(outbuf, scanline,
                outheight, xstep, ystep, sprite, render_alpha, v339);
      }
    }
    else if ( lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4 )
    {
      if ( lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ )
      {
          LbSpriteDrawRemapUsingScalingSub3(outbuf, scanline,
                outheight, xstep, ystep, sprite, render_alpha, v339);
      }
      else
      {
          LbSpriteDrawRemapUsingScalingSub4(outbuf, scanline,
                outheight, xstep, ystep, sprite, render_alpha, v339);
      }
    } else {
      if ( lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ )
      {
          LbSpriteDrawRemapUsingScalingSub5(outbuf, scanline,
                outheight, xstep, ystep, sprite, render_alpha, v339);
      }
      else
      {
          LbSpriteDrawRemapUsingScalingSub6(outbuf, scanline,
                outheight, xstep, ystep, sprite, render_alpha, v339);
      }
    }
  } else {
    if ( lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8 )
    {
      if ( lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ )
      {
            LbSpriteDrawRemapUsingScalingSub7(outbuf, scanline,
                outheight, xstep, ystep, sprite, render_alpha, v339);
      }
      else
      {
            LbSpriteDrawRemapUsingScalingSub08(outbuf, scanline,
                outheight, xstep, ystep, sprite, render_alpha, v339);
      }
    }
    else if ( lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4 )
    {
      if ( lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ )
      {
            LbSpriteDrawRemapUsingScalingSub09(outbuf, scanline,
                outheight, xstep, ystep, sprite, render_alpha, v339);
      }
      else
      {
            LbSpriteDrawRemapUsingScalingSub10(outbuf, scanline,
                outheight, xstep, ystep, sprite, render_alpha, v339);
      }
    }
    else {
      if ( lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ )
      {
            LbSpriteDrawRemapUsingScalingSub11(outbuf, scanline,
                outheight, xstep, ystep, sprite, render_alpha, v339);
      }
      else
      {
            LbSpriteDrawRemapUsingScalingSub12(outbuf, scanline,
                outheight, xstep, ystep, sprite, render_alpha, v339);
      }
    }
  }
}
/******************************************************************************/
