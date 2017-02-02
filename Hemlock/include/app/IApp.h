#pragma once

namespace hemlock {
    namespace app {
        class IApp {
            typedef std::unordered_map<char*, IScreen*> ScreenList;
            typedef std::pair<char*, IScreen*> Screen;
        public:
            IApp() : m_window(nullptr), m_currentScreen(nullptr) {};
            ~IApp() {};

            hg::Window* getWindow() const { return m_window; }

            virtual void init();
            virtual void dispose();

            virtual void run();

            virtual void quit();
        protected:      
            virtual void prepareScreens() = 0;

            void addScreens(ScreenList screens);
            void addScreens(Screen* screens, ui32 count);
            void addScreen(Screen screen);

            IScreen* getCurrentScreen() { return m_currentScreen; }

            void handleScreenChange();

            IScreen* moveToNextScreen();
            IScreen* moveToPrevScreen();

            void disposeScreens();

            hg::Window* m_window;
            hui::InputManager* m_inputManager;

            ScreenList m_screens;
            IScreen* m_currentScreen;
        };
    }
}
namespace happ = hemlock::app;
