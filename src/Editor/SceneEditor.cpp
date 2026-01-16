#include "SceneEditor.h"
#include "../Render/Camera.h"
#include "../Render/OrbitCamera.h"
#include "../Render/FollowCamera.h"
#include "../Water/WaterSurface.h"
#include "../Physics/Boat.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

namespace WaterTown {

SceneEditor::SceneEditor()
    : m_currentMode(EditorMode::TERRAIN),
      m_orthoCamera(nullptr),
      m_orbitCamera(nullptr),
      m_followCamera(nullptr),
      m_waterSurface(nullptr),
      m_boat(nullptr),
      m_currentTerrainType(TerrainType::GRASS),
      m_currentObjectType(ObjectType::HOUSE) {
    
    // 初始化地形网格为水面
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            m_terrainGrid[i][j] = TerrainType::WATER;
        }
    }
}

SceneEditor::~SceneEditor() {
    delete m_orthoCamera;
    delete m_orbitCamera;
    delete m_followCamera;
    delete m_boat;
}

void SceneEditor::init(float aspectRatio) {
    // 创建三种相机
    m_orthoCamera = new OrthographicCamera(0.0f, 0.0f, 30.0f, 30.0f);
    m_orbitCamera = new OrbitCamera(glm::vec3(0.0f, 0.0f, 0.0f), 15.0f, 45.0f, aspectRatio);
    m_followCamera = new FollowCamera(45.0f, aspectRatio);
    
    // 创建船只（初始位置在水面中心）
    m_boat = new Boat(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
    m_boat->setBounds(-20.0f, 20.0f, -20.0f, 20.0f);  // 设置边界
    
    std::cout << "SceneEditor initialized." << std::endl;
}

void SceneEditor::switchMode(EditorMode mode) {
    if (m_currentMode == mode) return;
    
    EditorMode oldMode = m_currentMode;
    m_currentMode = mode;
    
    std::cout << "Switched from mode " << static_cast<int>(oldMode) 
              << " to mode " << static_cast<int>(mode) << std::endl;
    
    // 模式切换时的特殊处理
    if (mode == EditorMode::TERRAIN) {
        // 切换到地形编辑：重置正交相机
        m_orthoCamera->setCenter(0.0f, 0.0f);
    }
    else if (mode == EditorMode::BUILDING) {
        // 切换到建筑布置：重置轨道相机
        m_orbitCamera->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));
    }
    else if (mode == EditorMode::GAME) {
        // 切换到游戏模式：初始化追随相机和船只
        if (m_boat) {
            m_followCamera->setTarget(m_boat->getPosition(), m_boat->getRotation());
            m_followCamera->setPosition(m_boat->getPosition() + glm::vec3(0.0f, 3.0f, -6.0f));
            
            // 将所有建筑物添加为障碍物
            updateBoatObstacles();
        }
    }
}

Camera* SceneEditor::getCurrentCamera() {
    switch (m_currentMode) {
        case EditorMode::TERRAIN:
            return m_orthoCamera;
        case EditorMode::BUILDING:
            return m_orbitCamera;
        case EditorMode::GAME:
            return m_followCamera;
        default:
            return m_orbitCamera;
    }
}

void SceneEditor::update(float deltaTime) {
    // 游戏模式下更新追随相机和船只
    if (m_currentMode == EditorMode::GAME) {
        // 更新船只物理
        if (m_boat && m_waterSurface) {
            m_boat->update(deltaTime, m_waterSurface, static_cast<float>(glfwGetTime()));
        }
        
        // 更新追随相机
        if (m_followCamera && m_boat) {
            m_followCamera->setTarget(m_boat->getPosition(), m_boat->getRotation());
            m_followCamera->update(deltaTime);
        }
    }
}

