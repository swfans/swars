/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file arctan.c
 *     Implementation of related functions.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfmath.h"

unsigned short const lbArcTanFactors[] = {
     0,  1,  2,  3,  5,  6,  7,  8, 10, 11, 12, 13, 15, 16, 17, 19,
    20, 21, 22, 24, 25, 26, 27, 29, 30, 31, 32, 34, 35, 36, 38, 39,
    40, 41, 43, 44, 45, 46, 48, 49, 50, 51, 53, 54, 55, 56, 57, 59,
    60, 61, 62, 64, 65, 66, 67, 68, 70, 71, 72, 73, 75, 76, 77, 78,
    79, 81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 92, 94, 95, 96, 97,
    98, 99,101,102,103,104,105,106,107,109,110,111,112,113,114,115,
   116,118,119,120,121,122,123,124,125,126,127,129,130,131,132,133,
   134,135,136,137,138,139,140,141,142,143,144,145,147,148,149,150,
   151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,
   167,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,
   182,182,183,184,185,186,187,188,189,190,191,191,192,193,194,195,
   196,197,198,198,199,200,201,202,203,203,204,205,206,207,208,208,
   209,210,211,212,212,213,214,215,216,216,217,218,219,220,220,221,
   222,223,223,224,225,226,226,227,228,229,229,230,231,232,232,233,
   234,235,235,236,237,237,238,239,239,240,241,242,242,243,244,244,
   245,246,246,247,248,248,249,250,250,251,252,252,253,254,254,255,
   256 };

/** Computes angle between negative Y axis and the line that crosses (0,0) and given (x,y).
 *  Uses arctan(x/y) with proper shift to get the angle.
 *  Returning 0 means direction towards negative y; 512 is towards positive x;
 *  1024 towards positive y, and 1536 towards negative x. Output range is between 0 (=0 rad)
 *  and 2048 (=2*pi rad), zero included.
 *  Value of the angle is properly rounded, up or down.
 *
 * @param x
 * @param y
 * @return
 */
long LbArcTanAngle(long x,long y)
{
    // code at 0001:000a329c
    long ux,uy;
    unsigned long index;
    if ((x == 0) && (y == 0))
        return 0;
    if (x < 0)
    {
        ux = -x;
        if (y < 0)
        {
            uy = -y;
            // Make sure we'll have smaller value * 256 / greater value.
            // This way we won't exceed factors array bounds (which is 256 elements).
            if (ux < uy) {
                index = (ux << 8)/uy;
                return 2*LbFPMath_PI   - (long)lbArcTanFactors[index];
            } else {
                index = (uy << 8)/ux;
                return 3*LbFPMath_PI/2 + (long)lbArcTanFactors[index];
            }
        } else
        {
            uy = y;
            // Make sure we'll have smaller value * 256 / greater value.
            if (ux < uy) {
                index = (ux << 8)/uy;
                return   LbFPMath_PI   + (long)lbArcTanFactors[index];
            } else {
                index = (uy << 8)/ux;
                return 3*LbFPMath_PI/2 - (long)lbArcTanFactors[index];
            }
        }
    } else
    {
        ux = x;
        if (y < 0)
        {
            uy = -y;
            // Make sure we'll have smaller value * 256 / greater value.
            if (ux < uy) {
                index = (ux << 8)/uy;
                return                 (long)lbArcTanFactors[index];
            } else {
                index = (uy << 8)/ux;
                return LbFPMath_PI/2 - (long)lbArcTanFactors[index];
            }
        } else
        {
            uy = y;
            // Make sure we'll have smaller value * 256 / greater value.
            if (ux < uy) {
                index = (ux << 8)/uy;
                return LbFPMath_PI   - (long)lbArcTanFactors[index];
            } else {
                index = (uy << 8)/ux;
                return LbFPMath_PI/2 + (long)lbArcTanFactors[index];
            }
        }
    }
}


/******************************************************************************/
