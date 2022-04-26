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
    ULONG MyEDI; // offset=0
    ULONG MyESI; // offset=4
    ULONG MyEBP; // offset=8
    ULONG MyReserved; // offset=12
    ULONG MyEBX; // offset=16
    ULONG MyEDX; // offset=20
    ULONG MyECX; // offset=24
    ULONG MyEAX; // offset=28
    UWORD MyFlags; // offset=32
    UWORD MyES; // offset=34
    UWORD MyDS; // offset=36
    UWORD MyFS; // offset=38
    UWORD MyGS; // offset=40
    UWORD MyIP; // offset=42
    UWORD MyCS; // offset=44
    UWORD MySP; // offset=46
    UWORD MySS; // offset=48
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
