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

extern int32_t dword_1E2F24;
extern int32_t dword_1E2F28;
extern int32_t dword_1E2F2C;
extern int32_t dword_1E2F30;
extern uint8_t byte_1E2F34;
/******************************************************************************/
int joy_grip_unknsub_08(int val)
{
    if (!val)
        return -1;
    grip_unkvar080 = val;
    return 1;
}

int joy_func_251(int val, int acen, int amin, int amax)
{
    int ret;
    asm volatile (
      "call ASM_joy_func_251\n"
        : "=r" (ret) : "a" (val), "d" (acen), "b" (amin), "c" (amax));
    return ret;
}

void joy_func_065_lab93(struct DevInput *dinp, short ipos)
{
    long val, thresh;

    switch (dinp->ConfigType[ipos])
    {
    case 1:
    case 3:
    case 12:
        byte_1E2F34 = ~(byte_1E2F34 >> 4) & 0xF;
        dinp->Buttons[ipos] = (byte_1E2F34 | dinp->Buttons[ipos]) & 3;
        break;
    case 2:
    case 4:
    case 5:
    case 7:
    case 9:
    case 13:
    case 21:
        byte_1E2F34 = ~(byte_1E2F34 >> 4) & 0xF;
        dinp->Buttons[ipos] |= byte_1E2F34;
        if (dinp->ConfigType[ipos] == 9)
        {
            if (dword_1E2F2C < dinp->HatMax[ipos])
                dinp->Buttons[ipos] |= 0x10;
            if (dword_1E2F30 < dinp->HatMax[ipos])
                dinp->Buttons[ipos] |= 0x20;
        }
        break;
    case 6:
    case 8:
    case 10:
    case 11:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
        break;
    case 20:
        byte_1E2F34 = ~(byte_1E2F34 >> 4) & 0xF;
        dinp->Buttons[ipos] = 0;
        dinp->HatY[ipos] = 0;
        dinp->HatX[ipos] = dinp->HatY[ipos];
        switch (byte_1E2F34)
        {
        case 15:
            dinp->HatY[ipos] = -1;
            break;
        case 11:
            dinp->HatX[ipos] = 1;
            break;
        case 7:
            dinp->HatY[ipos] = 1;
            break;
        case 3:
            dinp->HatX[ipos] = -1;
            break;
        default:
            dinp->Buttons[ipos] = byte_1E2F34;
            break;
        }
        break;
    default:
        break;
    }

    {
        if (dinp->MinXAxis[ipos] > dword_1E2F24)
            dinp->MinXAxis[ipos] = dword_1E2F24;
        if (dinp->MaxXAxis[ipos] < dword_1E2F24)
            dinp->MaxXAxis[ipos] = dword_1E2F24;
    }
    {
        if (dinp->MinYAxis[ipos] > dword_1E2F28)
            dinp->MinYAxis[ipos] = dword_1E2F28;
        if (dinp->MaxYAxis[ipos] < dword_1E2F28)
            dinp->MaxYAxis[ipos] = dword_1E2F28;
    }
    if ((dinp->DeviceType[ipos] & 0x80) != 0)
    {
        if (dinp->MinZAxis[ipos] > dword_1E2F30)
            dinp->MinZAxis[ipos] = dword_1E2F30;
        if (dinp->MaxZAxis[ipos] < dword_1E2F30)
            dinp->MaxZAxis[ipos] = dword_1E2F30;
    }

    {
        dinp->XCentre[ipos] = dinp->MinXAxis[ipos] + ((dinp->MaxXAxis[ipos] - dinp->MinXAxis[ipos]) >> 1);
    }
    {
        dinp->YCentre[ipos] = dinp->MinYAxis[ipos] + ((dinp->MaxYAxis[ipos] - dinp->MinYAxis[ipos]) >> 1);
    }
    if ((dinp->DeviceType[ipos] & 0x80) != 0) {
        dinp->ZCentre[ipos] = dinp->MinZAxis[ipos] + ((dinp->MaxZAxis[ipos] - dinp->MinZAxis[ipos]) >> 1);
    }

    {
        thresh = dinp->XCentre[ipos] >> 1;
        val = 0;
        if (dinp->MinXAxis[ipos] + thresh > dword_1E2F24)
            val = -1;
        if (dinp->MaxXAxis[ipos] - thresh < dword_1E2F24)
            val = 1;
        dinp->DigitalX[ipos] = val;
    }
    {
        thresh = dinp->YCentre[ipos] >> 1;
        val = 0;
        if (dinp->MinYAxis[ipos] + thresh > dword_1E2F28)
            val = -1;
        if (dinp->MaxYAxis[ipos] - thresh < dword_1E2F28)
            val = 1;
        dinp->DigitalY[ipos] = val;
    }
    if ((dinp->DeviceType[ipos] & 0x80) != 0)
    {
        thresh = dinp->ZCentre[ipos] >> 1;
        val = 0;
        if (dinp->MinZAxis[ipos] + thresh > dword_1E2F30)
            val = -1;
        if (dinp->MaxZAxis[ipos] - thresh < dword_1E2F30)
            val = 1;
        dinp->DigitalZ[ipos] = val;
    }

    {
      val = joy_func_251(dword_1E2F24, dinp->XCentre[ipos], dinp->MinXAxis[ipos], dinp->MaxXAxis[ipos]);
      dinp->AnalogueX[ipos] = val;
    }
    {
      val = joy_func_251(dword_1E2F28, dinp->YCentre[ipos], dinp->MinYAxis[ipos], dinp->MaxYAxis[ipos]);
      dinp->AnalogueY[ipos] = val;
    }
    if ((dinp->DeviceType[ipos] & 0x80) != 0)
    {
        val = joy_func_251(dword_1E2F30, dinp->ZCentre[ipos], dinp->MinZAxis[ipos], dinp->MaxZAxis[ipos]);
        dinp->AnalogueZ[ipos] = val;
    }
}

