/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file linksmk.c
 *     Link between the main game and smacker playback library.
 * @par Purpose:
 *     Callback and setup functions to integrate bfsmacklib with the game.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2011 - 13 Jun 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "linksmk.h"

#include "bfsmack.h"

/******************************************************************************/
extern ulong smack_malloc_used_tot;

/******************************************************************************/
void *ASM_smack_malloc(int msize);
void ASM_smack_mfree(void *ptr);


void smack_malloc_free_all(void)
{
    smack_malloc_used_tot = 0;
}

void smack_malloc_setup(void)
{
    set_smack_malloc(ASM_smack_malloc);
    set_smack_free(ASM_smack_mfree);
}

/******************************************************************************/
