#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

namespace WaterTown {

class Shader;
class Camera;

/**
 * @brief 水面渲染类，实现 Gerstner Waves 波浪效果
 */
class WaterSurface {
public:
    /**
     * @brief 构造函数
     * @param centerX 水面中心 X 坐标
     * @param centerZ 水面中心 Z 坐标
     * @param width 水面宽度
     * @param height 水面高度（Z 方向）
     * @param resolution 网格分辨率（每边的格子数）
     */
    WaterSurface(float centerX = 0.0f, float centerZ = 0.0f, 
                 float width = 20.0f, float height = 20.0f, 
                 int resolution = 100);
    
    /**
     * @brief 析构函数
     */
    ~WaterSurface();
    
    // 禁止拷贝
    WaterSurface(const WaterSurface&) = delete;
    WaterSurface& operator=(const WaterSurface&) = delete;
    
    /**
     * @brief 渲染水面
     * @param shader 水面着色器
     * @param camera 当前相机
     * @param time 当前时间（秒）
        * @param boatPos 船只世界坐标（用于水面裁剪，防止水出现在船板上）
        * @param boatCutoutInner 船只裁剪内半径（<=0 表示禁用）
        * @param boatCutoutOuter 船只裁剪外半径（用于羽化边缘，需 >= inner）
     */
        void render(Shader* shader,
                 Camera* camera,
                 float time,
                 const glm::vec3& boatPos = glm::vec3(0.0f),
                 float boatCutoutInner = 0.0f,
                 float boatCutoutOuter = 0.0f,
                 glm::vec2 boatForwardXZ = glm::vec2(0.0f, 1.0f),
                 glm::vec2 boatHalfExtentsXZ = glm::vec2(0.0f),
                 float boatCutoutFeather = 0.0f);

    /**
     * @brief 更新水面网格（用于自定义形状的水面）
     * @param vertices 顶点数据 (x, y, z, u, v) x N
     */
    void updateMesh(const std::vector<float>& vertices);
    
    /**
     * @brief 获取指定位置的水面高度（用于船只浮力计算）
     * @param x 世界坐标 X
     * @param z 世界坐标 Z
     * @param time 当前时间
     * @return 该位置的水面高度（Y 坐标）
     */
    float getWaterHeight(float x, float z, float time) const;
    
    /**
     * @brief 设置波浪参数
     * @param waveCount 波浪数量（1-4）
     * @param amplitude 波浪振幅
     * @param wavelength 波长
     * @param speed 波速
     */
    void setWaveParameters(int waveCount, float amplitude, float wavelength, float speed);
    
    /**
     * @brief 获取水面基准高度
     */
    float getBaseHeight() const { return m_baseHeight; }
    
    /**
     * @brief 设置水面基准高度
     */
    void setBaseHeight(float height) { m_baseHeight = height; }

private:
    // 网格数据
    unsigned int m_VAO, m_VBO, m_EBO;
    int m_vertexCount;
    int m_indexCount;
    bool m_useCustomMesh; // 是否使用自定义网格
    
    // 水面参数
    float m_centerX, m_centerZ;
    float m_width, m_height;
    float m_baseHeight;  // 水面基准高度
    int m_resolution;
    
    // Gerstner Waves 参数
    struct WaveParams {
        glm::vec2 direction;  // 波浪方向
        float amplitude;      // 振幅
        float wavelength;     // 波长
        float speed;          // 速度
        float steepness;      // 陡峭度
    };
    std::vector<WaveParams> m_waves;
    
    /**
     * @brief 生成水面网格
     */
    void generateMesh();
    
    /**
     * @brief 计算 Gerstner Wave 在指定点的高度
     */
    float calculateGerstnerHeight(float x, float z, float time) const;
};

} // namespace WaterTown
