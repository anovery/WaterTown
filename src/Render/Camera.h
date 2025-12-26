#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace WaterTown {

/**
 * @brief 相机基类，定义通用接口
 */
class Camera {
public:
    virtual ~Camera() = default;
    
    /**
     * @brief 获取视图矩阵
     * @return 视图矩阵
     */
    virtual glm::mat4 getViewMatrix() const = 0;
    
    /**
     * @brief 获取投影矩阵
     * @return 投影矩阵
     */
    virtual glm::mat4 getProjectionMatrix() const = 0;
    
    /**
     * @brief 设置相机位置
     * @param position 位置坐标
     */
    virtual void setPosition(const glm::vec3& position) = 0;
    
    /**
     * @brief 获取相机位置
     * @return 位置坐标
     */
    virtual glm::vec3 getPosition() const = 0;
};

/**
 * @brief 自由相机，支持 WASD 移动和鼠标视角控制
 */
class FreeCamera : public Camera {
public:
    /**
     * @brief 构造函数
     * @param position 初始位置
     * @param fov 视野角度（度）
     * @param aspectRatio 宽高比
     */
    FreeCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
               float fov = 45.0f,
               float aspectRatio = 16.0f / 9.0f);
    
    /**
     * @brief 处理键盘输入
     * @param key GLFW 按键码
     * @param deltaTime 帧间隔时间（秒）
     */
    void processKeyboard(int key, float deltaTime);
    
    /**
     * @brief 处理鼠标移动
     * @param xoffset X 轴偏移量
     * @param yoffset Y 轴偏移量
     */
    void processMouseMovement(float xoffset, float yoffset);
    
    /**
     * @brief 处理鼠标滚轮
     * @param yoffset 滚轮偏移量
     */
    void processMouseScroll(float yoffset);
    
    // 实现基类接口
    glm::mat4 getViewMatrix() const override;
    glm::mat4 getProjectionMatrix() const override;
    void setPosition(const glm::vec3& position) override;
    glm::vec3 getPosition() const override;
    
    /**
     * @brief 更新宽高比
     * @param aspectRatio 新的宽高比
     */
    void updateAspectRatio(float aspectRatio);
    
    /**
     * @brief 获取前方向量
     * @return 前方向量
     */
    glm::vec3 getFront() const { return m_front; }
    
    /**
     * @brief 获取右方向量
     * @return 右方向量
     */
    glm::vec3 getRight() const { return m_right; }
    
    /**
     * @brief 获取上方向量
     * @return 上方向量
     */
    glm::vec3 getUp() const { return m_up; }

private:
    // 相机属性
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    
    // 欧拉角
    float m_yaw;    // 偏航角（绕 Y 轴旋转）
    float m_pitch;  // 俯仰角（绕 X 轴旋转）
    
    // 相机配置
    float m_fov;         // 视野角度
    float m_aspectRatio; // 宽高比
    float m_nearPlane;   // 近裁剪面
    float m_farPlane;    // 远裁剪面
    
    // 相机选项
    float m_moveSpeed;
    float m_mouseSensitivity;
    
    /**
     * @brief 根据欧拉角更新相机向量
     */
    void updateCameraVectors();
};

} // namespace WaterTown
