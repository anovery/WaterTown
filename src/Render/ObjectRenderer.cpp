#include "ObjectRenderer.h"
#include "Shader.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

namespace WaterTown {

ObjectRenderer::ObjectRenderer()
    : m_cubeVAO(0), m_cubeVBO(0), m_coneVAO(0), m_coneVBO(0),
      m_cylinderVAO(0), m_cylinderVBO(0), m_sphereVAO(0), m_sphereVBO(0),
      m_coneVertexCount(0), m_cylinderVertexCount(0), m_sphereVertexCount(0) {
    
    generateCube();
    generateCone();
    generateCylinder();
    generateSphere();
}

ObjectRenderer::~ObjectRenderer() {
    if (m_cubeVAO) glDeleteVertexArrays(1, &m_cubeVAO);
    if (m_cubeVBO) glDeleteBuffers(1, &m_cubeVBO);
    if (m_coneVAO) glDeleteVertexArrays(1, &m_coneVAO);
    if (m_coneVBO) glDeleteBuffers(1, &m_coneVBO);
    if (m_cylinderVAO) glDeleteVertexArrays(1, &m_cylinderVAO);
    if (m_cylinderVBO) glDeleteBuffers(1, &m_cylinderVBO);
    if (m_sphereVAO) glDeleteVertexArrays(1, &m_sphereVAO);
    if (m_sphereVBO) glDeleteBuffers(1, &m_sphereVBO);
}

void ObjectRenderer::generateCube() {
    float vertices[] = {
        // 位置 + 法线
        // 后面
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        // 前面
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        // 左面
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        // 右面
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        // 下面
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        // 上面
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    };
    
    glGenVertexArrays(1, &m_cubeVAO);
    glGenBuffers(1, &m_cubeVBO);
    
    glBindVertexArray(m_cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void ObjectRenderer::generateCone() {
    std::vector<float> vertices;
    const int segments = 32;
    const float radius = 0.5f;
    const float height = 1.0f;
    
    // 圆锥侧面
    for (int i = 0; i < segments; ++i) {
        float angle1 = (i / (float)segments) * 2.0f * glm::pi<float>();
        float angle2 = ((i + 1) / (float)segments) * 2.0f * glm::pi<float>();
        
        float x1 = radius * cos(angle1);
        float z1 = radius * sin(angle1);
        float x2 = radius * cos(angle2);
        float z2 = radius * sin(angle2);
        
        glm::vec3 normal1 = glm::normalize(glm::vec3(x1, height * 0.5f, z1));
        glm::vec3 normal2 = glm::normalize(glm::vec3(x2, height * 0.5f, z2));
        
        // 三角形：底部两点和顶点
        vertices.push_back(x1); vertices.push_back(0); vertices.push_back(z1);
        vertices.push_back(normal1.x); vertices.push_back(normal1.y); vertices.push_back(normal1.z);
        
        vertices.push_back(x2); vertices.push_back(0); vertices.push_back(z2);
        vertices.push_back(normal2.x); vertices.push_back(normal2.y); vertices.push_back(normal2.z);
        
        vertices.push_back(0); vertices.push_back(height); vertices.push_back(0);
        vertices.push_back(normal1.x); vertices.push_back(normal1.y); vertices.push_back(normal1.z);
    }
    
    m_coneVertexCount = vertices.size() / 6;
    
    glGenVertexArrays(1, &m_coneVAO);
    glGenBuffers(1, &m_coneVBO);
    
    glBindVertexArray(m_coneVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_coneVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void ObjectRenderer::generateCylinder() {
    std::vector<float> vertices;
    const int segments = 16;
    const float radius = 0.5f;
    const float height = 1.0f;
    
    // 圆柱侧面
    for (int i = 0; i < segments; ++i) {
        float angle1 = (i / (float)segments) * 2.0f * glm::pi<float>();
        float angle2 = ((i + 1) / (float)segments) * 2.0f * glm::pi<float>();
        
        float x1 = radius * cos(angle1);
        float z1 = radius * sin(angle1);
        float x2 = radius * cos(angle2);
        float z2 = radius * sin(angle2);
        
        glm::vec3 normal1 = glm::normalize(glm::vec3(x1, 0, z1));
        glm::vec3 normal2 = glm::normalize(glm::vec3(x2, 0, z2));
        
        // 两个三角形
        vertices.push_back(x1); vertices.push_back(0); vertices.push_back(z1);
        vertices.push_back(normal1.x); vertices.push_back(normal1.y); vertices.push_back(normal1.z);
        
        vertices.push_back(x2); vertices.push_back(0); vertices.push_back(z2);
        vertices.push_back(normal2.x); vertices.push_back(normal2.y); vertices.push_back(normal2.z);
        
        vertices.push_back(x2); vertices.push_back(height); vertices.push_back(z2);
        vertices.push_back(normal2.x); vertices.push_back(normal2.y); vertices.push_back(normal2.z);
        
        vertices.push_back(x1); vertices.push_back(0); vertices.push_back(z1);
        vertices.push_back(normal1.x); vertices.push_back(normal1.y); vertices.push_back(normal1.z);
        
        vertices.push_back(x2); vertices.push_back(height); vertices.push_back(z2);
        vertices.push_back(normal2.x); vertices.push_back(normal2.y); vertices.push_back(normal2.z);
        
        vertices.push_back(x1); vertices.push_back(height); vertices.push_back(z1);
        vertices.push_back(normal1.x); vertices.push_back(normal1.y); vertices.push_back(normal1.z);
    }
    
    m_cylinderVertexCount = vertices.size() / 6;
    
    glGenVertexArrays(1, &m_cylinderVAO);
    glGenBuffers(1, &m_cylinderVBO);
    
    glBindVertexArray(m_cylinderVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void ObjectRenderer::generateSphere() {
    std::vector<float> vertices;
    const int stacks = 10;
    const int slices = 16;
    const float radius = 0.5f;
    
    for (int i = 0; i < stacks; ++i) {
        float phi1 = glm::pi<float>() * i / stacks;
        float phi2 = glm::pi<float>() * (i + 1) / stacks;
        
        for (int j = 0; j < slices; ++j) {
            float theta1 = 2.0f * glm::pi<float>() * j / slices;
            float theta2 = 2.0f * glm::pi<float>() * (j + 1) / slices;
            
            // 四个顶点
            glm::vec3 v1(radius * sin(phi1) * cos(theta1), radius * cos(phi1), radius * sin(phi1) * sin(theta1));
            glm::vec3 v2(radius * sin(phi1) * cos(theta2), radius * cos(phi1), radius * sin(phi1) * sin(theta2));
            glm::vec3 v3(radius * sin(phi2) * cos(theta2), radius * cos(phi2), radius * sin(phi2) * sin(theta2));
            glm::vec3 v4(radius * sin(phi2) * cos(theta1), radius * cos(phi2), radius * sin(phi2) * sin(theta1));
            
            glm::vec3 n1 = glm::normalize(v1);
            glm::vec3 n2 = glm::normalize(v2);
            glm::vec3 n3 = glm::normalize(v3);
            glm::vec3 n4 = glm::normalize(v4);
            
            // 两个三角形
            vertices.push_back(v1.x); vertices.push_back(v1.y); vertices.push_back(v1.z);
            vertices.push_back(n1.x); vertices.push_back(n1.y); vertices.push_back(n1.z);
            
            vertices.push_back(v2.x); vertices.push_back(v2.y); vertices.push_back(v2.z);
            vertices.push_back(n2.x); vertices.push_back(n2.y); vertices.push_back(n2.z);
            
            vertices.push_back(v3.x); vertices.push_back(v3.y); vertices.push_back(v3.z);
            vertices.push_back(n3.x); vertices.push_back(n3.y); vertices.push_back(n3.z);
            
            vertices.push_back(v1.x); vertices.push_back(v1.y); vertices.push_back(v1.z);
            vertices.push_back(n1.x); vertices.push_back(n1.y); vertices.push_back(n1.z);
            
            vertices.push_back(v3.x); vertices.push_back(v3.y); vertices.push_back(v3.z);
            vertices.push_back(n3.x); vertices.push_back(n3.y); vertices.push_back(n3.z);
            
            vertices.push_back(v4.x); vertices.push_back(v4.y); vertices.push_back(v4.z);
            vertices.push_back(n4.x); vertices.push_back(n4.y); vertices.push_back(n4.z);
        }
    }
    
    m_sphereVertexCount = vertices.size() / 6;
    
    glGenVertexArrays(1, &m_sphereVAO);
    glGenBuffers(1, &m_sphereVBO);
    
    glBindVertexArray(m_sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void ObjectRenderer::addObject(ObjectType type, const glm::vec3& position, float rotation) {
    m_objects.push_back({type, position, rotation});
}

void ObjectRenderer::clear() {
    m_objects.clear();
}

void ObjectRenderer::render(Shader* shader, Camera* camera) {
    if (!shader || !camera) return;
    
    shader->use();
    shader->setMat4("uView", camera->getViewMatrix());
    shader->setMat4("uProjection", camera->getProjectionMatrix());
    shader->setVec3("uViewPos", camera->getPosition());
    shader->setVec3("uLightPos", 10.0f, 10.0f, 10.0f);
    shader->setVec3("uLightColor", 1.0f, 1.0f, 1.0f);
    
    for (const auto& obj : m_objects) {
        switch (obj.type) {
            case ObjectType::HOUSE:
                renderHouse(obj.position, obj.rotation, shader);
                break;
            case ObjectType::HOUSE_STYLE_1:
                renderHouseStyle1(obj.position, obj.rotation, shader);
                break;
            case ObjectType::HOUSE_STYLE_2:
                renderHouseStyle2(obj.position, obj.rotation, shader);
                break;
            case ObjectType::HOUSE_STYLE_3:
                renderHouseStyle3(obj.position, obj.rotation, shader);
                break;
            case ObjectType::HOUSE_STYLE_4:
                renderHouseStyle4(obj.position, obj.rotation, shader);
                break;
            case ObjectType::HOUSE_STYLE_5:
                renderHouseStyle5(obj.position, obj.rotation, shader);
                break;
            case ObjectType::BRIDGE:
                renderBridge(obj.position, obj.rotation, shader);
                break;
            case ObjectType::TREE:
                renderTree(obj.position, obj.rotation, shader);
                break;
            case ObjectType::BOAT:
                // 船由 BoatRenderer 单独处理
                break;
            case ObjectType::WALL:
                renderWall(obj.position, obj.rotation, shader);
                break;
            case ObjectType::PAVILION:
                renderPavilion(obj.position, obj.rotation, shader);
                break;
            case ObjectType::LONG_HOUSE:
                renderLongHouse(obj.position, obj.rotation, shader);
                break;
            case ObjectType::ARCH_BRIDGE:
                renderArchBridge(obj.position, obj.rotation, shader);
                break;
            case ObjectType::PAIFANG:
                renderPaifang(obj.position, obj.rotation, shader);
                break;
            case ObjectType::WATER_PAVILION:
                renderWaterPavilion(obj.position, obj.rotation, shader);
                break;
            case ObjectType::PIER:
                renderPier(obj.position, obj.rotation, shader);
                break;
            case ObjectType::TEMPLE:
                renderTemple(obj.position, obj.rotation, shader);
                break;
            case ObjectType::BAMBOO:
                renderBamboo(obj.position, obj.rotation, shader);
                break;
            case ObjectType::LOTUS_POND:
                renderLotusPond(obj.position, obj.rotation, shader);
                break;
            case ObjectType::FISHING_BOAT:
                renderFishingBoat(obj.position, obj.rotation, shader);
                break;
            case ObjectType::LANTERN:
                renderLantern(obj.position, obj.rotation, shader);
                break;
            case ObjectType::STONE_LION:
                renderStoneLion(obj.position, obj.rotation, shader);
                break;
        }
    }
}

void ObjectRenderer::renderHouse(const glm::vec3& position, float rotation, Shader* shader) {
    // 江南水乡特色民居：白墙黑瓦，飞檐翘角，木结构门窗
    
    // 基础尺寸
    float wallWidth = 3.0f;    // 墙体宽度（更宽敞）
    float wallDepth = 2.0f;    // 墙体深度
    float wallHeight = 2.2f;   // 墙体高度（更高）
    float roofHeight = 1.0f;   // 屋顶高度
    float roofOverhang = 0.5f; // 屋檐伸出长度（更大）
    float doorWidth = 0.6f;    // 门宽度
    float doorHeight = 1.8f;   // 门高度
    float windowSize = 0.4f;   // 窗户尺寸
    
    // 1. 主墙体（白墙）
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, wallHeight * 0.5f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(wallWidth, wallHeight, wallDepth));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.95f, 0.95f, 0.9f);  // 白灰墙
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 2. 屋顶主体（黑瓦）
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, wallHeight + roofHeight * 0.4f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(wallWidth + roofOverhang, roofHeight * 0.8f, wallDepth + roofOverhang));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.2f, 0.2f, 0.2f);  // 黑瓦
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 3. 飞檐（前檐翘角）
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, wallHeight + roofHeight * 0.6f, wallDepth * 0.7f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(wallWidth + roofOverhang * 1.8f, roofHeight * 0.3f, roofOverhang * 1.2f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.15f, 0.15f, 0.15f);  // 深黑瓦
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 4. 后檐
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, wallHeight + roofHeight * 0.6f, -wallDepth * 0.7f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(wallWidth + roofOverhang * 1.8f, roofHeight * 0.3f, roofOverhang * 1.2f));
    
    shader->setMat4("uModel", model);
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 5. 木门（深色）
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, doorHeight * 0.5f, wallDepth * 0.51f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(doorWidth, doorHeight, 0.05f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.3f, 0.2f, 0.1f);  // 深木色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 6. 窗户（两个侧面窗户）
    for (int i = 0; i < 2; i++) {
        float side = (i == 0) ? 1 : -1;
        model = glm::mat4(1.0f);
        model = glm::translate(model, position + glm::vec3(wallWidth * 0.35f * side, wallHeight * 0.6f, 0));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(windowSize, windowSize, 0.05f));
        
        shader->setMat4("uModel", model);
        shader->setVec3("uObjectColor", 0.4f, 0.6f, 0.8f);  // 浅蓝色窗户
        
        glBindVertexArray(m_cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
    // 7. 木柱支撑（四个角）
    for (int i = 0; i < 4; i++) {
        float x = (i % 2 == 0) ? wallWidth * 0.45f : -wallWidth * 0.45f;
        float z = (i / 2 == 0) ? wallDepth * 0.45f : -wallDepth * 0.45f;
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, position + glm::vec3(x, wallHeight * 0.5f, z));
        model = glm::scale(model, glm::vec3(0.1f, wallHeight, 0.1f));
        
        shader->setMat4("uModel", model);
        shader->setVec3("uObjectColor", 0.4f, 0.3f, 0.2f);  // 木柱色
        
        glBindVertexArray(m_cylinderVAO);
        glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    }
    
    glBindVertexArray(0);
}


void ObjectRenderer::renderLongHouse(const glm::vec3& position, float rotation, Shader* shader) {
    // 长屋 = 扩展的房子，长度更长
    
    // 基础尺寸
    float houseScale = 2.0f;      // 房子基础尺寸
    float houseHeight = 2.0f;     // 房子高度
    float houseRoofHeight = 1.0f; // 屋顶高度
    float houseRoofScale = 2.5f;  // 屋顶宽度
    float longHouseLength = 4.0f; // 长屋长度
    
    // 墙体（立方体）- 底部在地面上
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, houseHeight * 0.5f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(longHouseLength, houseHeight, houseScale));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.95f, 0.95f, 0.92f);  // 米白色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 屋顶（锥体）- 放在墙体顶部
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, houseHeight + houseRoofHeight * 0.5f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(longHouseLength * 1.1f, houseRoofHeight, houseRoofScale));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.5f, 0.5f, 0.5f);  // 灰色
    
