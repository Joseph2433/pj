#pragma once
#include <stack>
#include <memory>
#include <SFML/Graphics.hpp>

class GameState;

class StateManager
{
public:
    StateManager();
    ~StateManager();

    // 状态管理
    void pushState(std::unique_ptr<GameState> state);
    void popState();
    void changeState(std::unique_ptr<GameState> state);
    void clearStates();

    // 游戏循环
    void handleEvents(const sf::Event &event);
    void update(float deltaTime);
    void render(sf::RenderWindow &window);

    // 状态查询
    bool isEmpty() const;
    GameState *getCurrentState() const;

private:
    std::stack<std::unique_ptr<GameState>> m_states;

    // 延迟操作，避免在更新过程中修改栈
    enum class StateAction
    {
        None,
        Push,
        Pop,
        Change,
        Clear
    };

    StateAction m_pendingAction;
    std::unique_ptr<GameState> m_pendingState;

    void processPendingAction();
};