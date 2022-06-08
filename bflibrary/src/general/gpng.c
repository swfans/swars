/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file giff.c
 *     Implementation of Interchange File Format image format read/write.
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
#include "bfpng.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <png.h>

#include "bftypes.h"
#include "bffnuniq.h"
#include "bfmemory.h"
#include "bfscreen.h"
#include "privbflog.h"


TbResult
LbPngWrite(FILE *img_fh, const uint8_t *inp_buffer,
              const uint8_t *pal)
{
  png_structp png = NULL;
  png_infop info = NULL;
  png_color palette[256];
  png_byte **rows;
  size_t w, h, i;
  char *action;

  // get parameters
  w = lbDisplay.PhysicalScreenWidth;
  h = lbDisplay.PhysicalScreenHeight;
  rows = (png_byte**) LbMemoryAlloc (sizeof (png_byte*) * h);

  // set image rows for png processing
  for (i = 0; i < h; i++)
    rows[i] = (png_byte *) inp_buffer + i * w;

  // initialise
  action = "create png_create_write_struct";
  png = png_create_write_struct (PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if (png == NULL)
    goto err;

  action = "create png_create_info_struct";
  info = png_create_info_struct (png);
  if (info == NULL)
    goto err;

  png_init_io(png, img_fh);

  // write header
  png_set_IHDR(png, info, w, h, 8,
                PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_ADAM7,
                PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  for (i = 0; i < 256; i++)
    {
      palette[i].red   = pal[3 * i + 0] * 4;
      palette[i].green = pal[3 * i + 1] * 4;
      palette[i].blue  = pal[3 * i + 2] * 4;
    }

  png_set_PLTE(png, info, palette, 256);
  png_write_info(png, info);

  // write bytes
  png_write_image(png, rows);

  // finalise writing
  png_write_end(png, info);
  // cleanup heap allocation
  png_destroy_write_struct(&png, &info);
  LbMemoryFree(rows);
  return 1;

err: // handle error and cleanup heap allocation
  LOGERR("could not %s", action);
  if (png != NULL && info != NULL)
    png_destroy_write_struct(&png, &info);
  else if (png != NULL)
    png_destroy_write_struct(&png, (png_infopp) NULL);
  LbMemoryFree(rows);
  return 0;
}

TbResult LbPngSave(const char *fname, unsigned char *inp_buffer,
    unsigned char *pal, TbBool force_fname)
{
    char full_fname[FILENAME_MAX];
    FILE *img_fh;
    TbResult ret;

    if ( force_fname ) {
        sprintf(full_fname, "%s", fname);
    } else {
        LbPrepareImageFilename(full_fname, fname, "png");
    }
    img_fh = fopen(full_fname, "wb");
    if (!img_fh) {
        LOGERR("%s: cannot open: %s", full_fname, strerror(errno));
        return 0;
    }
    ret = LbPngWrite(img_fh, inp_buffer, pal);
    fclose(img_fh);
    return ret;
}

/******************************************************************************/
