#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace WaterTown {

class Camera;
class OrthographicCamera;
class OrbitCamera;
class FollowCamera;
class WaterSurface;
class Boat;

/**
 * @brief 编辑器模式枚举
 */
enum class EditorMode {
    TERRAIN,    // 地形编辑模式（正交视角）
    BUILDING,   // 建筑布置模式（轨道相机）
    GAME        // 游戏模式（自由相机/追随相机）
};

/**
 * @brief 地形类型
 */
enum class TerrainType {
    GRASS,      // 草地
    WATER,      // 水路
    STONE       // 石路
};

/**
 * @brief 物体类型
 */
enum class ObjectType {
    HOUSE,      // 房子
    BRIDGE,     // 桥
    TREE,       // 树
    BOAT        // 船
};

/**
 * @brief 场景编辑器，管理不同编辑模式和相机切换
 */
class SceneEditor {
public:
    SceneEditor();
    ~SceneEditor();
    
    /**
     * @brief 初始化编辑器
     */
    void init(float aspectRatio);
    
    /**
     * @brief 切换编辑模式
     */
    void switchMode(EditorMode mode);
    
    /**
     * @brief 获取当前编辑模式
     */
    EditorMode getCurrentMode() const { return m_currentMode; }
    
    /**
     * @brief 获取当前相机
     */
    Camera* getCurrentCamera();
    
    /**
     * @brief 更新编辑器
     */
    void update(float deltaTime);
    
    /**
     * @brief 处理鼠标输入（用于相机控制）
     */
    void handleMouseMovement(float deltaX, float deltaY, bool rightButtonPressed);
    void handleMouseScroll(float delta);
    
    /**
     * @brief 放置地形
     */
    void placeTerrain(int gridX, int gridZ, TerrainType type);
    
    /**
     * @brief 放置物体
     */
    void placeObject(ObjectType type, const glm::vec3& position);
    
    /**
     * @brief 设置水面引用（用于船只交互）
     */
    void setWaterSurface(WaterSurface* water) { m_waterSurface = water; }
    
    /**
     * @brief 更新宽高比（窗口大小改变时）
     */
    void updateAspectRatio(float aspectRatio);
    
    /**
     * @brief 处理游戏模式输入（WASD 控制船只）
     */
    void handleGameInput(float forward, float turn);
    
    /**
     * @brief 处理鼠标点击（用于放置地形/物体）
     * @param screenX 屏幕X坐标
     * @param screenY 屏幕Y坐标
     * @param screenWidth 屏幕宽度
     * @param screenHeight 屏幕高度
     */
    void handleMouseClick(float screenX, float screenY, int screenWidth, int screenHeight);
    
    /**
     * @brief 获取地形类型
     */
    TerrainType getTerrainAt(int gridX, int gridZ) const;
    
    /**
     * @brief 获取当前选中的地形类型
     */
    TerrainType getCurrentTerrainType() const { return m_currentTerrainType; }
    void setCurrentTerrainType(TerrainType type) { m_currentTerrainType = type; }
    
    /**
     * @brief 获取当前选中的物体类型
     */
    ObjectType getCurrentObjectType() const { return m_currentObjectType; }
    void setCurrentObjectType(ObjectType type) { m_currentObjectType = type; }
    
    /**
     * @brief 获取船只对象
     */
    Boat* getBoat() { return m_boat; }
    
    /**
     * @brief 获取所有放置的物体
     */
    const std::vector<std::pair<ObjectType, glm::vec3>>& getPlacedObjects() const { return m_placedObjects; }
    
    /**
     * @brief 更新船只的障碍物碰撞体（将所有建筑物添加为障碍物）
     */
    void updateBoatObstacles();
    
    /**
     * @brief 删除最近放置的建筑物
     */
    void removeLastObject();
    
    /**
     * @brief 删除指定位置附近的建筑物
     * @param worldPos 世界坐标
     * @param radius 搜索半径
     * @return 是否成功删除
     */
    bool removeObjectNear(const glm::vec3& worldPos, float radius = 1.0f);
    
    /**
     * @brief 清空所有放置的建筑物
     */
    void clearAllObjects();
    
    /**
     * @brief 清空整个场景（地形+建筑）
     */
    void clearScene();
    
    /**
     * @brief 保存场景到文件
     * @param filename 文件名
     * @return 是否成功
     */
    bool saveScene(const std::string& filename);
    
    /**
     * @brief 从文件加载场景
     * @param filename 文件名
     * @return 是否成功
     */
    bool loadScene(const std::string& filename);
    
    /**
     * @brief 射线投射到地面
     * @return 是否成功，以及交点的网格坐标
     */
    bool raycastToGround(float screenX, float screenY, int screenWidth, int screenHeight, int& outGridX, int& outGridZ);

private:
    EditorMode m_currentMode;
    
    // 三种相机
    OrthographicCamera* m_orthoCamera;
    OrbitCamera* m_orbitCamera;
    FollowCamera* m_followCamera;
    
    // 水面引用
    WaterSurface* m_waterSurface;
    
    // 船只（游戏模式）
    Boat* m_boat;
    
    // 网格数据（简化的地形系统）
    static const int GRID_SIZE = 50;
    TerrainType m_terrainGrid[GRID_SIZE][GRID_SIZE];
    
    // 当前选中的类型
    TerrainType m_currentTerrainType;
    ObjectType m_currentObjectType;
    
    // 放置的物体列表
    std::vector<std::pair<ObjectType, glm::vec3>> m_placedObjects;
};

} // namespace WaterTown
