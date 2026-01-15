#pragma once

#include "SceneEditor.h"

namespace WaterTown {

/**
 * @brief 编辑器 UI 管理类，处理 ImGui 界面
 */
class EditorUI {
public:
    EditorUI();
    ~EditorUI();
    
    /**
     * @brief 初始化 UI
     */
    void init(SceneEditor* editor);
    
    /**
     * @brief 渲染 UI
     */
    void render();
    
    /**
     * @brief 获取当前选中的地形类型
     */
    TerrainType getSelectedTerrainType() const { return m_selectedTerrainType; }
    
    /**
     * @brief 获取当前选中的物体类型
     */
    ObjectType getSelectedObjectType() const { return m_selectedObjectType; }
    
    /**
     * @brief 是否显示网格
     */
    bool shouldShowGrid() const { return m_showGrid; }
    
    /**
     * @brief 获取网格大小
     */
    float getGridSize() const { return m_gridSize; }

private:
    SceneEditor* m_editor;
    
    // 当前选择
    TerrainType m_selectedTerrainType;
    ObjectType m_selectedObjectType;
    
    // 显示设置
    bool m_showGrid;
    bool m_showWater;
    bool m_showObjects;
    float m_gridSize;
    
    // 统计信息
    float m_fps;
    int m_terrainCount[3];  // 草地、水路、石路数量
    
    /**
     * @brief 渲染模式切换面板
     */
    void renderModePanel();
    
    /**
     * @brief 渲染工具选择面板
     */
    void renderToolPanel();
    
    /**
     * @brief 渲染设置面板
     */
    void renderSettingsPanel();
    
    /**
     * @brief 渲染统计信息面板
     */
    void renderStatsPanel();
    
    /**
     * @brief 渲染场景管理面板
     */
    void renderScenePanel();
};

} // namespace WaterTown
