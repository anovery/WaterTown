#pragma once

#include "Window.h"
#include <memory>

namespace WaterTown {

/**
 * @brief 应用程序基类，使用模板方法模式管理程序生命周期
 */
class Application {
public:
    /**
     * @brief 构造函数
     * @param width 窗口宽度
     * @param height 窗口高度
     * @param title 窗口标题
     */
    Application(int width, int height, const char* title);
    
    /**
     * @brief 虚析构函数
     */
    virtual ~Application();
    
    /**
     * @brief 运行应用程序主循环
     */
    void run();

protected:
    /**
     * @brief 初始化资源（派生类重写）
     * 在主循环开始之前调用，用于加载着色器、创建VAO/VBO等
     */
    virtual void onInit() {}
    
    /**
     * @brief 每帧更新逻辑（派生类重写）
     * @param deltaTime 帧间隔时间（秒）
     */
    virtual void onUpdate(float deltaTime) {}
    
    /**
     * @brief 渲染逻辑（派生类重写）
     * 在此方法中执行 OpenGL 绘制调用
     */
    virtual void onRender() {}
    
    /**
     * @brief ImGui 界面（派生类重写）
     * 在此方法中添加 ImGui 控件
     */
    virtual void onImGui() {}
    
    /**
     * @brief 清理资源（派生类重写）
     * 在程序退出前调用，用于释放 VAO/VBO、着色器等
     */
    virtual void onShutdown() {}
    
    /**
     * @brief 获取窗口指针
     * @return 窗口指针
     */
    Window* getWindow() const { return m_window.get(); }

private:
    std::unique_ptr<Window> m_window;
    float m_lastFrameTime;
    
    /**
     * @brief 初始化 ImGui
     */
    void initImGui();
    
    /**
     * @brief 清理 ImGui
     */
    void shutdownImGui();
};

} // namespace WaterTown
