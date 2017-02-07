#pragma once

#include "graphics\Window.h"
#include "ui\InputManager.h"

namespace hemlock {
    namespace ui {
        struct InputEvent {
            CommonMouseState mouse;
            KeyModifiers     modifiers;
        };

        struct MouseEvent : InputEvent {
            MouseCoords coords;
        };
        struct MouseButtonEvent : MouseEvent {
            hui::MouseButton name;
            ui8              buttonID;
            ui8              presses;
        };
        struct MouseWheelScrollEvent : MouseEvent {
            i32 dx, dy;
        };
        struct MouseMoveEvent : MouseEvent {
            i32 dx, dy;
        };

        struct KeyboardButtonEvent : InputEvent {
            PhysicalKey physicalKey;
            VirtualKey  virtualKey;
            ButtonState state;
            ui8         presses;
        };

        struct TextInputEvent : InputEvent {
            ui8 text[32];
        };
        struct TextEditingEvent : TextInputEvent {
            ui32 start;
            ui32 length;
        };
                
        struct TextDropEvent {
            const char* text;
        };
        struct FileDropEvent {
            const char* filename;
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
            ~InputDispatcher() {
                dispose();
            };

            void init(hg::Window* window, hui::InputManager* manager);
            void dispose();

            void setTextMode(bool on);

            static i32 handleInputEvent(void* data, SDL_Event* event);

            PriorityEvent<>                      onMouseFocusLost;
            PriorityEvent<>                      onMouseFocusGained;
            PriorityEvent<MouseMoveEvent>        onMouseMove;
            PriorityEvent<MouseButtonEvent>      onMouseButtonDown;
            PriorityEvent<MouseButtonEvent>      onMouseButtonUp;
            PriorityEvent<MouseWheelScrollEvent> onMouseWheelScroll;

            PriorityEvent<>                      onKeyboardFocusLost;
            PriorityEvent<>                      onKeyboardFocusGained;
            PriorityEvent<KeyboardButtonEvent>   onKeyboardButtonDown;
            PriorityEvent<KeyboardButtonEvent>   onKeyboardButtonUp;

            PriorityEvent<TextInputEvent>        onTextInput;
            PriorityEvent<TextEditingEvent>      onTextEditing;

            PriorityEvent<>                      onDropBegin;
            PriorityEvent<>                      onDropComplete;
            PriorityEvent<TextDropEvent>         onTextDrop;
            PriorityEvent<FileDropEvent>         onFileDrop;

            RPriorityEvent<bool>                 onQuit;
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
