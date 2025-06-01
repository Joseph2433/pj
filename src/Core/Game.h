#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class StateManager;

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    void init();
    void handleEvents();
    void update(float deltaTime);
    void render();
    void cleanup();

    sf::RenderWindow m_window;
    sf::Clock m_clock;
    bool m_isRunning;

    std::unique_ptr<StateManager> m_stateManager;
};