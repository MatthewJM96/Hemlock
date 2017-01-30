#pragma once

#ifndef HEMLOCK_USING_PCH
#include "types.h"
#endif

namespace hemlock {
    namespace graphics {
        enum class SwapInterval {
            UNCAPPED = 0,
            V_SYNC = 1,
            CAPPED = 2
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
        struct WindowSettings {
            char* name;
            WindowDimensions dimensions;
            ui8 screenIndex;
            bool fullscreen;
            bool resizable;
            bool borderless;
            SwapInterval swapInterval;
        };

        struct OnResizeEvent {
            WindowDimensions before;
            WindowDimensions now;
        };

        const WindowSettings DEFAULT_WINDOW_SETTINGS = { "Hemlock Window", {800, 600}, 0, false, false, false, SwapInterval::V_SYNC };

        class Window {
        public:
            Window(WindowSettings settings) : m_settings(settings), m_window(nullptr), m_aspectRatioString(nullptr) {
                init();
            }
            Window() : Window(DEFAULT_WINDOW_SETTINGS) {}
            Window(char* name, ui32 width, ui32 height, ui8 screenIndex = DEFAULT_WINDOW_SETTINGS.screenIndex, bool fullscreen = DEFAULT_WINDOW_SETTINGS.fullscreen,
                    bool resizable = DEFAULT_WINDOW_SETTINGS.resizable, bool borderless = DEFAULT_WINDOW_SETTINGS.borderless,
                    SwapInterval swapInterval = DEFAULT_WINDOW_SETTINGS.swapInterval)
                : Window(WindowSettings{ name, {width, height}, screenIndex, fullscreen, resizable, borderless, swapInterval }) {}
            ~Window() {
                SDL_GL_DeleteContext(m_context);
                SDL_DestroyWindow(m_window);
                m_context = NULL;
                m_window  = nullptr;
            }
            
            char* getName() {
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
                return getWidth() / getHeight();
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
            void setSwapInterval(SwapInterval swapInterval);

            void sync();

            Event<> onFullscreen;
            Event<> onExitFullscreen;
            Event<OnResizeEvent> onResize;
        private:
            void init();
            void getAllowedDisplayResolutions();
            void calculateAspectRatio();

            WindowSettings m_settings;
            char* m_aspectRatioString;
            SDL_Window* m_window;
            SDL_GLContext m_context;
            std::map<ui8, std::vector<WindowDimensions>> m_allowedResolutions;
        };
    }
}
namespace hg = hemlock::graphics;
