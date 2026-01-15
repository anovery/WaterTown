#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace WaterTown {

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // 1. 从文件加载着色器源代码
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);
    
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    
    // 2. 编译着色器
    unsigned int vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);
    unsigned int fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);
    
    // 3. 链接着色器程序
    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertex);
    glAttachShader(m_programID, fragment);
    glLinkProgram(m_programID);
    checkCompileErrors(m_programID, "PROGRAM");
    
    // 4. 删除着色器对象（已经链接到程序中，不再需要）
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    std::cout << "Shader program created successfully (ID: " << m_programID << ")" << std::endl;
}

Shader::~Shader() {
    glDeleteProgram(m_programID);
}

void Shader::use() const {
    glUseProgram(m_programID);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_programID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(m_programID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(m_programID, name.c_str()), x, y, z);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(m_programID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(m_programID, name.c_str()), x, y);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

std::string Shader::loadShaderSource(const char* path) {
    std::string code;
    std::ifstream shaderFile;
    
    // 确保 ifstream 对象可以抛出异常
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        // 打开文件
        shaderFile.open(path);
        std::stringstream shaderStream;
        
        // 读取文件缓冲到数据流中
        shaderStream << shaderFile.rdbuf();
        
        // 关闭文件
        shaderFile.close();
        
        // 转换数据流到字符串
        code = shaderStream.str();
        
        std::cout << "Loaded shader: " << path << std::endl;
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << path << std::endl;
        std::cerr << "Exception: " << e.what() << std::endl;
        throw;
    }
    
    return code;
}

unsigned int Shader::compileShader(const char* source, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    std::string typeStr = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
    checkCompileErrors(shader, typeStr);
    
    return shader;
}

void Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];
    
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << std::endl;
            std::cerr << infoLog << std::endl;
            std::cerr << "========================================" << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << std::endl;
            std::cerr << infoLog << std::endl;
            std::cerr << "========================================" << std::endl;
        }
    }
}

} // namespace WaterTown
