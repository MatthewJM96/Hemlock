#pragma once

#ifndef HEMLOCK_USING_PCH
#include "types.h"
#include <glm/glm.hpp>
#include "Event.hpp"
#endif

#include "graphics/Window.h"

namespace hemlock {
    namespace camera {
        class BasicCamera {
        public:
            BasicCamera() {};
            ~BasicCamera() {};
            
            virtual void attachToWindow(hg::Window* window);

            virtual void update();

            virtual void setAspectRatio(f32 aspectRatio);
            virtual void setFov(f32 fov);
            virtual void setNearClipping(f32 nearClipping);
            virtual void setFarClipping(f32 farClipping);
            virtual void setClipping(f32 nearClipping, f32 farClipping);
            virtual void setPosition(glm::f32vec3 position);

            virtual void offsetPosition(glm::f32vec3 offsets);
            virtual void offsetPosition(f32 xOff, f32 yOff, f32 zOff);

            virtual void applyRotation(glm::f32quat quaternion);
            virtual void rotateFromMouse(f32 dx, f32 dy, f32 speed);
            virtual void rollFromMouse(f32 dx, f32 speed);

            virtual f32          getAspectRatio()          const { return m_aspectRatio; };
            virtual f32          getFov()                  const { return m_fov; };
            virtual f32          getNearClipping()         const { return m_nearClipping; };
            virtual f32          getFarClipping()          const { return m_farClipping; };
            virtual glm::f32vec3 getPosition()             const { return m_position; };
            virtual glm::f32vec3 getDirection()            const { return m_direction; };
            virtual glm::f32vec3 getRight()                const { return m_right; };
            virtual glm::f32vec3 getUp()                   const { return m_up; };
            virtual glm::f32mat4 getProjectionMatrix()     const { return m_projectionMatrix; };
            virtual glm::f32mat4 getViewMatrix()           const { return m_viewMatrix; };
            virtual glm::f32mat4 getViewProjectionMatrix() const { return m_viewProjectionMatrix; };
        protected:
            virtual void handleWindowResize(h::Sender window, hg::ResizeEvent event);

            f32 m_aspectRatio        = 4.0f/3.0f;
            f32 m_fov                = 90.0f;
            f32 m_nearClipping       = 0.1f;
            f32 m_farClipping        = 10000.0f;

            glm::f32vec3 m_position  = glm::f32vec3(0.0f);
            glm::f32vec3 m_direction = glm::f32vec3(1.0f, 0.0f, 0.0f);
            glm::f32vec3 m_right     = glm::f32vec3(0.0f, 0.0f, 1.0f);
            glm::f32vec3 m_up        = glm::f32vec3(0.0f, 1.0f, 0.0f);

            glm::f32mat4 m_viewMatrix;
            glm::f32mat4 m_projectionMatrix;
            glm::f32mat4 m_viewProjectionMatrix;

            bool m_viewChanged = true;
            bool m_projectionChanged = true;
        };
    }
}
namespace hcam = hemlock::camera;
