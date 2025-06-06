#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

// 前向声明
class GameState;
class Game; // <--- 前向声明 Game 类

class StateManager
{
public:
    StateManager(Game *game); // <--- 构造函数接收 Game*
    ~StateManager();          // 如果需要，可以添加清理逻辑

    void pushState(std::unique_ptr<GameState> state);
    void popState();
    void changeState(std::unique_ptr<GameState> state);
    void clearStates(); // 清空所有状态

    void update(float deltaTime);
    void render(sf::RenderWindow &window);
    void handleEvent(const sf::Event &event);

    GameState *getCurrentState() const;
    bool isEmpty() const;

    Game *getGame() const; // <--- 公开的 getGame() 方法

private:
    std::vector<std::unique_ptr<GameState>> m_states;
    Game *m_game;
};