int joy_func_065_sub8(struct DevInput *dinp, short ipos)
{
    long val, thresh;

#if defined(DOS)||defined(GO32)
    {
        struct SpwRawData spla;

        SpwSimpleGet(0, &spla);
        dinp->Init[ipos] = 1;

        dinp->AnalogueX[ipos] = spla.anx + 512;
        dinp->AnalogueY[ipos] = spla.any + 512;
        dinp->AnalogueZ[ipos] = spla.anz + 512;
        dinp->AnalogueR[ipos] = spla.anr + 512;
        dinp->AnalogueU[ipos] = spla.anu + 512;
        dinp->AnalogueV[ipos] = spla.anv + 512;
        dinp->Buttons[ipos] = spla.butn;
    }
#endif

    thresh = dinp->XCentre[ipos] >> 1;
    val = 0;
    if (dinp->AnalogueX[ipos] < dinp->MinXAxis[ipos] + thresh)
        val = -1;
    if (dinp->AnalogueX[ipos] > dinp->MaxXAxis[ipos] - thresh)
        val = 1;
    dinp->DigitalX[ipos] = val;

    thresh = dinp->YCentre[ipos] >> 1;
    val = 0;
    if (dinp->AnalogueY[ipos] < dinp->MinYAxis[ipos] + thresh)
        val = -1;
    if (dinp->AnalogueY[ipos] > dinp->MaxYAxis[ipos] - thresh)
        val = 1;
    dinp->DigitalY[ipos] = val;

    thresh = dinp->ZCentre[ipos] >> 1;
    val = 0;
    if (dinp->AnalogueZ[ipos] < dinp->MinZAxis[ipos] + thresh)
        val = -1;
    if (dinp->AnalogueZ[ipos] > dinp->MaxZAxis[ipos] - thresh)
        val = 1;
    dinp->DigitalZ[ipos] = val;

    thresh = dinp->RCentre[ipos] >> 1;
    val = 0;
    if (dinp->AnalogueR[ipos] < dinp->MinRAxis[ipos] + thresh)
        val = -1;
    if (dinp->AnalogueR[ipos] > dinp->MaxRAxis[ipos] - thresh)
        val = 1;
    dinp->DigitalR[ipos] = val;

    thresh = dinp->UCentre[ipos] >> 1;
    val = 0;
    if (dinp->AnalogueU[ipos] < dinp->MinUAxis[ipos] + thresh)
        val = -1;
    if (dinp->AnalogueU[ipos] > dinp->MaxUAxis[ipos] - thresh)
        val = 1;
    dinp->DigitalU[ipos] = val;

    thresh = dinp->VCentre[ipos] >> 1;
    val = 0;
    if (dinp->AnalogueV[ipos] < dinp->MinVAxis[ipos] + thresh)
        val = -1;
    if (dinp->AnalogueV[ipos] > dinp->MaxVAxis[ipos] - thresh)
        val = 1;
    dinp->DigitalV[ipos] = val;

    val = joy_func_251(dinp->AnalogueX[ipos], dinp->XCentre[ipos], dinp->MinXAxis[ipos], dinp->MaxXAxis[ipos]);
    dinp->AnalogueX[ipos] = val;
    val = joy_func_251(dinp->AnalogueY[ipos], dinp->YCentre[ipos], dinp->MinYAxis[ipos], dinp->MaxYAxis[ipos]);
    dinp->AnalogueY[ipos] = val;
    val = joy_func_251(dinp->AnalogueZ[ipos], dinp->ZCentre[ipos], dinp->MinZAxis[ipos], dinp->MaxZAxis[ipos]);
    dinp->AnalogueZ[ipos] = val;
    val = joy_func_251(dinp->AnalogueR[ipos], dinp->RCentre[ipos], dinp->MinRAxis[ipos], dinp->MaxRAxis[ipos]);
    dinp->AnalogueR[ipos] = val;
    val = joy_func_251(dinp->AnalogueU[ipos], dinp->UCentre[ipos], dinp->MinUAxis[ipos], dinp->MaxUAxis[ipos]);
    dinp->AnalogueU[ipos] = val;
    val = joy_func_251(dinp->AnalogueV[ipos], dinp->VCentre[ipos], dinp->MinVAxis[ipos], dinp->MaxVAxis[ipos]);
    dinp->AnalogueV[ipos] = val;
    return 0;
}

