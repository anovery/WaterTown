#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace WaterTown {

class Shader;
class Camera;
class Boat;

/**
 * @brief 船只渲染器，使用简单几何体表示船只
 */
class BoatRenderer {
public:
    BoatRenderer();
    ~BoatRenderer();
    
    /**
     * @brief 渲染船只
     * @param boat 船只对象
     * @param shader 着色器
     * @param camera 相机
     */
    void render(const Boat* boat, Shader* shader, Camera* camera);
    
private:
    GLuint m_vao;
    GLuint m_vbo;
    int m_vertexCount;
    
    /**
     * @brief 生成船只网格（船体 + 船帆）
     */
    void generateMesh();
};

} // namespace WaterTown
