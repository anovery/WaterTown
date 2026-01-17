#include "FollowCamera.h"

namespace WaterTown {

FollowCamera::FollowCamera(float fov, float aspectRatio)
    : m_position(0.0f), m_targetPos(0.0f), m_targetRotation(0.0f),
      m_offset(0.0f, 12.0f, -25.0f),  // 默认：后上方
      m_smoothSpeed(5.0f),
      m_yawOffset(0.0f), m_pitchOffset(0.0f), // 初始化
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
    glm::vec3 desiredPosition = getDesiredPosition();
    
    // 平滑插值
    float t = 1.0f - exp(-m_smoothSpeed * deltaTime);
    m_position = glm::mix(m_position, desiredPosition, t);
}

void FollowCamera::rotate(float deltaYaw, float deltaPitch) {
    m_yawOffset += deltaYaw * 0.3f;
    m_pitchOffset += deltaPitch * 0.3f;
    
    // 限制俯仰角，避免穿地或视角过高
    // m_pitchOffset 对应的是额外的垂直视角变化
    // m_offset 本身有一定的俯仰，所以这里限制不要太过分
    if (m_pitchOffset > 45.0f) m_pitchOffset = 45.0f;
    if (m_pitchOffset < -10.0f) m_pitchOffset = -10.0f;
}

glm::vec3 FollowCamera::getDesiredPosition() const {
    // 1. 计算基于目标朝向的基础旋转
    float baseRotRad = glm::radians(m_targetRotation);
    glm::mat4 baseRotation = glm::rotate(glm::mat4(1.0f), baseRotRad, glm::vec3(0.0f, 1.0f, 0.0f));
    
    // 2. 计算用户控制的额外旋转 (围绕目标)
    // 这是一个球坐标旋转 offset。
    // 为了简单，我们只旋转 offset 矢量。
    
    // 使用 rotate 矩阵叠加
    float yawRad = glm::radians(m_yawOffset);
    float pitchRad = glm::radians(m_pitchOffset);
    
    glm::mat4 camRotation = glm::rotate(glm::mat4(1.0f), yawRad, glm::vec3(0.0f, 1.0f, 0.0f));
    camRotation = glm::rotate(camRotation, pitchRad, glm::vec3(1.0f, 0.0f, 0.0f));
    
    // 组合旋转：先应用相机的额外旋转，再应用船的基本朝向
    glm::mat4 finalRotation = baseRotation * camRotation;
    
    glm::vec3 worldOffset = glm::vec3(finalRotation * glm::vec4(m_offset, 0.0f));
    
    return m_targetPos + worldOffset;
}

} // namespace WaterTown