void SceneEditor::handleMouseMovement(float deltaX, float deltaY, bool rightButtonPressed) {
    if (!rightButtonPressed) return;
    
    switch (m_currentMode) {
        case EditorMode::TERRAIN:
            // 正交相机：右键拖拽平移
            if (m_orthoCamera) {
                m_orthoCamera->pan(deltaX * 0.05f, deltaY * 0.05f);
            }
            break;
            
        case EditorMode::BUILDING:
            // 轨道相机：右键拖拽旋转
            if (m_orbitCamera) {
                m_orbitCamera->rotate(deltaX, deltaY);
            }
            break;
            
        case EditorMode::GAME:
            // 游戏模式暂不处理鼠标旋转（由 FreeCamera 或船只控制）
            break;
    }
}

void SceneEditor::handleMouseScroll(float delta) {
    switch (m_currentMode) {
        case EditorMode::TERRAIN:
            if (m_orthoCamera) {
                m_orthoCamera->zoom(delta);
            }
            break;
            
        case EditorMode::BUILDING:
            if (m_orbitCamera) {
                m_orbitCamera->zoom(delta * 0.5f);
            }
            break;
            
        case EditorMode::GAME:
            // 游戏模式不缩放
            break;
    }
}

void SceneEditor::placeTerrain(int gridX, int gridZ, TerrainType type) {
    if (gridX < 0 || gridX >= GRID_SIZE || gridZ < 0 || gridZ >= GRID_SIZE) {
        return;
    }
    
    m_terrainGrid[gridX][gridZ] = type;
    std::cout << "Placed terrain type " << static_cast<int>(type) 
              << " at (" << gridX << ", " << gridZ << ")" << std::endl;
}

void SceneEditor::placeObject(ObjectType type, const glm::vec3& position) {
    std::cout << "Placed object type " << static_cast<int>(type) 
              << " at (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    
    // 存储物体
    m_placedObjects.push_back({type, position});
    
    // 如果当前在游戏模式，立即更新障碍物
    if (m_currentMode == EditorMode::GAME) {
        updateBoatObstacles();
    }
}

void SceneEditor::updateAspectRatio(float aspectRatio) {
    if (m_orbitCamera) {
        m_orbitCamera->updateAspectRatio(aspectRatio);
    }
    if (m_followCamera) {
        m_followCamera->updateAspectRatio(aspectRatio);
    }
}

void SceneEditor::handleGameInput(float forward, float turn) {
    if (m_currentMode == EditorMode::GAME && m_boat) {
        m_boat->processInput(forward, turn);
    }
}

TerrainType SceneEditor::getTerrainAt(int gridX, int gridZ) const {
    if (gridX < 0 || gridX >= GRID_SIZE || gridZ < 0 || gridZ >= GRID_SIZE) {
        return TerrainType::GRASS;
    }
    return m_terrainGrid[gridX][gridZ];
}

bool SceneEditor::raycastToGround(float screenX, float screenY, int screenWidth, int screenHeight, int& outGridX, int& outGridZ) {
    Camera* camera = getCurrentCamera();
    if (!camera) return false;
    
    // 将屏幕坐标转换为NDC (-1到1)
    float x = (2.0f * screenX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * screenY) / screenHeight;
    
    // 获取相机矩阵
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 invVP = glm::inverse(projection * view);
    
    // 近平面和远平面上的点
    glm::vec4 rayStartNDC(x, y, -1.0f, 1.0f);
    glm::vec4 rayEndNDC(x, y, 1.0f, 1.0f);
    
    glm::vec4 rayStartWorld = invVP * rayStartNDC;
    glm::vec4 rayEndWorld = invVP * rayEndNDC;
    
    rayStartWorld /= rayStartWorld.w;
    rayEndWorld /= rayEndWorld.w;
    
    glm::vec3 rayStart(rayStartWorld);
    glm::vec3 rayEnd(rayEndWorld);
    glm::vec3 rayDir = glm::normalize(rayEnd - rayStart);
    
    // 与 Y = 0.0 平面相交（陆地高度）
    float groundY = 0.0f;
    
    if (std::abs(rayDir.y) < 0.001f) return false;  // 射线平行于地面
    
    float t = (groundY - rayStart.y) / rayDir.y;
    if (t < 0) return false;  // 交点在相机后面
    
    glm::vec3 hitPoint = rayStart + rayDir * t;
    
    // 转换为网格坐标（考虑0.5的格子大小）
    float cellSize = 0.5f;
    float worldX = hitPoint.x / cellSize + GRID_SIZE / 2.0f;
    float worldZ = hitPoint.z / cellSize + GRID_SIZE / 2.0f;
    
    outGridX = static_cast<int>(worldX);
    outGridZ = static_cast<int>(worldZ);
    
    // 检查范围
    if (outGridX < 0 || outGridX >= GRID_SIZE || outGridZ < 0 || outGridZ >= GRID_SIZE) {
        return false;
    }
    
    return true;
}

