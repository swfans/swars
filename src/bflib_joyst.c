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
extern struct TbInputHandler *InputHandler;
extern uint8_t joy_grip_initialized;
extern uint8_t joy_spbal_initialized;

extern int32_t dword_1E2F24;
extern int32_t dword_1E2F28;
extern int32_t dword_1E2F2C;
extern int32_t dword_1E2F30;
extern uint8_t byte_1E2F34;

extern int16_t word_1E6078;
extern int16_t word_1E607A;
extern int16_t word_1E607C;

extern struct UnkVFXStruct1 vfxunk1;
/******************************************************************************/
int JoySetInterrupt(short val)
{
    if (!val)
        return -1;
    InputHandler->InterruptNo = val;
    return 1;
}

int vfx1_unkn_func_03(void)
{
    int ret;
    asm volatile (
      "call ASM_vfx1_unkn_func_03\n"
        : "=r" (ret) : );
    return ret;
}

int vfx1_unkn_func_12(struct UnkVFXStruct1 *a1, short a2)
{
    int ret;
    asm volatile (
      "call ASM_vfx1_unkn_func_12\n"
        : "=r" (ret) : "a" (a1), "d" (a2));
    return ret;
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

#if defined(DOS)||defined(GO32)

static int CallRealModeInterrupt(ushort intno, struct DPMI_REGS *dpmi_regs)
{
    union REGS regs;
    struct SREGS sregs;

    memset(&regs, 0, sizeof(union REGS));
    /* Use DMPI call 300h to issue the DOS interrupt */
    regs.x.eax = 0x300;
    regs.x.ebx = intno;
    segread(&sregs);
    regs.x.edi = (unsigned int)dpmi_regs;
    int386x(49, &regs, &regs, &sregs);
    return dpmi_regs->eax;
}

int CallJoy(ushort a1)
{
    struct DPMI_REGS dpmi_regs;

    InputHandler->field_2 = a1;
    memset(&dpmi_regs, 0, sizeof(struct DPMI_REGS));
    return CallRealModeInterrupt(InputHandler->InterruptNo, &dpmi_regs);
}

#endif

int joy_func_065_sub6(struct DevInput *dinp, short ipos)
{
    if (InputHandler == NULL) {
        dinp->Type = 17;
        return -1;
    }
#if defined(DOS)||defined(GO32)
    CallJoy(2);
    memcpy(dinp, &InputHandler->Input, sizeof(struct DevInput));
#endif
    dinp->Type = 17;
    return 1;
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

int joy_func_065_sub7(struct DevInput *dinp, short ipos)
{
#if defined(DOS)||defined(GO32)
    short gridx, i;
    ushort classmap;
    short val;
    long lval;

    GrRefresh(0);
    for (gridx = 1, i = 0; gridx < 5; gridx++, i++)
    {
        classmap = GrGetClassMap(gridx);
        if ((classmap & 0x02) == 0) {
            dinp->DeviceType[i] = 0;
            continue;
        }
        dinp->Init[i] = 1;

        val = GrGetValue(gridx, 2, 0) - 1;
        dinp->DigitalX[i] = val;
        val = 1 - GrGetValue(gridx, 2, 1);
        dinp->DigitalY[i] = val;

        val = GrGetPackedValues(gridx, 1, 0, 9);
        lval = val >> 2;
        if (val & 0x01)
            lval |= 0x80;
        if (val & 0x02)
            lval |= 0x40;
        dinp->Buttons[i] = lval;

        val = dinp->DigitalX[i];
        if (val == -1)
            dinp->AnalogueX[i] = -0x7FFF;
        else if (val == 1)
            dinp->AnalogueX[i] = 0x7FFF;

        val = dinp->DigitalY[i];
        if (val == -1)
            dinp->AnalogueY[i] = -0x7FFF;
        else if (val == 1)
            dinp->AnalogueY[i] = 0x7FFF;

        dinp->DeviceType[i] = 112;
    }
#endif
    if (dinp->NumberOfDevices == 0)
        return -1;
    return 1;
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

int joy_func_065_sub4(struct DevInput *dinp, short ipos)
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
        if ((val & 0xF) == 0)
          break;
        if (val & 0x01)
          ++dword_1E2F24;
        if (val & 0x02)
          ++dword_1E2F28;
        if (val & 0x04)
          ++dword_1E2F2C;
        if (val & 0x08)
          ++dword_1E2F30;
    }
    _enable();
    byte_1E2F34 = val;
    if (i == 0)
        return -1;
#endif
    dinp->HatMax[ipos] = (dword_1E2F30 + dword_1E2F2C) >> 2;
    joy_func_065_lab93(dinp, ipos);
    return 0;
}

int joy_func_065_sub1(struct DevInput *dinp, short ipos)
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
        if ((val & 3) == 0)
          break;
        if (val & 0x01)
          ++dword_1E2F24;
        if (val & 0x02)
          ++dword_1E2F28;
    }
    _enable();
    byte_1E2F34 = val;
    if (i == 0)
        return -1;
