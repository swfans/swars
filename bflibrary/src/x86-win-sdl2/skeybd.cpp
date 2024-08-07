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
#include <map>
#include <cstring>
#include <SDL.h>
#include "bfkeybd.h"

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

/** @internal
 *  Creates mapping for translating SDL key codes into bflibrary key codes.
 */
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
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_PLUS, KC_ADD));
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
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(178, KC_GRAVE));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_DELETE, KC_DELETE));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_0, KC_NUMPAD0));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_1, KC_NUMPAD1));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_2, KC_NUMPAD2));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_3, KC_NUMPAD3));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_4, KC_NUMPAD4));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_5, KC_NUMPAD5));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_6, KC_NUMPAD6));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_7, KC_NUMPAD7));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_8, KC_NUMPAD8));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_KP_9, KC_NUMPAD9));
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
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_NUMLOCKCLEAR, KC_NUMLOCK));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_CAPSLOCK, KC_CAPITAL));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_SCROLLLOCK, KC_SCROLL));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_RSHIFT, KC_RSHIFT));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_LSHIFT, KC_LSHIFT));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_RCTRL, KC_RCONTROL));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_LCTRL, KC_LCONTROL));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_RALT, KC_RALT));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_LALT, KC_LALT));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_LGUI, KC_LWIN));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_RGUI, KC_RWIN));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_MODE, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_HELP, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_PRINTSCREEN, KC_UNASSIGNED));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_SYSREQ, KC_SYSRQ));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_MENU, KC_APPS));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_POWER, KC_POWER));
    keymap_sdl_to_bf.insert(pair<int, TbKeyCode>(SDLK_UNDO, KC_UNASSIGNED));
}

/** @internal
 *  Maps keyboard event to bflibrary key code.
 */
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
    case SDLK_LGUI:
    case SDLK_RGUI:
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

extern "C" {
TbResult KEvent(const SDL_Event *ev);
};

/** @internal
 * Triggers keyboard control function for given SDL keyboard event.
 * @return SUCCESS if the event was processed, FAIL if key isn't supported, OK if no keyboard event.
 */
TbResult KEvent(const SDL_Event *ev)
{
    TbKeyCode code;
    TbKeyMods modifiers;
    TbKeyAction action;
    TbResult ret;

    switch (ev->type)
    {
    case SDL_KEYDOWN:
        action = KActn_KEYDOWN;
        code = KeyboardKeysMapping(&ev->key);
        if (code != KC_UNASSIGNED) {
            modifiers = KeyboardModsMapping(&ev->key);
            ret = keyboardControl(action, code, modifiers);
            return ret;
        }
        return Lb_FAIL;

    case SDL_KEYUP:
        action = KActn_KEYUP;
        code = KeyboardKeysMapping(&ev->key);
        if (code != KC_UNASSIGNED) {
            modifiers = KeyboardModsMapping(&ev->key);
            ret = keyboardControl(action, code, modifiers);
            return ret;
        }
        return Lb_FAIL;
    }
    return Lb_OK;
}

TbResult LbIKeyboardOpen(void)
{
    memset(lbKeyOn, 0, 256);
    KeyboardMappingPrepare();
    return 1;
}

TbResult LbIKeyboardClose(void)
{
    return 1;
}

/******************************************************************************/
