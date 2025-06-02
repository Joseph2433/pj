#pragma once

#include "Core/GameState.h"
#include "../Systems/Grid.h"         // 包含Grid头文件
#include "../Systems/PlantManager.h" // 包含PlantManager头文件
#include <SFML/Graphics.hpp>

class StateManager;
// class Game; // 如果需要通过StateManager获取Game对象来访问ResourceManager

class GamePlayState : public GameState
{
public:
    // 构造函数现在需要一个指向 StateManager 的指针
    // 以便能够访问 ResourceManager (通常通过 Game 对象)
    GamePlayState(StateManager *stateManager);
    ~GamePlayState() override = default;

    void enter() override;
    void exit() override;
    void handleEvent(const sf::Event &event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

private:
    // 字体相关
    sf::Font m_font;
    bool m_useCustomFont = false;

    // UI元素
    sf::RectangleShape m_background;
    sf::Text m_titleText;
    sf::Text m_instructionText; // 恢复说明文本，用于提示操作
    sf::Text m_gameInfoText;    // 可以保留用于显示其他信息

    // 游戏相关
    float m_gameTime;
    sf::Vector2i m_mousePos;

    // --- 新增成员 ---
    Grid m_grid;                     // 游戏网格系统
    PlantManager m_plantManager;     // 植物管理器
    PlantType m_currentPlantToPlace; // 当前选择种植的植物类型 (默认为向日葵)
    sf::Text m_plantingModeText;     // 用于显示当前选择的植物类型

    // 辅助函数，用于更新种植模式文本
    void updatePlantingModeText();
};