#endif
    joy_func_065_lab93(dinp, ipos);
    return 0;
}

int joy_func_065_sub2(struct DevInput *dinp, short ipos)
{
    int i;
#if defined(DOS)||defined(GO32)
    ubyte val;

    val = dinp->ConfigType[ipos] - 1;
    _disable();
    __outbyte(0x201, val);
    for (i = 10000; i > 0; i--)
    {
        val = __inbyte(0x201);
        if ((val & 0xB) == 0)
          break;
        if (val & 0x01)
          ++dword_1E2F24;
        if (val & 0x02)
          ++dword_1E2F28;
        if (val & 0x08)
          ++dword_1E2F30;
    }
    _enable();
    byte_1E2F34 = val;
    if (i == 0)
        return -1;
#endif
    if (dword_1E2F30 > dinp->HatMax[ipos])
        dinp->HatMax[ipos] = dword_1E2F30;
    if (!dinp->HatMax[0])
        dinp->HatMax[0] = 1;
    i = 8 * dword_1E2F30 / dinp->HatMax[0];
    switch (i)
    {
    case 0:
        dinp->HatX[0] = 0;
        dinp->HatY[0] = -1;
        break;
    case 1:
    case 2:
        dinp->HatX[0] = 1;
        dinp->HatY[0] = 0;
        break;
    case 3:
    case 4:
        dinp->HatX[0] = 0;
        dinp->HatY[0] = 1;
        break;
    case 5:
    case 6:
        dinp->HatX[0] = -1;
        dinp->HatY[0] = 0;
        break;
    default:
        dinp->HatX[0] = 0;
        dinp->HatY[0] = 0;
        break;
    }
    joy_func_065_lab93(dinp, ipos);
    return 0;
}

void joy_func_065_sub3(struct DevInput *dinp, short ipos, ubyte v115)
{
    int ret;

    if (!dinp->Init[ipos])
        return;
    ret = vfx1_unkn_func_12(&vfxunk1, v115 + 1);
    if (ret == -1)
        return;

    if (vfxunk1.field_0 & 0x08) {
        dinp->DigitalY[ipos] = -1;
        dinp->AnalogueY[ipos] = -0x7FFF;
    } else if (vfxunk1.field_0 & 0x04) {
        dinp->DigitalY[ipos] = 1;
        dinp->AnalogueY[ipos] = 0x7FFF;
    } else {
        dinp->DigitalY[ipos] = 0;
        dinp->AnalogueY[ipos] = 0;
    }

    if (vfxunk1.field_0 & 0x02) {
        dinp->DigitalX[ipos] = -1;
        dinp->AnalogueX[ipos] = -0x7FFF;
    } else if (vfxunk1.field_0 & 0x01) {
        dinp->DigitalX[ipos] = 1;
        dinp->AnalogueX[ipos] = 0x7FFF;
    } else {
        dinp->DigitalX[ipos] = 0;
        dinp->AnalogueX[ipos] = 0;
    }

    dinp->Buttons[ipos] = vfxunk1.field_4;
    if (vfxunk1.field_2 & 0x01) {
        dinp->MenuButtons[ipos] |= 0x04;
        dinp->MenuButtons[ipos] |= 0x0200;
    }
    if (vfxunk1.field_2 & 0x02) {
        dinp->MenuButtons[ipos] |= 0x01;
        dinp->Buttons[ipos] |= 0x0100;
    }
}

