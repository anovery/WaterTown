#include "BoatRenderer.h"
#include "ModelLoader.h"
#include "Shader.h"
#include "Camera.h"
#include "../Physics/Boat.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cfloat>
#include <algorithm>
#include <vector>
#include <iostream>

namespace WaterTown {

BoatRenderer::BoatRenderer()
    : m_boatMesh(nullptr),
      m_modelAxisCorrection(1.0f),
      m_modelLocalOffset(0.0f),
            m_hasAutoModelTransform(false),
            m_autoCenter(0.0f),
            m_autoKeelY(0.0f),
            m_autoHullHeight(1.0f),
                        m_hasAutoHullMetrics(false),
                        m_autoHalfExtentsXZ(0.0f) {
    loadBoatModel();
}

BoatRenderer::~BoatRenderer() {
    delete m_boatMesh;
}

void BoatRenderer::loadBoatModel() {
    m_boatMesh = ModelLoader::loadModel("assets/models/boat.glb");
    
    if (!m_boatMesh) {
        std::cerr << "Failed to load boat model!" << std::endl;
        return;
    }

    computeAutoModelTransform();
}

void BoatRenderer::computeAutoModelTransform() {
    m_modelAxisCorrection = glm::mat4(1.0f);
    m_modelLocalOffset = glm::vec3(0.0f);
    m_hasAutoModelTransform = false;

    m_autoCenter = glm::vec3(0.0f);
    m_autoKeelY = 0.0f;
    m_autoHullHeight = 1.0f;
    m_hasAutoHullMetrics = false;
    m_autoHalfExtentsXZ = glm::vec2(0.0f);

    if (!m_boatMesh || m_boatMesh->vertices.size() < 6) return;

    glm::vec3 rawMin(FLT_MAX), rawMax(-FLT_MAX);
    for (size_t i = 0; i + 5 < m_boatMesh->vertices.size(); i += 6) {
        glm::vec3 p(m_boatMesh->vertices[i + 0], m_boatMesh->vertices[i + 1], m_boatMesh->vertices[i + 2]);
        rawMin = glm::min(rawMin, p);
        rawMax = glm::max(rawMax, p);
    }

    glm::vec3 rawExt = rawMax - rawMin;

    // 按你的模型特征：最长=前后，次高=高度，最窄=左右
    // => lengthAxis = max, widthAxis = min, heightAxis = remaining
    int widthAxis = 1;
    if (rawExt.x <= rawExt.y && rawExt.x <= rawExt.z) widthAxis = 0;
    else if (rawExt.z <= rawExt.x && rawExt.z <= rawExt.y) widthAxis = 2;

    int lengthAxis = 1;
    if (rawExt.x >= rawExt.y && rawExt.x >= rawExt.z) lengthAxis = 0;
    else if (rawExt.z >= rawExt.x && rawExt.z >= rawExt.y) lengthAxis = 2;

    int heightAxis = 3 - widthAxis - lengthAxis;

    // corrected.x = model[widthAxis]
    // corrected.y = model[heightAxis]
    // corrected.z = model[lengthAxis]
    glm::mat4 permute(0.0f);
    permute[3][3] = 1.0f;
    permute[widthAxis][0] = 1.0f;
    permute[heightAxis][1] = 1.0f;
    permute[lengthAxis][2] = 1.0f;

    // 先用置换后的坐标做一次 bbox，用顶面法线判断是否上下颠倒
    glm::vec3 pMin(FLT_MAX), pMax(-FLT_MAX);
    for (size_t i = 0; i + 5 < m_boatMesh->vertices.size(); i += 6) {
        glm::vec3 p(m_boatMesh->vertices[i + 0], m_boatMesh->vertices[i + 1], m_boatMesh->vertices[i + 2]);
        glm::vec3 pf = glm::vec3(permute * glm::vec4(p, 1.0f));
        pMin = glm::min(pMin, pf);
        pMax = glm::max(pMax, pf);
    }

    glm::vec3 pExt = pMax - pMin;
    float pHeight = pExt.y;
    if (pHeight < 1e-4f) pHeight = 1.0f;
    float topY = pMax.y;
    float topBand = std::max(0.02f * pHeight, 1e-4f);

    glm::vec3 avgTopNormal(0.0f);
    int topCount = 0;
    for (size_t i = 0; i + 5 < m_boatMesh->vertices.size(); i += 6) {
        glm::vec3 p(m_boatMesh->vertices[i + 0], m_boatMesh->vertices[i + 1], m_boatMesh->vertices[i + 2]);
        glm::vec3 n(m_boatMesh->vertices[i + 3], m_boatMesh->vertices[i + 4], m_boatMesh->vertices[i + 5]);
        glm::vec3 pf = glm::vec3(permute * glm::vec4(p, 1.0f));
        if (pf.y >= topY - topBand) {
            glm::vec3 nf = glm::normalize(glm::vec3(permute * glm::vec4(n, 0.0f)));
            avgTopNormal += nf;
            topCount++;
        }
    }

    bool upsideDown = false;
    if (topCount > 0) {
        avgTopNormal /= static_cast<float>(topCount);
        upsideDown = avgTopNormal.y < 0.0f;
    }

    glm::mat4 axisFix = permute;
    if (upsideDown) {
        // 绕 Z 翻转 180°：不改变前后(Z)，只把上下(Y)翻回来
        axisFix = glm::rotate(glm::mat4(1.0f), glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)) * axisFix;
    }

