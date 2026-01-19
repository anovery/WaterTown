#include "Core/Application.h"
#include "Render/Shader.h"
#include "Render/Camera.h"
#include "Render/BoatRenderer.h"
#include "Render/TerrainRenderer.h"
#include "Render/ObjectRenderer.h"
#include "Water/WaterSurface.h"
#include "Editor/SceneEditor.h"
#include "Editor/EditorUI.h"
#include "Physics/Boat.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <iostream>

using namespace WaterTown;

/**
 * @brief 场景编辑器应用
 */
class WaterTownApp : public Application {
public:
    WaterTownApp() : Application(1280, 720, "WaterTown - Scene Editor") {}
    
protected:
    void onInit() override {
        std::cout << "Initializing WaterTown App..." << std::endl;
        
        // 启用深度测试
        glEnable(GL_DEPTH_TEST);
        
        // 创建立方体顶点数据（位置 + 法线）
        createCubeData();
        
        // 加载着色器
        m_shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
        m_waterShader = new Shader("assets/shaders/water.vert", "assets/shaders/water.frag");
        m_grassShader = new Shader("assets/shaders/grass.vert", "assets/shaders/grass.frag");
        m_stoneShader = new Shader("assets/shaders/stone.vert", "assets/shaders/stone.frag");
        
        // 创建水面
        m_waterSurface = new WaterSurface(0.0f, 0.0f, 160.0f, 160.0f, 100); // 320 * 0.5 = 160
        m_waterSurface->setBaseHeight(SceneEditor::WATER_LEVEL);  // 水面高度
        
        // 创建场景编辑器
        m_sceneEditor = new SceneEditor();
        float aspectRatio = getWindow()->getAspectRatio();
        m_sceneEditor->init(aspectRatio);
        m_sceneEditor->setWaterSurface(m_waterSurface);
        
        // 创建船只渲染器
        m_boatRenderer = new BoatRenderer();
        
        // 创建地形渲染器
        m_terrainRenderer = new TerrainRenderer(SceneEditor::GRID_SIZE);
        
        // 创建物体渲染器
        m_objectRenderer = new ObjectRenderer();
        
        // 创建编辑器 UI
        m_editorUI = new EditorUI();
        m_editorUI->init(m_sceneEditor);
        
        // 使用编辑器的相机（默认从地形编辑模式开始）
        m_camera = m_sceneEditor->getCurrentCamera();
        
        // 设置窗口回调
        auto* window = getWindow()->getGLFWWindow();
        glfwSetWindowUserPointer(window, this);
        
        // 窗口大小改变回调
        auto resizeCallback = [](GLFWwindow* win, int width, int height) {
            glViewport(0, 0, width, height);
            auto* app = static_cast<WaterTownApp*>(glfwGetWindowUserPointer(win));
            if (app && app->m_sceneEditor) {
                float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
                app->m_sceneEditor->updateAspectRatio(aspectRatio);
            }
        };
        getWindow()->setResizeCallback(resizeCallback);
        
        // 不手动设置鼠标回调，让 ImGui 处理
        // 我们将在 onUpdate 中直接获取鼠标位置
        
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
        
        // 检测鼠标左键(用于地形/建筑编辑)
        bool wantCaptureMouse = ImGui::GetIO().WantCaptureMouse;
        static bool leftButtonPressed = false;
        int leftButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        
        // 检查是否按住 Ctrl 键
        bool ctrlPressed = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ||
                          (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);
        
        // Ctrl+Z 撤销快捷键
        static bool zKeyPressed = false;
        if (ctrlPressed && glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !zKeyPressed) {
            zKeyPressed = true;
            if (m_sceneEditor) {
                m_sceneEditor->undoLastAction();
            }
        }
        else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE) {
            zKeyPressed = false;
        }
        
