#pragma once

#include "Keys.hpp"

namespace hemlock {
    namespace ui {
        enum class MouseButton : ui8 {
            LEFT = 0,
            RIGHT,
            MIDDLE,
            UNKNOWN
        };

        struct MouseCoords {
            i32 x, y;
        };
        
        struct KeyModifiers {
            bool lshift  : 1;
            bool rshift  : 1;
            bool lctrl   : 1;
            bool rctrl   : 1;
            bool lalt    : 1;
            bool ralt    : 1;
            bool lcmd    : 1;
            bool rcmd    : 1;
            bool numlck  : 1;
            bool capslck : 1;
            bool scrllck : 1;
        };

        struct CommonMouseState {
            bool left : 1;
            bool right : 1;
            bool middle : 1;
        };

        struct ButtonState {
            bool pressed;
            ui32 lastPressTime;
            ui32 lastReleaseTime;
            KeyModifiers lastPressModifiers;
            KeyModifiers lastReleaseModifiers;
            ui32 pressCount;
        };

        class InputManager {
        public:
            void dispose() {}

            KeyModifiers getCurrentKeyModifierState() { return m_keyModifierStates; }
            ButtonState  getMouseButtonState(ui8 button);
            ButtonState  getKeyboardKeyState(PhysicalKey key);

            MouseCoords      getMouseCoords() { return m_mouseCoords; }
            CommonMouseState getCommonMouseState();

            void setMouseCoords(MouseCoords coords) { m_mouseCoords = coords; }

            void setMouseFocus(bool focus) { m_mouseFocus = focus; }
            void setKeyboardFocus(bool focus) { m_keyboardFocus = focus; }
            
            bool isMouseFocused() { return m_mouseFocus; }
            bool isKeyboardFocused() { return m_keyboardFocus; }

            bool isPressed(ui8 button);
            bool isPressed(PhysicalKey key);

            void press(ui8 button);
            void release(ui8 button);

            void press(PhysicalKey key);
            void release(PhysicalKey key);
        private:
            bool m_mouseFocus;
            bool m_keyboardFocus;
            MouseCoords m_mouseCoords;
            KeyModifiers m_keyModifierStates;
            ButtonState m_mouseButtonStates[255];
            std::unordered_map<PhysicalKey, ButtonState> m_keyStates;
        };
    }
}
namespace hui = hemlock::ui;
