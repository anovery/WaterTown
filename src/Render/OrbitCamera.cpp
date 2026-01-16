#include "OrbitCamera.h"
#include <algorithm>

namespace WaterTown {

// ========== OrthographicCamera 实现 ==========

OrthographicCamera::OrthographicCamera(float centerX, float centerZ, 
                                       float width, float height,
                                       float near, float far)
    : m_centerX(centerX), m_centerZ(centerZ), m_width(width), m_height(height),
      m_near(near), m_far(far), m_cameraHeight(20.0f),
      m_minZoom(2.0f), m_maxZoom(100.0f) {
}

glm::mat4 OrthographicCamera::getViewMatrix() const {
    // 相机位置：在目标点正上方
    glm::vec3 position(m_centerX, m_cameraHeight, m_centerZ);
    glm::vec3 target(m_centerX, 0.0f, m_centerZ);
    glm::vec3 up(0.0f, 0.0f, -1.0f);  // Z 轴负方向作为"上"
    
    return glm::lookAt(position, target, up);
}

glm::mat4 OrthographicCamera::getProjectionMatrix() const {
    float left = m_centerX - m_width / 2.0f;
    float right = m_centerX + m_width / 2.0f;
    float bottom = m_centerZ - m_height / 2.0f;
    float top = m_centerZ + m_height / 2.0f;
    
    return glm::ortho(left, right, bottom, top, m_near, m_far);
}

void OrthographicCamera::setPosition(const glm::vec3& position) {
    m_centerX = position.x;
    m_centerZ = position.z;
    m_cameraHeight = position.y;
}

glm::vec3 OrthographicCamera::getPosition() const {
    return glm::vec3(m_centerX, m_cameraHeight, m_centerZ);
}

void OrthographicCamera::setCenter(float x, float z) {
    m_centerX = x;
    m_centerZ = z;
}

void OrthographicCamera::setViewSize(float width, float height) {
    m_width = std::max(m_minZoom, std::min(width, m_maxZoom));
    m_height = std::max(m_minZoom, std::min(height, m_maxZoom));
}

void OrthographicCamera::zoom(float delta) {
    float scale = 1.0f - delta * 0.1f;
    m_width = std::max(m_minZoom, std::min(m_width * scale, m_maxZoom));
    m_height = std::max(m_minZoom, std::min(m_height * scale, m_maxZoom));
}

void OrthographicCamera::pan(float deltaX, float deltaZ) {
    m_centerX += deltaX;
    m_centerZ += deltaZ;
}

// ========== OrbitCamera 实现 ==========

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

glm::vec3 OrbitCamera::calculatePosition() const {
    float yawRad = glm::radians(m_yaw);
    float pitchRad = glm::radians(m_pitch);
    
    float x = m_distance * cos(pitchRad) * cos(yawRad);
    float y = m_distance * sin(pitchRad);
    float z = m_distance * cos(pitchRad) * sin(yawRad);
    
    return m_target + glm::vec3(x, y, z);
}

} // namespace WaterTown
