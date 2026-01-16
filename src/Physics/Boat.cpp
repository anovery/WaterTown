#include "Boat.h"
#include "../Water/WaterSurface.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

namespace WaterTown {

Boat::Boat(const glm::vec3& position, float rotation)
    : m_position(position), m_rotation(rotation), m_speed(0.0f),
      m_angularVelocity(0.0f), m_pitch(0.0f), m_roll(0.0f),
      m_forwardInput(0.0f), m_turnInput(0.0f), m_hasBounds(false),
      m_minX(-100.0f), m_maxX(100.0f), m_minZ(-100.0f), m_maxZ(100.0f) {
}

void Boat::update(float deltaTime, WaterSurface* waterSurface, float currentTime) {
    // 更新运动
    updateMotion(deltaTime);
    
    // 检查碰撞
    handleCollisions();
    
    // 更新浮力和姿态
    if (waterSurface) {
        updateBuoyancy(waterSurface, currentTime);
    }
}

void Boat::processInput(float forward, float turn) {
    // 手动实现 clamp (C++14 没有 std::clamp)
    m_forwardInput = std::max(-1.0f, std::min(forward, 1.0f));
    m_turnInput = std::max(-1.0f, std::min(turn, 1.0f));
}

void Boat::updateMotion(float deltaTime) {
    // 加速/减速
    if (std::abs(m_forwardInput) > 0.01f) {
        float targetSpeed = m_forwardInput * MAX_SPEED;
        float accel = (targetSpeed > m_speed) ? ACCELERATION : DECELERATION;
        m_speed += (targetSpeed - m_speed) * accel * deltaTime;
    } else {
        // 水阻力减速
        m_speed *= exp(-DRAG * deltaTime);
        if (std::abs(m_speed) < 0.01f) m_speed = 0.0f;
    }
    
    // 转向（转向速度与前进速度相关）
    float speedFactor = std::min(std::abs(m_speed) / MAX_SPEED, 1.0f);
    m_angularVelocity = m_turnInput * TURN_SPEED * speedFactor;
    m_rotation += m_angularVelocity * deltaTime;
    
    // 保持旋转角度在 [0, 360) 范围
    if (m_rotation < 0.0f) m_rotation += 360.0f;
    if (m_rotation >= 360.0f) m_rotation -= 360.0f;
    
    // 更新位置
    float rotRad = glm::radians(m_rotation);
    glm::vec3 forward(sin(rotRad), 0.0f, cos(rotRad));
    m_position += forward * m_speed * deltaTime;
    
    // 轻微的左右摇晃（前进时）
    if (std::abs(m_speed) > 0.1f) {
        float wobble = sin(glfwGetTime() * 2.0f) * 0.01f * speedFactor;
        m_roll = wobble * 2.0f;  // 度
    } else {
        m_roll *= 0.95f;  // 平滑归零
    }
}

void Boat::updateBuoyancy(WaterSurface* waterSurface, float currentTime) {
    if (!waterSurface) return;
    
    // 在船头、船尾、左侧、右侧采样水面高度
    float rotRad = glm::radians(m_rotation);
    glm::vec3 forward(sin(rotRad), 0.0f, cos(rotRad));
    glm::vec3 right(cos(rotRad), 0.0f, -sin(rotRad));
    
    // 采样点
    glm::vec3 bow = m_position + forward * (BOAT_LENGTH * 0.5f);      // 船头
    glm::vec3 stern = m_position - forward * (BOAT_LENGTH * 0.5f);    // 船尾
    glm::vec3 portSide = m_position - right * (BOAT_WIDTH * 0.5f);    // 左舷
    glm::vec3 starboard = m_position + right * (BOAT_WIDTH * 0.5f);   // 右舷
    
    float heightBow = sampleWaterHeight(bow, waterSurface, currentTime);
    float heightStern = sampleWaterHeight(stern, waterSurface, currentTime);
    float heightPort = sampleWaterHeight(portSide, waterSurface, currentTime);
    float heightStarboard = sampleWaterHeight(starboard, waterSurface, currentTime);
    
    // 船体中心高度（平均值）
    float avgHeight = (heightBow + heightStern + heightPort + heightStarboard) / 4.0f;
    m_position.y = avgHeight + 0.3f;  // 船体稍微浮在水面上
    
    // 计算俯仰角（船头船尾高度差）
    float pitchDiff = heightBow - heightStern;
    m_pitch = atan2(pitchDiff, BOAT_LENGTH) * (180.0f / 3.14159f);  // 转换为度
    m_pitch *= 0.3f;  // 缩小幅度，更真实
    
    // 计算翻滚角（左右高度差）
    float rollDiff = heightStarboard - heightPort;
    float waterRoll = atan2(rollDiff, BOAT_WIDTH) * (180.0f / 3.14159f);
    m_roll = m_roll * 0.5f + waterRoll * 0.5f;  // 混合水波和运动摇晃
}

float Boat::sampleWaterHeight(const glm::vec3& worldPos, WaterSurface* waterSurface, float time) const {
    return waterSurface->getWaterHeight(worldPos.x, worldPos.z, time);
}

void Boat::addObstacle(const glm::vec3& position, float radius) {
    m_obstacles.push_back({position, radius});
}

void Boat::clearObstacles() {
    m_obstacles.clear();
}

void Boat::handleCollisions() {
    // 边界检测
    if (m_hasBounds) {
        bool collided = false;
        glm::vec3 pushBack(0.0f);
        
        if (m_position.x - BOAT_RADIUS < m_minX) {
            pushBack.x = m_minX + BOAT_RADIUS - m_position.x;
            collided = true;
        }
        if (m_position.x + BOAT_RADIUS > m_maxX) {
            pushBack.x = m_maxX - BOAT_RADIUS - m_position.x;
            collided = true;
        }
        if (m_position.z - BOAT_RADIUS < m_minZ) {
            pushBack.z = m_minZ + BOAT_RADIUS - m_position.z;
            collided = true;
        }
        if (m_position.z + BOAT_RADIUS > m_maxZ) {
            pushBack.z = m_maxZ - BOAT_RADIUS - m_position.z;
            collided = true;
        }
        
        if (collided) {
            m_position += pushBack;
            m_speed *= 0.3f;  // 碰撞后减速
        }
    }
    
    // 障碍物碰撞检测（圆形碰撞）
    for (const auto& obstacle : m_obstacles) {
        glm::vec3 diff = m_position - obstacle.position;
        diff.y = 0.0f;  // 只检测水平方向
        float distance = glm::length(diff);
        float minDist = BOAT_RADIUS + obstacle.radius;
        
        if (distance < minDist && distance > 0.001f) {
            // 碰撞，推开
            glm::vec3 pushDir = glm::normalize(diff);
            float pushAmount = minDist - distance;
            m_position += pushDir * pushAmount;
            m_speed *= 0.3f;  // 碰撞后减速
        }
    }
}

} // namespace WaterTown