int joy_func_065_sub5(struct DevInput *dinp, short ipos)
{
    vfx1_unkn_func_03();
    dword_1E2F24 = word_1E6078;
    dinp->AnalogueX[ipos] = dword_1E2F24;
    dword_1E2F28 = word_1E607A;
     dinp->AnalogueY[ipos] = dword_1E2F28;
    dword_1E2F30 = word_1E607C;
    dinp->AnalogueZ[ipos] = dword_1E2F30;
    joy_func_065_lab93(dinp, ipos);
    return 0;
}

int joy_func_065(struct DevInput *dinp)
{
    short i, ipos;
    ubyte v115;
    int ret;

    v115 = 0;
    if (dinp->Type == -1)
        return -1;

    for (i = 0; i < 16; i++)
    {
        dinp->NumberOfButtons[i] = 0;
        dinp->Buttons[i] = 0;
        dinp->HatMax[i] = 0;
        dinp->HatY[i] = 0;
        dinp->HatX[i] = 0;
        dinp->AnalogueZ[i] = 0;
        dinp->AnalogueY[i] = 0;
        dinp->AnalogueY[i] = 0;
        dinp->DigitalZ[i] = 0;
        dinp->DigitalY[i] = 0;
        dinp->DigitalX[i] = 0;
        dinp->AnalogueR[i] = 0;
        dinp->AnalogueV[i] = 0;
        dinp->AnalogueU[i] = 0;
        dinp->DigitalR[i] = 0;
        dinp->DigitalV[i] = 0;
        dinp->DigitalU[i] = 0;
    }
    dword_1E2F28 = 0;
    dword_1E2F24 = 0;
    dword_1E2F2C = 0;
    dword_1E2F30 = 0;

    for (ipos = 0; ipos < 16; ipos++)
    {
        if (dinp->Init[ipos] == 0)
            continue;
        switch (dinp->ConfigType[ipos])
        {
        case 1:
        case 2:
        case 5:
        case 0xC:
        case 0xD:
            ret = joy_func_065_sub1(dinp, ipos);
            if (ret != 0)
                return ret;
            break;
        case 3:
        case 4:
        case 7:
            ret = joy_func_065_sub2(dinp, ipos);
            if (ret != 0)
                return ret;
            break;
        case 6:
            joy_func_065_sub3(dinp, ipos, v115);
            ++v115;
            break;
        case 9:
        case 0xE:
        case 0xF:
            ret = joy_func_065_sub4(dinp, ipos);
            if (ret != 0)
                return ret;
            break;
        case 0xB:
        case 0x16:
            ret = joy_func_065_sub5(dinp, ipos);
            if (ret != 0)
                return ret;
            break;
        case 0x11:
            joy_func_065_sub6(dinp, ipos);
            return ret;
            break;
        case 0x12:
            ret = joy_func_065_sub7(dinp, ipos);
            if (ret != 0)
                return ret;
            break;
        case 0x13:
            ret = joy_func_065_sub8(dinp, ipos);
            if (ret != 0)
                return ret;
            break;
        case 0x14:
        case 0x15:
            ret = joy_func_065_sub9(dinp, ipos);
            if (ret != 0)
                return ret;
            break;
        default:
            joy_func_065_lab93(dinp, ipos);
            break;
        }
    }
    return 1;
}

int joy_func_067(struct DevInput *dinp, int a2)
{
    int ret;
    asm volatile ("call ASM_joy_func_067\n"
        : "=r" (ret) : "a" (dinp), "d" (a2));
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
