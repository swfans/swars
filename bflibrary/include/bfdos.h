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

struct SREGS { // sizeof=12
    uint16_t es; // offset=0
    uint16_t cs; // offset=2
    uint16_t ss; // offset=4
    uint16_t ds; // offset=6
    uint16_t fs; // offset=8
    uint16_t gs; // offset=10
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
    uint16_t ax; // offset=0
    uint16_t _1; // offset=2
    uint16_t bx; // offset=4
    uint16_t _2; // offset=6
    uint16_t cx; // offset=8
    uint16_t _3; // offset=10
    uint16_t dx; // offset=12
    uint16_t _4; // offset=14
    uint16_t si; // offset=16
    uint16_t _5; // offset=18
    uint16_t di; // offset=20
    uint16_t _6; // offset=22
    unsigned int cflag; // offset=24
};

typedef struct WORDREGS WORDREGS;

struct BYTEREGS { // sizeof=16
    uint8_t al; // offset=0
    uint8_t ah; // offset=1
    uint16_t _1; // offset=2
    uint8_t bl; // offset=4
    uint8_t bh; // offset=5
    uint16_t _2; // offset=6
    uint8_t cl; // offset=8
    uint8_t ch; // offset=9
    uint16_t _3; // offset=10
    uint8_t dl; // offset=12
    uint8_t dh; // offset=13
    uint16_t _4; // offset=14
};

typedef struct BYTEREGS BYTEREGS;

union REGS { // sizeof=28
    BYTEREGS h; // offset=0
    WORDREGS w; // offset=1
    DWORDREGS x; // offset=2
};


#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFDOS_H_
/******************************************************************************/
