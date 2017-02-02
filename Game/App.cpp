#include "stdafx.h"

#include "App.h"

#include "MainMenuScreen.h"

void App::prepareScreens() {
    Screen screen;
    screen.first = "main_menu";
    screen.second = new MainMenuScreen();
    addScreen(screen);

    setCurrentScreen(screen.first);
}
