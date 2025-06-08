#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class GameState;
class Game;

class StateManager
{
public:
    StateManager(Game *game);
    ~StateManager();

    void pushState(std::unique_ptr<GameState> state);
    void popState();
    void changeState(std::unique_ptr<GameState> state);
    void clearStates();

    void update(float deltaTime);
    void render(sf::RenderWindow &window);
    void handleEvent(const sf::Event &event);

    GameState *getCurrentState() const;
    bool isEmpty() const;

    Game *getGame() const;

private:
    std::vector<std::unique_ptr<GameState>> m_states;
    Game *m_game;
};
