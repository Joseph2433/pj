#pragma once
#include <SFML/Graphics.hpp>

class StateManager; // 前向声明

class GameState
{
public:
    GameState(StateManager *stateManager) : m_stateManager(stateManager) {}
    virtual ~GameState() = default;

    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void handleEvent(const sf::Event &event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow &window) = 0;

protected:
    StateManager *m_stateManager; // 指向状态管理器
};
