#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"
#include "game.h"

#define KEYBOARD_BUFFER_SIZE 16

extern uint32_t	keyboard_buffer[KEYBOARD_BUFFER_SIZE];
extern uint32_t	keyboard_buffer_read_index;
extern uint32_t	keyboard_buffer_write_index;
extern uint32_t keyboard_last_key;
extern uint32_t keyboard_last_key_prefixed;
extern uint8_t 	keyboard_table[256];

static uint8_t
sdlkey_to_scan_code (SDLKey key)
{
  switch (key)
    {
    case SDLK_ESCAPE:		return 0x01;
    case SDLK_1:		return 0x02;
    case SDLK_2:		return 0x03;
    case SDLK_3:		return 0x04;
    case SDLK_4:		return 0x05;
    case SDLK_5:		return 0x06;
    case SDLK_6:		return 0x07;
    case SDLK_7:		return 0x08;
    case SDLK_8:		return 0x09;
    case SDLK_9:		return 0x0a;
    case SDLK_0:		return 0x0b;
    case SDLK_MINUS:		return 0x0c;
    case SDLK_EQUALS:		return 0x0d;
    case SDLK_BACKSPACE:	return 0x0e;
    case SDLK_TAB:		return 0x0f;
    case SDLK_q:		return 0x10;
    case SDLK_w:		return 0x11;
    case SDLK_e:		return 0x12;
    case SDLK_r:		return 0x13;
    case SDLK_t:		return 0x14;
    case SDLK_y:		return 0x15;
    case SDLK_u:		return 0x16;
    case SDLK_i:		return 0x17;
    case SDLK_o:		return 0x18;
    case SDLK_p:		return 0x19;
    case SDLK_LEFTBRACKET:	return 0x1a;
    case SDLK_RIGHTBRACKET:	return 0x1b;
    case SDLK_KP_ENTER:
    case SDLK_RETURN:		return 0x1c;
    case SDLK_LCTRL:
    case SDLK_RCTRL:		return 0x1d;
    case SDLK_a:		return 0x1e;
    case SDLK_s:		return 0x1f;
    case SDLK_d:		return 0x20;
    case SDLK_f:		return 0x21;
    case SDLK_g:		return 0x22;
    case SDLK_h:		return 0x23;
    case SDLK_j:		return 0x24;
    case SDLK_k:		return 0x25;
    case SDLK_l:		return 0x26;
    case SDLK_SEMICOLON:	return 0x27;
    case SDLK_QUOTE:		return 0x28;
    case SDLK_BACKQUOTE:	return 0x29;
    case SDLK_LSHIFT:		return 0x2a;
    case SDLK_BACKSLASH:	return 0x2b;
    case SDLK_z:		return 0x2c;
    case SDLK_x:		return 0x2d;
    case SDLK_c:		return 0x2e;
    case SDLK_v:		return 0x2f;
    case SDLK_b:		return 0x30;
    case SDLK_n:		return 0x31;
    case SDLK_m:		return 0x32;
    case SDLK_COMMA:		return 0x33;
    case SDLK_PERIOD:		return 0x34;
    case SDLK_SLASH:		return 0x35;
    case SDLK_RSHIFT:		return 0x36;
    case SDLK_KP_MULTIPLY:	return 0x37;
    case SDLK_LALT:
    case SDLK_RALT:
    case SDLK_MODE:		return 0x38;
    case SDLK_SPACE:		return 0x39;
    case SDLK_CAPSLOCK:		return 0x3a;
    case SDLK_F1:		return 0x3b;
    case SDLK_F2:		return 0x3c;
    case SDLK_F3:		return 0x3d;
    case SDLK_F4:		return 0x3e;
    case SDLK_F5:		return 0x3f;
    case SDLK_F6:		return 0x40;
    case SDLK_F7:		return 0x41;
    case SDLK_F8:		return 0x42;
    case SDLK_F9:		return 0x43;
    case SDLK_F10:		return 0x44;
    case SDLK_NUMLOCK:		return 0x45;
    case SDLK_SCROLLOCK:	return 0x46;
    case SDLK_KP7:
    case SDLK_HOME:		return 0x47;
    case SDLK_KP8:
    case SDLK_UP:		return 0x48;
    case SDLK_KP9:
    case SDLK_PAGEUP:		return 0x49;
    case SDLK_KP_MINUS:		return 0x4a;
    case SDLK_KP4:
    case SDLK_LEFT:		return 0x4b;
    case SDLK_KP5:		return 0x4c;
    case SDLK_KP6:
    case SDLK_RIGHT:		return 0x4d;
    case SDLK_KP_PLUS:		return 0x4e;
    case SDLK_KP1:
    case SDLK_END:		return 0x4f;
    case SDLK_KP2:
    case SDLK_DOWN:		return 0x50;
    case SDLK_KP3:
    case SDLK_PAGEDOWN:		return 0x51;
    case SDLK_KP0:
    case SDLK_INSERT:		return 0x52;
    case SDLK_KP_PERIOD:
    case SDLK_DELETE:		return 0x53;
    case SDLK_SYSREQ:		return 0x54;

    case SDLK_F11:		return 0x57;
    case SDLK_F12:		return 0x58;

    case SDLK_F13:
    case SDLK_LSUPER:		return 0x5b;
    case SDLK_F14:
    case SDLK_RSUPER:		return 0x5c;
    case SDLK_F15:
    case SDLK_MENU:		return 0x5d;

    default:			return 0x00;
    }
}

