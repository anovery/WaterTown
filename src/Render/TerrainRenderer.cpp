#include "TerrainRenderer.h"
#include "Shader.h"
#include "Camera.h"
#include "../Editor/SceneEditor.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace WaterTown {

TerrainRenderer::TerrainRenderer(int gridSize)
    : m_gridSize(gridSize), m_vao(0), m_vbo(0), m_vertexCount(0) {
    generateMesh();
}

TerrainRenderer::~TerrainRenderer() {
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
}

void TerrainRenderer::generateMesh() {
    // 为每个格子生成6个顶点（2个三角形）
    std::vector<float> vertices;
    
    for (int z = 0; z < m_gridSize; ++z) {
        for (int x = 0; x < m_gridSize; ++x) {
            // 计算格子的四个角（世界坐标），缩小到0.5x0.5
            float cellSize = 0.5f;
            float x0 = (x - m_gridSize / 2.0f) * cellSize;
            float x1 = (x + 1 - m_gridSize / 2.0f) * cellSize;
            float z0 = (z - m_gridSize / 2.0f) * cellSize;
            float z1 = (z + 1 - m_gridSize / 2.0f) * cellSize;
            float y = 0.0f;  // 陆地高度（水面Y=-1.0，陆地在水面上方1.0单位）
            
            // 两个三角形组成一个格子
            // 三角形1
            vertices.push_back(x0); vertices.push_back(y); vertices.push_back(z0); // 位置
            vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f); // 法线
            
            vertices.push_back(x1); vertices.push_back(y); vertices.push_back(z0);
            vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
            
            vertices.push_back(x1); vertices.push_back(y); vertices.push_back(z1);
            vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
            
            // 三角形2
            vertices.push_back(x0); vertices.push_back(y); vertices.push_back(z0);
            vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
            
            vertices.push_back(x1); vertices.push_back(y); vertices.push_back(z1);
            vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
            
            vertices.push_back(x0); vertices.push_back(y); vertices.push_back(z1);
            vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
        }
    }
    
    m_vertexCount = vertices.size() / 6;
    
    // 创建VAO和VBO
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // 法线属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

glm::vec3 TerrainRenderer::getTerrainColor(TerrainType type) const {
    switch (type) {
        case TerrainType::GRASS:
            return glm::vec3(0.3f, 0.7f, 0.3f);  // 绿色
        case TerrainType::WATER:
            return glm::vec3(0.2f, 0.4f, 0.8f);  // 蓝色
        case TerrainType::STONE:
            return glm::vec3(0.5f, 0.5f, 0.5f);  // 灰色
        default:
            return glm::vec3(1.0f, 1.0f, 1.0f);  // 白色
    }
}

void TerrainRenderer::render(SceneEditor* editor, Shader* shader, Camera* camera) {
    if (!editor || !shader || !camera) return;
    
    shader->use();
    shader->setMat4("uView", camera->getViewMatrix());
    shader->setMat4("uProjection", camera->getProjectionMatrix());
    shader->setVec3("uViewPos", camera->getPosition());
    shader->setVec3("uLightPos", 10.0f, 10.0f, 10.0f);
    shader->setVec3("uLightColor", 1.0f, 1.0f, 1.0f);
    
    glBindVertexArray(m_vao);
    
    // 渲染每个格子（只渲染陆地，跳过水面）
    for (int z = 0; z < m_gridSize; ++z) {
        for (int x = 0; x < m_gridSize; ++x) {
            // 获取地形类型
            TerrainType type = editor->getTerrainAt(x, z);
            
            // 跳过水面类型（让真实的WaterSurface显示）
            if (type == TerrainType::WATER) {
                continue;
            }
            
            // 顶点已经包含了正确的世界坐标，使用单位矩阵
            glm::mat4 model = glm::mat4(1.0f);
            
            shader->setMat4("uModel", model);
            shader->setVec3("uObjectColor", getTerrainColor(type));
            
            // 绘制该格子（6个顶点）
            int baseVertex = (z * m_gridSize + x) * 6;
            glDrawArrays(GL_TRIANGLES, baseVertex, 6);
        }
    }
    
    glBindVertexArray(0);
}

} // namespace WaterTown
