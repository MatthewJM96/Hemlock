#include "stdafx.h"

#include "ui/InputDispatcher.h"

#include "app/IApp.h"

void happ::IApp::init() {
    if (m_initialised) return;
    m_initialised = true;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_window = new hg::Window();
    if (m_window->init() != hg::WindowError::NONE) {
        puts("Window could not be initialised...\n");
        exit(1);
    }
    m_inputManager = new hui::InputManager();
    hui::InputDispatcher::getInstance()->init(m_window, m_inputManager);

    ilutRenderer(ILUT_OPENGL);
    ilutEnable(ILUT_OPENGL_CONV); // TODO(Matthew): Make this optional, some projects may consider on-board texture conversions fine.
    
    m_currentTimes = {};
    m_previousTimes = {};

    m_fpsLimiter = new FpsLimiter();
    m_fpsLimiter->init(60.0); // TODO(Matthew): Get max FPS from user preferences.

    prepareScreens();
}

void happ::IApp::dispose() {
    if (!m_initialised) return;
    m_initialised = false;

    disposeScreens();
    m_currentScreen = nullptr;

    m_window->dispose();
    delete m_window;
    m_window = nullptr;

    m_inputManager->dispose();
    delete m_inputManager;
    m_inputManager = nullptr;
}

void happ::IApp::run() {
    init();

    while (!m_shouldQuit) {
        m_fpsLimiter->begin();

        calculateNewTimes();

        SDL_PumpEvents();

        handleCurrentScreenRequests();

        m_currentScreen->update(m_currentTimes);

        if (!handleCurrentScreenRequests()) {
            m_currentScreen->draw(m_currentTimes);
        }

        m_window->sync();
        
        m_fpsLimiter->end();
    }
}

bool happ::IApp::setCurrentScreen(const char* name) {
    auto it = m_screens.find(name);
    if (it == m_screens.end()) return false;

    if (m_currentScreen != nullptr) {
        m_currentScreen->setNextScreen((*it).first);
        IScreen* temp = m_currentScreen;
        ((*it).second)->setPrevScreen(temp->getName());
    }
    m_currentScreen = (*it).second;
    return true;
}

void happ::IApp::quit() {
    m_currentScreen->end(m_previousTimes);
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

bool happ::IApp::handleCurrentScreenRequests() {
    typedef happ::ScreenState SState;
    SState state = m_currentScreen->getState();
    switch (state) {
        case SState::CHANGE_NEXT:
            m_currentScreen->end(m_currentTimes);
            moveToNextScreen();
            if (m_currentScreen != nullptr) {
                m_currentScreen->start(m_currentTimes);
            }
            break;
        case SState::CHANGE_PREV:
            m_currentScreen->end(m_currentTimes);
            moveToPrevScreen();
            if (m_currentScreen != nullptr) {
                m_currentScreen->start(m_currentTimes);
            }
            break;
        case SState::EXIT_APPLICATION:
            m_shouldQuit = true;
            break;
        default:
            return false;
    }
    return true;
}

void happ::IApp::moveToNextScreen() {
    if (m_currentScreen == nullptr) return;

    const char* nextScreenName = m_currentScreen->getNextScreen();
    if (nextScreenName == nullptr) m_currentScreen = nullptr;

    auto it = m_screens.find(nextScreenName);
    if (it == m_screens.end()) {
        m_currentScreen->setNextScreen(nullptr);
        m_currentScreen = nullptr; // TODO(Matthew): Instead of nullptr, maybe a default object that at least linked back to previous screen?
        return;
    }
    
    IScreen* temp = m_currentScreen;
    m_currentScreen = (*it).second;
    m_currentScreen->setPrevScreen(temp->getName());
}

void happ::IApp::moveToPrevScreen() {
    if (m_currentScreen == nullptr) return;

    const char* prevScreenName = m_currentScreen->getPrevScreen();
    if (prevScreenName == nullptr) m_currentScreen = nullptr;

    auto it = m_screens.find(prevScreenName);
    if (it == m_screens.end()) {
        m_currentScreen->setPrevScreen(nullptr);
        m_currentScreen = nullptr; // TODO(Matthew): Instead of nullptr, maybe a default object that at least linked back to "next" screen?
        return;
    }

    IScreen* temp = m_currentScreen;
    m_currentScreen = (*it).second;
    m_currentScreen->setNextScreen(temp->getName());
}

void happ::IApp::disposeScreens() {
    for (auto& screen : m_screens) {
        screen.second->dispose();
        delete screen.first;
    }
    ScreenList().swap(m_screens);
}

void happ::IApp::calculateNewTimes() {
    m_previousTimes = m_currentTimes;
    m_currentTimes.frame = m_fpsLimiter->getFrameTime();
    m_currentTimes.total += m_currentTimes.frame;
}
