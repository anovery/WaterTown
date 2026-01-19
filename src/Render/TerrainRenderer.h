#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "../Editor/SceneEditor.h"

namespace WaterTown {

class Shader;
class Camera;

/**
 * @brief 地形网格渲染器
 */
class TerrainRenderer {
public:
    TerrainRenderer(int gridSize);
    ~TerrainRenderer();
    
    /**
     * @brief 渲染地形网格
     * @param editor 场景编辑器（获取地形数据）
     * @param shader 着色器
     * @param camera 相机
     */
    void render(SceneEditor* editor, Shader* shader, Camera* camera);
    
    /**
     * @brief 按地形类型渲染
     * @param editor 场景编辑器
     * @param shader 着色器
     * @param camera 相机
     * @param type 要渲染的地形类型
     */
    void renderByType(SceneEditor* editor, Shader* shader, Camera* camera, TerrainType type);
    
    /**
     * @brief 设置网格大小
     */
    void setGridSize(int size) { m_gridSize = size; }
    
private:
    int m_gridSize;
    
    struct TerrainVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;
        int terrainType;  // 0=EMPTY, 1=GRASS, 2=WATER, 3=STONE
    };
    
    GLuint m_planeVAO, m_planeVBO;
    
    void addWallBricks(std::vector<TerrainVertex>& vertices, float x, float z, float size, 
                      bool top, bool bottom, bool left, bool right);
                      
    void buildTerrainVertices(SceneEditor* editor, std::vector<TerrainVertex>& outVertices);
    glm::vec3 getTerrainColor(TerrainType type) const;
    float getTerrainHeight(TerrainType type) const;
};

} // namespace WaterTown
