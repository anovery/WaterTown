#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
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
     * @brief 设置网格大小
     */
    void setGridSize(int size) { m_gridSize = size; }
    
private:
    int m_gridSize;
    GLuint m_vao;
    GLuint m_vbo;
    int m_vertexCount;
    
    /**
     * @brief 生成网格mesh
     */
    void generateMesh();
    
    /**
     * @brief 根据地形类型获取颜色
     */
    glm::vec3 getTerrainColor(TerrainType type) const;
};

} // namespace WaterTown
