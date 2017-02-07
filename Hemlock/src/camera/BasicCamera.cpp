#include "stdafx.h"

#include "camera\BasicCamera.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

void hcam::BasicCamera::attachToWindow(hg::Window* window) {
    window->onWindowResize += makeDelegate(*this, &hcam::BasicCamera::handleWindowResize);
}

void hcam::BasicCamera::update() {
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

void hcam::BasicCamera::setAspectRatio(f32 aspectRatio) {
    m_aspectRatio = aspectRatio;
    m_projectionChanged = true;
}

void hcam::BasicCamera::setFov(f32 fov) {
    m_fov = fov;
    m_projectionChanged = true;
}

void hcam::BasicCamera::setNearClipping(f32 nearClipping) {
    m_nearClipping = nearClipping;
    m_projectionChanged = true;
}

void hcam::BasicCamera::setFarClipping(f32 farClipping) {
    m_farClipping = farClipping;
    m_projectionChanged = true;
}

void hcam::BasicCamera::setClipping(f32 nearClipping, f32 farClipping) {
    m_nearClipping = nearClipping;
    m_farClipping = farClipping;
    m_projectionChanged = true;
}

void hcam::BasicCamera::setPosition(glm::f32vec3 position) {
    m_position = position;
    m_viewChanged = true;
}

void hcam::BasicCamera::offsetPosition(glm::f32vec3 offsets) {
    m_position += offsets;
    m_viewChanged = true;
}

void hcam::BasicCamera::offsetPosition(f32 xOff, f32 yOff, f32 zOff) {
    offsetPosition(glm::f32vec3(xOff, yOff, zOff));
}

void hcam::BasicCamera::applyRotation(glm::f32quat quaternion) {
    m_direction = glm::normalize(quaternion * m_direction);
    m_right     = glm::normalize(quaternion * m_right);
    m_up        = glm::normalize(glm::cross(m_right, m_direction));
    //m_right = glm::normalize(glm::cross(m_direction, glm::vec3(0.0f, 1.0f, 0.0f)));

    m_viewChanged = true;
}

void hcam::BasicCamera::rotateFromMouse(f32 dx, f32 dy, f32 speed) {
    glm::f32quat up = glm::angleAxis(dy * speed, m_right);
    glm::f32quat right = glm::angleAxis(dx * speed, m_up);

    applyRotation(up * right);
}

void hcam::BasicCamera::rollFromMouse(f32 dx, f32 speed) {
    glm::f32quat forward = glm::angleAxis(dx * speed, m_direction);

    applyRotation(forward);
}

void hcam::BasicCamera::handleWindowResize(h::Sender window, hg::ResizeEvent event) {
    f32 newAspectRatio = (f32)event.now.width / (f32)event.now.height;
    if (getAspectRatio() != newAspectRatio) {
        setAspectRatio(newAspectRatio);
    }
}
