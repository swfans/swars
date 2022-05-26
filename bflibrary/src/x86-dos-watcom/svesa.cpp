/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file svesa.cpp
 *     Implementation of related functions.
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
#include "svesa.h"

#pragma pack(1)

struct VbeInfoBlock {
    char signature[4];	/*< must be "VESA" to indicate valid VBE support */
    ushort version;		/*< VBE version; high byte is major ver, low byte is minor ver */
    ulong oem;			/*< segment:offset pointer to OEM */
    ulong capabilities;	/*< bitfield that describes card capabilities */
    ulong video_modes;	/*< segment:offset pointer to list of supported video modes */
    ushort video_memory;	/*< amount of video memory in 64KB blocks */
    ushort software_rev;	/*< software revision */
    ulong vendor;		/*< segment:offset to card vendor string */
    ulong product_name;	/*< segment:offset to card model name */
    ulong product_rev;	/*< segment:offset pointer to product revision */
    char reserved[222];	/*< reserved for future expansion */
};

#pragma pack()


int LbVesaGetGran()
{
// code at 0001:000b2730
}

int LbVesaSetMode()
{
// code at 0001:000b2838
}

TbResult LbVesaSetPage(ushort pageNo)
{
  if (lbVesaPage != pageNo)
  {
    // VESA page switch interrupt should be called here
    lbVesaPage = pageNo;
  }
    return Lb_SUCCESS;
}

int LbVesaGetInfo()
{
// code at 0001:000b2914
}

TbBool LbVesaIsModeAvailable(long mode)
{
    ushort *md;
	struct VbeInfoBlock *vbeInfo;
    ulong md_seg, md_off;

    if (LbVesaGetInfo() != Lb_SUCCESS)
        return false;
    vbeInfo = (struct VbeInfoBlock *)bVesaData;
    md_off = (vbeInfo->video_modes) & 0xffff;
    md_seg = (vbeInfo->video_modes >> 16) & 0xffff;
    for (md = (ushort *)(16 * md_seg + md_off); *md != 0xFFFF; md++) {
      if (*md == mode)
        return true;
    }
    return false;
}

int lbVesaBytesPerLine;

int lbVesaHRes;

int lbVesaVRes;

int lbVesaData;

int lbVesaPage;

int lbVesaGran;


/******************************************************************************/
