#include "Application.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

namespace WaterTown {

Application::Application(int width, int height, const char* title)
    : m_lastFrameTime(0.0f) {
    
    // 创建窗口
    m_window = std::make_unique<Window>(width, height, title);
    
    // 初始化 ImGui
    initImGui();
    
    std::cout << "Application initialized successfully!" << std::endl;
}

Application::~Application() {
    onShutdown();
    shutdownImGui();
}

void Application::run() {
    // 调用派生类的初始化方法
    onInit();
    
    std::cout << "Entering main loop..." << std::endl;
    
    // 主循环
    while (!m_window->shouldClose()) {
        // 计算帧间隔时间
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - m_lastFrameTime;
        m_lastFrameTime = currentTime;
        
        // 更新逻辑
        onUpdate(deltaTime);
        
        // 清空屏幕
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // 渲染场景
        onRender();
        
        // 启动 ImGui 帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // 调用派生类的 ImGui 方法
        onImGui();
        
        // 渲染 ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // 交换缓冲区并处理事件
        m_window->swapBuffers();
        m_window->pollEvents();
    }
    
    std::cout << "Exiting main loop." << std::endl;
}

void Application::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // 设置 ImGui 样式
    ImGui::StyleColorsDark();
    
    // 初始化 ImGui 平台/渲染器后端
    ImGui_ImplGlfw_InitForOpenGL(m_window->getGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    std::cout << "ImGui initialized." << std::endl;
}

void Application::shutdownImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    std::cout << "ImGui shutdown." << std::endl;
}

} // namespace WaterTown
