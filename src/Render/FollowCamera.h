#pragma once

#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace WaterTown {

/**
 * @brief 追随相机，用于游戏模式（跟随目标物体，如船只）
 */
class FollowCamera : public Camera {
public:
    /**
     * @brief 构造函数
     * @param fov 视野角度
     * @param aspectRatio 宽高比
     */
    FollowCamera(float fov = 45.0f, float aspectRatio = 16.0f / 9.0f);
    
    // 实现基类接口
    glm::mat4 getViewMatrix() const override;
    glm::mat4 getProjectionMatrix() const override;
    void setPosition(const glm::vec3& position) override;
    glm::vec3 getPosition() const override;
    
    /**
     * @brief 设置目标（要跟随的物体）
     * @param targetPosition 目标位置
     * @param targetRotation 目标旋转角度（Y 轴，度）
     */
    void setTarget(const glm::vec3& targetPosition, float targetRotation);

    /**
     * @brief 获取目标点
     */
    glm::vec3 getTarget() const { return m_targetPos; }
    
    /**
     * @brief 更新相机（平滑跟随）
     * @param deltaTime 帧时间
     */
    void update(float deltaTime);
    
    /**
     * @brief 设置相机偏移（相对于目标的位置）
     * @param offset 偏移量（局部坐标系）
     */
    void setOffset(const glm::vec3& offset) { m_offset = offset; }

    /**
     * @brief 旋转相机（围绕船只）
     * @param deltaYaw 偏航角增量
     * @param deltaPitch 俯仰角增量
     */
    void rotate(float deltaYaw, float deltaPitch);
    
    /**
     * @brief 获取偏移量
     */
    glm::vec3 getOffset() const { return m_offset; }
    
    /**
     * @brief 设置平滑跟随速度
     */
    void setSmoothSpeed(float speed) { m_smoothSpeed = speed; }
    
    /**
     * @brief 更新宽高比
     */
    void updateAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }

    /**
     * @brief 计算期望的相机位置（考虑目标旋转和偏移）
     */
    glm::vec3 getDesiredPosition() const;

private:
    glm::vec3 m_position;           // 当前相机位置
    glm::vec3 m_targetPos;          // 目标位置
    float m_targetRotation;         // 目标旋转
    
    glm::vec3 m_offset;             // 相机偏移（局部坐标）
    float m_smoothSpeed;            // 平滑跟随速度
    
    // 额外的旋转偏移
    float m_yawOffset;
    float m_pitchOffset;
    
    float m_fov;                    // 视野角度
    float m_aspectRatio;            // 宽高比
    float m_near, m_far;            // 裁剪面

};

} // namespace WaterTown
