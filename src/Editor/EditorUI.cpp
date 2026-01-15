#include "EditorUI.h"
#include <imgui.h>
#include <iostream>

namespace WaterTown {

EditorUI::EditorUI()
    : m_editor(nullptr),
      m_selectedTerrainType(TerrainType::GRASS),
      m_selectedObjectType(ObjectType::HOUSE),
      m_showGrid(true),
      m_showWater(true),
      m_showObjects(true),
      m_gridSize(1.0f),
      m_fps(0.0f) {
    
    m_terrainCount[0] = 0;
    m_terrainCount[1] = 0;
    m_terrainCount[2] = 0;
}

EditorUI::~EditorUI() {
}

void EditorUI::init(SceneEditor* editor) {
    m_editor = editor;
    std::cout << "EditorUI initialized." << std::endl;
}

void EditorUI::render() {
    if (!m_editor) return;
    
    m_fps = ImGui::GetIO().Framerate;
    
    // 主控制面板
    renderModePanel();
    renderToolPanel();
    renderSettingsPanel();
    renderStatsPanel();
    renderScenePanel();
}

void EditorUI::renderModePanel() {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(250, 150), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Mode Selection");
    
    ImGui::Text("Current Mode:");
    EditorMode currentMode = m_editor->getCurrentMode();
    
    switch (currentMode) {
        case EditorMode::TERRAIN:
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "TERRAIN EDITING");
            break;
        case EditorMode::BUILDING:
            ImGui::TextColored(ImVec4(0.0f, 0.7f, 1.0f, 1.0f), "BUILDING PLACEMENT");
            break;
        case EditorMode::GAME:
            ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.0f, 1.0f), "GAME MODE");
            break;
    }
    
    ImGui::Separator();
    
    // 模式切换按钮
    if (ImGui::Button("Terrain Edit", ImVec2(-1, 30))) {
        m_editor->switchMode(EditorMode::TERRAIN);
    }
    
    if (ImGui::Button("Building Place", ImVec2(-1, 30))) {
        m_editor->switchMode(EditorMode::BUILDING);
    }
    
    if (ImGui::Button("Game Mode", ImVec2(-1, 30))) {
        m_editor->switchMode(EditorMode::GAME);
    }
    
    ImGui::End();
}

void EditorUI::renderToolPanel() {
    ImGui::SetNextWindowPos(ImVec2(10, 170), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(250, 300), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Tools");
    
    EditorMode currentMode = m_editor->getCurrentMode();
    
    if (currentMode == EditorMode::TERRAIN) {
        ImGui::Text("Terrain Types:");
        ImGui::Separator();
        
        // 地形类型选择
        if (ImGui::RadioButton("Grass", m_selectedTerrainType == TerrainType::GRASS)) {
            m_selectedTerrainType = TerrainType::GRASS;
            m_editor->setCurrentTerrainType(TerrainType::GRASS);
        }
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.3f, 1.0f), "[Green]");
        
        if (ImGui::RadioButton("Water", m_selectedTerrainType == TerrainType::WATER)) {
            m_selectedTerrainType = TerrainType::WATER;
            m_editor->setCurrentTerrainType(TerrainType::WATER);
        }
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.2f, 0.5f, 0.9f, 1.0f), "[Blue]");
        
        if (ImGui::RadioButton("Stone", m_selectedTerrainType == TerrainType::STONE)) {
            m_selectedTerrainType = TerrainType::STONE;
            m_editor->setCurrentTerrainType(TerrainType::STONE);
        }
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "[Gray]");
        
        ImGui::Separator();
        ImGui::Text("Left Click: Place terrain");
        ImGui::Text("Right Click + Drag: Pan view");
        ImGui::Text("Scroll: Zoom in/out");
    }
    else if (currentMode == EditorMode::BUILDING) {
        ImGui::Text("Object Types:");
        ImGui::Separator();
        
        // 物体类型选择
        if (ImGui::RadioButton("House", m_selectedObjectType == ObjectType::HOUSE)) {
            m_selectedObjectType = ObjectType::HOUSE;
            m_editor->setCurrentObjectType(ObjectType::HOUSE);
        }
        
        if (ImGui::RadioButton("Bridge", m_selectedObjectType == ObjectType::BRIDGE)) {
            m_selectedObjectType = ObjectType::BRIDGE;
            m_editor->setCurrentObjectType(ObjectType::BRIDGE);
        }
        
        if (ImGui::RadioButton("Tree", m_selectedObjectType == ObjectType::TREE)) {
            m_selectedObjectType = ObjectType::TREE;
            m_editor->setCurrentObjectType(ObjectType::TREE);
        }
        
        if (ImGui::RadioButton("Boat", m_selectedObjectType == ObjectType::BOAT)) {
            m_selectedObjectType = ObjectType::BOAT;
            m_editor->setCurrentObjectType(ObjectType::BOAT);
        }
        
        ImGui::Separator();
        ImGui::Text("Left Click: Place object");
        ImGui::Text("Ctrl + Left Click: Delete object");
        ImGui::Text("Right Click + Drag: Rotate view");
        ImGui::Text("Scroll: Zoom in/out");
        
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Object Management:");
        if (ImGui::Button("Remove Last Object", ImVec2(-1, 0))) {
            m_editor->removeLastObject();
        }
        if (ImGui::Button("Clear All Objects", ImVec2(-1, 0))) {
            m_editor->clearAllObjects();
        }
    }
    else {
        ImGui::Text("Game Mode Controls:");
        ImGui::Separator();
        ImGui::BulletText("WASD: Move");
        ImGui::BulletText("Space/Shift: Up/Down");
        ImGui::BulletText("Right Mouse: Look around");
        ImGui::BulletText("ESC: Exit");
    }
    
    ImGui::End();
}

