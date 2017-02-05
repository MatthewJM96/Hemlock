#include "stdafx.h"

#include "App.h"

#include "MainMenuScreen.h"


void App::init() {
    if (m_initialised) return;
    IApp::init();

    m_window->setDimensions({ 640, 320 });

    const auto& dispatcher = hui::InputDispatcher::getInstance();
    dispatcher->onMouseButtonDown += h::makeDelegate(*this, &App::handleButtonDown);
    dispatcher->onMouseMove += h::makeDelegate(*this, &App::handleMouseMovement);
    dispatcher->onMouseButtonUp += h::makeDelegate(&App::handleButtonUp);
}

void App::prepareScreens() {
    Screen screen;
    screen.first = "main_menu";
    screen.second = new MainMenuScreen();
    addScreen(screen);

    setCurrentScreen(screen.first);
}

void App::handleButtonDown(h::Sender sender, hui::MouseButtonEvent event) {
    puts("A button was pressed!\n");

    char* bType;
    switch (event.button) {
        case hui::MouseButton::LEFT:
            bType = "left";
            break;
        case hui::MouseButton::RIGHT:
            bType = "right";
            break;
        case hui::MouseButton::MIDDLE:
            bType = "middle";
            break;
        default:
            bType = "unknown";
    }

    printf("Button type was %s.\n", bType);
}

void App::handleMouseMovement(h::Sender sender, hui::MouseMoveEvent event) const {
    printf("xRel: %d   ---   yRel: %d\n", event.dx, event.dy);
}

void App::handleButtonUp(h::Sender sender, hui::MouseButtonEvent event) {
    puts("A button was released!\n");

    char* bType;
    switch (event.button) {
        case hui::MouseButton::LEFT:
            bType = "left";
            break;
        case hui::MouseButton::RIGHT:
            bType = "right";
            break;
        case hui::MouseButton::MIDDLE:
            bType = "middle";
            break;
        default:
            bType = "unknown";
    }

    printf("Button type was %s.\n", bType);
}
