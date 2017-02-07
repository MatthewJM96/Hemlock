#include "stdafx.h"

#include "MainMenuScreen.h"

#include <IL\il.h>

#include <io\ImageLoader.h>
#include <graphics\Texture.h>
#include <glm\gtc\matrix_transform.hpp>

void MainMenuScreen::init(char* name) {
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
