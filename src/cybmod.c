/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file cybmod.c
 *     Agent mods related functions.
 * @par Purpose:
 *     Implement functions for handling cybernetic modifications.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "cybmod.h"
/******************************************************************************/
struct ModDef mod_defs_DISA[] = {
    {0, 0,   0, 0, {0},  0,    0,  0, 0},
    {3, 2,  30, 0, {0}, 16,  300, 10, 0},
    {5, 5, 120, 0, {0}, 17, 1200, 10, 0},
    {8, 8, 300, 0, {0}, 19, 3000, 10, 0},
    {2, 2,  25, 0, {0}, 20,  250, 10, 0},
    {4, 3, 100, 0, {0}, 24, 1000, 10, 0},
    {7, 6, 250, 0, {0}, 21, 2500, 10, 0},
    {3, 3,  50, 0, {0}, 15,  500, 10, 0},
    {5, 6, 200, 0, {0}, 23, 2000, 10, 0},
    {8, 8, 500, 0, {0}, 29, 5000, 10, 0},
    {1, 2,  45, 0, {0}, 33,  450, 10, 0},
    {2, 3, 180, 0, {0}, 30, 1800, 10, 0},
    {4, 6, 450, 0, {0}, 27, 4500, 10, 0},
    {1, 5, 200, 0, {0}, 25, 2000, 10, 0},
    {1, 4, 350, 0, {0}, 25, 3500, 10, 0},
    {1, 7, 600, 0, {0}, 28, 6000, 10, 0},
    {1, 2, 950, 0, {0}, 35, 9500, 10, 0},
};

/******************************************************************************/
