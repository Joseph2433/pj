#pragma once

#include "Core/GameState.h"
#include "../Systems/Grid.h"
#include "../Systems/ProjectileManager.h"
#include "../Systems/PlantManager.h"
#include "../Systems/ZombieManager.h"
#include "../UI/HUD.h"
#include "../Systems/SunManager.h"
#include "../Entities/Sun.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/WaveManager.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class StateManager;
class Plant;

class GamePlayState : public GameState
{
public:
    GamePlayState(StateManager *stateManager);
    ~GamePlayState() override = default;
    void enter() override;
    void exit() override;
    void handleEvent(const sf::Event &event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;
    void spawnSunFromPlant(Plant *plant);
    void resetLevel();

private:
    void loadAssets();
    void spawnSunFromSky();
    void spawnInitialZombiesForTesting();

    // 字体
    sf::Font m_primaryGameFont;
    sf::Font m_secondaryGameFont;
    bool m_fontsLoaded;

    // 背景
    sf::Sprite m_BackgroundSpite;

    // UI
    sf::Text m_debugInfoText;

    Grid m_grid;
    ProjectileManager m_projectileManager;
    PlantManager m_plantManager;
    SunManager m_sunManager;
    ZombieManager m_zombieManager;
    HUD m_hud;
    WaveManager m_waveManager;
    std::vector<std::unique_ptr<Sun>> m_activeSuns;

    // 天空阳光生成
    sf::Clock m_skySunSpawnTimer;
    float m_skySunSpawnIntervalMin;
    float m_skySunSpawnIntervalMax;
    float m_currentSkySunSpawnInterval;

    float m_gameTime;
    sf::Vector2i m_mousePixelPos;

    // 碰撞
    CollisionSystem m_collisionSystem;

    bool m_isGameOver;
};
