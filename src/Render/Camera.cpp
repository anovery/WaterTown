#include "Camera.h"
#include <GLFW/glfw3.h>
#include <algorithm>

namespace WaterTown {

FreeCamera::FreeCamera(glm::vec3 position, float fov, float aspectRatio)
    : m_position(position),
      m_worldUp(0.0f, 1.0f, 0.0f),
      m_yaw(-90.0f),      // 初始朝向 -Z 方向
      m_pitch(0.0f),
      m_fov(fov),
      m_aspectRatio(aspectRatio),
      m_nearPlane(0.1f),
      m_farPlane(100.0f),
      m_moveSpeed(2.5f),
      m_mouseSensitivity(0.1f) {
    
    updateCameraVectors();
}

void FreeCamera::processKeyboard(int key, float deltaTime) {
    float velocity = m_moveSpeed * deltaTime;
    
    if (key == GLFW_KEY_W)
        m_position += m_front * velocity;
    if (key == GLFW_KEY_S)
        m_position -= m_front * velocity;
    if (key == GLFW_KEY_A)
        m_position -= m_right * velocity;
    if (key == GLFW_KEY_D)
        m_position += m_right * velocity;
    if (key == GLFW_KEY_SPACE)
        m_position += m_worldUp * velocity;
    if (key == GLFW_KEY_LEFT_SHIFT)
        m_position -= m_worldUp * velocity;
}

void FreeCamera::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;
    
    m_yaw += xoffset;
    m_pitch += yoffset;
    
    // 限制俯仰角，避免翻转
    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;
    
    updateCameraVectors();
}

void FreeCamera::processMouseScroll(float yoffset) {
    m_fov -= yoffset;
    
    // 限制 FOV 范围
    if (m_fov < 1.0f)
        m_fov = 1.0f;
    if (m_fov > 45.0f)
        m_fov = 45.0f;
}

glm::mat4 FreeCamera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 FreeCamera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
}

void FreeCamera::setPosition(const glm::vec3& position) {
    m_position = position;
}

glm::vec3 FreeCamera::getPosition() const {
    return m_position;
}

void FreeCamera::updateAspectRatio(float aspectRatio) {
    m_aspectRatio = aspectRatio;
}

void FreeCamera::updateCameraVectors() {
    // 计算新的前向量
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    
    // 重新计算右向量和上向量
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

} // namespace WaterTown
