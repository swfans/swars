/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gsprite.c
 *     Graphics sprites support library.
 * @par Purpose:
 *     Functions for reading/writing, decoding/encodeing of sprites.
 * @par Comment:
 *     Part of 8-bit graphics canvas drawing library.
 *     Used for drawing sprites on screen.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfsprite.h"

#include "bflog.h"

ubyte * lbSpriteReMapPtr;

short LbSpriteSetup(struct TbSprite *start, const struct TbSprite *end, const unsigned char * data)
{
    struct TbSprite *sprt;
    int n;
    n = 0;
    sprt = start;
    while (sprt < end)
    {
      if ((unsigned long)sprt->Data < (unsigned long)data)
      {
        sprt->Data += (unsigned long)data;
        n++;
      }
      sprt++;
    }
    LIBLOG("initied %d of %d sprites",n, (sprt-start));
    return 1;
}

int LbSpriteSetupAll(struct TbSetupSprite t_setup[])
{
    struct TbSetupSprite *stp_sprite;
    int idx;
    idx=0;
    stp_sprite=&t_setup[idx];
    while (stp_sprite->Data != NULL)
    {
      if ((stp_sprite->Start != NULL) && (stp_sprite->End != NULL))
        LbSpriteSetup(*(stp_sprite->Start), *(stp_sprite->End), (unsigned char *)*(stp_sprite->Data));
      idx++;
      stp_sprite=&t_setup[idx];
    }
    LIBLOG("Initiated %d SetupSprite lists", idx);
    return 1;
}

int LbSpriteClearAll(struct TbSetupSprite t_setup[])
{
  struct TbSetupSprite *stp_sprite;
  int idx;
  idx=0;
  stp_sprite=&t_setup[idx];
  while (stp_sprite->Data != NULL)
  {
    if ((stp_sprite->Start != NULL) && (stp_sprite->End != NULL))
    {
      *(stp_sprite->Start) = NULL;
      *(stp_sprite->End) = NULL;
      *(stp_sprite->Data) = 0;
    }
    idx++;
    stp_sprite=&t_setup[idx];
  }
  LIBLOG("Cleaned %d SetupSprite lists", idx);
  return 1;
}
/******************************************************************************/
