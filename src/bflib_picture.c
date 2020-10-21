/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_picture.c
 *     Pictures handling routines.
 * @par Purpose:
 *     Functions to save or load pictures.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Oct 2020 - 16 Oct 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_picture.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <png.h>

#include "bflib_basics.h"
#include "bflib_memory.h"
#include "bflib_video.h"
#include "bflib_fileio.h"
#include "game_data.h"

static char game_dir_screenshots[] = "qdata/screenshots";

/** Gives highest file number in existing files matching given mask.
 *
 * @param fnmask the mask to match
 * @param no_pos_in_fnmask position within the mask where the number is
 * @param fndir directory part of the mask (not really needed, we could use
 *          strrchr('/') instead; but this is how it was implemented by Bullfrog)
 * @return
 */
unsigned int get_highest_file_no(const char *fnmask, int no_pos_in_fnmask, const char *fndir)
{
    unsigned int highest_no, curr_no;
    struct TbFileFind ffind;
    int no_pos_in_fname;
    /* This is what we could do if we didn't have fndir param
    char *last_sep;
    last_sep = strrchr(fnmask, '/');
    if (last_sep == NULL)
        last_sep = strrchr(fnmask, '\\');
    if (last_sep != NULL)
        no_pos_in_fname = no_pos_in_fnmask - (last_sep - fnmask + 1);
    else
        no_pos_in_fname = no_pos_in_fnmask;
    */
    no_pos_in_fname = no_pos_in_fnmask - (strlen(fndir) + 1);
    highest_no = 0;
    if ( LbFileFindFirst(fnmask, &ffind, 0x21u) != -1 )
    {
        do {
            curr_no = atol(ffind.Filename + no_pos_in_fname);
            if (curr_no > highest_no)
                highest_no = curr_no;
        } while ( LbFileFindNext(&ffind) != -1 );
    }
    return highest_no;
}

static TbResult prepare_screenshot_file_name(char *fname, const char *base,
    const char *ext)
{
    unsigned int i;
    unsigned int highest_num;
    sprintf(fname, "%s/%-5s", game_dir_screenshots, base);
    for (i = 1; i < DISKPATH_SIZE; i++)
    {
        int ch;
        ch = fname[i];
        if (ch == ' ')
            break;
        if (ch == '\0')
            break;
    }
    sprintf(&fname[i], "*.%s", ext);
    highest_num = get_highest_file_no(fname, i, game_dir_screenshots);
    BFLIB_DEBUGLOG(0,"%s: highest_num = %d", fname, highest_num);
    sprintf(&fname[i], "%03d.%s", highest_num + 1, ext);

    if (LbDirectoryMake(game_dir_screenshots, true) != Lb_FAIL)
      return 0;

    return 1;
}

