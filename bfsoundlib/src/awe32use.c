/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file awe32use.c
 *     Reimplementation of Bullfrog Sound Library.
 * @par Purpose:
 *     Routines use of AWE32 specific features.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "awe32.h"
#include "dpmi.h"
#include "aildebug.h"
/******************************************************************************/

/** Global variable used for transferring parameters to MDI driver. */
SF_DATA *awe_data = NULL;
/** DOS selector for awe_data memory block allocation. */
uint16_t awe_data_sel = 0;

/******************************************************************************/

long AWEGetTotalRAM(MDI_DRIVER *mdidrv)
{
    VDI_CALL regs;

    regs.CX = AWESF_GETTOTALRAM;

    AIL_call_driver(mdidrv->drvr, MDI_VSE, &regs, &regs);
    if(regs.AX == 0)
        return (regs.DX << 16) + regs.SI;
    else
        return -1;
}

int AWEDefMemMap(MDI_DRIVER *mdidrv, short part_cnt, long *memMap, uint16_t memMap_sel)
{
    VDI_CALL regs;

    if(awe_data == NULL)
    {
        awe_data = (SF_DATA *)AllocDOSmem(&awe_data_sel, sizeof(SF_DATA));
        if(awe_data == NULL)
            return -1;
    }

    awe_data->parm = part_cnt;
#if defined(DOS)||defined(GO32)
    awe_data->data_seg = GetDOSseg(FP_SEG(memMap));
#else
    awe_data->data_ptr = memMap;
#endif

    regs.CX = AWESF_DEFBANKSIZES;
#if defined(DOS)||defined(GO32)
    regs.DX = GetDOSseg(FP_SEG(awe_data));
    regs.SI = 0;
#else
    assert(sizeof(VDI_CALL) - offsetof(VDI_CALL, DX) >= sizeof(SF_DATA *));
    *(SF_DATA **)(&regs.DX) = awe_data;
#endif
    AIL_call_driver(mdidrv->drvr, MDI_VSE, &regs, &regs);
    return regs.AX;
}

SF_INFO *AWEGetSFInfo(MDI_DRIVER *mdidrv, short bank_no, uint8_t *hdr_data, uint16_t hdr_data_sel)
{
    VDI_CALL regs;

    if (awe_data == NULL)
    {
        awe_data = (SF_DATA *)AllocDOSmem(&awe_data_sel, sizeof(SF_DATA));
        if(awe_data == NULL)
            return NULL;
    }

    awe_data->parm = bank_no;
#if defined(DOS)||defined(GO32)
    awe_data->data_seg = GetDOSseg(FP_SEG(hdr_data));
#else
    awe_data->data_ptr = hdr_data;
#endif

    regs.CX = AWESF_LOADREQ;
#if defined(DOS)||defined(GO32)
    regs.DX = GetDOSseg(FP_SEG(awe_data));
    regs.SI = 0;
#else
    assert(sizeof(VDI_CALL) - offsetof(VDI_CALL, DX) >= sizeof(SF_DATA *));
    *(SF_DATA **)(&regs.DX) = awe_data;
#endif
    AIL_call_driver(mdidrv->drvr, MDI_VSE, &regs, &regs);

    if(regs.AX == 0) {
        SF_INFO *ret;
#if defined(DOS)||defined(GO32)
        ret = MK_FP(CreateSelector(regs.DX), regs.SI);
#else
        ret = *(SF_INFO **)(&regs.DX);
#endif
        return ret;
    }
    return NULL;
}

int AWEStreamSample(MDI_DRIVER *mdidrv, short bank_no, uint8_t *smpl_data, uint16_t smpl_data_sel)
{
    VDI_CALL regs;

    if (awe_data == NULL)
    {
        awe_data = (SF_DATA *)AllocDOSmem(&awe_data_sel, sizeof(SF_DATA));
        if(awe_data == NULL)
            return -1;
    }

    awe_data->parm = bank_no;
#if defined(DOS)||defined(GO32)
    awe_data->data_seg = GetDOSseg(FP_SEG(smpl_data));
#else
    awe_data->data_ptr = smpl_data;
#endif

    regs.CX = AWESF_STREAMSMPL;
#if defined(DOS)||defined(GO32)
    regs.DX = GetDOSseg(FP_SEG(awe_data));
    regs.SI = 0;
#else
    assert(sizeof(VDI_CALL) - offsetof(VDI_CALL, DX) >= sizeof(SF_DATA *));
    *(SF_DATA **)(&regs.DX) = awe_data;
#endif

    AIL_call_driver(mdidrv->drvr, MDI_VSE, &regs, &regs);
    return regs.AX;
}

int AWELoadPreset(MDI_DRIVER *mdidrv, short bank_no, uint8_t *preset_data, uint16_t preset_data_sel)
{
    VDI_CALL regs;

    if (awe_data == NULL)
    {
        awe_data = (SF_DATA *)AllocDOSmem(&awe_data_sel, sizeof(SF_DATA));
        if(awe_data == NULL)
            return -1;
    }

    awe_data->parm = bank_no;
#if defined(DOS)||defined(GO32)
    awe_data->data_seg = GetDOSseg(FP_SEG(preset_data));
#else
    awe_data->data_ptr = preset_data;
#endif

    regs.CX = AWESF_SETPRESETS;
#if defined(DOS)||defined(GO32)
    regs.DX = GetDOSseg(FP_SEG(awe_data));
    regs.SI = 0;
#else
    assert(sizeof(VDI_CALL) - offsetof(VDI_CALL, DX) >= sizeof(SF_DATA *));
    *(SF_DATA **)(&regs.DX) = awe_data;
#endif
    AIL_call_driver(mdidrv->drvr, MDI_VSE, &regs, &regs);
    return regs.AX;
}

int AWEFreeMem(MDI_DRIVER *mdidrv, short bank_no)
{
    VDI_CALL regs;

    FreeDOSmem(awe_data, awe_data_sel);
    awe_data = NULL;
    awe_data_sel = 0;

    regs.CX = AWESF_FREEBANK;
    regs.DX = bank_no;
    AIL_call_driver(mdidrv->drvr, MDI_VSE, &regs, &regs);
    return regs.AX;
}

/******************************************************************************/
