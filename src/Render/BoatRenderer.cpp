#include "BoatRenderer.h"
#include "Shader.h"
#include "Camera.h"
#include "../Physics/Boat.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace WaterTown {

BoatRenderer::BoatRenderer() : m_vao(0), m_vbo(0), m_vertexCount(0) {
    generateMesh();
}

BoatRenderer::~BoatRenderer() {
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
}

void BoatRenderer::generateMesh() {
    // 简单的船只模型：长方体船体 + 三角形船帆
    std::vector<float> vertices;
    
    // 船体（长方体，2.0 x 0.4 x 0.8）
    float halfLength = 1.0f;
    float halfHeight = 0.2f;
    float halfWidth = 0.4f;
    
    // 前面（船头）
    vertices.insert(vertices.end(), {
        -halfWidth, -halfHeight,  halfLength,  0.0f, 0.0f, 1.0f,  // 法线
        -halfWidth,  halfHeight,  halfLength,  0.0f, 0.0f, 1.0f,
         halfWidth,  halfHeight,  halfLength,  0.0f, 0.0f, 1.0f,
        -halfWidth, -halfHeight,  halfLength,  0.0f, 0.0f, 1.0f,
         halfWidth,  halfHeight,  halfLength,  0.0f, 0.0f, 1.0f,
         halfWidth, -halfHeight,  halfLength,  0.0f, 0.0f, 1.0f,
    });
    
    // 后面（船尾）
    vertices.insert(vertices.end(), {
        -halfWidth, -halfHeight, -halfLength,  0.0f, 0.0f, -1.0f,
         halfWidth,  halfHeight, -halfLength,  0.0f, 0.0f, -1.0f,
        -halfWidth,  halfHeight, -halfLength,  0.0f, 0.0f, -1.0f,
        -halfWidth, -halfHeight, -halfLength,  0.0f, 0.0f, -1.0f,
         halfWidth, -halfHeight, -halfLength,  0.0f, 0.0f, -1.0f,
         halfWidth,  halfHeight, -halfLength,  0.0f, 0.0f, -1.0f,
    });
    
    // 左侧
    vertices.insert(vertices.end(), {
        -halfWidth, -halfHeight, -halfLength,  -1.0f, 0.0f, 0.0f,
        -halfWidth,  halfHeight,  halfLength,  -1.0f, 0.0f, 0.0f,
        -halfWidth,  halfHeight, -halfLength,  -1.0f, 0.0f, 0.0f,
        -halfWidth, -halfHeight, -halfLength,  -1.0f, 0.0f, 0.0f,
        -halfWidth, -halfHeight,  halfLength,  -1.0f, 0.0f, 0.0f,
        -halfWidth,  halfHeight,  halfLength,  -1.0f, 0.0f, 0.0f,
    });
    
    // 右侧
    vertices.insert(vertices.end(), {
         halfWidth, -halfHeight, -halfLength,  1.0f, 0.0f, 0.0f,
         halfWidth,  halfHeight, -halfLength,  1.0f, 0.0f, 0.0f,
         halfWidth,  halfHeight,  halfLength,  1.0f, 0.0f, 0.0f,
         halfWidth, -halfHeight, -halfLength,  1.0f, 0.0f, 0.0f,
         halfWidth,  halfHeight,  halfLength,  1.0f, 0.0f, 0.0f,
         halfWidth, -halfHeight,  halfLength,  1.0f, 0.0f, 0.0f,
    });
    
    // 顶部
    vertices.insert(vertices.end(), {
        -halfWidth,  halfHeight, -halfLength,  0.0f, 1.0f, 0.0f,
         halfWidth,  halfHeight,  halfLength,  0.0f, 1.0f, 0.0f,
        -halfWidth,  halfHeight,  halfLength,  0.0f, 1.0f, 0.0f,
        -halfWidth,  halfHeight, -halfLength,  0.0f, 1.0f, 0.0f,
         halfWidth,  halfHeight, -halfLength,  0.0f, 1.0f, 0.0f,
         halfWidth,  halfHeight,  halfLength,  0.0f, 1.0f, 0.0f,
    });
    
    // 底部
    vertices.insert(vertices.end(), {
        -halfWidth, -halfHeight, -halfLength,  0.0f, -1.0f, 0.0f,
        -halfWidth, -halfHeight,  halfLength,  0.0f, -1.0f, 0.0f,
         halfWidth, -halfHeight,  halfLength,  0.0f, -1.0f, 0.0f,
        -halfWidth, -halfHeight, -halfLength,  0.0f, -1.0f, 0.0f,
         halfWidth, -halfHeight,  halfLength,  0.0f, -1.0f, 0.0f,
         halfWidth, -halfHeight, -halfLength,  0.0f, -1.0f, 0.0f,
    });
    
    // 船帆（简单的三角形）
    float mastHeight = 1.5f;
    vertices.insert(vertices.end(), {
        0.0f,  halfHeight,  0.0f,       1.0f, 0.0f, 0.0f,  // 桅杆底部
        0.0f,  mastHeight,  0.0f,       1.0f, 0.0f, 0.0f,  // 桅杆顶部
        0.5f,  halfHeight + 0.7f, 0.0f, 1.0f, 0.0f, 0.0f,  // 帆
    });
    
    m_vertexCount = vertices.size() / 6;
    
    // 创建 VAO 和 VBO
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

void BoatRenderer::render(const Boat* boat, Shader* shader, Camera* camera) {
    if (!boat || !shader || !camera) return;
    
    shader->use();
    
    // 构建模型矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, boat->getPosition());
    model = glm::rotate(model, glm::radians(boat->getRotation()), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(boat->getPitch()), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(boat->getRoll()), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // 设置矩阵
    shader->setMat4("uModel", model);
    shader->setMat4("uView", camera->getViewMatrix());
    shader->setMat4("uProjection", camera->getProjectionMatrix());
    
    // 设置光照参数
    shader->setVec3("uViewPos", camera->getPosition());
    shader->setVec3("uLightPos", 10.0f, 10.0f, 10.0f);
    shader->setVec3("uObjectColor", 0.6f, 0.4f, 0.2f);  // 木色
    shader->setVec3("uLightColor", 1.0f, 1.0f, 1.0f);
    
    // 绘制船只
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}

} // namespace WaterTown
