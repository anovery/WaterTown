#include "Core/Application.h"
#include "Render/Shader.h"
#include "Render/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <iostream>

using namespace WaterTown;

/**
 * @brief 渲染带光照的立方体
 */
class WaterTownApp : public Application {
public:
    WaterTownApp() : Application(1280, 720, "WaterTown") {}
    
protected:
    void onInit() override {
        std::cout << "Initializing WaterTown App..." << std::endl;
        
        // 启用深度测试
        glEnable(GL_DEPTH_TEST);
        
        // 创建立方体顶点数据（位置 + 法线）
        createCubeData();
        
        // 加载着色器
        m_shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
        
        // 创建相机
        float aspectRatio = getWindow()->getAspectRatio();
        m_camera = new FreeCamera(glm::vec3(0.0f, 0.0f, 5.0f), 45.0f, aspectRatio);
        
        // 设置窗口回调
        auto* window = getWindow()->getGLFWWindow();
        glfwSetWindowUserPointer(window, this);
        
        // 窗口大小改变回调
        auto resizeCallback = [](GLFWwindow* win, int width, int height) {
            glViewport(0, 0, width, height);
            auto* app = static_cast<WaterTownApp*>(glfwGetWindowUserPointer(win));
            if (app && app->m_camera) {
                float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
                app->m_camera->updateAspectRatio(aspectRatio);
            }
        };
        getWindow()->setResizeCallback(resizeCallback);
        
        // 鼠标移动回调
        auto cursorCallback = [](GLFWwindow* win, double xpos, double ypos) {
            auto* app = static_cast<WaterTownApp*>(glfwGetWindowUserPointer(win));
            if (!app) return;
            
            if (app->m_firstMouse) {
                app->m_lastX = static_cast<float>(xpos);
                app->m_lastY = static_cast<float>(ypos);
                app->m_firstMouse = false;
            }
            
            float xoffset = static_cast<float>(xpos) - app->m_lastX;
            float yoffset = app->m_lastY - static_cast<float>(ypos);
            
            app->m_lastX = static_cast<float>(xpos);
            app->m_lastY = static_cast<float>(ypos);
            
            if (app->m_camera && app->m_mouseCaptured) {
                app->m_camera->processMouseMovement(xoffset, yoffset);
            }
        };
        getWindow()->setCursorPosCallback(cursorCallback);
        
        // 默认不启用鼠标捕获，按住鼠标右键时才启用
        m_mouseCaptured = false;
        getWindow()->setCursorCapture(false);
        
        std::cout << "WaterTown App initialized successfully!" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  WASD - Move camera" << std::endl;
        std::cout << "  Space/Shift - Up/Down" << std::endl;
        std::cout << "  Hold Right Mouse Button - Look around" << std::endl;
        std::cout << "  ESC - Exit" << std::endl;
    }
    
    void onUpdate(float deltaTime) override {
        auto* window = getWindow()->getGLFWWindow();
        
        // 处理键盘输入
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        // 检测鼠标右键状态
        int rightButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
        bool shouldCapture = (rightButtonState == GLFW_PRESS);
        
        // 只有在状态改变时才更新鼠标捕获模式
        if (shouldCapture != m_mouseCaptured) {
            m_mouseCaptured = shouldCapture;
            getWindow()->setCursorCapture(m_mouseCaptured);
            
            // 重置鼠标位置，避免跳动
            if (m_mouseCaptured) {
                m_firstMouse = true;
            }
        }
        
        // 相机移动
        if (m_camera) {
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                m_camera->processKeyboard(GLFW_KEY_W, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                m_camera->processKeyboard(GLFW_KEY_S, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                m_camera->processKeyboard(GLFW_KEY_A, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                m_camera->processKeyboard(GLFW_KEY_D, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                m_camera->processKeyboard(GLFW_KEY_SPACE, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                m_camera->processKeyboard(GLFW_KEY_LEFT_SHIFT, deltaTime);
        }
    }
    
    void onRender() override {
        if (!m_shader || !m_camera) return;
        
        // 使用着色器
        m_shader->use();
        
        // 设置 MVP 矩阵
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        
        glm::mat4 view = m_camera->getViewMatrix();
        glm::mat4 projection = m_camera->getProjectionMatrix();
        
        m_shader->setMat4("uModel", model);
        m_shader->setMat4("uView", view);
        m_shader->setMat4("uProjection", projection);
        
        // 设置光照参数
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        m_shader->setVec3("uLightPos", lightPos);
        m_shader->setVec3("uViewPos", m_camera->getPosition());
        m_shader->setVec3("uObjectColor", 1.0f, 0.5f, 0.31f);  // 橙色立方体
        m_shader->setVec3("uLightColor", 1.0f, 1.0f, 1.0f);    // 白光
        
        // 绘制立方体
        glBindVertexArray(m_cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
    
    void onImGui() override {
        ImGui::Begin("WaterTown Demo");
        
        ImGui::Text("Basic Rendering System");
        ImGui::Separator();
        
        ImGui::Text("Performance");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
        
        ImGui::Separator();
        ImGui::Text("Camera Info");
        if (m_camera) {
            glm::vec3 pos = m_camera->getPosition();
            ImGui::Text("Position: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
        }
        ImGui::Text("Mouse Captured: %s", m_mouseCaptured ? "Yes" : "No");
        
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Status: All Systems OK!");
        
        ImGui::Separator();
        ImGui::Text("Controls:");
        ImGui::BulletText("WASD - Move camera");
        ImGui::BulletText("Space/Shift - Up/Down");
        ImGui::BulletText("Hold Right Mouse - Look around");
        ImGui::BulletText("ESC - Exit");
        
        ImGui::Separator();
        ImGui::Text("Demo Checklist:");
        ImGui::BulletText("Window system");
        ImGui::BulletText("Shader system");
        ImGui::BulletText("Camera system");
        ImGui::BulletText("Phong lighting");
        ImGui::BulletText("ImGui integration");
        
        ImGui::End();
    }
    
    void onShutdown() override {
        std::cout << "Shut down" << std::endl;
        
        // 清理资源
        if (m_cubeVAO) {
            glDeleteVertexArrays(1, &m_cubeVAO);
            glDeleteBuffers(1, &m_cubeVBO);
        }
        
        delete m_shader;
        delete m_camera;
        
        std::cout << "WaterTown Demo shutdown complete." << std::endl;
    }

private:
    Shader* m_shader = nullptr;
    FreeCamera* m_camera = nullptr;
    
    unsigned int m_cubeVAO = 0;
    unsigned int m_cubeVBO = 0;
    
    bool m_firstMouse = true;
    float m_lastX = 640.0f;
    float m_lastY = 360.0f;
    bool m_mouseCaptured = false;
    
    void createCubeData() {
        // 立方体顶点数据（位置 + 法线）
        float vertices[] = {
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
        
        // 创建 VAO 和 VBO
        glGenVertexArrays(1, &m_cubeVAO);
        glGenBuffers(1, &m_cubeVBO);
        
        glBindVertexArray(m_cubeVAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        // 位置属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // 法线属性
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glBindVertexArray(0);
        
        std::cout << "Cube VAO/VBO created successfully." << std::endl;
    }
};

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "WaterTown - Basic Rendering System" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        WaterTownApp app;
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return -1;
    }
    
    std::cout << "========================================" << std::endl;
    std::cout << "Program exited successfully." << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}
