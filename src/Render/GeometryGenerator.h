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
class TerrainGrid {
public:
    TerrainGrid(int size = 50);
    ~TerrainGrid();
    
    /**
     * @brief 渲染地形网格
     */
    void render(Shader* shader, Camera* camera);
    
    /**
     * @brief 设置网格数据
     */
    void setTerrainData(const TerrainType grid[][50], int size);
    
    /**
     * @brief 更新网格颜色
     */
    void updateGrid();

private:
    unsigned int m_VAO, m_VBO;
    int m_gridSize;
    std::vector<float> m_vertices;
    
    void generateGridMesh();
};

/**
 * @brief 简单几何体生成器
 */
class GeometryGenerator {
public:
    /**
     * @brief 生成立方体
     */
    static void createCube(std::vector<float>& vertices, std::vector<unsigned int>& indices,
                          float width = 1.0f, float height = 1.0f, float depth = 1.0f);
    
    /**
     * @brief 生成平面
     */
    static void createPlane(std::vector<float>& vertices, std::vector<unsigned int>& indices,
                           float width, float height, int divisions = 1);
    
    /**
     * @brief 生成网格
     */
    static void createGrid(std::vector<float>& vertices, std::vector<unsigned int>& indices,
                          int gridSize, float cellSize);
};

} // namespace WaterTown
