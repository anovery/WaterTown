#pragma once

#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace WaterTown {

/**
 * @brief 轨道相机，用于建筑布置模式（围绕目标点旋转）
 */
class OrbitCamera : public Camera {
public:
    /**
     * @brief 构造函数
     * @param target 目标点（相机围绕的中心）
     * @param distance 距离目标的距离
     * @param fov 视野角度
     * @param aspectRatio 宽高比
     */
    OrbitCamera(glm::vec3 target = glm::vec3(0.0f),
               float distance = 10.0f,
               float fov = 45.0f,
               float aspectRatio = 16.0f / 9.0f);
    
    // 实现基类接口
    glm::mat4 getViewMatrix() const override;
    glm::mat4 getProjectionMatrix() const override;
    void setPosition(const glm::vec3& position) override;
    glm::vec3 getPosition() const override;
    
    /**
     * @brief 旋转相机（鼠标拖拽）
     * @param deltaYaw 水平旋转增量
     * @param deltaPitch 垂直旋转增量
     */
    void rotate(float deltaYaw, float deltaPitch);
    
    /**
     * @brief 缩放（改变距离）
     * @param delta 距离变化量
     */
    void zoom(float delta);
    
    /**
     * @brief 平移目标点
     * @param deltaX X 方向平移
     * @param deltaY Y 方向平移
     * @param deltaZ Z 方向平移
     */
    void panTarget(float deltaX, float deltaY, float deltaZ);

    /**
     * @brief 平移 (适配 SceneEditor 调用)
     */
    void pan(float deltaX, float deltaZ) { panTarget(deltaX, 0.0f, deltaZ); }
    
    /**
     * @brief 设置目标点
     */
    void setTarget(const glm::vec3& target) { m_target = target; }
    
    /**
     * @brief 获取目标点
     */
    glm::vec3 getTarget() const { return m_target; }
    
    /**
     * @brief 更新宽高比
     */
    void updateAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }

     /**
      * @brief 直接设置距离和角度，方便快速切换预设视角
      */
    void setDistance(float distance);
    void setAngles(float yawDegrees, float pitchDegrees);
    float getDistance() const { return m_distance; }
    float getYawDegrees() const { return m_yaw; }
    float getPitchDegrees() const { return m_pitch; }

private:
    glm::vec3 m_target;          // 目标点
    float m_distance;            // 距离
    float m_yaw;                 // 水平角度（度）
    float m_pitch;               // 垂直角度（度）
    
    float m_fov;                 // 视野角度
    float m_aspectRatio;         // 宽高比
    float m_near, m_far;         // 裁剪面
    
    float m_minDistance, m_maxDistance;  // 距离限制
    float m_minPitch, m_maxPitch;        // 俯仰角限制
    
    /**
     * @brief 根据球坐标计算相机位置
     */
    glm::vec3 calculatePosition() const;
};

} // namespace WaterTown
