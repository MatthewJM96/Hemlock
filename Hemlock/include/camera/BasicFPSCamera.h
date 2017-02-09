#pragma once

#include "camera\BasicCamera.h"

namespace hemlock {
    namespace camera {
        class BasicFPSCamera : public BasicCamera {
        public:
            BasicFPSCamera() {};
            ~BasicFPSCamera() {};

            virtual void update();
            
            virtual void applyRotation(glm::f32quat quaternion);
            virtual void rotateFromMouse(f32 dx, f32 dy, f32 speed);
            virtual void rollFromMouse(f32 dx, f32 speed);
        protected:
            const glm::f32vec3 WORLD_UP = glm::f32vec3(0.0f, 0.0f, 1.0f);

            f32 m_pitch;
            f32 m_yaw;
            f32 m_roll;
        };
    }
}
namespace hcam = hemlock::camera;
