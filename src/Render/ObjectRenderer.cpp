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
        // 底面
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        // 顶面
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
    const int segments = 16;
    const float radius = 0.5f;
    const float height = 1.0f;
    
    // 锥体侧面
    for (int i = 0; i < segments; ++i) {
        float angle1 = (i / (float)segments) * 2.0f * glm::pi<float>();
        float angle2 = ((i + 1) / (float)segments) * 2.0f * glm::pi<float>();
        
        float x1 = radius * cos(angle1);
        float z1 = radius * sin(angle1);
        float x2 = radius * cos(angle2);
        float z2 = radius * sin(angle2);
        
        // 计算侧面法线
        glm::vec3 v1(x1, 0, z1);
        glm::vec3 v2(x2, 0, z2);
        glm::vec3 top(0, height, 0);
        glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, top - v1));
        
        // 三角形
        vertices.push_back(0); vertices.push_back(height); vertices.push_back(0);
        vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
        
        vertices.push_back(x1); vertices.push_back(0); vertices.push_back(z1);
        vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
        
        vertices.push_back(x2); vertices.push_back(0); vertices.push_back(z2);
        vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
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
            case ObjectType::BRIDGE:
                renderBridge(obj.position, obj.rotation, shader);
                break;
            case ObjectType::TREE:
                renderTree(obj.position, obj.rotation, shader);
                break;
            case ObjectType::BOAT:
                renderBoat(obj.position, obj.rotation, shader);
                break;
        }
    }
}

void ObjectRenderer::renderHouse(const glm::vec3& position, float rotation, Shader* shader) {
    // 房子 = 白色立方体 + 灰色三角锥
    
    // 墙体（立方体）
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, 0.35f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.95f, 0.95f, 0.95f);  // 白色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 屋顶（锥体）
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, 0.7f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(1.7f, 0.8f, 1.7f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.5f, 0.5f, 0.5f);  // 灰色
    
    glBindVertexArray(m_coneVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_coneVertexCount);
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderBridge(const glm::vec3& position, float rotation, Shader* shader) {
    // 桥 = 灰色平板 + 柱子
    
    // 桥面（扁平的立方体）
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, 0.3f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(3.0f, 0.2f, 1.5f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.6f, 0.6f, 0.6f);  // 灰色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 左侧柱子
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(-1.2f, 0, 0));
    model = glm::scale(model, glm::vec3(0.3f, 0.6f, 0.3f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.5f, 0.5f, 0.5f);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // 右侧柱子
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(1.2f, 0, 0));
    model = glm::scale(model, glm::vec3(0.3f, 0.6f, 0.3f));
    
    shader->setMat4("uModel", model);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderTree(const glm::vec3& position, float rotation, Shader* shader) {
    // 树 = 棕色圆柱 + 绿色球体
    
    // 树干（圆柱）
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, 0.5f, 0));
    model = glm::scale(model, glm::vec3(0.3f, 1.0f, 0.3f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.4f, 0.25f, 0.1f);  // 棕色
    
    glBindVertexArray(m_cylinderVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
    
    // 树冠（球体）
    model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, 1.5f, 0));
    model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.2f, 0.7f, 0.2f);  // 绿色
    
    glBindVertexArray(m_sphereVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_sphereVertexCount);
    
    glBindVertexArray(0);
}

void ObjectRenderer::renderBoat(const glm::vec3& position, float rotation, Shader* shader) {
    // 船 = 棕色拉长立方体（已有专门的 BoatRenderer，这里简化版）
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, 0.3f, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(0.8f, 0.4f, 2.0f));
    
    shader->setMat4("uModel", model);
    shader->setVec3("uObjectColor", 0.5f, 0.3f, 0.1f);  // 棕色
    
    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
}

} // namespace WaterTown
