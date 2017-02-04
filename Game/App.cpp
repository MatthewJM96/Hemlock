#include "stdafx.h"

#include "App.h"

#include "MainMenuScreen.h"


void App::init() {
    if (m_initialised) return;
    IApp::init();

    const auto& dispatcher = hui::InputDispatcher::getInstance();
    dispatcher->onMouseButtonDown += h::makeDelegate(*this, &App::handleButtonDown);
    dispatcher->onMouseMove += h::makeDelegate(*this, &App::handleMouseMovement);
    dispatcher->onMouseButtonUp += h::makeDelegate(&App::handleButtonUp);
    
    h::RPriorityEvent<i32>::PriorityCallback first = h::RPriorityEvent<i32>::PriorityCallback(2, h::makeRDelegate(*this, &App::rPriorityEventTest1));
    dispatcher->onRPriorityEventTest += first;
    dispatcher->onRPriorityEventTest += h::RPriorityEvent<i32>::PriorityCallback(4, h::makeRDelegate(*this, &App::rPriorityEventTest1));
    dispatcher->onRPriorityEventTest += h::RPriorityEvent<i32>::PriorityCallback(2, h::makeRDelegate(*this, &App::rPriorityEventTest2));
    dispatcher->onRPriorityEventTest += h::RPriorityEvent<i32>::PriorityCallback(3, h::makeRDelegate(*this, &App::rPriorityEventTest3));
    dispatcher->onRPriorityEventTest += h::RPriorityEvent<i32>::PriorityCallback(1, h::makeRDelegate(*this, &App::rPriorityEventTest4));
    dispatcher->onRPriorityEventTest += h::RPriorityEvent<i32>::PriorityCallback(2, h::makeRDelegate(*this, &App::rPriorityEventTest1));
    dispatcher->onRPriorityEventTest -= first;
    dispatcher->onRPriorityEventTest -= h::makeRDelegate(*this, &App::rPriorityEventTest1);

    dispatcher->onPriorityEventTest += h::PriorityEvent<>::PriorityCallback(1, h::makeRDelegate(*this, &App::priorityEventTest1));
    dispatcher->onPriorityEventTest += h::PriorityEvent<>::PriorityCallback(2, h::makeRDelegate(*this, &App::priorityEventTest2));
    dispatcher->onPriorityEventTest += h::PriorityEvent<>::PriorityCallback(2, h::makeRDelegate(*this, &App::priorityEventTest3));
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

void App::operator()(h::Sender sender, hui::MouseWheelScrollEvent event) {
    puts("The mouse whell was scrolled!\n");

    printf("xRel: %d   ---   yRel: %d\n", event.dx, event.dy);
}

h::RPriorityEvent<i32>::Return App::rPriorityEventTest1(h::Sender sender) {
    puts("1\n");
    return {
        true,
        1
    };
}

h::RPriorityEvent<i32>::Return App::rPriorityEventTest2(h::Sender sender) {
    puts("2");
    return {
        true,
        2
    };
}

h::RPriorityEvent<i32>::Return App::rPriorityEventTest3(h::Sender sender) {
    puts("3");
    return {
        true,
        3
    };
}

h::RPriorityEvent<i32>::Return App::rPriorityEventTest4(h::Sender sender) {
    puts("4");
    return {
        true,
        4
    };
}

h::PriorityEvent<>::Return App::priorityEventTest1(h::Sender sender) {
    puts("noret 1");
    return true;
}

h::PriorityEvent<>::Return App::priorityEventTest2(h::Sender sender) {
    puts("noret 2");
    return false;
}

h::PriorityEvent<>::Return App::priorityEventTest3(h::Sender sender) {
    puts("noret 3");
    return true;
}
