/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file svesa.hpp
 *     Header file for svesa.cpp.
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
#ifndef BFLIBRARY_SVESA_H_
#define BFLIBRARY_SVESA_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

struct TbRMREGS2 { // sizeof=50
    ulong MyEDI; // offset=0
    ulong MyESI; // offset=4
    ulong MyEBP; // offset=8
    ulong MyReserved; // offset=12
    ulong MyEBX; // offset=16
    ulong MyEDX; // offset=20
    ulong MyECX; // offset=24
    ulong MyEAX; // offset=28
    ushort MyFlags; // offset=32
    ushort MyES; // offset=34
    ushort MyDS; // offset=36
    ushort MyFS; // offset=38
    ushort MyGS; // offset=40
    ushort MyIP; // offset=42
    ushort MyCS; // offset=44
    ushort MySP; // offset=46
    ushort MySS; // offset=48
};

typedef struct TbRMREGS2 TbRMREGS2;

int LbVesaGetGran();
int LbVesaSetMode();
int LbVesaSetPage();
int LbVesaGetInfo();
int LbVesaIsModeAvailable();
extern int lbVesaBytesPerLine;
extern int lbVesaHRes;
extern int lbVesaVRes;
extern int lbVesaData;
extern int lbVesaPage;
extern int lbVesaGran;

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_SVESA_H_
/******************************************************************************/