    glBindVertexArray(m_coneVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_coneVertexCount);
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderHouseStyle4(const glm::vec3& position, float rotation, Shader* shader) {
    // 现代中式别墅：融合传统与现代的豪华住宅
    
    // 基础尺寸
    float baseWidth = 4.0f;   // 别墅宽度
    float baseDepth = 3.0f;   // 别墅深度
    float floorHeight = 2.5f; // 每层高度
    float roofHeight = 1.2f;  // 屋顶高度
    
    // 1. 主楼（两层）
    for (int floor = 0; floor < 2; floor++) {
        float yOffset = floor * floorHeight + floorHeight * 0.5f;
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position + glm::vec3(0, yOffset, 0));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(baseWidth, floorHeight, baseDepth));
        
        shader->setMat4("uModel", model);
        shader->setVec3("uObjectColor", 0.85f, 0.85f, 0.8f);  // 浅米色墙
        
        glBindVertexArray(m_cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
    // 2. 现代中式屋顶（平顶+翘角）
    // 主屋顶
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, floorHeight * 2 + roofHeight * 0.3f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(baseWidth + 0.5f, roofHeight * 0.6f, baseDepth + 0.5f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.2f, 0.2f, 0.2f);  // 深灰瓦
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 翘角装饰
    for (int i = 0; i < 4; i++) {
        float x = (i % 2 == 0) ? baseWidth * 0.6f : -baseWidth * 0.6f;
        float z = (i / 2 == 0) ? baseDepth * 0.6f : -baseDepth * 0.6f;
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, position + glm::vec3(x, floorHeight * 2 + roofHeight * 0.8f, z));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(0.3f, roofHeight * 0.4f, 0.3f));
        