void SceneEditor::handleMouseClick(float screenX, float screenY, int screenWidth, int screenHeight) {
    int gridX, gridZ;
    
    if (m_currentMode == EditorMode::TERRAIN) {
        // 地形编辑模式：放置地形
        if (raycastToGround(screenX, screenY, screenWidth, screenHeight, gridX, gridZ)) {
            placeTerrain(gridX, gridZ, m_currentTerrainType);
        }
    }
    else if (m_currentMode == EditorMode::BUILDING) {
        // 建筑布置模式：放置物体
        if (raycastToGround(screenX, screenY, screenWidth, screenHeight, gridX, gridZ)) {
            // 计算世界坐标（考虑0.5的格子大小）
            float cellSize = 0.5f;
            float worldX = (gridX - GRID_SIZE / 2.0f + 0.5f) * cellSize;
            float worldZ = (gridZ - GRID_SIZE / 2.0f + 0.5f) * cellSize;
            placeObject(m_currentObjectType, glm::vec3(worldX, 0.0f, worldZ));
        }
    }
}

void SceneEditor::updateBoatObstacles() {
    if (!m_boat) return;
    
    // 清空所有障碍物
    m_boat->clearObstacles();
    
    // 根据物体类型添加碰撞体
    for (const auto& obj : m_placedObjects) {
        ObjectType type = obj.first;
        glm::vec3 position = obj.second;
        float radius = 0.0f;
        
        // 根据物体类型设定碰撞半径
        switch (type) {
            case ObjectType::HOUSE:
                radius = 1.5f;  // 房子较大
                break;
            case ObjectType::TREE:
                radius = 0.5f;  // 树较小
                break;
            case ObjectType::BRIDGE:
                radius = 1.0f;  // 桥中等大小
                break;
            case ObjectType::BOAT:
                radius = 0.8f;  // 其他船只
                break;
        }
        
        // 添加障碍物
        m_boat->addObstacle(position, radius);
    }
    
    std::cout << "Updated boat obstacles: " << m_placedObjects.size() << " objects added." << std::endl;
}

void SceneEditor::removeLastObject() {
    if (m_placedObjects.empty()) {
        std::cout << "No objects to remove." << std::endl;
        return;
    }
    
    m_placedObjects.pop_back();
    std::cout << "Removed last object. Remaining: " << m_placedObjects.size() << std::endl;
    
    // 如果在游戏模式，更新障碍物
    if (m_currentMode == EditorMode::GAME) {
        updateBoatObstacles();
    }
}

bool SceneEditor::removeObjectNear(const glm::vec3& worldPos, float radius) {
    for (auto it = m_placedObjects.begin(); it != m_placedObjects.end(); ++it) {
        glm::vec3 diff = it->second - worldPos;
        diff.y = 0.0f;  // 只检测水平距离
        float dist = glm::length(diff);
        
        if (dist <= radius) {
            std::cout << "Removed object at (" << it->second.x << ", " << it->second.z << ")" << std::endl;
            m_placedObjects.erase(it);
            
            // 如果在游戏模式，更新障碍物
            if (m_currentMode == EditorMode::GAME) {
                updateBoatObstacles();
            }
            return true;
        }
    }
    
    std::cout << "No object found near (" << worldPos.x << ", " << worldPos.z << ")" << std::endl;
    return false;
}

void SceneEditor::clearAllObjects() {
    m_placedObjects.clear();
    std::cout << "Cleared all objects." << std::endl;
    
    // 如果在游戏模式，更新障碍物
    if (m_currentMode == EditorMode::GAME) {
        updateBoatObstacles();
    }
}

