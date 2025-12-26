#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace WaterTown {

/**
 * @brief 窗口管理类，封装 GLFW 窗口的创建、配置和事件处理
 */
class Window {
public:
    /**
     * @brief 构造函数，创建窗口并初始化 OpenGL 上下文
     * @param width 窗口宽度
     * @param height 窗口高度
     * @param title 窗口标题
     */
    Window(int width, int height, const char* title);
    
    /**
     * @brief 析构函数，清理 GLFW 资源
     */
    ~Window();
    
    // 禁止拷贝和赋值
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    
    /**
     * @brief 检查窗口是否应该关闭
     * @return 如果窗口应该关闭返回 true，否则返回 false
     */
    bool shouldClose() const;
    
    /**
     * @brief 处理窗口事件
     */
    void pollEvents();
    
    /**
     * @brief 交换前后缓冲区
     */
    void swapBuffers();
    
    /**
     * @brief 获取 GLFW 窗口指针
     * @return GLFW 窗口指针
     */
    GLFWwindow* getGLFWWindow() const { return m_window; }
    
    /**
     * @brief 设置窗口大小改变回调函数
     * @param callback 回调函数指针
     */
    void setResizeCallback(GLFWframebuffersizefun callback);
    
    /**
     * @brief 设置鼠标移动回调函数
     * @param callback 回调函数指针
     */
    void setCursorPosCallback(GLFWcursorposfun callback);
    
    /**
     * @brief 设置鼠标捕获模式
     * @param enabled 是否启用鼠标捕获
     */
    void setCursorCapture(bool enabled);
    
    /**
     * @brief 获取窗口宽度
     * @return 窗口宽度
     */
    int getWidth() const { return m_width; }
    
    /**
     * @brief 获取窗口高度
     * @return 窗口高度
     */
    int getHeight() const { return m_height; }
    
    /**
     * @brief 获取窗口宽高比
     * @return 宽高比
     */
    float getAspectRatio() const { return static_cast<float>(m_width) / static_cast<float>(m_height); }

private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
    std::string m_title;
    
    /**
     * @brief 初始化 GLFW
     * @return 初始化成功返回 true，否则返回 false
     */
    bool initGLFW();
    
    /**
     * @brief 初始化 GLAD
     * @return 初始化成功返回 true，否则返回 false
     */
    bool initGLAD();
};

} // namespace WaterTown