        shader->setMat4("uModel", model);
        glBindVertexArray(m_cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
    // 3. 玻璃幕墙（现代元素）
    // 前 facade 玻璃
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, floorHeight * 0.8f, baseDepth * 0.51f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(baseWidth * 0.8f, floorHeight * 0.6f, 0.05f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.6f, 0.8f, 0.9f);  // 浅蓝玻璃
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 4. 古典柱子（现代简约风格）
    for (int i = 0; i < 4; i++) {
        float x = (i % 2 == 0) ? baseWidth * 0.4f : -baseWidth * 0.4f;
        float z = (i / 2 == 0) ? baseDepth * 0.4f : -baseDepth * 0.4f;
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, position + glm::vec3(x, floorHeight, z));
        model = glm::scale(model, glm::vec3(0.08f, floorHeight * 2, 0.08f));
        
        shader->setMat4("uModel", model);
        shader->setVec3("uObjectColor", 0.5f, 0.4f, 0.3f);  // 现代木色
        
        glBindVertexArray(m_cylinderVAO);
        glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    }
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderHouseStyle5(const glm::vec3& position, float rotation, Shader* shader) {
    // 古朴农舍：简朴的乡村住宅，茅草屋顶
    
    // 基础尺寸
    float shedWidth = 2.5f;   // 农舍宽度
    float shedDepth = 2.0f;   // 农舍深度
    float shedHeight = 1.8f;  // 农舍高度
    float roofHeight = 1.5f;  // 茅草屋顶高度
    
    // 1. 石砌基础
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, 0.1f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(shedWidth + 0.2f, 0.2f, shedDepth + 0.2f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.5f, 0.5f, 0.5f);  // 石灰色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 2. 木墙主体
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, shedHeight * 0.5f + 0.1f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(shedWidth, shedHeight, shedDepth));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.6f, 0.4f, 0.2f);  // 深木色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 3. 茅草屋顶（圆锥形）
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, shedHeight + roofHeight * 0.5f + 0.1f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(shedWidth + 0.8f, roofHeight, shedDepth + 0.8f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.4f, 0.3f, 0.1f);  // 茅草色
    
    glBindVertexArray(m_coneVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_coneVertexCount);
    
    // 4. 小门
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, shedHeight * 0.4f + 0.1f, shedDepth * 0.51f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(0.5f, shedHeight * 0.5f, 0.05f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.3f, 0.2f, 0.1f);  // 旧木门
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 5. 烟囱（小砖砌）
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(shedWidth * 0.3f, shedHeight + roofHeight * 0.8f + 0.1f, 0));
    model = glm::scale(model, glm::vec3(0.15f, roofHeight * 0.4f, 0.15f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.6f, 0.3f, 0.3f);  // 砖红色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 6. 木柱支撑
    for (int i = 0; i < 4; i++) {
        float x = (i % 2 == 0) ? shedWidth * 0.4f : -shedWidth * 0.4f;
        float z = (i / 2 == 0) ? shedDepth * 0.4f : -shedDepth * 0.4f;
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, position + glm::vec3(x, shedHeight * 0.5f + 0.1f, z));
        model = glm::scale(model, glm::vec3(0.08f, shedHeight, 0.08f));
        
        shader->setMat4("uModel", model);
        shader->setVec3("uObjectColor", 0.4f, 0.3f, 0.2f);  // 粗糙木色
        
        glBindVertexArray(m_cylinderVAO);
        glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    }
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderBridge(const glm::vec3& position, float rotation, Shader* shader) {
    // 石头 = 灰色立方体
    
    // 基础尺寸
    float bridgeScale = 1.5f;  // 石头尺寸
    float bridgeHeight = 1.0f; // 石头高度
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, bridgeHeight * 0.5f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(bridgeScale, bridgeHeight, bridgeScale));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.6f, 0.6f, 0.6f);  // 灰色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderTree(const glm::vec3& position, float rotation, Shader* shader) {
    // 树 = 棕色圆柱（树干） + 绿色球体（树冠）
    
    // 基础尺寸
    float treeScale = 0.3f;      // 树干粗细
    float treeHeight = 3.0f;     // 树干高度
    float treeCrownScale = 2.0f; // 树冠尺寸
    
    // 树干（圆柱）- 底部在地面上
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, treeHeight * 0.5f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(treeScale, treeHeight, treeScale));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.4f, 0.25f, 0.1f);  // 棕色
    
    glBindVertexArray(m_cylinderVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    
    // 树冠（球体）- 放在树干顶部
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, treeHeight + treeCrownScale * 0.3f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(treeCrownScale, treeCrownScale, treeCrownScale));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.2f, 0.7f, 0.2f);  // 绿色
    
    glBindVertexArray(m_sphereVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_sphereVertexCount);
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderWall(const glm::vec3& position, float rotation, Shader* shader) {
    // 围墙 = 灰色长方体
    
    // 基础尺寸
    float wallLength = 5.0f; // 围墙长度
    float wallHeight = 2.0f; // 围墙高度
    float wallWidth = 0.2f;  // 围墙宽度
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, wallHeight * 0.5f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(wallLength, wallHeight, wallWidth));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.6f, 0.6f, 0.6f);  // 灰色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void ObjectRenderer::renderPavilion(const glm::vec3& position, float rotation, Shader* shader) {
    // 凉亭 = 红色柱子 + 绿色屋顶
    float pavilionSize = 2.0f;
    float pavilionHeight = 2.5f;
    
    // 四个柱子
    for (int i = 0; i < 4; i++) {
        float angle = i * glm::pi<float>() * 0.5f;
        glm::vec3 offset = glm::vec3(cos(angle) * pavilionSize * 0.4f, pavilionHeight * 0.3f, sin(angle) * pavilionSize * 0.4f);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position + offset);
        model = glm::scale(model, glm::vec3(0.2f, pavilionHeight * 0.6f, 0.2f));
        
        shader->setMat4("uModel", model);
        shader->setVec3("uObjectColor", 0.8f, 0.3f, 0.3f);  // 红色
        
        glBindVertexArray(m_cylinderVAO);
        glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    }
    
    // 屋顶（圆锥）
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, pavilionHeight * 0.8f, 0));
    model = glm::scale(model, glm::vec3(pavilionSize * 0.8f, pavilionHeight * 0.4f, pavilionSize * 0.8f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.2f, 0.6f, 0.2f);  // 绿色
    
    glBindVertexArray(m_coneVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_coneVertexCount);
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderArchBridge(const glm::vec3& position, float rotation, Shader* shader) {
    // 拱桥 = 石灰色桥身 + 拱形结构
    
    // 基础尺寸
    float archBridgeLength = 8.0f; // 桥长度
    float archBridgeHeight = 3.0f; // 桥高度
    float archBridgeWidth = 2.0f;  // 桥宽度
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, archBridgeHeight * 0.3f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(archBridgeLength, archBridgeHeight * 0.6f, archBridgeWidth));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.7f, 0.7f, 0.6f);  // 石灰色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 拱形部分（多个半圆柱）
    for (int i = 0; i < 5; i++) {
        float x = (i - 2) * archBridgeLength * 0.2f;
        model = glm::mat4(1.0f);
        model = glm::translate(model, position + glm::vec3(x, archBridgeHeight * 0.8f, 0));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
        model = glm::scale(model, glm::vec3(archBridgeWidth * 0.3f, archBridgeLength * 0.15f, archBridgeWidth * 0.3f));
        
        shader->setMat4("uModel", model);
        glBindVertexArray(m_cylinderVAO);
        glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    }
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderHouseStyle1(const glm::vec3& position, float rotation, Shader* shader) {
    // 江南水乡特色民居：两层楼房，带天井
    
    // 基础尺寸
    float wallWidth = 3.0f;    // 墙体宽度
    float wallDepth = 2.0f;    // 墙体深度
    float wallHeight = 1.5f;   // 墙体高度
    float roofHeight = 0.6f;   // 屋顶高度
    float roofOverhang = 0.4f; // 屋檐伸出长度
    
    // 1. 底层墙体（白墙）
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, wallHeight * 0.5f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(wallWidth, wallHeight, wallDepth));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.9f, 0.9f, 0.85f);  // 白墙
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 2. 二层墙体（更小的）
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, wallHeight + 1.0f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(wallWidth * 0.8f, 0.8f, wallDepth * 0.8f));
    
    shader->setMat4("uModel", model);
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 3. 屋顶（黑瓦）
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, wallHeight * 2.0f + roofHeight * 0.3f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(wallWidth + roofOverhang, roofHeight, wallDepth + roofOverhang));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.25f, 0.25f, 0.25f);  // 黑瓦
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 4. 天井（中间空出的庭院）
    // 底层门廊
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, wallHeight * 0.7f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(wallWidth * 0.6f, wallHeight * 0.4f, wallDepth * 0.6f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.1f, 0.1f, 0.1f);  // 天井（深色表示阴影）
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 5. 柱子（木质）
    for (int i = 0; i < 4; i++) {
        float angle = i * glm::pi<float>() * 0.5f;
        glm::vec3 offset = glm::vec3(cos(angle) * wallWidth * 0.4f, wallHeight * 0.5f, sin(angle) * wallDepth * 0.4f);
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, position + offset);
        model = glm::scale(model, glm::vec3(0.1f, wallHeight, 0.1f));
        
        shader->setMat4("uModel", model);
        shader->setVec3("uObjectColor", 0.4f, 0.25f, 0.1f);  // 木色
        
        glBindVertexArray(m_cylinderVAO);
        glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    }
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderHouseStyle2(const glm::vec3& position, float rotation, Shader* shader) {
    // 精致庭院住宅：带花园的豪华住宅
    
    // 基础尺寸
    float mainWidth = 4.0f;   // 主建筑宽度
    float mainDepth = 2.5f;   // 主建筑深度
    float mainHeight = 2.0f;  // 主建筑高度
    float wingWidth = 2.0f;   // 侧翼宽度
    float wingDepth = 1.5f;   // 侧翼深度
    float roofHeight = 0.7f;  // 屋顶高度
    
    // 1. 主建筑
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, mainHeight * 0.5f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(mainWidth, mainHeight, mainDepth));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.85f, 0.85f, 0.8f);  // 浅白墙
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 2. 左侧翼
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(-mainWidth * 0.6f, mainHeight * 0.4f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(wingWidth, mainHeight * 0.8f, wingDepth));
    
    shader->setMat4("uModel", model);
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 3. 右侧翼
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(mainWidth * 0.6f, mainHeight * 0.4f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(wingWidth, mainHeight * 0.8f, wingDepth));
    
    shader->setMat4("uModel", model);
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 4. 精致屋顶（多层）
    // 主屋顶
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, mainHeight + roofHeight * 0.4f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(mainWidth + 0.5f, roofHeight * 0.8f, mainDepth + 0.5f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.2f, 0.2f, 0.2f);  // 深黑瓦
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 翼屋顶
    for (int i = 0; i < 2; i++) {
        float xOffset = (i == 0) ? -mainWidth * 0.6f : mainWidth * 0.6f;
        model = glm::mat4(1.0f);
        model = glm::translate(model, position + glm::vec3(xOffset, mainHeight * 0.8f + roofHeight * 0.3f, 0));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(wingWidth + 0.3f, roofHeight * 0.6f, wingDepth + 0.3f));
        
        shader->setMat4("uModel", model);
        glBindVertexArray(m_cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
    // 5. 花园装饰（小池塘）
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, 0.05f, mainDepth * 0.7f));
    model = glm::scale(model, glm::vec3(1.5f, 0.1f, 1.0f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.3f, 0.6f, 0.8f);  // 水蓝色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderHouseStyle3(const glm::vec3& position, float rotation, Shader* shader) {
    // 传统祠堂：庄严肃穆的家族祠堂
    
    // 基础尺寸
    float hallWidth = 5.0f;   // 大厅宽度
    float hallDepth = 3.0f;   // 大厅深度
    float hallHeight = 2.5f;  // 大厅高度
    float roofHeight = 1.0f;  // 屋顶高度
    float porchWidth = 2.0f;  // 门廊宽度
    float porchDepth = 1.0f;  // 门廊深度
    
    // 1. 主厅
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, hallHeight * 0.5f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(hallWidth, hallHeight, hallDepth));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.8f, 0.8f, 0.75f);  // 古旧白墙
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 2. 门廊
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, hallHeight * 0.3f, hallDepth * 0.6f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(porchWidth, hallHeight * 0.6f, porchDepth));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.1f, 0.1f, 0.1f);  // 门廊（深色）
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 3. 庄重屋顶（多重檐）
    // 底层屋顶
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, hallHeight + roofHeight * 0.3f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(hallWidth + 0.8f, roofHeight * 0.4f, hallDepth + 0.8f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.15f, 0.15f, 0.15f);  // 深黑瓦
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 上层屋顶
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, hallHeight + roofHeight * 0.8f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(hallWidth + 0.4f, roofHeight * 0.3f, hallDepth + 0.4f));
    
    shader->setMat4("uModel", model);
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 4. 柱子（粗壮的木柱）
    for (int i = 0; i < 6; i++) {
        float x = (i % 3 - 1) * hallWidth * 0.3f;
        float z = (i / 3) * hallDepth * 0.5f;
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, position + glm::vec3(x, hallHeight * 0.5f, z));
        model = glm::scale(model, glm::vec3(0.15f, hallHeight, 0.15f));
        
        shader->setMat4("uModel", model);
        shader->setVec3("uObjectColor", 0.3f, 0.2f, 0.1f);  // 深木色
        
        glBindVertexArray(m_cylinderVAO);
        glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    }
    
    // 5. 牌匾位置（装饰性立方体）
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, hallHeight * 0.8f, hallDepth * 0.52f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.05f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.8f, 0.6f, 0.2f);  // 金黄色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
}


