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
                      float near = 0.1f, float far = 1000.0f);
    
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
    void setHeight(float height);
    
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

} // namespace WaterTown
