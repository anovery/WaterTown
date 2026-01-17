#include "OrbitCamera.h"
#include <algorithm>

namespace WaterTown {

OrbitCamera::OrbitCamera(glm::vec3 target, float distance, float fov, float aspectRatio)
    : m_target(target), m_distance(distance), m_fov(fov), m_aspectRatio(aspectRatio),
      m_yaw(45.0f), m_pitch(30.0f),
      m_near(0.1f), m_far(1000.0f),
      m_minDistance(2.0f), m_maxDistance(50.0f),
      m_minPitch(5.0f), m_maxPitch(85.0f) {
}

glm::mat4 OrbitCamera::getViewMatrix() const {
    glm::vec3 position = calculatePosition();
    return glm::lookAt(position, m_target, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 OrbitCamera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_near, m_far);
}

void OrbitCamera::setPosition(const glm::vec3& position) {
    // 根据位置计算球坐标
    glm::vec3 dir = position - m_target;
    m_distance = glm::length(dir);
    
    if (m_distance > 0.0f) {
        dir = glm::normalize(dir);
        m_pitch = glm::degrees(asin(dir.y));
        m_yaw = glm::degrees(atan2(dir.z, dir.x));
    }
}

glm::vec3 OrbitCamera::getPosition() const {
    return calculatePosition();
}

void OrbitCamera::rotate(float deltaYaw, float deltaPitch) {
    m_yaw += deltaYaw * 0.3f;      // 降低灵敏度
    m_pitch += deltaPitch * 0.3f;
    
    // 限制俯仰角
    m_pitch = std::max(m_minPitch, std::min(m_pitch, m_maxPitch));
    
    // 保持偏航角在 [0, 360) 范围内
    if (m_yaw < 0.0f) m_yaw += 360.0f;
    if (m_yaw >= 360.0f) m_yaw -= 360.0f;
}

void OrbitCamera::zoom(float delta) {
    m_distance -= delta;
    m_distance = std::max(m_minDistance, std::min(m_distance, m_maxDistance));
}

void OrbitCamera::panTarget(float deltaX, float deltaY, float deltaZ) {
    // 根据相机方向调整平移
    float yawRad = glm::radians(m_yaw);
    glm::vec3 right(cos(yawRad), 0.0f, sin(yawRad));
    glm::vec3 forward(-sin(yawRad), 0.0f, cos(yawRad));
    
    m_target += right * deltaX + glm::vec3(0.0f, deltaY, 0.0f) + forward * deltaZ;
}

void OrbitCamera::setDistance(float distance) {
    m_distance = std::max(m_minDistance, std::min(distance, m_maxDistance));
}

void OrbitCamera::setAngles(float yawDegrees, float pitchDegrees) {
    m_yaw = yawDegrees;
    if (m_yaw < 0.0f) m_yaw += 360.0f;
    if (m_yaw >= 360.0f) m_yaw -= 360.0f;
    m_pitch = std::max(m_minPitch, std::min(pitchDegrees, m_maxPitch));
}

glm::vec3 OrbitCamera::calculatePosition() const {
    float yawRad = glm::radians(m_yaw);
    float pitchRad = glm::radians(m_pitch);
    
    float x = m_distance * cos(pitchRad) * cos(yawRad);
    float y = m_distance * sin(pitchRad);
    float z = m_distance * cos(pitchRad) * sin(yawRad);
    
    return m_target + glm::vec3(x, y, z);
}

} // namespace WaterTown
