/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file skeybd.cpp
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
#include "bfkeybd.h"

#include <map>
#include <cstring>
#include <SDL/SDL.h>

using namespace std;

extern KeyboardEventHandler lbKEventCustomHandler;

TbResult LbKeyboardOpen(void)
{
    return 1;
}

TbResult LbKeyboardClose(void)
{
    return 1;
}

static std::map<int, TbKeyCode> keymap_sdl_to_bf;

static void KeyboardMappingPrepare(void)
{
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_a, KC_A));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_b, KC_B));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_c, KC_C));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_d, KC_D));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_e, KC_E));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_f, KC_F));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_g, KC_G));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_h, KC_H));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_i, KC_I));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_j, KC_J));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_k, KC_K));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_l, KC_L));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_m, KC_M));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_n, KC_N));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_o, KC_O));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_p, KC_P));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_q, KC_Q));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_r, KC_R));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_s, KC_S));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_t, KC_T));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_u, KC_U));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_v, KC_V));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_w, KC_W));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_x, KC_X));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_y, KC_Y));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_z, KC_Z));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F1, KC_F1));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F2, KC_F2));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F3, KC_F3));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F4, KC_F4));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F5, KC_F5));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F6, KC_F6));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F7, KC_F7));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F8, KC_F8));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F9, KC_F9));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F10, KC_F10));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F11, KC_F11));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F12, KC_F12));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F13, KC_F13));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F14, KC_F14));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_F15, KC_F15));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_BACKSPACE, KC_BACK));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_TAB, KC_TAB));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_CLEAR, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_RETURN, KC_RETURN));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_PAUSE, KC_PAUSE));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_ESCAPE, KC_ESCAPE));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_SPACE, KC_SPACE));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_EXCLAIM, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_QUOTEDBL, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_HASH, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_DOLLAR, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_AMPERSAND, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_QUOTE, KC_APOSTROPHE));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_LEFTPAREN, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_RIGHTPAREN, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_ASTERISK, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_PLUS, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_COMMA, KC_COMMA));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_MINUS, KC_MINUS));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_PERIOD, KC_PERIOD));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_SLASH, KC_SLASH));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_0, KC_0));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_1, KC_1));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_2, KC_2));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_3, KC_3));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_4, KC_4));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_5, KC_5));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_6, KC_6));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_7, KC_7));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_8, KC_8));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_9, KC_9));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_COLON, KC_COLON));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_SEMICOLON, KC_SEMICOLON));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_LESS, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_EQUALS, KC_EQUALS));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_GREATER, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_QUESTION, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_AT, KC_AT));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_LEFTBRACKET, KC_LBRACKET));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_BACKSLASH, KC_BACKSLASH));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_RIGHTBRACKET, KC_RBRACKET));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_CARET, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_UNDERSCORE, KC_UNDERLINE));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_BACKQUOTE, KC_GRAVE));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_DELETE, KC_DELETE));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP0, KC_NUMPAD0));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP1, KC_NUMPAD1));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP2, KC_NUMPAD2));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP3, KC_NUMPAD3));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP4, KC_NUMPAD4));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP5, KC_NUMPAD5));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP6, KC_NUMPAD6));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP7, KC_NUMPAD7));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP8, KC_NUMPAD8));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP9, KC_NUMPAD9));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_PERIOD, KC_DECIMAL));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_DIVIDE, KC_DIVIDE));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_MULTIPLY, KC_MULTIPLY));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_MINUS, KC_SUBTRACT));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_PLUS, KC_ADD));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_ENTER, KC_NUMPADENTER));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_EQUALS, KC_NUMPADEQUALS));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_UP, KC_UP));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_DOWN, KC_DOWN));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_RIGHT, KC_RIGHT));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_LEFT, KC_LEFT));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_INSERT, KC_INSERT));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_HOME, KC_HOME));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_END, KC_END));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_PAGEUP, KC_PGUP));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_PAGEDOWN, KC_PGDOWN));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_NUMLOCK, KC_NUMLOCK));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_CAPSLOCK, KC_CAPITAL));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_SCROLLOCK, KC_SCROLL));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_RSHIFT, KC_RSHIFT));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_LSHIFT, KC_LSHIFT));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_RCTRL, KC_RCONTROL));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_LCTRL, KC_LCONTROL));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_RALT, KC_RALT));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_LALT, KC_LALT));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_RMETA, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_LMETA, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_LSUPER, KC_LWIN));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_RSUPER, KC_RWIN));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_MODE, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_COMPOSE, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_HELP, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_PRINT, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_SYSREQ, KC_SYSRQ));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_BREAK, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_MENU, KC_APPS));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_POWER, KC_POWER));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_EURO, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_UNDO, KC_UNASSIGNED));
}