void ObjectRenderer::renderPaifang(const glm::vec3& position, float rotation, Shader* shader) {
    // 牌坊 = 红色柱子和横梁
    
    // 基础尺寸
    float paifangWidth = 4.0f;  // 牌坊宽度
    float paifangHeight = 5.0f; // 牌坊高度
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, paifangHeight * 0.5f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(paifangWidth, paifangHeight, 0.3f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.9f, 0.2f, 0.2f);  // 深红色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 装饰性拱顶
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, paifangHeight * 0.9f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(paifangWidth * 0.8f, paifangHeight * 0.2f, 0.4f));
    
    shader->setMat4("uModel", model);
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderWaterPavilion(const glm::vec3& position, float rotation, Shader* shader) {
    // 水榭 = 建在水上的凉亭，带平台
    
    // 基础尺寸
    float waterPavilionSize = 6.0f; // 水榭尺寸
    
    // 平台
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, 0.1f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(waterPavilionSize, 0.2f, waterPavilionSize));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.8f, 0.8f, 0.7f);  // 浅灰色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 柱子和屋顶（类似凉亭但更精致）
    renderPavilion(position + glm::vec3(0, 0.2f, 0), rotation, shader);
}

void ObjectRenderer::renderPier(const glm::vec3& position, float rotation, Shader* shader) {
    // 码头 = 木质平台伸入水中
    float pierLength = 3.0f;  // 码头长度
    float pierWidth = 1.5f;   // 码头宽度
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, 0.05f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(pierLength, 0.1f, pierWidth));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.6f, 0.4f, 0.2f);  // 木色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 支撑柱子
    for (int i = 0; i < 6; i++) {
        float z = (i - 2.5f) * pierWidth * 0.3f;
        model = glm::mat4(1.0f);
        model = glm::translate(model, position + glm::vec3((i - 2.5f) * pierLength * 0.15f, -0.5f, z));
        model = glm::scale(model, glm::vec3(0.1f, 1.0f, 0.1f));
        
        shader->setMat4("uModel", model);
        glBindVertexArray(m_cylinderVAO);
        glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    }
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderTemple(const glm::vec3& position, float rotation, Shader* shader) {
    // 寺庙 = 多层建筑，带屋檐
    
    // 基础尺寸
    float templeSize = 4.0f;   // 寺庙基础尺寸
    float templeHeight = 3.5f; // 寺庙高度
    
    // 主体建筑
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, templeHeight * 0.4f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(templeSize, templeHeight * 0.8f, templeSize));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.8f, 0.8f, 0.6f);  // 米色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 屋顶
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, templeHeight * 0.9f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(templeSize * 1.2f, templeHeight * 0.3f, templeSize * 1.2f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.7f, 0.3f, 0.3f);  // 红色屋顶
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderBamboo(const glm::vec3& position, float rotation, Shader* shader) {
    // 竹子 = 绿色细长圆柱，带关节
    float bambooHeight = 2.5f;      // 竹子高度
    float bambooDensity = 0.8f;     // 竹子密度
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, bambooHeight * 0.5f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(bambooDensity * 0.1f, bambooHeight, bambooDensity * 0.1f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.3f, 0.8f, 0.3f);  // 竹绿色
    
    glBindVertexArray(m_cylinderVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    
    // 竹节（环状装饰）
    for (int i = 1; i < 4; i++) {
        float y = i * bambooHeight * 0.25f;
        model = glm::mat4(1.0f);
        model = glm::translate(model, position + glm::vec3(0, y, 0));
        model = glm::scale(model, glm::vec3(bambooDensity * 0.12f, bambooDensity * 0.02f, bambooDensity * 0.12f));
        
        shader->setMat4("uModel", model);
        glBindVertexArray(m_cylinderVAO);
        glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    }
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderLotusPond(const glm::vec3& position, float rotation, Shader* shader) {
    // 荷花池 = 水面 + 荷叶 + 荷花
    float lotusPondSize = 3.0f;     // 荷花池大小
    
    // 水面（浅蓝色圆形区域）
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, 0.01f, 0));
    model = glm::scale(model, glm::vec3(lotusPondSize, 0.02f, lotusPondSize));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.4f, 0.7f, 0.9f);  // 浅蓝色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 荷叶（绿色扁平圆形）
    for (int i = 0; i < 5; i++) {
        float angle = i * glm::pi<float>() * 0.4f;
        float radius = lotusPondSize * 0.3f + (i % 2) * lotusPondSize * 0.2f;
        glm::vec3 offset = glm::vec3(cos(angle) * radius, 0.05f, sin(angle) * radius);
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, position + offset);
        model = glm::scale(model, glm::vec3(0.5f, 0.01f, 0.5f));
        
        shader->setMat4("uModel", model);
        shader->setVec3("uObjectColor", 0.2f, 0.6f, 0.2f);  // 绿色
        
        glBindVertexArray(m_cylinderVAO);
        glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    }
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderFishingBoat(const glm::vec3& position, float rotation, Shader* shader) {
    // 渔船 = 小型船只，带桅杆
    float fishingBoatLength = 2.5f; // 渔船长度
    float fishingBoatWidth = 0.8f;  // 渔船宽度
    
    // 船身
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, 0.2f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(fishingBoatLength, 0.4f, fishingBoatWidth));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.6f, 0.4f, 0.2f);  // 木色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 桅杆
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, 1.5f, 0));
    model = glm::scale(model, glm::vec3(0.05f, 1.0f, 0.05f));
    
    shader->setMat4("uModel", model);
    glBindVertexArray(m_cylinderVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderLantern(const glm::vec3& position, float rotation, Shader* shader) {
    // 灯笼 = 红色圆柱 + 顶部装饰
    float lanternHeight = 1.2f;     // 灯笼高度
    float lanternSize = 0.3f;       // 灯笼大小
    
    // 灯笼主体
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, lanternHeight * 0.4f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(lanternSize, lanternHeight * 0.8f, lanternSize));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.9f, 0.2f, 0.2f);  // 红色
    
    glBindVertexArray(m_cylinderVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    
    // 顶部装饰
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, lanternHeight * 0.9f, 0));
    model = glm::scale(model, glm::vec3(lanternSize * 1.2f, lanternSize * 0.1f, lanternSize * 1.2f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.8f, 0.8f, 0.2f);  // 金色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderStoneLion(const glm::vec3& position, float rotation, Shader* shader) {
    // 石狮子 = 灰色雕像
    float stoneLionSize = 0.8f;     // 石狮子大小
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, stoneLionSize * 0.5f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(stoneLionSize, stoneLionSize, stoneLionSize));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.5f, 0.5f, 0.5f);  // 灰色
    
    glBindVertexArray(m_sphereVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_sphereVertexCount);
    
    glBindVertexArray(0);
}


} // namespace WaterTown
