#pragma once

#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace WaterTown {

/**
 * @brief 正交相机，用于地形编辑模式（自上而下视角）
 */
class OrthographicCamera : public Camera {
public:
    /**
     * @brief 构造函数
     * @param centerX 视野中心 X 坐标
     * @param centerZ 视野中心 Z 坐标
     * @param width 视野宽度
     * @param height 视野高度
     * @param near 近裁剪面
     * @param far 远裁剪面
     */
    OrthographicCamera(float centerX = 0.0f, float centerZ = 0.0f,
                      float width = 20.0f, float height = 20.0f,
                      float near = 0.1f, float far = 100.0f);
    
    // 实现基类接口
    glm::mat4 getViewMatrix() const override;
    glm::mat4 getProjectionMatrix() const override;
    void setPosition(const glm::vec3& position) override;
    glm::vec3 getPosition() const override;
    
    /**
     * @brief 设置视野中心（在 XZ 平面上）
     */
    void setCenter(float x, float z);
    
    /**
     * @brief 获取视野中心
     */
    glm::vec2 getCenter() const { return glm::vec2(m_centerX, m_centerZ); }
    
    /**
     * @brief 设置视野大小
     */
    void setViewSize(float width, float height);
    
    /**
     * @brief 缩放视野（用于鼠标滚轮）
     */
    void zoom(float delta);
    
    /**
     * @brief 平移视野
     */
    void pan(float deltaX, float deltaZ);
    
    /**
     * @brief 设置相机高度
     */
    void setHeight(float height) { m_cameraHeight = height; }
    
    /**
     * @brief 获取相机高度
     */
    float getHeight() const { return m_cameraHeight; }

private:
    float m_centerX, m_centerZ;     // 视野中心
    float m_width, m_height;        // 视野大小
    float m_near, m_far;            // 裁剪面
    float m_cameraHeight;           // 相机高度（Y 坐标）
    float m_minZoom, m_maxZoom;     // 缩放限制
};

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
