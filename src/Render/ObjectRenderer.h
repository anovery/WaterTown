#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "../Editor/SceneEditor.h"

namespace WaterTown {

class Shader;
class Camera;

/**
 * @brief 场景物体结构
 */
struct SceneObject {
    ObjectType type;
    glm::vec3 position;
    float rotation;  // Y轴旋转角度
};

/**
 * @brief 建筑物和物体渲染器（使用简单几何体拼接）
 */
class ObjectRenderer {
public:
    ObjectRenderer();
    ~ObjectRenderer();
    
    /**
     * @brief 添加物体
     */
    void addObject(ObjectType type, const glm::vec3& position, float rotation = 0.0f);
    
    /**
     * @brief 清空所有物体
     */
    void clear();
    
    /**
     * @brief 渲染所有物体
     */
    void render(Shader* shader, Camera* camera);
    
private:
    std::vector<SceneObject> m_objects;
    
    // 几何体VAO/VBO
    GLuint m_cubeVAO, m_cubeVBO;
    GLuint m_coneVAO, m_coneVBO;
    GLuint m_cylinderVAO, m_cylinderVBO;
    GLuint m_sphereVAO, m_sphereVBO;
    
    int m_coneVertexCount;
    int m_cylinderVertexCount;
    int m_sphereVertexCount;
    
    /**
     * @brief 生成基础几何体
     */
    void generateCube();
    void generateCone();
    void generateCylinder();
    void generateSphere();
    
    /**
     * @brief 渲染不同类型的物体
     */
    void renderHouse(const glm::vec3& position, float rotation, Shader* shader);
    void renderBridge(const glm::vec3& position, float rotation, Shader* shader);
    void renderTree(const glm::vec3& position, float rotation, Shader* shader);
    void renderBoat(const glm::vec3& position, float rotation, Shader* shader);
};

} // namespace WaterTown