        // 地形编辑模式:支持按住鼠标左键连续绘制
        if (m_sceneEditor && m_sceneEditor->getCurrentMode() == EditorMode::TERRAIN) {
            if (leftButtonState == GLFW_PRESS && !wantCaptureMouse) {
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                int width, height;
                glfwGetWindowSize(window, &width, &height);
                m_sceneEditor->handleMouseClick(static_cast<float>(xpos), static_cast<float>(ypos), width, height);
            }
        }
        // 建筑放置模式:单击放置
        else if (leftButtonState == GLFW_PRESS && !leftButtonPressed && !wantCaptureMouse) {
            leftButtonPressed = true;
            
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            
            if (m_sceneEditor) {
                if (ctrlPressed && m_sceneEditor->getCurrentMode() == EditorMode::BUILDING) {
                    // Ctrl + 左键:删除建筑物
                    int gridX, gridZ;
                    if (m_sceneEditor->raycastToGround(static_cast<float>(xpos), static_cast<float>(ypos), width, height, gridX, gridZ)) {
                        float cellSize = 0.5f;
                        float worldX = (gridX - 25 + 0.5f) * cellSize;
                        float worldZ = (gridZ - 25 + 0.5f) * cellSize;
                        m_sceneEditor->removeObjectNear(glm::vec3(worldX, 0.0f, worldZ), 1.0f);
                    }
                } else {
                    // 普通左键:放置建筑
                    m_sceneEditor->handleMouseClick(static_cast<float>(xpos), static_cast<float>(ypos), width, height);
                }
            }
        }
        
        if (leftButtonState == GLFW_RELEASE) {
            leftButtonPressed = false;
        }
        
        // 检测鼠标右键状态，但只在鼠标不在 ImGui 窗口上时捕获
        int rightButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
        bool shouldCapture = (rightButtonState == GLFW_PRESS) && !wantCaptureMouse;
        
        // 只有在状态改变时才更新鼠标捕获模式
        if (shouldCapture != m_mouseCaptured) {
            m_mouseCaptured = shouldCapture;
            getWindow()->setCursorCapture(m_mouseCaptured);
            
            // 重置鼠标位置，避免跳动
            if (m_mouseCaptured) {
                m_firstMouse = true;
            }
        }
        
        // 处理鼠标移动（当鼠标被捕获时）
        if (m_mouseCaptured && m_sceneEditor) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            
            if (m_firstMouse) {
                m_lastX = static_cast<float>(xpos);
                m_lastY = static_cast<float>(ypos);
                m_firstMouse = false;
            }
            
            float xoffset = static_cast<float>(xpos) - m_lastX;
            float yoffset = m_lastY - static_cast<float>(ypos);
            
            m_lastX = static_cast<float>(xpos);
            m_lastY = static_cast<float>(ypos);
            