TbResult LbIffWrite(FILE *img_fh, unsigned char *inp_buffer,
    unsigned char *pal)
{
    uint8_t pxcol;
    unsigned short img_w;
    unsigned short img_h;
    unsigned int i, k;
    uint8_t *palptr;
    uint8_t transClr;
    unsigned long lenChunk;
    unsigned long pxpos;
    unsigned char *inp;

    palptr = pal;
    pal[0] = 0;
    pal[1] = 0;
    pal[2] = 0;
    transClr = 0;

    img_w = lbDisplay.PhysicalScreenWidth;
    img_h = lbDisplay.PhysicalScreenHeight;
    unsigned long lenCmapChunk, lenTinyChunk, lenBodyChunk;
    unsigned int algn_w = (lbDisplay.PhysicalScreenWidth + 3) & ~0x3;
    unsigned int algn_h = (lbDisplay.PhysicalScreenHeight + 3) & ~0x3;
    unsigned int tiny_w = algn_w >> 2;
    unsigned int tiny_h = algn_h >> 2;
    lenCmapChunk = 3 * 256;
    lenTinyChunk = 4 + tiny_w * tiny_h;
    lenBodyChunk = img_h * img_w;
    // Format descriptor - chunkID
    fputc('F', img_fh);
    fputc('O', img_fh);
    fputc('R', img_fh);
    fputc('M', img_fh);
    // Format descriptor - lenChunk
    lenChunk = 4 + 8 + 20 + 8 + lenCmapChunk + 8 + lenBodyChunk + 8 + lenTinyChunk;
    fputc((lenChunk >> 24) & 0xFF, img_fh);
    fputc((lenChunk >> 16) & 0xFF, img_fh);
    fputc((lenChunk >>  8) & 0xFF, img_fh);
    fputc((lenChunk >>  0) & 0xFF, img_fh);
    // formatID
    fputc('P', img_fh);
    fputc('B', img_fh);
    fputc('M', img_fh);
    fputc(' ', img_fh);
    // Bitmap Header - chunkID
    fputc('B', img_fh);
    fputc('M', img_fh);
    fputc('H', img_fh);
    fputc('D', img_fh);
    // Bitmap Header - lenChunk
    fputc(0, img_fh);
    fputc(0, img_fh);
    fputc(0, img_fh);
    fputc(20, img_fh);
    // Bitmap Header - width
    fputc((img_w >> 8) & 0xFF, img_fh);
    fputc((img_w >> 0) & 0xFF, img_fh);
    // Bitmap Header - height
    fputc((img_h >> 8) & 0xFF, img_fh);
    fputc((img_h >> 0) & 0xFF, img_fh);
    // Bitmap Header - xOrigin
    fputc(0, img_fh);
    fputc(0, img_fh);
    // Bitmap Header - yOrigin
    fputc(0, img_fh);
    fputc(0, img_fh);
    // Bitmap Header - numPlanes
    fputc(8, img_fh);
    // Bitmap Header - mask
    fputc(0, img_fh);
    // Bitmap Header - compression
    fputc(0, img_fh);
    // Bitmap Header - pad1
    fputc(0, img_fh);
    // Bitmap Header - transClr
    fputc(transClr, img_fh);
    fputc(64, img_fh);
    // Bitmap Header - xAspect
    fputc(5, img_fh);
    // Bitmap Header - yAspect
    fputc(6, img_fh);
    // Bitmap Header - pageWidth
    fputc((img_w >> 8) & 0xFF, img_fh);
    fputc((img_w >> 0) & 0xFF, img_fh);
    // Bitmap Header - pageHeight
    fputc((img_h >> 8) & 0xFF, img_fh);
    fputc((img_h >> 0) & 0xFF, img_fh);
    // Palette - chunkID
    fputc('C', img_fh);
    fputc('M', img_fh);
    fputc('A', img_fh);
    fputc('P', img_fh);
    // Palette - lenChunk
    lenChunk = lenCmapChunk;
    fputc((lenChunk >> 24) & 0xff, img_fh);
    fputc((lenChunk >> 16) & 0xFF, img_fh);
    fputc((lenChunk >>  8) & 0xff, img_fh);
    fputc((lenChunk      ) & 0xff, img_fh);
    // Palette - data
    for (i = 0; i != 768; i++) {
        fputc(4 * (*palptr), img_fh);
        palptr++;
    }
    // Miniature - chunkID
    fputc('T', img_fh);
    fputc('I', img_fh);
    fputc('N', img_fh);
    fputc('Y', img_fh);
    // Miniature - lenChunk
    lenChunk = lenTinyChunk;
    fputc((lenChunk >> 24) & 0xff, img_fh);
    fputc((lenChunk >> 16) & 0xFF, img_fh);
    fputc((lenChunk >>  8) & 0xff, img_fh);
    fputc((lenChunk      ) & 0xff, img_fh);
    // Miniature - width
    fputc((tiny_w >> 8) & 0xFF, img_fh);
    fputc((tiny_w >> 0) & 0xFF, img_fh);
    // Miniature - height
    fputc((tiny_h >> 8) & 0xFF, img_fh);
    fputc((tiny_h >> 0) & 0xFF, img_fh);
    // Miniature - data
    inp = inp_buffer;
    for (i = 0; i != (ulong)lbDisplay.PhysicalScreenHeight / 4; i++)
    {
        for (k = 0; k != (ulong)lbDisplay.PhysicalScreenWidth / 4; k++)
        {
            pxcol = *inp;
            inp += 4;
            fputc(pxcol, img_fh);
        }
        inp += 3 * lbDisplay.PhysicalScreenWidth;
    }
    // Image data - chunkID
    fputc('B', img_fh);
    fputc('O', img_fh);
    fputc('D', img_fh);
    fputc('Y', img_fh);
    // Image data - lenChunk
    lenChunk = lenBodyChunk;
    fputc((lenChunk >> 24) & 0xff, img_fh);
    fputc((lenChunk >> 16) & 0xFF, img_fh);
    fputc((lenChunk >>  8) & 0xff, img_fh);
    fputc((lenChunk      ) & 0xff, img_fh);
    // Image data - actual pixel data
    inp = inp_buffer;
    for (pxpos = lenChunk; pxpos > 0; pxpos--)
    {
        pxcol = *inp++;
        fputc(pxcol, img_fh);
    }
    return 1;
}

/** Save "ILBM" IFF Interleaved Bitmap
 *
 * @param fname
 * @param a2
 * @param a3
 * @param force_fname
 * @return
 */
TbResult LbIffSave(const char *fname, unsigned char *inp_buffer,
    unsigned char *pal, TbBool force_fname)
{
    char full_fname[DISKPATH_SIZE];
    FILE *img_fh;
    TbResult ret;

    if ( force_fname ) {
        sprintf(full_fname, "%s", fname);
    } else {
        prepare_screenshot_file_name(full_fname, fname, "lbm");
    }
    img_fh = fopen(full_fname, "wb");
    if (!img_fh) {
        BFLIB_ERRORLOG("%s: Cannot open: %s", full_fname, strerror(errno));
        return 0;
    }
    ret = LbIffWrite(img_fh, inp_buffer, pal);
    fclose(img_fh);
    return ret;
}

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
  BFLIB_ERRORLOG("Could not %s", action);
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
    char full_fname[DISKPATH_SIZE];
    FILE *img_fh;
    TbResult ret;

    if ( force_fname ) {
        sprintf(full_fname, "%s", fname);
    } else {
        prepare_screenshot_file_name(full_fname, fname, "png");
    }
    img_fh = fopen(full_fname, "wb");
    if (!img_fh) {
        BFLIB_ERRORLOG("%s: Cannot open: %s", full_fname, strerror(errno));
        return 0;
    }
    ret = LbPngWrite(img_fh, inp_buffer, pal);
    fclose(img_fh);
    return ret;
}

/******************************************************************************/
