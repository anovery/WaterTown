#include "OrthographicCamera.h"
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

void OrthographicCamera::setHeight(float height) {
    m_cameraHeight = height;
    // 确保远裁剪面覆盖到地面，避免相机抬升后看不到地形
    float requiredFar = height + 50.0f;
    if (requiredFar > m_far) {
        m_far = requiredFar;
    }
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

} // namespace WaterTown
