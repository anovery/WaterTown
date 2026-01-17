#include "WaterSurface.h"
#include "../Render/Shader.h"
#include "../Render/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <iostream>

namespace WaterTown {

WaterSurface::WaterSurface(float centerX, float centerZ, float width, float height, int resolution)
    : m_centerX(centerX), m_centerZ(centerZ), m_width(width), m_height(height),
      m_baseHeight(0.0f), m_resolution(resolution), m_VAO(0), m_VBO(0), m_EBO(0),
      m_vertexCount(0), m_indexCount(0), m_useCustomMesh(false) {
    
    // 初始化默认波浪参数（4 个不同方向的波浪）
    m_waves.push_back({glm::vec2(1.0f, 0.0f), 0.15f, 2.0f, 1.0f, 0.3f});
    m_waves.push_back({glm::vec2(0.7f, 0.7f), 0.1f, 1.5f, 1.2f, 0.2f});
    m_waves.push_back({glm::vec2(0.0f, 1.0f), 0.08f, 1.0f, 0.8f, 0.25f});
    m_waves.push_back({glm::vec2(-0.5f, 0.5f), 0.05f, 0.8f, 1.5f, 0.15f});
    
    generateMesh();
    
    std::cout << "WaterSurface created: " << m_vertexCount << " vertices, " 
              << m_indexCount / 3 << " triangles" << std::endl;
}

WaterSurface::~WaterSurface() {
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_EBO) glDeleteBuffers(1, &m_EBO);
}

