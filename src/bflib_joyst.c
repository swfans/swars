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
#include <stdio.h>
#include "bfmemory.h"
#include "swlog.h"

#if defined(DOS)||defined(GO32)
#include "joy_grip.h"
#include "joy_splib.h"
#endif

/******************************************************************************/
extern uint32_t grip_unkvar080;
extern uint8_t joy_grip_initialized;
extern uint8_t joy_spbal_initialized;

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
    int ret;
    asm volatile (
      "call ASM_joy_func_065\n"
        : "=r" (ret) : "a" (dinp));
    return ret;
}

/** Gravis Grip joystick driver initialization.
 *
 * GrIP joystick support library initialization.
 * Real-mode implementation based on "GrIP API Demo Program" from SDK.
 */
int joy_grip_init(void)
{
#if defined(DOS)||defined(GO32)
    // Requires declarations from "joy_grip.h"

    // Full path for GrIP library
    char lib[128];
    // File handle for library
    FILE* flib;
    // Size of library file (bytes)
    long size;
    // Memory image of library file
    void* image;

    // Search for the GrIP library: GRIP.GLL
    //
    // First search in the current directory, then search in any directories
    // specified by the "SET GRIP=" environment variable, then search in any
    // directories in the path, and finally search in "C:\GRIP\".
    //
    // Note that the filename for the library ("GRIP.GLL"), the environment
    // variable name ("GRIP") and the default install directory ("C:\GRIP\")
    // are defined in the module GRIP.C as GrLibName, GrLibEnv, and
    // GrLibDir respectively.
    _searchenv(GrLibName, GrLibEnv, lib);
    if (lib[0] == 0)
        _searchenv(GrLibName, "PATH", lib);
    if (lib[0] == 0) {
        strcpy(lib, GrLibDir);
        strcat(lib, GrLibName);
    }
    // Open the loadable library file and determine its size
    if ((flib = fopen(lib, "rb")) == 0)
    {
        LOGERR("Unable to load library file \"%s\"", lib);
        return -1;
    }
    fseek(flib, 0, SEEK_END);
    size = ftell(flib);

    // Allocate memory for the library and read it all into memory
    //
    // (If the size is greater than 32k, then this is definitely not
    // the correct file, so signal an error.)
    if (size > 32*1024L)
    {
        LOGERR("Invalid library file \"%s\" (File too large)", lib);
        return -1;
    }
    image = LbMemoryAlloc(size);
    if (image == NULL)
    {
        LOGERR("Unable to allocate sufficient memory for library", lib);
        return -1;
    }
    fseek(flib, 0, SEEK_SET);
    fread(image, 1, size, flib);
    fclose(flib);

    // Link to the library (and free up the temporary memory)
    if (!GrLink(image, size))
    {
        LOGERR("Unable to initialize loadable library!");
        return -1;
    }
    LbMemoryFree(image);
    return 1;
#else
    // No joystick support lib required for modern OSes
    return 0;
#endif
}

/** SpaceBall from Spacetec IMC game controller driver initialization.
 */
int joy_spaceball_init(void)
{
#if defined(DOS)||defined(GO32)
    // Requires declarations from "joy_splib.h"
    short ret;
    ret = SpwSimpleOpen(0);
    return (ret != 0);
#else
    // No joystick support lib required for modern OSes
    return 0;
#endif
}

/** Joystick drivers initialization.
 */
int joy_driver_init(void)
{
#if 0
    int ret;
    asm volatile (
      "call ASM_joy_driver_init\n"
        : "=r" (ret) :);
    return ret;
#else
    int ret;
    if (!joy_grip_initialized)
    {
        ret = joy_grip_init();
        if (ret == 1)
            joy_grip_initialized = 1;
    }
    if (!joy_spbal_initialized)
    {
        ret = joy_spaceball_init();
        if (ret)
            joy_spbal_initialized = 1;
    }
    return 1;
#endif
}

/******************************************************************************/
