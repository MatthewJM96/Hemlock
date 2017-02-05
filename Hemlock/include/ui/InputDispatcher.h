#pragma once

#include "graphics\Window.h"
#include "ui\InputManager.h"

namespace hemlock {
    namespace ui {
        // Input Events:
        //   MouseButtonDown
        //   MouseButtonUp
        //   MouseWheelScroll
        //   MouseMove
        //   MouseFocusGained
        //   MouseFocusLost
        //   KeyboardButtonDown
        //   KeyboardButtonUp
        //   TextInput
        //   TextEdit

        // TODO(Matthew): Replace mouse and keyboard state objects with pointer to InputManager.
        struct MouseState {
            bool left : 1;
            bool right : 1;
            bool middle : 1;
        };
        struct MouseEvent {
            ui32 x, y;
            MouseState state; // Bit field of whether left, right or middle mouse buttons are pressed.
        };
        struct MouseButtonEvent : MouseEvent {
            hui::MouseButton button;
        };
        struct MouseWheelScrollEvent : MouseEvent {
            i32 dx, dy;
        };
        struct MouseMoveEvent : MouseEvent {
            i32 dx, dy;
        };

        struct KeyboardState { // Encodes common key modifier states.
            bool lctrl   : 1;
            bool lalt    : 1;
            bool lshft   : 1;
            bool rctrl   : 1;
            bool ralt    : 1;
            bool rshft   : 1;
        };
        struct KeyboardEvent {
            KeyboardState state;
        };
        struct KeyboardButtonEvent : KeyboardEvent {
            ui32 physicalKey, virtualKey;
        };

        class InputDispatcher {
        public:
            static InputDispatcher* getInstance() {
                if (m_instance == nullptr) {
                    m_instance = new InputDispatcher();
                }
                return m_instance;
            }
            InputDispatcher(InputDispatcher const&) = delete;
            void operator=(InputDispatcher const&)  = delete;
            ~InputDispatcher() {};

            void init(hg::Window* window, hui::InputManager* manager);
            void dispose();

            static i32 handleInputEvent(void* data, SDL_Event* event);

            // TODO(Matthew): Better place for these?
            static Event<>                      onMouseFocusLost;
            static Event<>                      onMouseFocusGained;
            static Event<MouseMoveEvent>        onMouseMove;
            static Event<MouseButtonEvent>      onMouseButtonDown;
            static Event<MouseButtonEvent>      onMouseButtonUp;
            static Event<MouseWheelScrollEvent> onMouseWheelScroll;

            static Event<>                      onKeyboardFocusLost;
            static Event<>                      onKeyboardFocusGained;
            static Event<KeyboardButtonEvent>   onKeyboardButtonDown;
            static Event<KeyboardButtonEvent>   onKeyboardButtonUp;

            static Event<>                      onQuit;
        private:
            InputDispatcher() :
                m_window(nullptr) {};

            static InputDispatcher* m_instance;

            bool m_initialised;
            hg::Window* m_window;
            hui::InputManager* m_manager;
        };
    }
}
namespace hui = hemlock::ui;
