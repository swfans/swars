#ifndef MOUSE_H
#define MOUSE_H

#include "bftypes.h"

struct TbPoint;

TbResult load_mouse_pointers_sprites_for_current_mode(void);

ubyte process_mouse_imputs(void);
void do_change_mouse(ushort mouse);

#endif
