#pragma once

#include <SFML/Graphics.hpp>
#include "StateManager.h"
#include "ResourceManager.h"
#include "../Utils/SoundManager.h"

class Game
{
public:
    Game();
    ~Game() = default;
    void run();

    ResourceManager &getResourceManager();
    StateManager &getStateManager();
    sf::RenderWindow &getWindow();
    SoundManager &getSoundManager();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void loadGlobalResources();

    sf::RenderWindow m_window;
    ResourceManager m_resourceManager;
    StateManager m_stateManager;
    SoundManager m_soundManager;
};
