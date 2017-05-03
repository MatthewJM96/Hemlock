#include "stdafx.h"

#include "ui/InputManager.h"

#include "ui/Keys.hpp"

hui::ButtonState hui::InputManager::getMouseButtonState(ui8 button) {
    return m_mouseButtonStates[button - 1];
}

hui::ButtonState hui::InputManager::getKeyboardKeyState(PhysicalKey key) {
    auto it = m_keyStates.find(key);
    if (it == m_keyStates.end()) return {};
    return (*it).second;
}

hui::CommonMouseState hui::InputManager::getCommonMouseState() {
    CommonMouseState cms;
    cms.left = m_mouseButtonStates[(ui8)MouseButton::LEFT].pressed;
    cms.right = m_mouseButtonStates[(ui8)MouseButton::RIGHT].pressed;
    cms.middle = m_mouseButtonStates[(ui8)MouseButton::MIDDLE].pressed;
    return cms;
}

bool hemlock::ui::InputManager::isPressed(ui8 button) {
    return m_mouseButtonStates[button].pressed;
}

bool hemlock::ui::InputManager::isPressed(PhysicalKey key) {
    auto it = m_keyStates.find(key);
    if (it == m_keyStates.end()) return false;
    return (*it).second.pressed;
}

void hui::InputManager::press(ui8 button) {
    m_mouseButtonStates[button].pressed = true;
    ++m_mouseButtonStates[button].pressCount;
    m_mouseButtonStates[button].lastPressTime = SDL_GetTicks();
}

void hui::InputManager::release(ui8 button) {
    m_mouseButtonStates[button].pressed = false;
    m_mouseButtonStates[button].lastReleaseTime = SDL_GetTicks();
}

void hui::InputManager::press(PhysicalKey key) {
    switch (key) {
        case PhysicalKey::H_LSHIFT:
            m_keyModifierStates.lshift = true;
            break;
        case PhysicalKey::H_RSHIFT:
            m_keyModifierStates.rshift = true;
            break;
        case PhysicalKey::H_LCTRL:
            m_keyModifierStates.lctrl = true;
            break;
        case PhysicalKey::H_RCTRL:
            m_keyModifierStates.rctrl = true;
            break;
        case PhysicalKey::H_LALT:
            m_keyModifierStates.lalt = true;
            break;
        case PhysicalKey::H_RALT:
            m_keyModifierStates.ralt = true;
            break;
        case PhysicalKey::H_LCMD:
            m_keyModifierStates.lcmd = true;
            break;
        case PhysicalKey::H_RCMD:
            m_keyModifierStates.rcmd = true;
            break;
        case PhysicalKey::H_NUMLOCKCLEAR:
            m_keyModifierStates.numlck = !m_keyModifierStates.numlck;
            break;
        case PhysicalKey::H_CAPSLOCK:
            m_keyModifierStates.capslck = !m_keyModifierStates.capslck;
            break;
        case PhysicalKey::H_SCROLLLOCK:
            m_keyModifierStates.scrllck = !m_keyModifierStates.scrllck;
            break;
        default:
            break;
    }

    auto it = m_keyStates.find(key);
    if (it == m_keyStates.end()) {
        m_keyStates[key] = {
            true,
            SDL_GetTicks(),
            0,
            m_keyModifierStates,
            {},
            1
        };
    } else {
        (*it).second.pressed = true;
        ++(*it).second.pressCount;
        (*it).second.lastPressTime = SDL_GetTicks();
        (*it).second.lastPressModifiers = m_keyModifierStates;
    }
}

void hui::InputManager::release(PhysicalKey key) {
    switch (key) {
        case PhysicalKey::H_LSHIFT:
            m_keyModifierStates.lshift = false;
            break;
        case PhysicalKey::H_RSHIFT:
            m_keyModifierStates.rshift = false;
            break;
        case PhysicalKey::H_LCTRL:
            m_keyModifierStates.lctrl = false;
            break;
        case PhysicalKey::H_RCTRL:
            m_keyModifierStates.rctrl = false;
            break;
        case PhysicalKey::H_LALT:
            m_keyModifierStates.lalt = false;
            break;
        case PhysicalKey::H_RALT:
            m_keyModifierStates.ralt = false;
            break;
        case PhysicalKey::H_LCMD:
            m_keyModifierStates.lcmd = false;
            break;
        case PhysicalKey::H_RCMD:
            m_keyModifierStates.rcmd = false;
            break;
        default:
            break;
    }

    auto it = m_keyStates.find(key);
    if (it == m_keyStates.end()) {
        m_keyStates[key] = {
            true,
            0,
            SDL_GetTicks(),
            {},
            m_keyModifierStates,
            0
        };
    } else {
        (*it).second.pressed = false;
        (*it).second.lastReleaseTime = SDL_GetTicks();
        (*it).second.lastReleaseModifiers = m_keyModifierStates;
    }
}
