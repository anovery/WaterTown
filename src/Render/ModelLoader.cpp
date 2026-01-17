#include "ModelLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <iostream>

namespace WaterTown {

Mesh* ModelLoader::loadModel(const std::string& filePath) {
    Assimp::Importer importer;
    
    // 加载模型，设置 post-processing flags
    const aiScene* scene = importer.ReadFile(
        filePath,
        aiProcess_Triangulate |          // 三角化所有面
        aiProcess_FlipUVs |              // 翻转 UV 坐标
        aiProcess_CalcTangentSpace |     // 计算切线空间
        aiProcess_GenNormals             // 生成法线（如果没有的话）
    );
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return nullptr;
    }
    
    Mesh* mesh = new Mesh();
    
    // 处理所有 Mesh
    for (unsigned int meshIdx = 0; meshIdx < scene->mNumMeshes; ++meshIdx) {
        aiMesh* aiMesh = scene->mMeshes[meshIdx];
        
        // 提取顶点数据（位置 + 法线）
        unsigned int vertexOffset = mesh->vertices.size() / 6;
        
        for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i) {
            // 位置（船模型底部可能是负数，需要向上移）
            mesh->vertices.push_back(aiMesh->mVertices[i].x);
            mesh->vertices.push_back(aiMesh->mVertices[i].y + 2.5f);  // 向上平移2.5单位
            mesh->vertices.push_back(aiMesh->mVertices[i].z);
            
            // 法线
            if (aiMesh->HasNormals()) {
                mesh->vertices.push_back(aiMesh->mNormals[i].x);
                mesh->vertices.push_back(aiMesh->mNormals[i].y);
                mesh->vertices.push_back(aiMesh->mNormals[i].z);
            } else {
                // 如果没有法线，使用默认值
                mesh->vertices.push_back(0.0f);
                mesh->vertices.push_back(1.0f);
                mesh->vertices.push_back(0.0f);
            }
        }
        
        // 提取索引
        for (unsigned int i = 0; i < aiMesh->mNumFaces; ++i) {
            aiFace face = aiMesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                mesh->indices.push_back(face.mIndices[j] + vertexOffset);
            }
        }
    }
    
    // 设置 VAO/VBO/EBO
    mesh->setupMesh();
    
    std::cout << "Model loaded successfully: " << filePath << std::endl;
    std::cout << "  Vertices: " << (mesh->vertices.size() / 6) << std::endl;
    std::cout << "  Indices: " << mesh->indices.size() << std::endl;
    
    return mesh;
}

} // namespace WaterTown