int joy_func_065_sub9(struct DevInput *dinp, short ipos)
{
#if defined(DOS)||defined(GO32)
    int i;
    ubyte val;

    val = dinp->ConfigType[ipos] - 1;
    _disable();
    __outbyte(0x201, val);
    for (i = 10000; i > 0; i--)
    {
        val = __inbyte(0x201);
        if ((val & 0xB) == 0)
          break;
        if ((val & 1) == 0)
          ++dword_1E2F24;
        if ((val & 2) == 0)
          ++dword_1E2F28;
        if ((val & 8) == 0)
          ++dword_1E2F30;
    }
    _enable();
    byte_1E2F34 = val;
    if (i == 0)
        return 0;
#endif
    joy_func_065_lab93(dinp, ipos);
    return 0;
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

int joy_grip_shutdown(void)
{
#if defined(DOS)||defined(GO32)
    // Shutdown the GrIP system
    GrShutdown();
    // Disconnect from the GrIP library
    GrUnlink();
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

int joy_spaceball_shutdown(void)
{
#if defined(DOS)||defined(GO32)
    short ret;
    ret = SpwSimpleClose(0);
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
        if (ret == 1)
            joy_spbal_initialized = 1;
    }
    return 1;
#endif
}

/** Joystick drivers shutdown.
 */
int joy_driver_shutdown(void)
{
    if (joy_grip_initialized)
    {
        joy_grip_shutdown();
        joy_grip_initialized = 0;
    }
    if (joy_spbal_initialized)
    {
        joy_spaceball_shutdown();
        joy_spbal_initialized = 0;
    }
    return 1;
}
/******************************************************************************/
