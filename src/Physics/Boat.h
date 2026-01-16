#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace WaterTown {

class WaterSurface;

/**
 * @brief 障碍物结构体
 */
struct Obstacle {
    glm::vec3 position;
    float radius;
};

/**
 * @brief 船只物理模拟类
 */
class Boat {
public:
    /**
     * @brief 构造函数
     * @param position 初始位置
     * @param rotation 初始旋转（Y 轴，度）
     */
    Boat(const glm::vec3& position = glm::vec3(0.0f), float rotation = 0.0f);
    
    /**
     * @brief 更新船只物理
     * @param deltaTime 帧时间
     * @param waterSurface 水面引用（用于浮力计算）
     * @param currentTime 当前时间
     */
    void update(float deltaTime, WaterSurface* waterSurface, float currentTime);
    
    /**
     * @brief 处理输入控制
     * @param forward 前进输入 (-1 到 1)
     * @param turn 转向输入 (-1 到 1)
     */
    void processInput(float forward, float turn);
    
    /**
     * @brief 获取位置
     */
    glm::vec3 getPosition() const { return m_position; }
    
    /**
     * @brief 获取旋转角度
     */
    float getRotation() const { return m_rotation; }
    
    /**
     * @brief 获取速度
     */
    float getSpeed() const { return m_speed; }
    
    /**
     * @brief 获取船体倾斜角度（俯仰和翻滚）
     */
    float getPitch() const { return m_pitch; }
    float getRoll() const { return m_roll; }
    
    /**
     * @brief 设置位置
     */
    void setPosition(const glm::vec3& position) { m_position = position; }
    
    /**
     * @brief 设置旋转
     */
    void setRotation(float rotation) { m_rotation = rotation; }
    
    /**
     * @brief 设置边界限制
     * @param minX 最小 X 坐标
     * @param maxX 最大 X 坐标
     * @param minZ 最小 Z 坐标
     * @param maxZ 最大 Z 坐标
     */
    void setBounds(float minX, float maxX, float minZ, float maxZ) {
        m_minX = minX; m_maxX = maxX;
        m_minZ = minZ; m_maxZ = maxZ;
        m_hasBounds = true;
    }
    
    /**
     * @brief 添加障碍物（简化版：圆形碰撞体）
     * @param position 障碍物位置
     * @param radius 障碍物半径
     */
    void addObstacle(const glm::vec3& position, float radius);
    void clearObstacles();

private:
    // 物理参数
    glm::vec3 m_position;       // 位置
    float m_rotation;           // 旋转（Y 轴，度）
    float m_speed;              // 速度（m/s）
    float m_angularVelocity;    // 角速度（度/s）
    
    // 船体姿态
    float m_pitch;              // 俯仰角
    float m_roll;               // 翻滚角
    
    // 控制参数
    float m_forwardInput;       // 前进输入
    float m_turnInput;          // 转向输入
    
    // 物理常数
    const float MAX_SPEED = 5.0f;           // 最大速度
    const float ACCELERATION = 2.0f;        // 加速度
    const float DECELERATION = 1.0f;        // 减速度
    const float TURN_SPEED = 60.0f;         // 转向速度（度/s）
    const float DRAG = 0.5f;                // 水阻力
    const float BOAT_LENGTH = 2.0f;         // 船长
    const float BOAT_WIDTH = 0.8f;          // 船宽
    const float BOAT_RADIUS = 1.0f;         // 碰撞半径
    
    // 边界和碰撞
    bool m_hasBounds;
    float m_minX, m_maxX, m_minZ, m_maxZ;
    std::vector<Obstacle> m_obstacles;
    
    /**
     * @brief 更新运动
     */
    void updateMotion(float deltaTime);
    
    /**
     * @brief 更新浮力和姿态
     */
    void updateBuoyancy(WaterSurface* waterSurface, float currentTime);
    
    /**
     * @brief 计算采样点的水面高度
     */
    float sampleWaterHeight(const glm::vec3& worldPos, WaterSurface* waterSurface, float time) const;
    
    /**
     * @brief 检查并处理碰撞
     */
    void handleCollisions();
};

} // namespace WaterTown