void SceneEditor::clearScene() {
    // 清空建筑物
    clearAllObjects();
    
    // 重置地形为水面
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            m_terrainGrid[i][j] = TerrainType::WATER;
        }
    }
    
    std::cout << "Scene cleared (terrain reset to water, all objects removed)." << std::endl;
}

bool SceneEditor::saveScene(const std::string& filename) {
    std::string fullPath = filename;
    if (fullPath.find(".txt") == std::string::npos) {
        fullPath += ".txt";
    }
    
    std::ofstream file(fullPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for saving: " << fullPath << std::endl;
        return false;
    }
    
    // 写入文件头
    file << "# WaterTown Scene File\n";
    file << "# Version 1.0\n\n";
    
    // 保存地形数据
    file << "[TERRAIN]\n";
    file << "GRID_SIZE " << GRID_SIZE << "\n";
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            file << static_cast<int>(m_terrainGrid[i][j]);
            if (j < GRID_SIZE - 1) file << " ";
        }
        file << "\n";
    }
    
    // 保存建筑物数据
    file << "\n[OBJECTS]\n";
    file << "COUNT " << m_placedObjects.size() << "\n";
    for (const auto& obj : m_placedObjects) {
        file << static_cast<int>(obj.first) << " "
             << obj.second.x << " " << obj.second.y << " " << obj.second.z << "\n";
    }
    
    file.close();
    std::cout << "Scene saved to: " << fullPath << std::endl;
    return true;
}

bool SceneEditor::loadScene(const std::string& filename) {
    std::string fullPath = filename;
    if (fullPath.find(".txt") == std::string::npos) {
        fullPath += ".txt";
    }
    
    std::ifstream file(fullPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for loading: " << fullPath << std::endl;
        return false;
    }
    
    std::string line;
    bool readingTerrain = false;
    bool readingObjects = false;
    int terrainRow = 0;
    
    // 先清空当前场景
    clearScene();
    
    while (std::getline(file, line)) {
        // 跳过注释和空行
        if (line.empty() || line[0] == '#') continue;
        
        if (line == "[TERRAIN]") {
            readingTerrain = true;
            readingObjects = false;
            continue;
        }
        else if (line == "[OBJECTS]") {
            readingTerrain = false;
            readingObjects = true;
            continue;
        }
        
        if (readingTerrain) {
            if (line.find("GRID_SIZE") != std::string::npos) {
                // 验证网格大小
                int loadedSize;
                std::istringstream iss(line);
                std::string dummy;
                iss >> dummy >> loadedSize;
                if (loadedSize != GRID_SIZE) {
                    std::cerr << "Grid size mismatch! Expected " << GRID_SIZE << ", got " << loadedSize << std::endl;
                    file.close();
                    return false;
                }
            }
            else {
                // 读取地形行
                std::istringstream iss(line);
                int terrainType;
                int col = 0;
                while (iss >> terrainType && col < GRID_SIZE) {
                    m_terrainGrid[terrainRow][col] = static_cast<TerrainType>(terrainType);
                    col++;
                }
                terrainRow++;
            }
        }
        else if (readingObjects) {
            if (line.find("COUNT") != std::string::npos) {
                // 物体数量信息（可选）
                continue;
            }
            else {
                // 读取物体数据
                std::istringstream iss(line);
                int objType;
                float x, y, z;
                if (iss >> objType >> x >> y >> z) {
                    m_placedObjects.push_back({static_cast<ObjectType>(objType), glm::vec3(x, y, z)});
                }
            }
        }
    }
    
    file.close();
    
    // 如果在游戏模式，更新障碍物
    if (m_currentMode == EditorMode::GAME) {
        updateBoatObstacles();
    }
    
    std::cout << "Scene loaded from: " << fullPath << std::endl;
    std::cout << "  Terrain: " << GRID_SIZE << "x" << GRID_SIZE << " grid" << std::endl;
    std::cout << "  Objects: " << m_placedObjects.size() << " loaded" << std::endl;
    return true;
}

} // namespace WaterTown
