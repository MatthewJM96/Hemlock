#pragma once

#ifndef HEMLOCK_USING_PCH
#include "types.h"
#endif

namespace hemlock {
    namespace ui {
        class InputDispatcher;
    }

    namespace graphics {
        // Events:
        //   WindowClose
        //   WindowResize
        //   WindowMinimised
        //   WindowMaximised
        //   WindowFullscreen
        //   WindowFullScreenExit

        enum class WindowError {
            NONE = 1,
            SDL_WINDOW = -1,
            SDL_GL_CONTEXT = -2,
            GLEW_INITIALISATION = -3
        };

        struct WindowDimensions {
            ui32 width, height;
            bool operator==(const WindowDimensions& rhs) {
                return this->width == rhs.width && this->height == rhs.height;
            }
            bool operator!=(const WindowDimensions& rhs) {
                return !(this->width == rhs.width && this->height == rhs.height);
            }
        };

        enum class SwapInterval {
            UNCAPPED = 0,
            V_SYNC = 1,
            CAPPED = 2
        };

        struct WindowSettings {
            const char* name;
            WindowDimensions dimensions;
            ui8 screenIndex;
            bool fullscreen;
            bool resizable;
            bool borderless;
            bool maximised;
            SwapInterval swapInterval;
        };

        struct ResizeEvent {
            WindowDimensions before;
            WindowDimensions now;
        };

        const WindowSettings DEFAULT_WINDOW_SETTINGS = { "Hemlock Window", {800, 600}, 0, false, true, false, false, SwapInterval::V_SYNC };
        
        class Window {
            friend class hemlock::ui::InputDispatcher;
            typedef std::map<ui8, std::vector<WindowDimensions>> WindowDimensionMap;
        public:
            Window(WindowSettings settings) : m_initialised(false), m_window(nullptr), m_settings(settings), m_aspectRatioString(nullptr) {}
            Window() : Window(DEFAULT_WINDOW_SETTINGS) {}
            Window(char* name, ui32 width, ui32 height, ui8 screenIndex = DEFAULT_WINDOW_SETTINGS.screenIndex, bool fullscreen = DEFAULT_WINDOW_SETTINGS.fullscreen,
                    bool resizable = DEFAULT_WINDOW_SETTINGS.resizable, bool borderless = DEFAULT_WINDOW_SETTINGS.borderless, bool maximised = DEFAULT_WINDOW_SETTINGS.maximised,
                    SwapInterval swapInterval = DEFAULT_WINDOW_SETTINGS.swapInterval)
                : Window(WindowSettings{ name, {width, height}, screenIndex, fullscreen, resizable, borderless, maximised, swapInterval }) {}
            ~Window() {
                dispose();
            }

            WindowError init();
            void dispose();
            
            const char* getName() {
                return m_settings.name;
            }
            WindowDimensions getDimensions() {
                return m_settings.dimensions;
            }
            ui32 getWidth() {
                return m_settings.dimensions.width;
            }
            ui32 getHeight() {
                return m_settings.dimensions.height;
            }
            char* getAspectRatioAsString() {
                return m_aspectRatioString;
            }
            f32 getAspectRatio() {
                return (f32)getWidth() / (f32)getHeight();
            }
            bool isFullscreen() {
                return m_settings.fullscreen;
            }
            bool isResizable() {
                return m_settings.resizable;
            }
            bool isBorderless() {
                return m_settings.borderless;
            }
            bool isMaximised() {
                return m_settings.maximised;
            }
            SwapInterval getSwapInterval() {
                return m_settings.swapInterval;
            }

            void setName(char* name);
            void setDimensions(WindowDimensions dimensions);
            void setWidth(ui32 width);
            void setHeight(ui32 height);
            void setFullscreen(bool fullscreen);
            void setResizable(bool resizable);
            void setBorderless(bool borderless);
            void setMaximised(bool maximised);
            void setSwapInterval(SwapInterval swapInterval);

            void requestClose();

            void sync();

            Event<>            onWindowClose;
            Event<>            onWindowMinimised;
            Event<>            onWindowMaximised;
            Event<>            onWindowFullscreen;
            Event<>            onWindowFullscreenExit;
            Event<ResizeEvent> onWindowResize;
        protected:
            void windowDimensionsHaveChanged(ui32 width, ui32 height);
        private:
            void getAllowedDisplayResolutions();
            void calculateAspectRatio();

            bool                                         m_initialised;

            SDL_Window*                                  m_window;
            SDL_GLContext                                m_context;

            WindowSettings                               m_settings;
            char*                                        m_aspectRatioString;
            WindowDimensionMap                           m_allowedResolutions;
        };
    }
}
namespace hg = hemlock::graphics;
