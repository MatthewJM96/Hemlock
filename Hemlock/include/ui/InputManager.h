#pragma once

namespace hemlock {
    namespace ui {
        enum class MouseButton {
            LEFT,
            RIGHT,
            MIDDLE,
            UNKOWN
        };

        class InputManager {
        public:
            void dispose() {}
        };
    }
}
namespace hui = hemlock::ui;
