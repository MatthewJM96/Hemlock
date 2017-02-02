#include "stdafx.h"

#include "app\IScreen.h"
#include "graphics\Window.h"
#include "ui\InputDispatcher.h"
#include "ui\InputManager.h"

#include "app\IApp.h"

void happ::IApp::init() {
    m_window = new hg::Window();
    m_inputManager = new hui::InputManager();
    hui::InputDispatcher::getInstance()->init(m_window, m_inputManager);

    prepareScreens();
}

void happ::IApp::dispose() {
    disposeScreens();
    m_currentScreen = nullptr;

    m_window->dispose();
    delete m_window;
    m_window = nullptr;

    m_inputManager->dispose();
    delete m_inputManager;
    m_inputManager = nullptr;
}

void happ::IApp::quit() {
    m_currentScreen->end();
    dispose();
}

void happ::IApp::addScreens(ScreenList screens) {
    for (auto& screen : screens) {
        addScreen(screen);
    }
}

void happ::IApp::addScreens(Screen* screens, ui32 count) {
    for (ui32 i = 0; i < count; ++i) {
        addScreen(screens[i]);
    }
}

void happ::IApp::addScreen(Screen screen) {
    m_screens.insert(screen);
    screen.second->init(screen.first);
}

happ::IScreen* happ::IApp::moveToNextScreen() {
    if (m_currentScreen == nullptr) return nullptr;

    char* nextScreenName = m_currentScreen->getNextScreen();
    if (nextScreenName == nullptr) return nullptr;

    auto& it = m_screens.find(nextScreenName);
    if (it == m_screens.end()) return nullptr;

    IScreen* temp = m_currentScreen;
    m_currentScreen = (*it).second;
    m_currentScreen->setPrevScreen(temp->getName());
    return m_currentScreen;
}

happ::IScreen* happ::IApp::moveToPrevScreen() {
    if (m_currentScreen == nullptr) return nullptr;

    char* prevScreenName = m_currentScreen->getPrevScreen();
    if (prevScreenName == nullptr) return nullptr;

    auto& it = m_screens.find(prevScreenName);
    if (it == m_screens.end()) return nullptr;

    IScreen* temp = m_currentScreen;
    m_currentScreen = (*it).second;
    m_currentScreen->setNextScreen(temp->getName());
    return m_currentScreen;
}

void happ::IApp::disposeScreens() {
    for (auto& screen : m_screens) {
        screen.second->dispose();
        delete screen.first;
    }
    ScreenList().swap(m_screens);
}