    glm::vec3 fixedMin(FLT_MAX), fixedMax(-FLT_MAX);
    for (size_t i = 0; i + 5 < m_boatMesh->vertices.size(); i += 6) {
        glm::vec3 p(m_boatMesh->vertices[i + 0], m_boatMesh->vertices[i + 1], m_boatMesh->vertices[i + 2]);
        glm::vec3 pf = glm::vec3(axisFix * glm::vec4(p, 1.0f));
        fixedMin = glm::min(fixedMin, pf);
        fixedMax = glm::max(fixedMax, pf);
    }

    glm::vec3 fixedExt = fixedMax - fixedMin;
    glm::vec3 center = (fixedMin + fixedMax) * 0.5f;

    // 记录 XZ 半长半宽（纠正后的坐标系：X=左右宽，Z=前后长）
    // 用于水面 OBB 裁剪，避免船舱/甲板看到水。
    m_autoHalfExtentsXZ = glm::vec2(std::max(0.0f, fixedExt.x * 0.5f), std::max(0.0f, fixedExt.z * 0.5f));

    // 计算更鲁棒的水线：剔除顶部桅杆/装饰等离群点，避免把“整体高度”算得过大
    std::vector<float> ys;
    ys.reserve(m_boatMesh->vertices.size() / 6);
    for (size_t i = 0; i + 5 < m_boatMesh->vertices.size(); i += 6) {
        glm::vec3 p(m_boatMesh->vertices[i + 0], m_boatMesh->vertices[i + 1], m_boatMesh->vertices[i + 2]);
        glm::vec3 pf = glm::vec3(axisFix * glm::vec4(p, 1.0f));
        ys.push_back(pf.y);
    }

    // 底部(龙骨)用真实最小值，避免“底部顶点占比太少”导致 yLow 被抬高。
    float yKeel = fixedMin.y;
    float yDeck = fixedMax.y;

    // 顶部用 98% 分位数：排除桅杆/旗杆/装饰等离群点，否则水线会被拉得过高，整船看起来沉没。
    if (ys.size() >= 10) {
        const float topQuantile = 0.98f;
        size_t n = ys.size();
        size_t deckIdx = static_cast<size_t>(topQuantile * static_cast<float>(n));
        if (deckIdx >= n) deckIdx = n - 1;
        std::nth_element(ys.begin(), ys.begin() + deckIdx, ys.end());
        yDeck = ys[deckIdx];
        if (yDeck - yKeel < 1e-4f) {
            yDeck = fixedMax.y;
        }
    }

