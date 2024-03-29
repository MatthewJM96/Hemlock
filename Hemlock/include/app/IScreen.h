#pragma once

namespace hemlock {
    namespace app {
        // TODO(Matthew): Allow screen to pass some amount of information to next/previous screen.
        enum class ScreenState {
            NONE,            // Screen is doing nothing.
            RUNNING,         // Screen is running and being drawn.
            CHANGE_NEXT,     // Screen wants application to move onto next screen.
            CHANGE_PREV,     // Screen wants application to move back to last screen.
            EXIT_APPLICATION // Screen wants application to quit.
        };
        
        // Not just for game screens, can also be used for UI screens with navigation back and forth.
        class IScreen {
        public:
            IScreen() : m_initialised(false), m_name(nullptr), m_next(nullptr), m_prev(nullptr) {};
            ~IScreen() {
                dispose();
            }

            virtual void init(const char* name);
            virtual void dispose(); 

            virtual void start(TimeData time);
            virtual void end(TimeData time);

            virtual void update(TimeData time) = 0;
            virtual void draw(TimeData time)   = 0;

            bool isInitialised()   const { return m_initialised; }
            bool isRunning()       const { return m_state == ScreenState::RUNNING; }
            const char* getName()  const { return m_name; }
            ScreenState getState() const { return m_state; }

            const char* getNextScreen()  const { return m_next; }
            const char* getPrevScreen()  const { return m_prev; }

            void setNextScreen(const char* name) { m_next = name; }
            void setPrevScreen(const char* name) { m_prev = name; }
        protected:
            bool        m_initialised;
            ScreenState m_state;
            const char* m_name;

            const char* m_next;
            const char* m_prev;
        };
    }
}
namespace happ = hemlock::app;
