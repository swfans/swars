/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file enginfexpl.c
 *     Make 3D objects explode into a cloud of faces.
 * @par Purpose:
 *     Implement functions for handling object explosions and debrit movement.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     13 Oct 2024 - 06 Nov 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "enginfexpl.h"

#include <limits.h>

#include "display.h"
#include "enginbckt.h"
#include "engindrwlstm.h"
#include "engindrwlstx.h"
#include "engintrns.h"
#include "game_data.h"
#include "swlog.h"
/******************************************************************************/
extern ulong next_ex_face;

void init_free_explode_faces(void)
{
    asm volatile ("call ASM_init_free_explode_faces\n"
        :  :  : "eax" );
}

void animate_explode(void)
{
    asm volatile ("call ASM_animate_explode\n"
        :  :  : "eax" );
}


void draw_explode(void)
{
#if 0
    asm volatile ("call ASM_draw_explode\n"
        :  :  : "eax" );
    return;
#endif
    ushort exface;

    if (next_ex_face != 0)
        animate_explode();

    if (dont_bother_with_explode_faces)
        return;

    for (exface = 1; exface < EXPLODE_FACES_COUNT; exface++)
    {
        struct ShEnginePoint sp1, sp2, sp3, sp4;
        struct ExplodeFace3 *p_exface;
        struct SpecialPoint *p_specpt;
        int cor_x, cor_y, cor_z;
        ushort npoints;
        ushort flags_all;
        short depth_max;
        ushort pt;

        p_exface = &ex_faces[exface];

        if (next_screen_point + 4 > mem_game[30].N)
            break;

        if (p_exface->Timer == 0)
            continue;

        switch (p_exface->Type)
        {
        case 1:
        case 2:
            npoints = (p_exface->Type == 2) ? 4 : 3;
            pt = next_screen_point;
            p_exface->PointOffset = pt;
            next_screen_point += npoints;

            cor_x = p_exface->X + p_exface->X0 - engn_xc;
            cor_z = p_exface->Z + p_exface->Z0 - engn_zc;
            cor_y = p_exface->Y + p_exface->Y0 - engn_yc;
            transform_shpoint(&sp1, cor_x, cor_y - 8 * engn_yc, cor_z);

            p_specpt = &game_screen_point_pool[pt + 0];
            p_specpt->X = sp1.X;
            p_specpt->Y = sp1.Y;

            cor_x = p_exface->X + p_exface->X1 - engn_xc;
            cor_z = p_exface->Z + p_exface->Z1 - engn_zc;
            cor_y = p_exface->Y + p_exface->Y1 - engn_yc;
            transform_shpoint(&sp2, cor_x, cor_y - 8 * engn_yc, cor_z);

            p_specpt = &game_screen_point_pool[pt + 1];
            p_specpt->X = sp2.X;
            p_specpt->Y = sp2.Y;

            cor_x = p_exface->X + p_exface->X2 - engn_xc;
            cor_z = p_exface->Z + p_exface->Z2 - engn_zc;
            cor_y = p_exface->Y + p_exface->Y2 - engn_yc;
            transform_shpoint(&sp3, cor_x, cor_y - 8 * engn_yc, cor_z);

            p_specpt = &game_screen_point_pool[pt + 2];
            p_specpt->X = sp3.X;
            p_specpt->Y = sp3.Y;

            depth_max = SHRT_MIN;
            if (depth_max < sp1.Depth)
                depth_max = sp1.Depth;
            if (depth_max < sp2.Depth)
                depth_max = sp2.Depth;
            if (depth_max < sp3.Depth)
                depth_max = sp3.Depth;

            flags_all = sp3.Flags & sp2.Flags & sp1.Flags;

            if (npoints > 3)
            {
                cor_x = p_exface->X + p_exface->X3 - engn_xc;
                cor_z = p_exface->Z + p_exface->Z3 - engn_zc;
                cor_y = p_exface->Y + p_exface->Y3 - engn_yc;
                transform_shpoint(&sp4, cor_x, cor_y - 8 * engn_yc, cor_z);

                p_specpt = &game_screen_point_pool[pt + 3];
                p_specpt->X = sp4.X;
                p_specpt->Y = sp4.Y;

                flags_all &= sp4.Flags;
                if (depth_max < sp4.Depth)
                    depth_max = sp4.Depth;
            }

            if ((flags_all & 0xF) != 0)
                break;

            dword_176D68++;
            draw_item_add(DrIT_Unkn5, exface, depth_max + 5000);
            break;
        case 3:
        case 4:
            npoints = (p_exface->Type == 4) ? 4 : 3;
            pt = next_screen_point;
            p_exface->PointOffset = pt;
            next_screen_point += npoints;

            cor_x = p_exface->X0 - engn_xc;
            cor_z = p_exface->Z0 - engn_zc;
            cor_y = p_exface->Y0 - engn_yc;
            transform_shpoint(&sp1, cor_x, cor_y - 8 * engn_yc, cor_z);

            p_specpt = &game_screen_point_pool[pt + 0];
            p_specpt->X = sp1.X;
            p_specpt->Y = sp1.Y;

            cor_x = p_exface->X1 - engn_xc;
            cor_z = p_exface->Z1 - engn_zc;
            cor_y = p_exface->Y1 - engn_yc;
            transform_shpoint(&sp2, cor_x, cor_y - 8 * engn_yc, cor_z);

            p_specpt = &game_screen_point_pool[pt + 1];
            p_specpt->X = sp2.X;
            p_specpt->Y = sp2.Y;

            cor_x = p_exface->X2 - engn_xc;
            cor_z = p_exface->Z2 - engn_zc;
            cor_y = p_exface->Y2 - engn_yc;
            transform_shpoint(&sp3, cor_x, cor_y - 8 * engn_yc, cor_z);

            p_specpt = &game_screen_point_pool[pt + 2];
            p_specpt->X = sp3.X;
            p_specpt->Y = sp3.Y;

            depth_max = SHRT_MIN;
            if (depth_max < sp1.Depth)
                depth_max = sp1.Depth;
            if (depth_max < sp2.Depth)
                depth_max = sp2.Depth;
            if (depth_max < sp3.Depth)
                depth_max = sp3.Depth;

            flags_all = sp3.Flags & sp2.Flags & sp1.Flags;

            if (npoints > 3)
            {
                cor_x = p_exface->X3 - engn_xc;
                cor_z = p_exface->Z3 - engn_zc;
                cor_y = p_exface->Y3 - engn_yc;
                transform_shpoint(&sp4, cor_x, cor_y - 8 * engn_yc, cor_z);

                p_specpt = &game_screen_point_pool[pt + 3];
                p_specpt->X = sp4.X;
                p_specpt->Y = sp4.Y;

                flags_all &= sp4.Flags;
                if (depth_max < sp4.Depth)
                    depth_max = sp4.Depth;
            }

            if ((flags_all & 0xF) != 0)
                break;

            dword_176D68++;
            draw_item_add(DrIT_Unkn5, exface, depth_max + 5000);
            break;
        case 5:
        case 6:
            npoints = (p_exface->Type == 6) ? 4 : 3;
            pt = next_screen_point;
            p_exface->PointOffset = pt;
            next_screen_point += npoints;

            cor_x = p_exface->X + p_exface->X0 - engn_xc;
            cor_z = p_exface->Z + p_exface->Z0 - engn_zc;
            cor_y = p_exface->Y + p_exface->Y0 - engn_yc;
            transform_shpoint(&sp1, cor_x, cor_y - 8 * engn_yc, cor_z);

            p_specpt = &game_screen_point_pool[pt + 0];
            p_specpt->X = sp1.X;
            p_specpt->Y = sp1.Y;

            cor_x = p_exface->X + p_exface->X1 - engn_xc;
            cor_z = p_exface->Z + p_exface->Z1 - engn_zc;
            cor_y = p_exface->Y + p_exface->Y1 - engn_yc;
            transform_shpoint(&sp2, cor_x, cor_y - 8 * engn_yc, cor_z);

            p_specpt = &game_screen_point_pool[pt + 1];
            p_specpt->X = sp2.X;
            p_specpt->Y = sp2.Y;

            cor_x = p_exface->X + p_exface->X2 - engn_xc;
            cor_z = p_exface->Z + p_exface->Z2 - engn_zc;
            cor_y = p_exface->Y + p_exface->Y2 - engn_yc;
              transform_shpoint(&sp3, cor_x, cor_y - 8 * engn_yc, cor_z);

            p_specpt = &game_screen_point_pool[pt + 2];
            p_specpt->X = sp3.X;
            p_specpt->Y = sp3.Y;

            depth_max = SHRT_MIN;
            if (depth_max < sp1.Depth)
                depth_max = sp1.Depth;
            if (depth_max < sp2.Depth)
                depth_max = sp2.Depth;
            if (depth_max < sp3.Depth)
                depth_max = sp3.Depth;

            flags_all = sp3.Flags & sp2.Flags & sp1.Flags;

            if (npoints > 3)
            {
                cor_x = p_exface->X + p_exface->X3 - engn_xc;
                cor_z = p_exface->Z + p_exface->Z3 - engn_zc;
                cor_y = p_exface->Y + p_exface->Y3 - engn_yc;
                transform_shpoint(&sp4, cor_x, cor_y - 8 * engn_yc, cor_z);

                p_specpt = &game_screen_point_pool[pt + 3];
                p_specpt->X = sp4.X;
                p_specpt->Y = sp4.Y;

                flags_all &= sp4.Flags;
                if (depth_max < sp4.Depth)
                    depth_max = sp4.Depth;
            }

            if ((flags_all & 0xF) != 0)
                break;

            dword_176D68++;
            draw_item_add(DrIT_Unkn5, exface, depth_max + 5000);
            break;
        case 0:
        default:
            break;
        }
    }
}

/******************************************************************************/
