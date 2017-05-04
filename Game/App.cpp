#include "stdafx.h"

#include "App.h"

#include "MainMenuScreen.h"
#include "VoxelTestScreen.h"

void App::init() {
    if (m_initialised) return;
    IApp::init();
    
    hui::InputDispatcher::getInstance()->onKeyboardButtonUp += { 1, h::makeDelegate(this, &App::handleKeyboardButtonDown) };
}

void App::prepareScreens() {
    Screen mainmenu;
    mainmenu.first = "main_menu";
    mainmenu.second = new MainMenuScreen();
    addScreen(mainmenu);

    Screen voxeltest;
    voxeltest.first = "voxel_test";
    voxeltest.second = new VoxelTestScreen(m_inputManager);
    addScreen(voxeltest);

    setCurrentScreen(voxeltest.first);
}

bool App::handleKeyboardButtonDown(h::Sender sender, hui::KeyboardButtonEvent event) {
    if (event.physicalKey == hui::PhysicalKey::H_ESCAPE) {
        SDL_Quit();
        exit(0);
    }
    return true;
}
