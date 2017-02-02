#include "stdafx.h"

#include "ui\InputDispatcher.h"

static hui::MouseButton convertmb(ui8 sdlButton) {
    switch (sdlButton) {
        case SDL_BUTTON_LEFT:
            return hui::MouseButton::LEFT;
            break;
        case SDL_BUTTON_RIGHT:
            return hui::MouseButton::RIGHT;
            break;
        case SDL_BUTTON_MIDDLE:
            return hui::MouseButton::MIDDLE;
            break;
        default:
            return hui::MouseButton::UNKOWN;
    }
}

hui::InputDispatcher* hui::InputDispatcher::m_instance = nullptr;

h::Event<>                           hui::InputDispatcher::onMouseFocusLost(nullptr);
h::Event<>                           hui::InputDispatcher::onMouseFocusGained(nullptr);
h::Event<hui::MouseMoveEvent>        hui::InputDispatcher::onMouseMove(nullptr);
h::Event<hui::MouseButtonEvent>      hui::InputDispatcher::onMouseButtonDown(nullptr);
h::Event<hui::MouseButtonEvent>      hui::InputDispatcher::onMouseButtonUp(nullptr);
h::Event<hui::MouseWheelScrollEvent> hui::InputDispatcher::onMouseWheelScroll(nullptr);

h::Event<>                           hui::InputDispatcher::onKeyboardFocusLost(nullptr);
h::Event<>                           hui::InputDispatcher::onKeyboardFocusGained(nullptr);

h::Event<>                           hui::InputDispatcher::onQuit(nullptr);

void hui::InputDispatcher::init(hg::Window* window, hui::InputManager* manager) {
    if (m_initialised) return;
    m_initialised = true;
    m_window = window;
    m_manager = manager;

    SDL_SetEventFilter(hui::InputDispatcher::handleInputEvent, nullptr);
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
}

void hui::InputDispatcher::dispose() {
    if (!m_initialised) return;
    m_initialised = false;

    SDL_SetEventFilter(nullptr, nullptr);
    m_window = nullptr;
}

// Return 1 for unkown input, 0 for handled input.
i32 hui::InputDispatcher::handleInputEvent(void* data, SDL_Event* event) {
    switch (event->type) {
        // TODO(Matthew): Update InputManager with changes.
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN:
            MouseButtonEvent mbe;
            mbe.x      = event->button.x;
            mbe.y      = event->button.y;
            mbe.button = convertmb(event->button.button);
            mbe.state  = { false, false, false }; // TODO(Matthew): Get state from InputManager.
            if (event->button.type == SDL_MOUSEBUTTONDOWN) {
                onMouseButtonDown(mbe);
            } else {
                onMouseButtonUp(mbe);
            }
            break;
        case SDL_MOUSEMOTION:
            MouseMoveEvent mme;
            mme.x     = event->motion.x;
            mme.y     = event->motion.y;
            mme.dx    = event->motion.xrel;
            mme.dy    = event->motion.yrel;
            mme.state = { false, false, false };
            onMouseMove(mme);
            break;
        case SDL_MOUSEWHEEL:
            MouseWheelScrollEvent mwse;
            mwse.x     = 0; // TODO(Matthew): Get mouse position from InputManager.
            mwse.y     = 0;
            mwse.dx    = event->wheel.x;
            mwse.dy    = event->wheel.y;
            mwse.state = { false, false, false };
            if (event->wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                mwse.dx *= -1;
                mwse.dy *= -1;
            }
            onMouseWheelScroll(mwse);
            break;
        case SDL_QUIT:
            SDL_Quit();
            exit(0);
        case SDL_WINDOWEVENT:
            switch (event->window.event) {
                case SDL_WINDOWEVENT_ENTER:
                    onMouseFocusGained();
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    onMouseFocusLost();
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    onKeyboardFocusGained();
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    onKeyboardFocusLost();
                    break;
                default:
                    return 1;
            }
        default:
            return 1;
    }
    return 0;
}
