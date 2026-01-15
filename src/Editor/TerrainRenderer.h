#include "TerrainRenderer.h"#pragma once



































} // namespace WaterTown}    // ... 已实现的代码void EditorUI::renderModePanel() {}    std::cout << "TerrainRenderer initialized with grid size: " << gridSize << std::endl;    generateGridMesh();    m_gridSize = gridSize;void TerrainRenderer::init(int gridSize) {}    }        glDeleteBuffers(1, &m_terrainVBO);        glDeleteVertexArrays(1, &m_terrainVAO);    if (m_terrainVAO) {    }        glDeleteBuffers(1, &m_gridVBO);        glDeleteVertexArrays(1, &m_gridVAO);    if (m_gridVAO) {TerrainRenderer::~TerrainRenderer() {}      m_terrainVAO(0), m_terrainVBO(0), m_terrainEBO(0) {      m_gridVAO(0), m_gridVBO(0), m_gridEBO(0),    : m_gridSize(gridSize), m_cellSize(cellSize),TerrainRenderer::TerrainRenderer(int gridSize, float cellSize)namespace WaterTown {#include <iostream>#include "../Render/Camera.h"#include "../Render/Shader.h"
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
    /**
     * @brief 构造函数
     * @param gridSize 网格大小（单位：米）
     * @param gridCount 网格数量（每边）
     */
    TerrainGrid(int gridSize = 50, float cellSize = 1.0f);
    ~TerrainGrid();
    
    /**
     * @brief 渲染地形网格
     */
    void render(Shader* shader, Camera* camera);
    
    /**
     * @brief 设置地形类型
     */
    void setTerrain(int x, int z, TerrainType type);
    
    /**
     * @brief 获取地形类型
     */
    TerrainType getTerrain(int x, int z) const;
    
    /**
     * @brief 从屏幕坐标拾取网格
     */
    bool pickGrid(const glm::vec2& mousePos, const glm::vec2& screenSize, 
                  Camera* camera, int& outX, int& outZ);
    
    /**
     * @brief 清空场景
     */
    void clear();

private:
    static const int GRID_SIZE = 50;
    TerrainType m_grid[50][50];
    
    unsigned int m_gridVAO, m_gridVBO;
    unsigned int m_terrainVAO, m_terrainVBO, m_terrainEBO;
    int m_terrainIndexCount;
    
    /**
     * @brief 生成网格线
     */
    void generateGrid();
    
    /**
     * @brief 生成地形网格渲染数据
     */
    void generateTerrainMesh();
};

} // namespace WaterTown
