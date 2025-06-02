#pragma once

#include <SFML/Graphics.hpp>
#include "Core/StateManager.h"    // 包含 StateManager
#include "Core/ResourceManager.h" // 包含 ResourceManager
// #include "Utils/Constants.h"    // Constants.h 应该被各处需要的地方包含

class Game
{
public:
    Game();
    ~Game() = default;

    void run(); // 主游戏循环

    ResourceManager &getResourceManager();
    StateManager &getStateManager(); // 如果需要外部访问StateManager
    sf::RenderWindow &getWindow();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    sf::RenderWindow m_window;
    ResourceManager m_resourceManager;
    StateManager m_stateManager;

    // 控制游戏循环
    const sf::Time TimePerFrame = sf::seconds(1.f / 60.f); // 目标帧率
};
