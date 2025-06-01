#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "StateManager.h"
#include "ResourceManager.h"
#include "../Systems/Grid.h"

class Game
{
public:
    Game();
    ~Game();

    void run();

    // 获取游戏实例（单例模式）
    static Game &getInstance();

    // 获取各种管理器
    StateManager &getStateManager();
    ResourceManager &getResourceManager();
    Grid &getGrid();
    sf::RenderWindow &getWindow();

private:
    void initialize();
    void handleEvents();
    void update(float deltaTime);
    void render();
    void cleanup();

    sf::RenderWindow m_window;
    sf::Clock m_clock;

    std::unique_ptr<StateManager> m_stateManager;
    std::unique_ptr<ResourceManager> m_resourceManager;
    std::unique_ptr<Grid> m_grid;

    bool m_isRunning;

    static Game *s_instance;
};