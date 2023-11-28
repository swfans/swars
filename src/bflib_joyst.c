/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_joyst.c
 *     Joystick support.
 * @par Purpose:
 *     Implement Joystick input support for games.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     11 Feb 2010 - 11 Oct 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_joyst.h"

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>

/******************************************************************************/
extern uint32_t grip_unkvar080;

/******************************************************************************/
int joy_grip_unknsub_08(int val)
{
  if ( !val )
    return -1;
  grip_unkvar080 = val;
  return 1;
}

int joy_func_065(struct DevInput *dinp)
{
    struct SampleInfo *ret;
    asm volatile (
      "call ASM_joy_func_065\n"
        : "=r" (ret) : "a" (dinp));
    return ret;
}
/******************************************************************************/
