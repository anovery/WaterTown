#include "TerrainRenderer.h"
#include "Shader.h"
#include "Camera.h"
#include "../Editor/SceneEditor.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <algorithm>

namespace WaterTown {

TerrainRenderer::TerrainRenderer(int gridSize)
    : m_gridSize(gridSize), m_planeVAO(0), m_planeVBO(0) {
    glGenVertexArrays(1, &m_planeVAO);
    glGenBuffers(1, &m_planeVBO);
}

TerrainRenderer::~TerrainRenderer() {
    if (m_planeVAO) glDeleteVertexArrays(1, &m_planeVAO);
    if (m_planeVBO) glDeleteBuffers(1, &m_planeVBO);
}

glm::vec3 TerrainRenderer::getTerrainColor(TerrainType type) const {
    switch (type) {
        case TerrainType::GRASS:
            return glm::vec3(0.3f, 0.7f, 0.3f);
        case TerrainType::WATER:
            return glm::vec3(0.2f, 0.4f, 0.9f);
        case TerrainType::STONE:
            return glm::vec3(0.7f, 0.7f, 0.7f);
        case TerrainType::EMPTY:
            return glm::vec3(0.0f); // 透明/不可见
        default:
            return glm::vec3(1.0f, 1.0f, 1.0f);
    }
}

float TerrainRenderer::getTerrainHeight(TerrainType type) const {
    switch (type) {
        case TerrainType::GRASS:
            return 1.0f;
        case TerrainType::STONE:
            return 1.1f;
        case TerrainType::WATER:
            return SceneEditor::WATER_LEVEL;
        default:
            return 0.3f;
    }
}

void TerrainRenderer::buildTerrainVertices(SceneEditor* editor, std::vector<TerrainVertex>& outVertices) {
    const float cellSize = SceneEditor::CELL_SIZE;
    const float expand = cellSize * 0.05f; // slight overlap to avoid cracks on the plane
    const glm::vec3 upNormal(0.0f, 1.0f, 0.0f);
    const float waterSurface = getTerrainHeight(TerrainType::WATER);
    const float wallBase = waterSurface - 0.1f; // sink a bit into the river to avoid gaps
    const float brickScale = 4.0f;
    const float wallThickness = cellSize * 0.45f * brickScale;
    const float verticalGap = 0.01f * brickScale;
    const float horizontalGap = cellSize * 0.04f * brickScale;
    const float baseBrickHeight = cellSize * 0.15f;
    const float baseBrickLength = cellSize * 0.25f;
    const float scaledBrickHeight = baseBrickHeight * brickScale;
    const float scaledBrickLength = baseBrickLength * brickScale;
    const glm::vec3 wallColorDark(0.35f, 0.35f, 0.35f);
    const glm::vec3 wallColorLight(0.45f, 0.45f, 0.45f);

    outVertices.clear();
    outVertices.reserve(m_gridSize * m_gridSize * 18);

    auto addQuad = [&](const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
                       const glm::vec3& normal, const glm::vec3& color) {
        outVertices.push_back({v0, normal, color});
        outVertices.push_back({v1, normal, color});
        outVertices.push_back({v2, normal, color});
        outVertices.push_back({v0, normal, color});
        outVertices.push_back({v2, normal, color});
        outVertices.push_back({v3, normal, color});
    };

    auto addBox = [&](const glm::vec3& minCorner, const glm::vec3& maxCorner, const glm::vec3& color) {
        glm::vec3 v000(minCorner.x, minCorner.y, minCorner.z);
        glm::vec3 v001(minCorner.x, minCorner.y, maxCorner.z);
        glm::vec3 v010(minCorner.x, maxCorner.y, minCorner.z);
        glm::vec3 v011(minCorner.x, maxCorner.y, maxCorner.z);
        glm::vec3 v100(maxCorner.x, minCorner.y, minCorner.z);
        glm::vec3 v101(maxCorner.x, minCorner.y, maxCorner.z);
        glm::vec3 v110(maxCorner.x, maxCorner.y, minCorner.z);
        glm::vec3 v111(maxCorner.x, maxCorner.y, maxCorner.z);

        addQuad(v001, v101, v111, v011, glm::vec3(0.0f, 0.0f, 1.0f), color);   // front (+Z)
        addQuad(v100, v000, v010, v110, glm::vec3(0.0f, 0.0f, -1.0f), color);  // back (-Z)
        addQuad(v000, v001, v011, v010, glm::vec3(-1.0f, 0.0f, 0.0f), color);  // left (-X)
        addQuad(v101, v100, v110, v111, glm::vec3(1.0f, 0.0f, 0.0f), color);   // right (+X)
        addQuad(v010, v011, v111, v110, glm::vec3(0.0f, 1.0f, 0.0f), color);   // top (+Y)
        addQuad(v000, v100, v101, v001, glm::vec3(0.0f, -1.0f, 0.0f), color);  // bottom (-Y)
    };

    auto addWallBricks = [&](float minX, float maxX, float minZ, float maxZ, float topHeight, bool alongZ) {
        float usableHeight = topHeight - wallBase;
        if (usableHeight <= 0.05f) {
            return;
        }

        float runLength = alongZ ? (maxZ - minZ) : (maxX - minX);
        if (runLength <= 0.05f) {
            return;
        }

        float gapY = std::min(verticalGap, usableHeight * 0.25f);
        float gapRun = std::min(horizontalGap, runLength * 0.5f);
        float brickHeight = std::min(scaledBrickHeight, usableHeight);
        float brickLength = std::min(scaledBrickLength, runLength);

        if (brickHeight <= 0.0f || brickLength <= 0.0f) {
            return;
        }

        int layerIndex = 0;
        for (float y0 = wallBase; y0 < topHeight - 0.001f; y0 += brickHeight + gapY, ++layerIndex) {
            float y1 = std::min(y0 + brickHeight, topHeight);

            int segmentIndex = 0;
            for (float offset = 0.0f; offset < runLength - 0.001f; offset += brickLength + gapRun, ++segmentIndex) {
                float segStart = (alongZ ? minZ : minX) + offset;
                float segEnd = std::min(segStart + brickLength, alongZ ? maxZ : maxX);
                if (segEnd <= segStart + 0.0005f) {
                    break;
                }

                glm::vec3 minCorner;
                glm::vec3 maxCorner;
                if (alongZ) {
                    minCorner = glm::vec3(minX, y0, segStart);
                    maxCorner = glm::vec3(maxX, y1, segEnd);
                } else {
                    minCorner = glm::vec3(segStart, y0, minZ);
                    maxCorner = glm::vec3(segEnd, y1, maxZ);
                }

                glm::vec3 color = ((layerIndex + segmentIndex) % 2 == 0) ? wallColorDark : wallColorLight;
                addBox(minCorner, maxCorner, color);

                if (segEnd >= (alongZ ? maxZ : maxX) - 0.001f) {
                    break;
                }
            }
        }
    };

    for (int z = 0; z < m_gridSize; ++z) {
        for (int x = 0; x < m_gridSize; ++x) {
            TerrainType type = editor->getTerrainAt(x, z);
            // 修改点：同时跳过 WATER 和 EMPTY
            if (type == TerrainType::WATER || type == TerrainType::EMPTY) {
                continue; // 水面由 WaterSurface 渲染，空地不渲染
            }

            float height = getTerrainHeight(type);
            glm::vec3 color = getTerrainColor(type);

            float tileX0 = (x - m_gridSize / 2.0f) * cellSize;
            float tileZ0 = (z - m_gridSize / 2.0f) * cellSize;
            float tileX1 = tileX0 + cellSize;
            float tileZ1 = tileZ0 + cellSize;

            float x0 = tileX0 - expand * 0.5f;
            float x1 = tileX1 + expand * 0.5f;
            float z0 = tileZ0 - expand * 0.5f;
            float z1 = tileZ1 + expand * 0.5f;

            TerrainVertex v0{{x0, height, z0}, upNormal, color};
            TerrainVertex v1{{x1, height, z0}, upNormal, color};
            TerrainVertex v2{{x1, height, z1}, upNormal, color};
            TerrainVertex v3{{x0, height, z1}, upNormal, color};

            outVertices.push_back(v0);
            outVertices.push_back(v1);
            outVertices.push_back(v2);
            outVertices.push_back(v0);
            outVertices.push_back(v2);
            outVertices.push_back(v3);

            // 检查四个方向是否与河面相邻，生成挡水墙砖块
            const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
            for (const auto& dir : directions) {
                int neighborX = x + dir[0];
                int neighborZ = z + dir[1];
                
                // 注意：如果超出边界，getTerrainAt 可能返回默认值(EMPTY)，所以这里不仅要检测是否是 WATER，
                // 还要考虑是否是 EMPTY？不，河岸只在陆地和水之间生成。
                // 如果陆地旁边是空地，不需要生成墙。
                // 所以只检测邻居是否为 WATER 即可。
                if (editor->getTerrainAt(neighborX, neighborZ) != TerrainType::WATER) {
                    continue;
                }

                if (dir[0] != 0) {
                    float boundaryX = (dir[0] > 0) ? tileX1 : tileX0;
                    float minX = (dir[0] > 0) ? boundaryX : boundaryX - wallThickness;
                    float maxX = (dir[0] > 0) ? boundaryX + wallThickness : boundaryX;
                    addWallBricks(minX, maxX, tileZ0, tileZ1, height, true);
                } else {
                    float boundaryZ = (dir[1] > 0) ? tileZ1 : tileZ0;
                    float minZ = (dir[1] > 0) ? boundaryZ : boundaryZ - wallThickness;
                    float maxZ = (dir[1] > 0) ? boundaryZ + wallThickness : boundaryZ;
                    addWallBricks(tileX0, tileX1, minZ, maxZ, height, false);
                }
            }
        }
    }
}

void TerrainRenderer::render(SceneEditor* editor, Shader* shader, Camera* camera) {
    if (!editor || !shader || !camera) {
        return;
    }

    std::vector<TerrainVertex> vertices;
    buildTerrainVertices(editor, vertices);
    if (vertices.empty()) {
        return;
    }

    shader->use();
    shader->setBool("uUseVertexColor", true);
    shader->setMat4("uModel", glm::mat4(1.0f));
    shader->setMat4("uView", camera->getViewMatrix());
    shader->setMat4("uProjection", camera->getProjectionMatrix());
    shader->setVec3("uViewPos", camera->getPosition());
    shader->setVec3("uLightPos", 10.0f, 50.0f, 10.0f);
    shader->setVec3("uLightColor", 1.0f, 1.0f, 1.0f);

    glBindVertexArray(m_planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_planeVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TerrainVertex), vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)(2 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));

    glBindVertexArray(0);
    shader->setBool("uUseVertexColor", false);
}

} // namespace WaterTown
