#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <string>

namespace WaterTown {

class Camera;
class OrthographicCamera;
class OrbitCamera;
class FollowCamera;
class WaterSurface;
class Boat;
class ObjectRenderer;

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
    EMPTY,      // 空地(默认) - WaterTown 特有
    GRASS,      // 草地
    WATER,      // 水路
    STONE       // 石路
};

/**
 * @brief 物体类型 (已扩展至 WaterTown-sec 的 22 种)
 */
enum class ObjectType {
    HOUSE,          // 普通民居
    HOUSE_STYLE_1,  // 江南水乡特色民居
    HOUSE_STYLE_2,  // 精致庭院住宅
    HOUSE_STYLE_3,  // 传统祠堂
    HOUSE_STYLE_4,  // 现代中式别墅
    HOUSE_STYLE_5,  // 古朴农舍
    BRIDGE,         // 桥
    TREE,           // 树
    BOAT,           // 船
    WALL,           // 围墙
    PAVILION,       // 凉亭
    LONG_HOUSE,     // 长屋
    ARCH_BRIDGE,    // 拱桥
    PAIFANG,        // 牌坊
    WATER_PAVILION, // 水榭
    PIER,           // 码头
    TEMPLE,         // 寺庙
    BAMBOO,         // 竹子
    LOTUS_POND,     // 荷花池
    FISHING_BOAT,   // 渔船
    LANTERN,        // 灯笼
    STONE_LION      // 石狮子
};

/**
 * @brief 场景编辑器，管理不同编辑模式和相机切换
 */
class SceneEditor {
public:
    static constexpr int GRID_SIZE = 320;   // 扩大到 320x320
    static constexpr float CELL_SIZE = 0.5f;
    static constexpr float WATER_LEVEL = 0.0f;

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
    OrbitCamera* getOrbitCamera() const { return m_orbitCamera; }
    
    /**
     * @brief 更新编辑器
     */
    void update(float deltaTime);
    void updateBoat(float deltaTime);
    
    /**
     * @brief 处理鼠标输入（用于相机控制）
     */
    void handleMouseMovement(float deltaX, float deltaY, bool rightButtonPressed);
    void handleMiddleMouseMovement(float deltaX, float deltaY); // WaterTown 特有：中键平移
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
    void setWaterSurface(WaterSurface* water);
    
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
     * @brief 获取物体渲染器
     */
    ObjectRenderer* getObjectRenderer() { return m_objectRenderer; }
    
    /**
     * @brief 获取所有放置的物体
     */
    const std::vector<std::pair<ObjectType, glm::vec3>>& getPlacedObjects() const {
        return m_objectsHiddenForGame ? m_hiddenObjects : m_placedObjects;
    }
    
    /**
     * @brief 检查指定位置是否为水域
     */
    bool isWaterAt(int gridX, int gridZ) const;
    
    /**
     * @brief 检查是否已放置船只
     */
    bool hasBoatPlaced() const { return m_boatPlaced; }
    
    /**
     * @brief 获取船只放置位置
     */
    glm::vec3 getBoatPlacedPosition() const { return m_boatPlacedPosition; }
    
    /**
     * @brief 获取船只放置旋转角度
     */
    float getBoatPlacedRotation() const { return m_boatPlacedRotation; }
    
    /**
     * @brief 检查是否可以进入游戏模式
     */
    bool canEnterGameMode() const { return m_boatPlaced; }
    
    /**
     * @brief 撤销上一步操作
     */
    void undoLastAction();
    
    /**
     * @brief 更新船只的障碍物碰撞体（将所有建筑物添加为障碍物）
     */
    void updateBoatObstacles();
    
    /**
     * @brief 更新水面遮罩 (WaterTown 特有：仅在有水的地方渲染水面)
     */
    void updateWaterMesh();
    
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
     * @brief 清理水面上的非船对象（从数据中彻底删除）
     */
    void removeObjectsOnWaterExceptBoat();
    
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

    /**
     * @brief 获取地形单元格大小（世界坐标）
     */
    float getCellSize() const { return CELL_SIZE; }

    /**
     * @brief 获取地形整体世界尺寸（宽/深）
     */
    float getTerrainWorldSize() const { return GRID_SIZE * CELL_SIZE; }

    /**
     * @brief 获取河道世界坐标宽度
     */
    float getRiverWorldWidth() const { return (m_riverEndColumn - m_riverStartColumn) * CELL_SIZE; }

    /**
     * @brief 获取河道中心在世界坐标中的X位置
     */
    float getRiverCenterWorldX() const;

private:
    /**
     * @brief 生成默认的水陆分离地形布局 (WaterTown-sec 特性)
     */
    void initializeTerrainLayout();

    EditorMode m_currentMode;
    
    // 三种相机
    OrthographicCamera* m_orthoCamera;
    OrbitCamera* m_orbitCamera;
    FollowCamera* m_followCamera;
    
    // 水面引用
    WaterSurface* m_waterSurface;
    
    // 船只（游戏模式）
    Boat* m_boat;
    
    // 物体渲染器
    ObjectRenderer* m_objectRenderer;

    // 网格数据（简化的地形系统）
    TerrainType m_terrainGrid[GRID_SIZE][GRID_SIZE];
    
    // 河道范围
    int m_riverStartColumn; // 河道起始列
    int m_riverEndColumn;   // 河道结束列
    
    // 当前选中的类型
    TerrainType m_currentTerrainType;
    ObjectType m_currentObjectType;
    
    // 放置的物体列表
    std::vector<std::pair<ObjectType, glm::vec3>> m_placedObjects;
    std::vector<std::pair<ObjectType, glm::vec3>> m_hiddenObjects; // 游戏模式下隐藏的物体
    bool m_objectsHiddenForGame;
    
    // 船只放置状态 (WaterTown 特有的一层封装)
    bool m_boatPlaced;
    glm::vec3 m_boatPlacedPosition;
    float m_boatPlacedRotation; // Store rotation for sync

    // Transition Logic
    bool m_isTransitioning = false;
    float m_transitionTime = 0.0f;
    float m_transitionDuration = 1.0f;
    glm::vec3 m_transStartPos;
    glm::vec3 m_transStartTarget;
    glm::vec3 m_transEndPos;
    glm::vec3 m_transEndTarget;
    
    // Helper to get camera target (since Camera base class doesn't enforce getTarget)
    glm::vec3 getCameraTarget(Camera* cam, EditorMode mode);
    
    // We use a temporary camera holder for interpolation if needed,
    // or just modify the target camera's parameters?
    // Better: let main.cpp use 'getCurrentCamera' which might return a temporary interploated camera.
    // Or simpler: We just update the NEW camera to interpolate from old positions.
    // But different camera types have different update logic.
    // Let's use m_orbitCamera as a generic perspective camera during transition?
    // Or add a dedicated TransitionCamera.
    Camera* m_transitionCamera = nullptr; // Initialized in constructor
    
    // 撤销系统 - 地形操作记录
    struct TerrainAction {
        int gridX, gridZ;
        TerrainType oldType;
        TerrainType newType;
    };
    std::vector<TerrainAction> m_terrainHistory;
    
    // 撤销系统 - 建筑操作记录
    struct ObjectAction {
        ObjectType type;
        glm::vec3 position;
        bool isAdd;  // true=添加, false=删除
    };
    std::vector<ObjectAction> m_objectHistory;

    std::vector<std::pair<ObjectType, glm::vec3>>& getActiveObjectList() {
        return m_objectsHiddenForGame ? m_hiddenObjects : m_placedObjects;
    }
};

} // namespace WaterTown
