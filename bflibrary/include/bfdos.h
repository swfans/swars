/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfdos.h
 *     Header with basic DOS type definitions.
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
#ifndef BFLIBRARY_BFDOS_H_
#define BFLIBRARY_BFDOS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#pragma pack(1)

struct SREGS { // sizeof=12
    ushort es; // offset=0
    ushort cs; // offset=2
    ushort ss; // offset=4
    ushort ds; // offset=6
    ushort fs; // offset=8
    ushort gs; // offset=10
};

typedef struct SREGS SREGS;

struct DWORDREGS { // sizeof=28
    unsigned int eax; // offset=0
    unsigned int ebx; // offset=4
    unsigned int ecx; // offset=8
    unsigned int edx; // offset=12
    unsigned int esi; // offset=16
    unsigned int edi; // offset=20
    unsigned int cflag; // offset=24
};

typedef struct DWORDREGS DWORDREGS;

struct WORDREGS { // sizeof=28
    ushort ax; // offset=0
    ushort _1; // offset=2
    ushort bx; // offset=4
    ushort _2; // offset=6
    ushort cx; // offset=8
    ushort _3; // offset=10
    ushort dx; // offset=12
    ushort _4; // offset=14
    ushort si; // offset=16
    ushort _5; // offset=18
    ushort di; // offset=20
    ushort _6; // offset=22
    unsigned int cflag; // offset=24
};

typedef struct WORDREGS WORDREGS;

struct BYTEREGS { // sizeof=16
    ubyte al; // offset=0
    ubyte ah; // offset=1
    ushort _1; // offset=2
    ubyte bl; // offset=4
    ubyte bh; // offset=5
    ushort _2; // offset=6
    ubyte cl; // offset=8
    ubyte ch; // offset=9
    ushort _3; // offset=10
    ubyte dl; // offset=12
    ubyte dh; // offset=13
    ushort _4; // offset=14
};

typedef struct BYTEREGS BYTEREGS;

union REGS { // sizeof=28
    BYTEREGS h; // offset=0
    WORDREGS w; // offset=1
    DWORDREGS x; // offset=2
};

#pragma pack()


#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFDOS_H_
/******************************************************************************/
