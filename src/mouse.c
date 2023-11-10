#include <stdbool.h>
#include <stdint.h>

#include "mouse.h"
#include "bfscreen.h"
#include "bfplanar.h"
#include "bfsprite.h"
#include "bfmouse.h"
#include "display.h"

#pragma pack(1)

struct PointerHotspot {
    i8 x;
    i8 y;
    i8 z;
};

struct PointerHotspot pointer_hotspot[] = {
  {  0,  0, 0,},
  {  0,  0, 0,},
  { -7, -7, 0,},
  { -7, -7, 0,},
  { -5, -6, 0,},
  { -2, -2, 0,},
  { -7,-13, 0,},
  { -9, -1, 0,}, // mission ptr over platform
  {  0,  0, 0,}, // mission ptr normal
  { -7, -7, 0,},
  { -7, -7, 0,},
};


#pragma pack()

extern struct TbSprite *pointer_sprites;

void do_change_mouse(ushort mouse)
{
  LbMouseChangeSprite(&pointer_sprites[mouse]);
  LbMouseChangeSpriteOffset(pointer_hotspot[mouse].x, pointer_hotspot[mouse].y);
}

u8 process_mouse_imputs(void)
{
    int ret;
    asm volatile ("call ASM_process_mouse_imputs\n"
        : "=r" (ret) : );
    return ret;
}
