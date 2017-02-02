#pragma once

namespace hemlock {
    namespace app {
        class IApp {
            typedef std::unordered_map<char*, IScreen*> ScreenList;
            typedef std::pair<char*, IScreen*> Screen;
        public:
            IApp() : m_window(nullptr), m_currentScreen(nullptr) {};
            ~IApp() {};
        protected:
            virtual void init() = 0;
            virtual void dispose();

            virtual void run() = 0;
            virtual void quit();
            
            void addScreens(ScreenList screens);
            void addScreens(Screen* screens, ui32 count);
            void addScreen(Screen screen);

            IScreen* getCurrentScreen() { return m_currentScreen; }

            IScreen* moveToNextScreen();
            IScreen* moveToPrevScreen();

            void disposeScreens();

            hg::Window* m_window;

            ScreenList m_screens;
            IScreen* m_currentScreen;
        };
    }
}
namespace happ = hemlock::app;
