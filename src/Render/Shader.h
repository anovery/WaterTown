#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

namespace WaterTown {

/**
 * @brief 着色器管理类，支持从文件加载、编译、链接着色器程序
 */
class Shader {
public:
    /**
     * @brief 构造函数，加载并编译着色器
     * @param vertexPath 顶点着色器文件路径
     * @param fragmentPath 片段着色器文件路径
     */
    Shader(const char* vertexPath, const char* fragmentPath);
    
    /**
     * @brief 析构函数，删除着色器程序
     */
    ~Shader();
    
    // 禁止拷贝和赋值
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    
    /**
     * @brief 使用/激活着色器程序
     */
    void use() const;
    
    /**
     * @brief 获取着色器程序 ID
     * @return 着色器程序 ID
     */
    unsigned int getID() const { return m_programID; }
    
    // Uniform 设置方法
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

private:
    unsigned int m_programID;
    
    /**
     * @brief 从文件加载着色器源代码
     * @param path 文件路径
     * @return 着色器源代码字符串
     */
    std::string loadShaderSource(const char* path);
    
    /**
     * @brief 编译着色器
     * @param source 着色器源代码
     * @param type 着色器类型（GL_VERTEX_SHADER 或 GL_FRAGMENT_SHADER）
     * @return 编译后的着色器 ID
     */
    unsigned int compileShader(const char* source, GLenum type);
    
    /**
     * @brief 检查编译/链接错误
     * @param shader 着色器或程序 ID
     * @param type 类型（"VERTEX", "FRAGMENT", "PROGRAM"）
     */
    void checkCompileErrors(unsigned int shader, const std::string& type);
};

} // namespace WaterTown