static TbKeyCode KeyboardKeysMapping(const SDL_KeyboardEvent * key)
{
    /*
    key->keysym.scancode;         < hardware specific scancode
    key->keysym.sym;         < SDL virtual keysym
    key->keysym.unicode;         < translated character
    */
    int keycode = key->keysym.sym;
    std::map<int, TbKeyCode>::iterator iter;

    iter = keymap_sdl_to_bf.find(keycode);
    if (iter != keymap_sdl_to_bf.end())
    {
        return iter->second;
    }

    return KC_UNASSIGNED;
}

static TbKeyMods KeyboardModsMapping(const SDL_KeyboardEvent * key)
{
    TbKeyMods keymod = KMod_NONE;
    switch (key->keysym.sym)
    {
    // Pressing only a modifier will not treat the key as modifier.
    // If that happens, don't care, so that keyboard control won't try to fix anything.
    case SDLK_RSHIFT:
    case SDLK_LSHIFT:
    case SDLK_RCTRL:
    case SDLK_LCTRL:
    case SDLK_RALT:
    case SDLK_LALT:
    case SDLK_RMETA:
    case SDLK_LMETA:
    case SDLK_LSUPER:
    case SDLK_RSUPER:
        keymod = KMod_DONTCARE;
        break;
    // If pressed any other key, mind the modifiers, to allow keyboard control fixes.
    default:
        if ((key->keysym.mod & KMOD_CTRL) != 0)
            keymod |= KMod_CONTROL;
        if ((key->keysym.mod & KMOD_SHIFT) != 0)
            keymod |= KMod_SHIFT;
        if ((key->keysym.mod & KMOD_ALT) != 0)
            keymod |= KMod_ALT;
        break;
    }
    return keymod;
}

TbResult LbSDLKeyboard(void)
{
    // No need to do anything - keyboard read happens in SDL event handler
    return Lb_OK;
}

char LbKeyboard(void)
{
    LbSDLKeyboard();
    if (lbInkey < 128)
        return lbInkeyToAscii[lbInkey];
    return 0;
}

/** @internal
 *  Check for undetected/incorrectly maintained modifiers.
 */
static inline TbResult KEventModsCheck(TbKeyAction action, TbKeyMods modifiers)
{
    if (modifiers == KMod_DONTCARE)
        return Lb_OK;
    // If modifiers were supplied, make sure they are correctly set in lbKeyOn[]
    if (modifiers & KMod_SHIFT)
    {
        if (!lbKeyOn[KC_RSHIFT] && !lbKeyOn[KC_LSHIFT])
            lbKeyOn[KC_LSHIFT] = 1;
    } else
    {
        lbKeyOn[KC_LSHIFT] = 0;
        lbKeyOn[KC_RSHIFT] = 0;
    }
    if (modifiers & KMod_CONTROL)
    {
        if (!lbKeyOn[KC_RCONTROL] && !lbKeyOn[KC_LCONTROL])
            lbKeyOn[KC_LCONTROL] = 1;
    } else
    {
        lbKeyOn[KC_LCONTROL] = 0;
        lbKeyOn[KC_RCONTROL] = 0;
    }
    if (modifiers & KMod_ALT)
    {
        if (!lbKeyOn[KC_RALT] && !lbKeyOn[KC_LALT])
            lbKeyOn[KC_LALT] = 1;
    } else
    {
        lbKeyOn[KC_LALT] = 0;
        lbKeyOn[KC_RALT] = 0;
    }
    return Lb_SUCCESS;
}

