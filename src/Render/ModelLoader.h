#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace WaterTown {

/**
 * @brief 网格数据结构
 */
struct Mesh {
    std::vector<float> vertices;  // 位置 + 法线（每顶点6个float）
    std::vector<unsigned int> indices;
    GLuint VAO, VBO, EBO;
    
    Mesh() : VAO(0), VBO(0), EBO(0) {}
    
    ~Mesh() {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (EBO) glDeleteBuffers(1, &EBO);
    }
    
    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        
        // 位置属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // 法线属性
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glBindVertexArray(0);
    }
};

/**
 * @brief 模型加载器（支持 GLB/GLTF 格式）
 */
class ModelLoader {
public:
    /**
     * @brief 加载模型文件
     */
    static Mesh* loadModel(const std::string& filePath);
};

} // namespace WaterTown
