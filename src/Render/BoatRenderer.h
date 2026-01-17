#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace WaterTown {

class Shader;
class Camera;
class Boat;
struct Mesh;

/**
 * @brief 船只渲染器，使用 boat.glb 3D 模型
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

    // 为水面裁剪提供“贴合船体”的参数（OBB 矩形），用于避免第1/2模式下船板/船舱看到水。
    // 返回值为 world-space 半长半宽（XZ 平面），已考虑渲染侧统一缩放。
    glm::vec2 getWaterCutoutHalfExtentsXZ(float extraMargin = 0.4f) const;
    bool hasWaterCutoutMetrics() const { return m_hasAutoHullMetrics; }
    
private:
    Mesh* m_boatMesh;
    glm::mat4 m_modelAxisCorrection;
    glm::vec3 m_modelLocalOffset;
    bool m_hasAutoModelTransform;

    // 自动分析得到的船体关键尺寸（用于实时计算水线）
    glm::vec3 m_autoCenter;
    float m_autoKeelY;
    float m_autoHullHeight;
    bool m_hasAutoHullMetrics;

    // 纠正坐标系后的“船体在 XZ 平面”的半长半宽（模型空间，未缩放）
    glm::vec2 m_autoHalfExtentsXZ;

    // 固定参数（来自你截图中的 UI 设定）
    static constexpr float kFixedSubmergeRatio = 0.106f;
    static constexpr float kFixedExtraLift = 0.0f;
    
    /**
     * @brief 加载 boat.glb 模型
     */
    void loadBoatModel();
    void computeAutoModelTransform();
};

} // namespace WaterTown