void WaterSurface::updateMesh(const std::vector<float>& vertices) {
    m_useCustomMesh = true;
    
    if (m_VAO == 0) {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
    }
    
    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // 如果使用 updateMesh，通常不再使用 EBO (glDrawArrays)
    m_vertexCount = vertices.size() / 5; // 5 floats per vertex
    
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // UV 属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void WaterSurface::generateMesh() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    // 生成网格顶点（只需要位置，法线在着色器中计算）
    float stepX = m_width / m_resolution;
    float stepZ = m_height / m_resolution;
    float startX = m_centerX - m_width / 2.0f;
    float startZ = m_centerZ - m_height / 2.0f;
    
    for (int z = 0; z <= m_resolution; ++z) {
        for (int x = 0; x <= m_resolution; ++x) {
            float posX = startX + x * stepX;
            float posZ = startZ + z * stepZ;
            
            // 位置（始终以 0 为基准，在渲染阶段平移到 m_baseHeight）
            vertices.push_back(posX);
            vertices.push_back(0.0f);
            vertices.push_back(posZ);
            
            // UV 坐标（用于纹理或其他效果）
            vertices.push_back(static_cast<float>(x) / m_resolution);
            vertices.push_back(static_cast<float>(z) / m_resolution);
        }
    }
    
    // 生成索引
    for (int z = 0; z < m_resolution; ++z) {
        for (int x = 0; x < m_resolution; ++x) {
            int topLeft = z * (m_resolution + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * (m_resolution + 1) + x;
            int bottomRight = bottomLeft + 1;
            
            // 第一个三角形
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            
            // 第二个三角形
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
    
    m_vertexCount = (m_resolution + 1) * (m_resolution + 1);
    m_indexCount = static_cast<int>(indices.size());
    
    // 创建 VAO/VBO/EBO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // UV 属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void WaterSurface::render(Shader* shader,
                          Camera* camera,
                          float time,
                          const glm::vec3& boatPos,
                          float boatCutoutInner,
                          float boatCutoutOuter,
                          glm::vec2 boatForwardXZ,
                          glm::vec2 boatHalfExtentsXZ,
                          float boatCutoutFeather) {
    if (!shader || !camera) return;
    
    shader->use();
    
    // 设置 MVP 矩阵
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, m_baseHeight, 0.0f));
    shader->setMat4("uModel", model);
    shader->setMat4("uView", camera->getViewMatrix());
    shader->setMat4("uProjection", camera->getProjectionMatrix());
    
    // 设置时间和相机位置
    shader->setFloat("uTime", time);
    shader->setVec3("uViewPos", camera->getPosition());
    
    // 设置波浪参数（最多 4 个波浪）
    int waveCount = std::min(static_cast<int>(m_waves.size()), 4);
    shader->setInt("uWaveCount", waveCount);
    
    for (int i = 0; i < waveCount; ++i) {
        std::string prefix = "uWaves[" + std::to_string(i) + "].";
        shader->setVec2(prefix + "direction", m_waves[i].direction);
        shader->setFloat(prefix + "amplitude", m_waves[i].amplitude);
        shader->setFloat(prefix + "wavelength", m_waves[i].wavelength);
        shader->setFloat(prefix + "speed", m_waves[i].speed);
        shader->setFloat(prefix + "steepness", m_waves[i].steepness);
    }
    
    // 水面颜色参数
    shader->setVec3("uWaterColor", glm::vec3(0.1f, 0.3f, 0.5f));  // 深蓝色
    shader->setVec3("uLightDir", glm::normalize(glm::vec3(0.5f, 1.0f, 0.3f)));

    // 船只裁剪（避免水出现在船板上）
    bool useObb = (boatHalfExtentsXZ.x > 0.0f && boatHalfExtentsXZ.y > 0.0f && boatCutoutFeather > 0.0f);
    bool useCircle = (boatCutoutInner > 0.0f && boatCutoutOuter >= boatCutoutInner);
    bool useCutout = useObb || useCircle;

    shader->setInt("uUseBoatCutout", useCutout ? 1 : 0);
    shader->setVec3("uBoatPos", boatPos);
    shader->setFloat("uBoatCutoutInner", boatCutoutInner);
    shader->setFloat("uBoatCutoutOuter", boatCutoutOuter);
    shader->setInt("uBoatCutoutShape", useObb ? 1 : 0);
    shader->setVec2("uBoatForwardXZ", boatForwardXZ);
    shader->setVec2("uBoatHalfExtentsXZ", boatHalfExtentsXZ);
    shader->setFloat("uBoatCutoutFeather", boatCutoutFeather);
    
    // 启用混合（半透明效果）
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // 渲染水面
    glBindVertexArray(m_VAO);
    if (m_useCustomMesh) {
         glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    } else {
         glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
    
    glDisable(GL_BLEND);
}

float WaterSurface::getWaterHeight(float x, float z, float time) const {
    return m_baseHeight + calculateGerstnerHeight(x, z, time);
}

float WaterSurface::calculateGerstnerHeight(float x, float z, float time) const {
    float height = 0.0f;
    
    for (const auto& wave : m_waves) {
        float k = 2.0f * glm::pi<float>() / wave.wavelength;
        float c = wave.speed;
        float d_dot_p = wave.direction.x * x + wave.direction.y * z;
        float phase = k * d_dot_p - c * time;
        
        // Gerstner Wave 高度公式
        height += wave.amplitude * std::sin(phase);
    }
    
    return height;
}

void WaterSurface::setWaveParameters(int waveCount, float amplitude, float wavelength, float speed) {
    m_waves.clear();
    
    // 生成多个不同方向的波浪
    for (int i = 0; i < waveCount; ++i) {
        float angle = (2.0f * glm::pi<float>() * i) / waveCount;
        WaveParams wave;
        wave.direction = glm::normalize(glm::vec2(std::cos(angle), std::sin(angle)));
        wave.amplitude = amplitude * (1.0f - i * 0.2f);  // 逐渐减小
        wave.wavelength = wavelength * (1.0f + i * 0.3f);
        wave.speed = speed * (1.0f - i * 0.15f);
        wave.steepness = 0.3f;
        m_waves.push_back(wave);
    }
}

} // namespace WaterTown
