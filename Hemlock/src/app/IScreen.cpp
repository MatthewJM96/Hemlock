#include "stdafx.h"

#include "app\IScreen.h"

void happ::IScreen::init(char* name) {
    if (m_initialised) return;
    m_initialised = true;

    m_name = name;
}

void happ::IScreen::dispose() {
    if (!m_initialised) return;
    m_initialised = false;

    delete m_name;
    m_name = nullptr;
}

void happ::IScreen::start(TimeData time) {
    m_state = ScreenState::RUNNING;
}

void happ::IScreen::end(TimeData time) {
    m_state = ScreenState::NONE;
}