void EditorUI::renderSettingsPanel() {
    ImGui::SetNextWindowPos(ImVec2(10, 480), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(250, 180), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Display Settings");
    
    ImGui::Checkbox("Show Grid", &m_showGrid);
    ImGui::Checkbox("Show Water", &m_showWater);
    ImGui::Checkbox("Show Objects", &m_showObjects);
    
    ImGui::Separator();
    
    ImGui::SliderFloat("Grid Size", &m_gridSize, 0.5f, 2.0f, "%.1f");
    
    ImGui::End();
}

void EditorUI::renderStatsPanel() {
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 260, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(250, 150), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Statistics");
    
    ImGui::Text("Performance:");
    ImGui::Text("FPS: %.1f", m_fps);
    ImGui::Text("Frame Time: %.2f ms", 1000.0f / m_fps);
    
    ImGui::Separator();
    ImGui::Text("Terrain Count:");
    ImGui::Text("  Grass: %d", m_terrainCount[0]);
    ImGui::Text("  Water: %d", m_terrainCount[1]);
    ImGui::Text("  Stone: %d", m_terrainCount[2]);
    
    ImGui::End();
}

void EditorUI::renderScenePanel() {
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 260, 170), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(250, 150), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Scene Management");
    
    static char sceneName[128] = "test_scene";
    ImGui::InputText("Scene Name", sceneName, IM_ARRAYSIZE(sceneName));
    
    ImGui::Separator();
    
    if (ImGui::Button("Save Scene", ImVec2(-1, 30))) {
        if (m_editor->saveScene(sceneName)) {
            std::cout << "Scene saved successfully: " << sceneName << std::endl;
        } else {
            std::cout << "Failed to save scene: " << sceneName << std::endl;
        }
    }
    
    if (ImGui::Button("Load Scene", ImVec2(-1, 30))) {
        if (m_editor->loadScene(sceneName)) {
            std::cout << "Scene loaded successfully: " << sceneName << std::endl;
        } else {
            std::cout << "Failed to load scene: " << sceneName << std::endl;
        }
    }
    
    if (ImGui::Button("Clear Scene", ImVec2(-1, 30))) {
        m_editor->clearScene();
        std::cout << "Scene cleared" << std::endl;
    }
    
    ImGui::End();
}

} // namespace WaterTown
