#include "stdafx.h"

#include "camera/BasicFPSCamera.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

void hcam::BasicFPSCamera::update() {
    if (m_viewChanged) {
        m_viewMatrix = glm::lookAt(m_position, m_position + m_direction, m_up);
    }
    if (m_projectionChanged) {
        m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_nearClipping, m_farClipping);
    }

    if (m_viewChanged || m_projectionChanged) {
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

    m_viewChanged = false;
    m_projectionChanged = false;
}

void hcam::BasicFPSCamera::applyRotation(glm::f32quat quaternion) {
    m_direction = glm::normalize(quaternion * m_direction);
    m_right = glm::normalize(glm::cross(m_direction, WORLD_UP));
    m_up = glm::normalize(glm::cross(m_right, m_direction));

    m_viewChanged = true;
}

void hcam::BasicFPSCamera::rotateFromMouse(f32 dx, f32 dy, f32 speed) {
    glm::f32quat up = glm::angleAxis(dy * speed, m_right);
    glm::f32quat right = glm::angleAxis(dx * speed, m_up);
    
    glm::f32quat tot = up * right;

    glm::vec3 eulerAngles = glm::eulerAngles(tot);
    m_pitch += eulerAngles.x;
    m_yaw   += eulerAngles.y;
    m_roll  += eulerAngles.z;
    if (m_pitch > 89.0f) {
        f32 diff = m_pitch - 89.0f;
        tot *= glm::angleAxis(-diff, m_right);
    } else if (m_pitch < -89.0f) {
        f32 diff = (-1.0f * m_pitch) - 89.0f;
        tot *= glm::angleAxis(diff, m_right);
    }

    applyRotation(up * right);
}

void hcam::BasicFPSCamera::rollFromMouse(f32 dx, f32 speed) {
    glm::f32quat forward = glm::angleAxis(dx * speed, m_direction);

    applyRotation(forward);
}
