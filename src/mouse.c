#include <stdbool.h>
#include <stdint.h>

#include "mouse.h"
#include "bfscreen.h"
#include "bfplanar.h"
#include "bfmouse.h"
#include "display.h"


void mouse_initialise(void)
{
    SDL_ShowCursor(SDL_DISABLE);
}

#if 0
void do_change_mouse(ushort mouse)
{
  LbMouseChangeSprite(&pointer_sprites[mouse]);
  LbMouseChangeSpriteOffset(pointer_hotspot[mouse].x, pointer_hotspot[mouse].y);
}
#endif