static bool
is_key_prefixed (SDLKey key)
{
  switch (key)
    {
    case SDLK_RALT:
    case SDLK_MODE:
    case SDLK_RCTRL:
    case SDLK_KP_ENTER:
    case SDLK_KP_DIVIDE:
    case SDLK_HOME:
    case SDLK_UP:
    case SDLK_PAGEUP:
    case SDLK_LEFT:
    case SDLK_RIGHT:
    case SDLK_END:
    case SDLK_DOWN:
    case SDLK_PAGEDOWN:
    case SDLK_INSERT:
    case SDLK_DELETE:
      return true;

    default:
      return false;
    }
}

static uint8_t
get_key_table_index (SDLKey key)
{
  uint8_t index;

  index = sdlkey_to_scan_code (key);
  if (is_key_prefixed (key))
    index |= 0x80;

  return index;
}

static bool
is_key_pressed (SDLKey key)
{
  return (keyboard_table[get_key_table_index (key)] != 0);
}

static uint8_t
get_key_flags (void)
{
  uint8_t flags = 0;

  if (is_key_pressed (SDLK_LSHIFT) || is_key_pressed (SDLK_RSHIFT))
    flags |= 0x10;

  if (is_key_pressed (SDLK_LCTRL) || is_key_pressed (SDLK_RCTRL))
    flags |= 0x20;

  if (is_key_pressed (SDLK_LALT) || is_key_pressed (SDLK_RALT))
    flags |= 0x40;

  return flags;
}

static void
add_key_to_buffer (uint8_t key)
{
  uint32_t new_write_index;

  keyboard_buffer[keyboard_buffer_write_index] = key;

  new_write_index = (keyboard_buffer_write_index + 1) % KEYBOARD_BUFFER_SIZE;

  if (new_write_index != keyboard_buffer_read_index)
    keyboard_buffer_write_index = new_write_index;
}

static bool
handle_custom_key_press (const SDL_KeyboardEvent *ev)
{
  if (ev->type != SDL_KEYDOWN)
    return false;

  if ((is_key_pressed (SDLK_LCTRL) || is_key_pressed (SDLK_RCTRL)))
    {
      if (ev->keysym.sym == SDLK_q)
	{
	  game_quit ();
	  return true;
	}
    }

  return false;
}

void
keyboard_handle_event (const SDL_Event *ev)
{
  const SDL_KeyboardEvent *kev;
  uint8_t key_index;
  int scan_code;

  if (ev->type != SDL_KEYUP && ev->type != SDL_KEYDOWN)
    return;

  kev = (const SDL_KeyboardEvent *) ev;

  if (handle_custom_key_press (kev))
    return;

  scan_code = sdlkey_to_scan_code (kev->keysym.sym);

  if (scan_code == 0)
    return;

  keyboard_last_key_prefixed = is_key_prefixed (kev->keysym.sym);
  keyboard_last_key = scan_code;

  key_index = get_key_table_index (kev->keysym.sym);

  if (ev->type == SDL_KEYDOWN)
    {
      keyboard_table[key_index] = 1 | get_key_flags ();

      if (!keyboard_last_key_prefixed)
	add_key_to_buffer (scan_code);
    }
  else
    keyboard_table[key_index] = 0;
}

void
keyboard_initialise (void)
{
  SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
}

uint32_t
keyboard_read_key (void)
{
  uint32_t key;

  if (keyboard_buffer_read_index == keyboard_buffer_write_index)
    return 0;

  key = keyboard_buffer[keyboard_buffer_read_index];
  keyboard_buffer_read_index
    = (keyboard_buffer_read_index + 1) % KEYBOARD_BUFFER_SIZE;

  return key;
}

void
keyboard_clear_buffer (void)
{
  keyboard_buffer_read_index  = 0;
  keyboard_buffer_write_index = 0;
}
