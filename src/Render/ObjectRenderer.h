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
    
    // ===== 缩放参数（用于调整几何体和船的比例关系）=====
    float houseScale = 1.5f;        // 房子墙体宽度
    float houseHeight = 1.5f;       // 房子墙体高度
    float houseRoofScale = 1.0f;    // 屋顶底部宽度
    float houseRoofHeight = 1.0f;   // 屋顶高度
    
    float bridgeScale = 1.2f;       // 石头大小
    float bridgeHeight = 0.8f;      // 石头高度
    
    float treeScale = 0.3f;         // 树干粗细
    float treeHeight = 1.5f;        // 树干高度
    float treeCrownScale = 1.5f;    // 树冠大小
    
    // New parameters for modular pieces
    float wallLength = 1.0f;
    float wallHeight = 0.8f;
    float wallWidth = 0.1f;
    
    float longHouseLength = 2.0f;   // Length multiplier for long house
    
private:
    std::vector<SceneObject> m_objects;
    
    // 几何体VAO/VBO
    GLuint m_cubeVAO, m_cubeVBO;
    GLuint m_coneVAO, m_coneVBO;
    GLuint m_cylinderVAO, m_cylinderVBO;
    GLuint m_sphereVAO, m_sphereVBO;
    
    unsigned int m_coneVertexCount, m_cylinderVertexCount, m_sphereVertexCount;
    
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
    void renderHouseStyle1(const glm::vec3& position, float rotation, Shader* shader);
    void renderHouseStyle2(const glm::vec3& position, float rotation, Shader* shader);
    void renderHouseStyle3(const glm::vec3& position, float rotation, Shader* shader);
    void renderHouseStyle4(const glm::vec3& position, float rotation, Shader* shader);
    void renderHouseStyle5(const glm::vec3& position, float rotation, Shader* shader);
    void renderLongHouse(const glm::vec3& position, float rotation, Shader* shader);
    void renderBridge(const glm::vec3& position, float rotation, Shader* shader);
    void renderTree(const glm::vec3& position, float rotation, Shader* shader);
    void renderWall(const glm::vec3& position, float rotation, Shader* shader);
    void renderPavilion(const glm::vec3& position, float rotation, Shader* shader);
    void renderArchBridge(const glm::vec3& position, float rotation, Shader* shader);
    void renderPaifang(const glm::vec3& position, float rotation, Shader* shader);
    void renderWaterPavilion(const glm::vec3& position, float rotation, Shader* shader);
    void renderPier(const glm::vec3& position, float rotation, Shader* shader);
    void renderTemple(const glm::vec3& position, float rotation, Shader* shader);
    void renderBamboo(const glm::vec3& position, float rotation, Shader* shader);
    void renderLotusPond(const glm::vec3& position, float rotation, Shader* shader);
    void renderFishingBoat(const glm::vec3& position, float rotation, Shader* shader);
    void renderLantern(const glm::vec3& position, float rotation, Shader* shader);
    void renderStoneLion(const glm::vec3& position, float rotation, Shader* shader);
};

} // namespace WaterTown
