#include "stdafx.h"

#include "MainMenuScreen.h"

void MainMenuScreen::init(const char* name) {
    if (m_initialised) return;
    IScreen::init(name);
}

void MainMenuScreen::dispose() {
    if (!m_initialised) return;


    IScreen::dispose();
}

void MainMenuScreen::update(TimeData time) {
}

void MainMenuScreen::draw(TimeData time) {

}
