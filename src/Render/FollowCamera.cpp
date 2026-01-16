#include "FollowCamera.h"

namespace WaterTown {

FollowCamera::FollowCamera(float fov, float aspectRatio)
    : m_position(0.0f), m_targetPos(0.0f), m_targetRotation(0.0f),
      m_offset(0.0f, 2.5f, -5.0f),  // 默认：后上方
      m_smoothSpeed(5.0f),
      m_fov(fov), m_aspectRatio(aspectRatio),
      m_near(0.1f), m_far(1000.0f) {
}

glm::mat4 FollowCamera::getViewMatrix() const {
    // 相机看向目标点
    return glm::lookAt(m_position, m_targetPos, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 FollowCamera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_near, m_far);
}

void FollowCamera::setPosition(const glm::vec3& position) {
    m_position = position;
}

glm::vec3 FollowCamera::getPosition() const {
    return m_position;
}

void FollowCamera::setTarget(const glm::vec3& targetPosition, float targetRotation) {
    m_targetPos = targetPosition;
    m_targetRotation = targetRotation;
}

void FollowCamera::update(float deltaTime) {
    // 计算期望位置
    glm::vec3 desiredPosition = calculateDesiredPosition();
    
    // 平滑插值
    float t = 1.0f - exp(-m_smoothSpeed * deltaTime);
    m_position = glm::mix(m_position, desiredPosition, t);
}

glm::vec3 FollowCamera::calculateDesiredPosition() const {
    // 将偏移量从局部坐标转换到世界坐标
    float rotRad = glm::radians(m_targetRotation);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotRad, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 worldOffset = glm::vec3(rotation * glm::vec4(m_offset, 0.0f));
    
    return m_targetPos + worldOffset;
}

} // namespace WaterTown