            m_sceneEditor->handleMouseMovement(xoffset, yoffset, true);
        }
        
        // 检测中键拖动(用于建筑模式平移相机)
        static bool middleButtonPressed = false;
        static bool middleFirstMouse = true;
        static float middleLastX = 0.0f, middleLastY = 0.0f;
        int middleButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
        
        if (middleButtonState == GLFW_PRESS && !wantCaptureMouse && m_sceneEditor) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            
            if (!middleButtonPressed || middleFirstMouse) {
                middleLastX = static_cast<float>(xpos);
                middleLastY = static_cast<float>(ypos);
                middleFirstMouse = false;
                middleButtonPressed = true;
            } else {
                float xoffset = static_cast<float>(xpos) - middleLastX;
                float yoffset = static_cast<float>(ypos) - middleLastY;
                
                middleLastX = static_cast<float>(xpos);
                middleLastY = static_cast<float>(ypos);
                
                m_sceneEditor->handleMiddleMouseMovement(xoffset, yoffset);
            }
        }
        else if (middleButtonState == GLFW_RELEASE) {
            middleButtonPressed = false;
            middleFirstMouse = true;
        }
        
        // 游戏模式下处理船只控制 (WASD)
        if (m_sceneEditor && m_sceneEditor->getCurrentMode() == EditorMode::GAME) {
            float forward = 0.0f;
            float turn = 0.0f;
            
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) forward += 1.0f;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) forward -= 1.0f;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) turn += 1.0f;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) turn -= 1.0f;
            
            m_sceneEditor->handleGameInput(forward, turn);
        }
        
        // 更新场景编辑器
        if (m_sceneEditor) {
            m_sceneEditor->update(deltaTime);
            m_camera = m_sceneEditor->getCurrentCamera();  // 更新当前相机
        }
    }
    
    void onRender() override {
        if (!m_shader || !m_camera) return;
        
        // === 旋转立方体已注释 ===
        // m_shader->use();
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        // glm::mat4 view = m_camera->getViewMatrix();
        // glm::mat4 projection = m_camera->getProjectionMatrix();
        // m_shader->setMat4("uModel", model);
        // m_shader->setMat4("uView", view);
        // m_shader->setMat4("uProjection", projection);
        // glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        // m_shader->setVec3("uLightPos", lightPos);
        // m_shader->setVec3("uViewPos", m_camera->getPosition());
        // m_shader->setVec3("uObjectColor", 1.0f, 0.5f, 0.31f);
        // m_shader->setVec3("uLightColor", 1.0f, 1.0f, 1.0f);
        // glBindVertexArray(m_cubeVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0);
        
        // === 渲染地形网格(所有模式) ===
        if (m_sceneEditor && m_terrainRenderer) {
            if (m_sceneEditor->getCurrentMode() == EditorMode::TERRAIN) {
                // 地形编辑模式：使用纯色着色器渲染所有地形
                m_terrainRenderer->render(m_sceneEditor, m_shader, m_camera);
            } else {
                // 建筑/游戏模式：分类型使用独立着色器渲染
                if (m_grassShader) {
                    m_terrainRenderer->renderByType(m_sceneEditor, m_grassShader, m_camera, TerrainType::GRASS);
                }
                if (m_stoneShader) {
                    m_terrainRenderer->renderByType(m_sceneEditor, m_stoneShader, m_camera, TerrainType::STONE);
                }
            }
        }
        
        // === 渲染放置的物体(所有模式) ===
        if (m_sceneEditor && m_objectRenderer && m_shader) {
            m_objectRenderer->clear();
            const auto& objects = m_sceneEditor->getPlacedObjects();
            for (const auto& obj : objects) {
                m_objectRenderer->addObject(obj.first, obj.second);
            }
            m_objectRenderer->render(m_shader, m_camera);
        }
        
        // === 渲染水面(仅在非地形编辑模式) ===
        if (m_waterSurface && m_waterShader && m_sceneEditor) {
            if (m_sceneEditor->getCurrentMode() != EditorMode::TERRAIN) {
                m_waterSurface->render(m_waterShader, m_camera, static_cast<float>(glfwGetTime()));
            }
        }
        
        // === 渲染船只(建筑模式和游戏模式) ===
        if (m_sceneEditor && m_boatRenderer && m_shader) {
            EditorMode mode = m_sceneEditor->getCurrentMode();
            if (mode == EditorMode::GAME) {
                // 游戏模式:渲染可控船只
                if (m_sceneEditor->getBoat()) {
                    m_boatRenderer->render(m_sceneEditor->getBoat(), m_shader, m_camera);
                }
            }
            else if (mode == EditorMode::BUILDING && m_sceneEditor->hasBoatPlaced()) {
                // 建筑模式:渲染已放置的船只(静态显示)
                // 使用放置位置创建临时Boat渲染
                Boat tempBoat(m_sceneEditor->getBoatPlacedPosition(), m_sceneEditor->getBoatPlacedRotation());
                m_boatRenderer->render(&tempBoat, m_shader, m_camera);
            }
        }
    }
    
    void onImGui() override {
        // 使用编辑器 UI
        if (m_editorUI) {
            m_editorUI->render();
        }
    }
    
    void onShutdown() override {
        std::cout << "Shut down" << std::endl;
        
        // 清理资源
        if (m_cubeVAO) {
            glDeleteVertexArrays(1, &m_cubeVAO);
            glDeleteBuffers(1, &m_cubeVBO);
        }
        
        delete m_shader;
        delete m_waterShader;
        delete m_terrainShader;
        delete m_grassShader;
        delete m_stoneShader;
        delete m_waterSurface;
        delete m_sceneEditor;
        delete m_editorUI;
        delete m_boatRenderer;
        delete m_terrainRenderer;
        delete m_objectRenderer;
        // 注意：m_camera 由 SceneEditor 管理，不需要单独删除
        
        std::cout << "WaterTown Demo shutdown complete." << std::endl;
    }

private:
    Shader* m_shader = nullptr;
    Shader* m_waterShader = nullptr;
    Shader* m_terrainShader = nullptr;
    Shader* m_grassShader = nullptr;
    Shader* m_stoneShader = nullptr;
    WaterSurface* m_waterSurface = nullptr;
    SceneEditor* m_sceneEditor = nullptr;
    EditorUI* m_editorUI = nullptr;
    BoatRenderer* m_boatRenderer = nullptr;
    TerrainRenderer* m_terrainRenderer = nullptr;
    ObjectRenderer* m_objectRenderer = nullptr;
    Camera* m_camera = nullptr;  // 指向当前相机（由 SceneEditor 管理）
    
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
