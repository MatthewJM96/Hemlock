#pragma once

#include "Timing.h"
#include "app\IScreen.h"
#include "graphics\Window.h"
#include "ui\InputManager.h"

namespace hemlock {
    namespace app {
        class IApp {
        protected:
            typedef std::unordered_map<char*, IScreen*> ScreenList;
            typedef std::pair<char*, IScreen*> Screen;
        public:
            IApp() : m_window(nullptr), m_currentScreen(nullptr) {};
            ~IApp() {};

            hg::Window* getWindow() const { return m_window; }

            virtual void init();
            virtual void dispose();

            virtual void run();

            bool setCurrentScreen(char* name);

            virtual void quit();
        protected:      
            virtual void prepareScreens() = 0;

            void addScreens(ScreenList screens);
            void addScreens(Screen* screens, ui32 count);
            void addScreen(Screen screen);

            IScreen* getCurrentScreen() { return m_currentScreen; }

            bool handleCurrentScreenRequests();

            void moveToNextScreen();
            void moveToPrevScreen();

            void disposeScreens();

            void calculateNewTimes();

            bool               m_initialised = false;

            hg::Window*        m_window;
            hui::InputManager* m_inputManager;
            h::FpsLimiter*     m_fpsLimiter;

            TimeData           m_currentTimes;
            TimeData           m_previousTimes;

            bool               m_shouldQuit = false;

            ScreenList         m_screens;
            IScreen*           m_currentScreen;
        };
    }
}
namespace happ = hemlock::app;
