#pragma once
#include <SFML/Graphics.hpp>

class StateManager;

class GameState
{
public:
    GameState(StateManager *stateManager) : m_stateManager(stateManager) {}
    virtual ~GameState() = default;

    // 纯虚函数，子类必须实现
    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void handleEvent(const sf::Event &event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow &window) = 0;

    // 状态控制
    virtual bool isPausable() const { return true; }
    virtual bool isTransparent() const { return false; }

protected:
    StateManager *m_stateManager;
};