#pragma once
#include <SFML/Graphics.hpp>

class StateManager;

class GameState
{
public:
    GameState(StateManager *stateManager) : m_stateManager(stateManager) {}
    virtual ~GameState() = default;

    // 状态生命周期函数
    virtual void init() = 0;
    virtual void cleanup() = 0;

    // 游戏循环函数
    virtual void handleEvents(const sf::Event &event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow &window) = 0;

    // 状态控制
    virtual void pause() {}
    virtual void resume() {}

protected:
    StateManager *m_stateManager;
};