#include "stdafx.h"

#include "ui/InputDispatcher.h"

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
            return hui::MouseButton::UNKNOWN;
    }
}

hui::InputDispatcher* hui::InputDispatcher::m_instance = nullptr;

void hui::InputDispatcher::init(hg::Window* window, hui::InputManager* manager) {
    if (m_initialised) return;
    m_initialised = true;
    m_window = window;
    m_manager = manager;

    SDL_SetEventFilter(hui::InputDispatcher::handleInputEvent, (void*)this);
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    SDL_EventState(SDL_DROPBEGIN, SDL_ENABLE);
    SDL_EventState(SDL_DROPCOMPLETE, SDL_ENABLE);
    SDL_EventState(SDL_DROPTEXT, SDL_ENABLE);
}

void hui::InputDispatcher::dispose() {
    if (!m_initialised) return;
    m_initialised = false;

    SDL_SetEventFilter(nullptr, nullptr);
    m_window = nullptr;
}

void hui::InputDispatcher::setTextMode(bool on) {
    if (on) {
        SDL_StartTextInput();
    } else {
        SDL_StopTextInput();
    }
}

// Return 1 for unknown input, 0 for handled input.
i32 hui::InputDispatcher::handleInputEvent(void* data, SDL_Event* event) {
    InputDispatcher* inputDispatcher = static_cast<InputDispatcher*>(data);
    switch (event->type) {
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN:
            MouseButtonEvent mbe;
            mbe.coords    = { event->button.x, event->button.y };
            mbe.name      = convertmb(event->button.button);
            mbe.buttonID  = event->button.button - 1;
            mbe.presses   = event->button.clicks;
            mbe.mouse     = inputDispatcher->m_manager->getCommonMouseState();
            mbe.modifiers = inputDispatcher->m_manager->getCurrentKeyModifierState();
            if (event->button.type == SDL_MOUSEBUTTONDOWN) {
                inputDispatcher->onMouseButtonDown(mbe);
                inputDispatcher->m_manager->press(mbe.buttonID);
            } else {
                inputDispatcher->onMouseButtonUp(mbe);
                inputDispatcher->m_manager->release(mbe.buttonID);
            }
            break;
        case SDL_MOUSEMOTION:
            MouseMoveEvent mme;
            mme.coords    = { event->motion.x, event->motion.y };
            mme.dx        = event->motion.xrel;
            mme.dy        = event->motion.yrel;
            mme.mouse     = inputDispatcher->m_manager->getCommonMouseState();
            mme.modifiers = inputDispatcher->m_manager->getCurrentKeyModifierState();
            inputDispatcher->onMouseMove(mme);
            inputDispatcher->m_manager->setMouseCoords(mme.coords);
            break;
        case SDL_MOUSEWHEEL:
            MouseWheelScrollEvent mwse;
            mwse.coords    = inputDispatcher->m_manager->getMouseCoords();
            mwse.dx        = event->wheel.x;
            mwse.dy        = event->wheel.y;
            mwse.mouse     = inputDispatcher->m_manager->getCommonMouseState();
            mwse.modifiers = inputDispatcher->m_manager->getCurrentKeyModifierState();
            if (event->wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                mwse.dx *= -1;
                mwse.dy *= -1;
            }
            inputDispatcher->onMouseWheelScroll(mwse);
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            KeyboardButtonEvent kbe;
            kbe.physicalKey = (PhysicalKey)event->key.keysym.scancode;
            kbe.virtualKey  = (VirtualKey)event->key.keysym.sym;
            kbe.presses     = event->key.repeat;
            kbe.state       = inputDispatcher->m_manager->getKeyboardKeyState(kbe.physicalKey);
            kbe.mouse       = inputDispatcher->m_manager->getCommonMouseState();
            kbe.modifiers   = inputDispatcher->m_manager->getCurrentKeyModifierState();
            if (event->key.type == SDL_KEYDOWN) {
                inputDispatcher->onKeyboardButtonDown(kbe);
                inputDispatcher->m_manager->press(kbe.physicalKey);
            } else {
                inputDispatcher->onKeyboardButtonUp(kbe);
                inputDispatcher->m_manager->release(kbe.physicalKey);
            }
            break;
        case SDL_QUIT:
            if (!inputDispatcher->onQuit()) {
                SDL_Quit();
                exit(0);
            }
        case SDL_WINDOWEVENT:
            switch (event->window.event) {
                case SDL_WINDOWEVENT_ENTER:
                    inputDispatcher->onMouseFocusGained();
                    inputDispatcher->m_manager->setMouseFocus(true);
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    inputDispatcher->onMouseFocusLost();
                    inputDispatcher->m_manager->setMouseFocus(false);
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    inputDispatcher->onKeyboardFocusGained();
                    inputDispatcher->m_manager->setKeyboardFocus(true);
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    inputDispatcher->onKeyboardFocusLost();
                    inputDispatcher->m_manager->setKeyboardFocus(false);
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    inputDispatcher->m_window->windowDimensionsHaveChanged(event->window.data1, event->window.data2);
                default:
                    return 1;
            }
            break;
        case SDL_TEXTINPUT:
            TextInputEvent tie;
            std::memcpy(tie.text, event->text.text, 32);
            tie.mouse     = inputDispatcher->m_manager->getCommonMouseState();
            tie.modifiers = inputDispatcher->m_manager->getCurrentKeyModifierState();
            inputDispatcher->onTextInput(tie);
            break;
        case SDL_TEXTEDITING:
            TextEditingEvent tee;
            std::memcpy(tee.text, event->edit.text, 32);
            tee.start     = event->edit.start;
            tee.length    = event->edit.length;
            tee.mouse     = inputDispatcher->m_manager->getCommonMouseState();
            tee.modifiers = inputDispatcher->m_manager->getCurrentKeyModifierState();
            inputDispatcher->onTextEditing(tee);
            break;
        case SDL_DROPBEGIN:
            inputDispatcher->onDropBegin();
            break;
        case SDL_DROPCOMPLETE:
            inputDispatcher->onDropComplete();
            break;
        case SDL_DROPFILE:
            FileDropEvent fde;
            fde.filename = event->drop.file;
            inputDispatcher->onFileDrop(fde);
            break;
        case SDL_DROPTEXT:
            TextDropEvent tde;
            tde.text = event->drop.file;
            inputDispatcher->onTextDrop(tde);
            break;
        default:
            return 1;
    }
    return 0;
}