/** @internal
 * Update modifiers flags based on previously set lbKeyOn[] array.
 */
static inline TbResult KEventModsUpdate(TbKeyAction action, TbKeyCode code)
{
    lbInkeyFlags = 0;
    if (lbKeyOn[KC_LSHIFT] || lbKeyOn[KC_RSHIFT])
        lbInkeyFlags |= KMod_SHIFT;
    if (lbKeyOn[KC_LCONTROL] || lbKeyOn[KC_RCONTROL])
        lbInkeyFlags |= KMod_CONTROL;
    if (lbKeyOn[KC_LALT] || lbKeyOn[KC_RALT])
        lbInkeyFlags |= KMod_ALT;
    if (lbKeyOn[code] != 0)
        lbKeyOn[code] |= lbInkeyFlags;
    return Lb_OK;
}

/** @internal
 * Update IInkey values based on previously set lbInkey.
 */
static inline TbResult KEventIInkeyUpdate(TbKeyAction action, TbKeyCode code)
{
    if (lbInkey < 0x80)
    {
        if (lbIInkey == 0)
        {
            lbIInkey = lbInkey;
            lbIInkeyFlags = lbInkeyFlags;
        }
    }
    return Lb_OK;
}

/** @internal
 * Triggers keyboard control function for given SDL keyboard event.
 * @return SUCCESS if the event was processed, FAIL if key isn't supported, OK if no keyboard event.
 */
TbResult KEvent(const SDL_Event *ev)
{
    TbKeyCode code;
    TbKeyMods modifiers;
    TbKeyAction action;
    switch (ev->type)
    {
    case SDL_KEYDOWN:
        action = KActn_KEYDOWN;
        code = KeyboardKeysMapping(&ev->key);
        if (code != KC_UNASSIGNED) {
            lbKeyOn[code] = 1;
            lbInkey = code;
            modifiers = KeyboardModsMapping(&ev->key);
            KEventModsCheck(action, modifiers);
            KEventModsUpdate(action, code);
            KEventIInkeyUpdate(action, code);
            if (lbKEventCustomHandler != NULL)
                lbKEventCustomHandler(action, code);
            return Lb_SUCCESS;
        }
        return Lb_FAIL;

    case SDL_KEYUP:
        action = KActn_KEYUP;
        code = KeyboardKeysMapping(&ev->key);
        if (code != KC_UNASSIGNED) {
            lbKeyOn[code] = 0;
            lbExtendedKeyPress = 0;
            modifiers = KeyboardModsMapping(&ev->key);
            KEventModsCheck(action, modifiers);
            KEventModsUpdate(action, code);
            KEventIInkeyUpdate(action, code);
            if (lbKEventCustomHandler != NULL)
                lbKEventCustomHandler(action, code);
            return Lb_SUCCESS;
        }
        return Lb_FAIL;
    }
    return Lb_OK;
}

extern "C" {
void keyboard_handle_event (const SDL_Event *ev);
};

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
  return (lbKeyOn[get_key_table_index (key)] != 0);
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

void keyboard_handle_event (const SDL_Event *ev)
{
  const SDL_KeyboardEvent *kev;
  uint8_t key_index;
  int scan_code;

  if (ev->type != SDL_KEYUP && ev->type != SDL_KEYDOWN)
    return;

  kev = (const SDL_KeyboardEvent *) ev;

  scan_code = sdlkey_to_scan_code (kev->keysym.sym);

  if (scan_code == 0)
    return;

  lbInkey_prefixed = is_key_prefixed (kev->keysym.sym);
  lbInkey = scan_code;

  key_index = get_key_table_index (kev->keysym.sym);

  if (ev->type == SDL_KEYDOWN)
    {
      lbKeyOn[key_index] = 1 | get_key_flags ();

      lbKEventCustomHandler(KActn_KEYDOWN, scan_code);
    }
  else
    lbKeyOn[key_index] = 0;
}

TbResult LbIKeyboardOpen(void)
{
    memset(lbKeyOn, 0, 256);
    KeyboardMappingPrepare();
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
    return 1;
}

TbResult LbIKeyboardClose(void)
{
    return 1;
}

/******************************************************************************/
