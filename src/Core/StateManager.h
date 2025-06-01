#pragma once
#include "GameState.h"
#include <stack>
#include <memory>

class StateManager
{
public:
    StateManager() = default;
    ~StateManager() = default;

    // 状态管理
    void pushState(std::unique_ptr<GameState> state);
    void popState();
    void changeState(std::unique_ptr<GameState> state);
    void clearStates();

    // 更新和渲染
    void update(float deltaTime);
    void render(sf::RenderWindow &window);
    void handleEvent(const sf::Event &event);

    // 获取当前状态
    GameState *getCurrentState() const;
    bool isEmpty() const;

private:
    std::stack<std::unique_ptr<GameState>> m_states;
};