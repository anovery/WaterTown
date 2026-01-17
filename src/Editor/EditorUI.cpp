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
    
    // 游戏模式按钮:未放置船只时禁用
    bool canEnterGame = m_editor->canEnterGameMode();
    if (!canEnterGame) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
    }
    if (ImGui::Button("Game Mode", ImVec2(-1, 30))) {
        if (canEnterGame) {
            m_editor->switchMode(EditorMode::GAME);
        }
    }
    if (!canEnterGame) {
        ImGui::PopStyleVar();
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "Place a boat first!");
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
        
        // 地形类型选择 - 添加空地选项
        if (ImGui::RadioButton("Empty", m_selectedTerrainType == TerrainType::EMPTY)) {
            m_selectedTerrainType = TerrainType::EMPTY;
            m_editor->setCurrentTerrainType(TerrainType::EMPTY);
        }
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.6f, 0.5f, 0.4f, 1.0f), "[Brown]");
        
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
        ImGui::Text("Hold Left Click: Paint terrain");
        ImGui::Text("Right Click + Drag: Pan view");
        ImGui::Text("Scroll: Zoom in/out");
        ImGui::Text("Ctrl+Z: Undo");
        
        ImGui::Separator();
        if (ImGui::Button("Undo", ImVec2(-1, 0))) {
            m_editor->undoLastAction();
        }
    }
    else if (currentMode == EditorMode::BUILDING) {
        ImGui::Text("Object Types:");
        ImGui::Separator();
        
        if (ImGui::CollapsingHeader("Residential", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::RadioButton("Basic House", m_selectedObjectType == ObjectType::HOUSE)) {
                m_selectedObjectType = ObjectType::HOUSE;
                m_editor->setCurrentObjectType(ObjectType::HOUSE);
            }
            if (ImGui::RadioButton("Jiangnan House", m_selectedObjectType == ObjectType::HOUSE_STYLE_1)) {
                m_selectedObjectType = ObjectType::HOUSE_STYLE_1;
                m_editor->setCurrentObjectType(ObjectType::HOUSE_STYLE_1);
            }
            if (ImGui::RadioButton("Garden Villa", m_selectedObjectType == ObjectType::HOUSE_STYLE_2)) {
                m_selectedObjectType = ObjectType::HOUSE_STYLE_2;
                m_editor->setCurrentObjectType(ObjectType::HOUSE_STYLE_2);
            }
            if (ImGui::RadioButton("Ancestral Hall", m_selectedObjectType == ObjectType::HOUSE_STYLE_3)) {
                m_selectedObjectType = ObjectType::HOUSE_STYLE_3;
                m_editor->setCurrentObjectType(ObjectType::HOUSE_STYLE_3);
            }
            if (ImGui::RadioButton("Modern Villa", m_selectedObjectType == ObjectType::HOUSE_STYLE_4)) {
                m_selectedObjectType = ObjectType::HOUSE_STYLE_4;
                m_editor->setCurrentObjectType(ObjectType::HOUSE_STYLE_4);
            }
            if (ImGui::RadioButton("Farm House", m_selectedObjectType == ObjectType::HOUSE_STYLE_5)) {
                m_selectedObjectType = ObjectType::HOUSE_STYLE_5;
                m_editor->setCurrentObjectType(ObjectType::HOUSE_STYLE_5);
            }
            if (ImGui::RadioButton("Long House", m_selectedObjectType == ObjectType::LONG_HOUSE)) {
                m_selectedObjectType = ObjectType::LONG_HOUSE;
                m_editor->setCurrentObjectType(ObjectType::LONG_HOUSE);
            }
        }

        if (ImGui::CollapsingHeader("Water Structures", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::RadioButton("Stone Bridge", m_selectedObjectType == ObjectType::BRIDGE)) {
                m_selectedObjectType = ObjectType::BRIDGE;
                m_editor->setCurrentObjectType(ObjectType::BRIDGE);
            }
            if (ImGui::RadioButton("Arch Bridge", m_selectedObjectType == ObjectType::ARCH_BRIDGE)) {
                m_selectedObjectType = ObjectType::ARCH_BRIDGE;
                m_editor->setCurrentObjectType(ObjectType::ARCH_BRIDGE);
            }
            if (ImGui::RadioButton("Water Pavilion", m_selectedObjectType == ObjectType::WATER_PAVILION)) {
                m_selectedObjectType = ObjectType::WATER_PAVILION;
                m_editor->setCurrentObjectType(ObjectType::WATER_PAVILION);
            }
            if (ImGui::RadioButton("Wooden Pier", m_selectedObjectType == ObjectType::PIER)) {
                m_selectedObjectType = ObjectType::PIER;
                m_editor->setCurrentObjectType(ObjectType::PIER);
            }
            if (ImGui::RadioButton("Player Boat", m_selectedObjectType == ObjectType::BOAT)) {
                m_selectedObjectType = ObjectType::BOAT;
                m_editor->setCurrentObjectType(ObjectType::BOAT);
            }
            if (ImGui::RadioButton("Fishing Boat", m_selectedObjectType == ObjectType::FISHING_BOAT)) {
                m_selectedObjectType = ObjectType::FISHING_BOAT;
                m_editor->setCurrentObjectType(ObjectType::FISHING_BOAT);
            }
        }

        if (ImGui::CollapsingHeader("Nature & Decor", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::RadioButton("Tree", m_selectedObjectType == ObjectType::TREE)) {
                m_selectedObjectType = ObjectType::TREE;
                m_editor->setCurrentObjectType(ObjectType::TREE);
            }
            if (ImGui::RadioButton("Bamboo", m_selectedObjectType == ObjectType::BAMBOO)) {
                m_selectedObjectType = ObjectType::BAMBOO;
                m_editor->setCurrentObjectType(ObjectType::BAMBOO);
            }
            if (ImGui::RadioButton("Lotus Pond", m_selectedObjectType == ObjectType::LOTUS_POND)) {
                m_selectedObjectType = ObjectType::LOTUS_POND;
                m_editor->setCurrentObjectType(ObjectType::LOTUS_POND);
            }
            if (ImGui::RadioButton("Wall", m_selectedObjectType == ObjectType::WALL)) {
                m_selectedObjectType = ObjectType::WALL;
                m_editor->setCurrentObjectType(ObjectType::WALL);
            }
            if (ImGui::RadioButton("Pavilion", m_selectedObjectType == ObjectType::PAVILION)) {
                m_selectedObjectType = ObjectType::PAVILION;
                m_editor->setCurrentObjectType(ObjectType::PAVILION);
            }
            if (ImGui::RadioButton("Paifang (Gate)", m_selectedObjectType == ObjectType::PAIFANG)) {
                m_selectedObjectType = ObjectType::PAIFANG;
                m_editor->setCurrentObjectType(ObjectType::PAIFANG);
            }
            if (ImGui::RadioButton("Temple", m_selectedObjectType == ObjectType::TEMPLE)) {
                m_selectedObjectType = ObjectType::TEMPLE;
                m_editor->setCurrentObjectType(ObjectType::TEMPLE);
            }
            if (ImGui::RadioButton("Lantern", m_selectedObjectType == ObjectType::LANTERN)) {
                m_selectedObjectType = ObjectType::LANTERN;
                m_editor->setCurrentObjectType(ObjectType::LANTERN);
            }
            if (ImGui::RadioButton("Stone Lion", m_selectedObjectType == ObjectType::STONE_LION)) {
                m_selectedObjectType = ObjectType::STONE_LION;
                m_editor->setCurrentObjectType(ObjectType::STONE_LION);
            }
        }
        
        ImGui::Separator();
        ImGui::Text("Left Click: Place object");
        ImGui::Text("Ctrl + Left Click: Delete object");
        ImGui::Text("Right Click + Drag: Rotate view");
        ImGui::Text("Scroll: Zoom in/out");
        ImGui::Text("Ctrl+Z: Undo");
        
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Placement Rules:");
        ImGui::BulletText("Boat: Water only, max 1");
        ImGui::BulletText("House/Tree: Land only");
        
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Object Management:");
        if (ImGui::Button("Undo", ImVec2(-1, 0))) {
            m_editor->undoLastAction();
        }
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
    
    // 游戏模式下禁用场景操作
    bool isGameMode = m_editor->getCurrentMode() == EditorMode::GAME;
    
    static char sceneName[128] = "test_scene";
    ImGui::InputText("Scene Name", sceneName, IM_ARRAYSIZE(sceneName));
    
    ImGui::Separator();
    
    if (isGameMode) {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Exit game mode first!");
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
    }
    
    if (ImGui::Button("Save Scene", ImVec2(-1, 30)) && !isGameMode) {
        if (m_editor->saveScene(sceneName)) {
            std::cout << "Scene saved successfully: " << sceneName << std::endl;
        } else {
            std::cout << "Failed to save scene: " << sceneName << std::endl;
        }
    }
    
    if (ImGui::Button("Load Scene", ImVec2(-1, 30)) && !isGameMode) {
        if (m_editor->loadScene(sceneName)) {
            std::cout << "Scene loaded successfully: " << sceneName << std::endl;
        } else {
            std::cout << "Failed to load scene: " << sceneName << std::endl;
        }
    }
    
    if (ImGui::Button("Clear Scene", ImVec2(-1, 30)) && !isGameMode) {
        m_editor->clearScene();
        std::cout << "Scene cleared" << std::endl;
    }
    
    if (isGameMode) {
        ImGui::PopStyleVar();
    }
    
    ImGui::End();
}

} // namespace WaterTown