    float hullHeight = (yDeck - yKeel);
    if (hullHeight < 1e-4f) hullHeight = std::max(fixedExt.y, 1.0f);

    // 记录船体关键数据，水线由 UI 实时控制
    m_autoCenter = center;
    m_autoKeelY = yKeel;
    m_autoHullHeight = hullHeight;
    m_hasAutoHullMetrics = true;

    float waterlineY = yKeel + hullHeight * kFixedSubmergeRatio;

    m_modelAxisCorrection = axisFix;
    m_modelLocalOffset = glm::vec3(-center.x, -waterlineY, -center.z);
    m_hasAutoModelTransform = true;
}

glm::vec2 BoatRenderer::getWaterCutoutHalfExtentsXZ(float extraMargin) const {
    // 与渲染一致的统一缩放
    const float uniformScale = 0.75f;

    glm::vec2 halfExt = m_autoHalfExtentsXZ * uniformScale;
    if (extraMargin > 0.0f) {
        halfExt += glm::vec2(extraMargin);
    }
    return halfExt;
}

void BoatRenderer::render(const Boat* boat, Shader* shader, Camera* camera) {
    if (!boat || !shader || !camera || !m_boatMesh) {
        return;
    }
    
    shader->use();
    
    // 获取船只位置和旋转
    glm::vec3 position = boat->getPosition();
    position.y += kFixedExtraLift;
    float rotation = boat->getRotation();
    
    // 构建模型矩阵（正确的变换顺序：平移 -> 旋转 -> 缩放）
    glm::mat4 model = glm::mat4(1.0f);
    
    // 1. 平移到世界坐标（使用物理计算的 Y 坐标，已包含水面高度 + 1.2f）
    model = glm::translate(model, position);
    
    // 2. 绕 Y 轴旋转 (添加 +180° 使模型船头与物理运动方向一致)
    // 物理上 rotation=0° 船向 +Z 移动，但自动轴校正后模型船头朝 -Z
    model = glm::rotate(model, glm::radians(rotation + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // 3. 缩放模型（统一缩放）
    const glm::vec3 scale(0.75f, 0.75f, 0.75f);

    // 3.5 水线/居中偏移：在“纠正后的坐标系”里沿 Y 轴移动，避免表现成前后移动
    if (m_hasAutoModelTransform) {
        glm::vec3 localOffset(0.0f);
        if (m_hasAutoHullMetrics) {
            float ratio = kFixedSubmergeRatio;
            float waterlineY = m_autoKeelY + m_autoHullHeight * ratio;
            localOffset = glm::vec3(-m_autoCenter.x, -waterlineY, -m_autoCenter.z);
        } else {
            localOffset = m_modelLocalOffset;
        }

        // localOffset 是“未缩放的模型单位”，这里换算到缩放后的空间（uniform scale）
        localOffset *= scale.x;

        model = glm::translate(model, localOffset);
        model = model * m_modelAxisCorrection;
    }

    model = glm::scale(model, scale);
    
    // 设置着色器 uniform
    shader->setMat4("uModel", model);
    shader->setMat4("uView", camera->getViewMatrix());
    shader->setMat4("uProjection", camera->getProjectionMatrix());
    shader->setVec3("uViewPos", camera->getPosition());
    shader->setVec3("uLightPos", 10.0f, 10.0f, 10.0f);
    shader->setVec3("uLightColor", 1.0f, 1.0f, 1.0f);
    shader->setVec3("uObjectColor", 0.6f, 0.4f, 0.2f);  // 棕色
    
    // 渲染网格
    glBindVertexArray(m_boatMesh->VAO);
    glDrawElements(GL_TRIANGLES, m_boatMesh->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace WaterTown
