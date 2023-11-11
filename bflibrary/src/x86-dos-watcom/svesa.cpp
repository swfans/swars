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
    u32 oem;			/*< segment:offset pointer to OEM */
    u32 capabilities;	/*< bitfield that describes card capabilities */
    u32 video_modes;	/*< segment:offset pointer to list of supported video modes */
    ushort video_memory;	/*< amount of video memory in 64KB blocks */
    ushort software_rev;	/*< software revision */
    u32 vendor;		/*< segment:offset to card vendor string */
    u32 product_name;	/*< segment:offset to card model name */
    u32 product_rev;	/*< segment:offset pointer to product revision */
    char reserved[222];	/*< reserved for future expansion */
};

struct TbRMREGS2 { // sizeof=50
    u32 MyEDI; // offset=0
    u32 MyESI; // offset=4
    u32 MyEBP; // offset=8
    u32 MyReserved; // offset=12
    u32 MyEBX; // offset=16
    u32 MyEDX; // offset=20
    u32 MyECX; // offset=24
    u32 MyEAX; // offset=28
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

#pragma pack()

ushort lbVesaBytesPerLine;
ushort lbVesaHRes;
ushort lbVesaVRes;
ubyte *lbVesaData;
u32 lbVesaPage;
int lbVesaGran;


int LbVesaGetGran()
{
// code at 0001:000b2730
}

TbResult LbVesaSetMode(i32 mode)
{
    DOS_Registers regs;
    DOS_Registers out_regs;

    memset(&regs, 0, 0x1Cu);
    memset(&out_regs, 0, 0x1Cu);
    regs.r_es = 20226;
    regs.r_ss = a1;
    int386(16, &regs, &out_regs);
    LbVesaGetGran(a1);
    return Lb_SUCCESS;
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

TbResult LbVesaGetInfo(void)
{
    char ptr1[50];
    char ptr3[28];
    char ptr2[28];
    char ptr4[12];

    memset(ptr1, 0, 0x32u);
    *(ushort *)&ptr1[34] = (u32)lbVesaData >> 4;
    *(ushort *)&ptr1[36] = (u32)lbVesaData >> 4;
    *(u32 *)ptr1 = 0;
    *(u32 *)&ptr1[28] = 20224;
    *(u32 *)&ptr1[16] = 257;
    memset(ptr2, 0, 28u);
    memset(ptr3, 0, 28u);
    memset(ptr4, 0, 12u);
    segread(ptr4);
    *(u32 *)ptr2 = 768;
    *(u32 *)&ptr2[4] = 16;
    *(u32 *)&ptr2[20] = ptr1;
    int386x(49, (int)ptr2, (int)ptr3, (int)ptr4);

    if (strncmp(lbVesaData, "VESA", 4) != 0)
        return Lb_FAIL;
    return Lb_SUCCESS;
}

TbBool LbVesaIsModeAvailable(i32 mode)
{
    ushort *md;
	struct VbeInfoBlock *vbeInfo;
    u32 md_seg, md_off;

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

/******************************************************************************/
