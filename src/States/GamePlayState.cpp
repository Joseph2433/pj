#include "States/GamePlayState.h"
#include "States/MenuState.h"
#include "States/GameOverState.h"
#include "States/VictoryState.h"
#include "Core/StateManager.h"
#include "States/PauseState.h"
#include "../Utils/SoundManager.h"
#include "Core/Game.h"
#include "Core/ResourceManager.h"
#include "../Utils/Constants.h"
#include "../Entities/Plant.h"
#include "../Entities/Zombie.h"
#include "../Entities/Projectile.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace
{
    float randomFloat(float min, float max)
    {
        if (min >= max)
            return min;
        if ((max - min) == 0.0f)
            return min;
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }
}

GamePlayState::GamePlayState(StateManager *stateManager)
    : GameState(stateManager),
      m_fontsLoaded(false),
      m_grid(),
      m_sunManager(INITIAL_SUN_AMOUNT),
      m_projectileManager(stateManager->getGame()->getResourceManager()),
      m_zombieManager(stateManager->getGame()->getResourceManager(), m_grid),
      m_waveManager(m_zombieManager, *stateManager->getGame()),
      m_plantManager(stateManager->getGame()->getResourceManager(), m_grid, *this, m_projectileManager, m_zombieManager),
      m_hud(stateManager->getGame()->getResourceManager(), m_sunManager, m_waveManager, m_primaryGameFont, m_secondaryGameFont),
      m_gameTime(0.0f),
      m_skySunSpawnIntervalMin(5.0f),
      m_skySunSpawnIntervalMax(12.0f),
      m_currentSkySunSpawnInterval(0.0f),
      m_collisionSystem(),
      m_isGameOver(false)
{
    std::cout << "GamePlayState 正在构造..." << std::endl;
    loadAssets();

    m_currentSkySunSpawnInterval = randomFloat(m_skySunSpawnIntervalMin, m_skySunSpawnIntervalMax);
    m_skySunSpawnTimer.restart();
    std::cout << "GamePlayState 构造完毕。" << std::endl;
}

void GamePlayState::loadAssets()
{
    std::cout << "GamePlayState:正在加载资源..." << std::endl;
    if (!m_stateManager || !m_stateManager->getGame())
    {
        std::cerr << "GamePlayState::loadAssets: Fatal error - StateManager or Game pointer is null!" << std::endl;
        return;
    }
    ResourceManager &resMan = m_stateManager->getGame()->getResourceManager();

    std::string gameplayBgTextureId = "GamePlayBackgroundTexture";
    std::string gameplayBgTexturePath = "../../assets/images/gameplay_background.png";

    if (!resMan.hasTexture(gameplayBgTextureId))
    {
        std::cout << "GamePlayState::loadAssets: Attempting to load gameplay background from: " << gameplayBgTexturePath << std::endl;
        if (!resMan.loadTexture(gameplayBgTextureId, gameplayBgTexturePath))
        {
            std::cerr << "GamePlayState::loadAssets: Failed to load gameplay background texture: " << gameplayBgTexturePath << std::endl;
        }
        else
        {
            std::cout << "GamePlayState::loadAssets: Successfully loaded gameplay background." << std::endl;
        }
    }
    else
    {
        std::cout << "GamePlayState::loadAssets: Gameplay background '" << gameplayBgTextureId << "' already loaded." << std::endl;
    }

    // 字体加载
    if (resMan.hasFont(FONT_ID_PRIMARY))
    {
        m_primaryGameFont = resMan.getFont(FONT_ID_PRIMARY);
        m_fontsLoaded = true;
    }
    else
    {
        std::cerr << "GamePlayState:主要字体 (ID: " << FONT_ID_PRIMARY << ") 未从 ResourceManager 加载!" << std::endl;
        if (m_primaryGameFont.loadFromFile(FONT_PATH_ARIAL))
        {
            m_fontsLoaded = true;
            std::cout << "GamePlayState: Loaded fallback primary font: " << FONT_PATH_ARIAL << std::endl;
        }
        else
        {
            std::cerr << "GamePlayState:备用主要字体也加载失败!" << std::endl;
        }
    }
    if (resMan.hasFont(FONT_ID_SECONDARY))
    {
        m_secondaryGameFont = resMan.getFont(FONT_ID_SECONDARY);
    }
    else
    {
        std::cerr << "GamePlayState:次要字体 (ID: " << FONT_ID_SECONDARY << ") 未从 ResourceManager 加载，尝试备用。" << std::endl;
        if (m_secondaryGameFont.loadFromFile(FONT_PATH_VERDANA))
        {
            std::cout << "GamePlayState: Loaded fallback secondary font: " << FONT_PATH_VERDANA << std::endl;
        }
        else
        {
            std::cerr << "GamePlayState:备用次要字体也加载失败, 将使用主要字体。" << std::endl;
            m_secondaryGameFont = m_primaryGameFont;
        }
    }
    if (!m_fontsLoaded && m_primaryGameFont.getInfo().family.empty())
    {
        std::cerr << "GamePlayState:警告 - 没有有效的字体被加载!UI文本可能无法显示。" << std::endl;
    }
    if (!resMan.hasTexture(BASIC_ZOMBIE_TEXTURE_KEY))
    {
        resMan.loadTexture(BASIC_ZOMBIE_TEXTURE_KEY, "../../assets/images/basic_zombie.png");
    }

    if (!resMan.hasTexture(BIG_ZOMBIE_TEXTURE_KEY))
    {
        resMan.loadTexture(BIG_ZOMBIE_TEXTURE_KEY, "../../assets/images/big_zombie.png");
    }

    if (!resMan.hasTexture(BOSS_ZOMBIE_TEXTURE_KEY))
    {
        resMan.loadTexture(BOSS_ZOMBIE_TEXTURE_KEY, "../../assets/images/boss_zombie.png");
    }

    if (!resMan.hasTexture(QUICK_ZOMBIE_TEXTURE_KEY))
    {
        resMan.loadTexture(QUICK_ZOMBIE_TEXTURE_KEY, "../../assets/images/quick_zombie.png");
    }
    if (!resMan.hasTexture(SUNFLOWER_TEXTURE_KEY))
    {
        resMan.loadTexture(SUNFLOWER_TEXTURE_KEY, "../../assets/images/sunflower.png");
    }
    if (!resMan.hasTexture(PEASHOOTER_TEXTURE_KEY))
    {
        resMan.loadTexture(PEASHOOTER_TEXTURE_KEY, "../../assets/images/peashooter.png");
    }
    if (!resMan.hasTexture(WALLNUT_TEXTURE_KEY))
    {
        resMan.loadTexture(WALLNUT_TEXTURE_KEY, "../../assets/images/wallnut.png");
    }
    if (!resMan.hasTexture(ICE_PEASHOOTER_TEXTURE_KEY))
    {
        resMan.loadTexture(ICE_PEASHOOTER_TEXTURE_KEY, "../../assets/images/ice_peashooter.png");
    }
    if (!resMan.hasTexture(SUN_TEXTURE_KEY))
    {
        resMan.loadTexture(SUN_TEXTURE_KEY, "../../assets/images/sun.png");
    }
    if (!resMan.hasTexture(PEA_TEXTURE_KEY))
    {
        resMan.loadTexture(PEA_TEXTURE_KEY, "../../assets/images/pea.png");
    }
    if (!resMan.hasTexture(ICE_PEA_TEXTURE_KEY))
    {
        resMan.loadTexture(ICE_PEA_TEXTURE_KEY, "../../assets/images/ice_pea.png");
    }
    std::cout << "GamePlayState:source load trying finish。" << std::endl;
}

void GamePlayState::enter()
{
    std::cout << "GamePlayState enter。" << std::endl;
    if (!m_stateManager || !m_stateManager->getGame())
    {
        std::cerr << "GamePlayState::enter: Fatal error - StateManager or Game pointer is null!" << std::endl;
        return;
    }
    ResourceManager &resManager = m_stateManager->getGame()->getResourceManager();

    std::string gameplayBgTextureId = "GamePlayBackgroundTexture";
    if (!resManager.hasTexture(gameplayBgTextureId))
    {
        std::cerr << "GamePlayState::enter: Gameplay background texture NOT FOUND. ID: " << gameplayBgTextureId << std::endl;
    }
    m_BackgroundSpite.setTexture(resManager.getTexture(gameplayBgTextureId));
    const sf::Texture *tex = m_BackgroundSpite.getTexture();
    if (tex && tex->getSize().x > 0 && tex->getSize().y > 0)
    {
        sf::Vector2u textureSize = tex->getSize();
        m_BackgroundSpite.setScale(
            static_cast<float>(WINDOW_WIDTH) / textureSize.x,
            static_cast<float>(WINDOW_HEIGHT) / textureSize.y);
    }
    m_BackgroundSpite.setPosition(0.f, 0.f);

    if (m_fontsLoaded)
    {
        m_debugInfoText.setFont(m_primaryGameFont);
    }
    else
    {
        std::cerr << "GamePlayState::enter - 调试文本无法设置字体，因字体未加载。" << std::endl;
    }
    m_debugInfoText.setCharacterSize(14);
    m_debugInfoText.setFillColor(sf::Color::White);
    m_debugInfoText.setPosition(10, WINDOW_HEIGHT - 50);

    m_grid.initialize();
    m_sunManager.reset();
    m_plantManager.clear();
    m_projectileManager.clear();
    m_zombieManager.clear();
    m_activeSuns.clear();
    m_skySunSpawnTimer.restart();
    m_currentSkySunSpawnInterval = randomFloat(m_skySunSpawnIntervalMin, m_skySunSpawnIntervalMax);
    m_isGameOver = false;
    m_waveManager.start();

    m_gameTime = 0.f;

    if (m_stateManager && m_stateManager->getGame())
    {
        SoundManager &soundMan = m_stateManager->getGame()->getSoundManager();
        if (soundMan.isMusicPlaying() && soundMan.getCurrentPlayingMusicId() != BGM_GAMEPLAY)
        { // SoundManager需要暴露m_currentPlayingMusicId或提供getter
            soundMan.stopMusic();
        }
        soundMan.playMusic(BGM_GAMEPLAY, true, 60.f);
    }
    else
    {
        std::cerr << "GamePlayState::enter - Cannot access SoundManager, Game or StateManager is null." << std::endl;
    }
    std::cout << "GamePlayState enter finish。" << std::endl;
}

void GamePlayState::exit()
{
    std::cout << "GamePlayState exit。" << std::endl;
    m_plantManager.clear();
    m_projectileManager.clear();
    m_zombieManager.clear();
    m_activeSuns.clear();
    m_waveManager.reset();
    if (m_stateManager && m_stateManager->getGame())
    {
        SoundManager &soundMan = m_stateManager->getGame()->getSoundManager();
        if (soundMan.getCurrentPlayingMusicId() == BGM_GAMEPLAY)
        {
            soundMan.stopMusic();
            std::cout << "GamePlayState: Stopped gameplay BGM." << std::endl;
        }
    }
}

void GamePlayState::handleEvent(const sf::Event &event)
{
    sf::RenderWindow &window = m_stateManager->getGame()->getWindow();
    sf::Vector2f mousePosView;

    if (event.type == sf::Event::MouseMoved)
    {
        m_mousePixelPos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
        mousePosView = window.mapPixelToCoords(m_mousePixelPos, window.getView());
    }
    else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
    {
        mousePosView = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), window.getView());
    }
    else
    {
        mousePosView = window.mapPixelToCoords(m_mousePixelPos, window.getView());
    }

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            if (m_hud.getCurrentInteractionMode() == HUDInteractionMode::SHOVEL_SELECTED)
            {
                m_hud.resetInteractionMode();
                std::cout << "GamePlayState: Shovel mode cancelled by ESC." << std::endl;
            }
            else
            {
                m_stateManager->pushState(std::make_unique<PauseState>(m_stateManager)); // 返回主菜单
            }
            return;
        }
        if (event.key.code == sf::Keyboard::F1)
        {
            m_sunManager.addSun(100);
            std::cout << "sun add to " << m_sunManager.getCurrentSun() << std::endl;
        }
    }

    // 2. 将所有事件（包括按键和鼠标）传递给 HUD 处理
    bool eventConsumedByHUD = m_hud.handleEvent(event, mousePosView);

    if (eventConsumedByHUD)
    {
        std::cout << "GamePlayState: Event was consumed by HUD. Game area logic for this mouse click will be skipped." << std::endl;
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            // --- 左键点击逻辑 ---
            if (m_hud.getCurrentInteractionMode() == HUDInteractionMode::SHOVEL_SELECTED)
            {
                // --- A. 铲子模式：尝试移除植物 ---
                sf::Vector2i gridCoords = m_grid.getGridPosition(mousePosView);

                if (m_grid.isValidGridPosition(gridCoords))
                {
                    Plant *plantToShovel = m_plantManager.getPlantAt(gridCoords);
                    if (plantToShovel)
                    {
                        std::cout << "GamePlayState: Shoveling plant at grid (" << gridCoords.x << "," << gridCoords.y << ")" << std::endl;
                        m_plantManager.removePlant(plantToShovel);
                    }
                    else
                    {
                        std::cout << "GamePlayState: Shovel clicked on empty grid cell (" << gridCoords.x << "," << gridCoords.y << ")." << std::endl;
                    }
                }
                else
                {
                    std::cout << "GamePlayState: Shovel clicked outside valid grid." << std::endl;
                }
                m_hud.resetInteractionMode();
                return;
            }
            else if (m_hud.getCurrentInteractionMode() == HUDInteractionMode::NORMAL)
            {
                // --- B. 正常模式：收集阳光或尝试种植 ---
                // B.1 尝试收集阳光
                bool sunCollectedThisClick = false;
                for (auto it = m_activeSuns.rbegin(); it != m_activeSuns.rend(); ++it)
                {
                    if (!(*it)->isCollected() && (*it)->handleClick(mousePosView))
                    {
                        (*it)->collect();
                        sunCollectedThisClick = true;
                        std::cout << "GamePlayState: Sun collected." << std::endl;
                        break;
                    }
                }
                if (sunCollectedThisClick)
                {
                    return;
                }

                // B.2 尝试种植植物
                bool clickedOnGridArea = mousePosView.y > (SEED_PACKET_UI_START_Y + SEED_PACKET_HEIGHT + SEED_PACKET_SPACING);
                if (clickedOnGridArea)
                {
                    sf::Vector2i gridCoords = m_grid.getGridPosition(mousePosView);
                    bool isValidPlantSelection = false;
                    PlantType selectedPlant = m_hud.getSelectedPlantTypeFromSeedManager(isValidPlantSelection);

                    if (m_grid.isValidGridPosition(gridCoords) && isValidPlantSelection)
                    {
                        if (!m_grid.isCellOccupied(gridCoords.x, gridCoords.y))
                        {
                            int cost = m_hud.getSelectedPlantCostFromSeedManager();
                            if (m_sunManager.getCurrentSun() >= cost)
                            {
                                if (m_plantManager.tryAddPlant(selectedPlant, gridCoords))
                                {
                                    m_sunManager.trySpendSun(cost);
                                    m_hud.notifyPlantPlacedToSeedManager(selectedPlant);
                                }
                                else
                                {
                                    std::cout << "GamePlayState: PlantManager failed to add plant." << std::endl;
                                }
                            }
                            else
                            {
                                std::cout << "GamePlayState: Sun is not enough to plant." << std::endl;
                            }
                        }
                        else
                        {
                            std::cout << "GamePlayState: Cell (" << gridCoords.x << "," << gridCoords.y << ") is already occupied!" << std::endl;
                        }
                    }
                    else
                    {
                        if (!m_grid.isValidGridPosition(gridCoords))
                            std::cout << "GamePlayState: Planting click on invalid grid position." << std::endl;
                        else if (!isValidPlantSelection)
                            std::cout << "GamePlayState: No valid plant selected from HUD for planting." << std::endl;
                    }
                }
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Right)
        {
            // --- 右键点击逻辑 ---
            if (m_hud.getCurrentInteractionMode() == HUDInteractionMode::SHOVEL_SELECTED)
            {
                m_hud.resetInteractionMode();
                std::cout << "GamePlayState: Shovel mode cancelled by Right Click." << std::endl;
                return;
            }
        }
    }
}

void GamePlayState::update(float deltaTime)
{
    if (m_isGameOver)
        return;

    m_gameTime += deltaTime;

    for (auto &sun : m_activeSuns)
    {
        sun->update(deltaTime);
    }
    m_activeSuns.erase(
        std::remove_if(m_activeSuns.begin(), m_activeSuns.end(),
                       [](const std::unique_ptr<Sun> &s)
                       { return s->isExpired(); }),
        m_activeSuns.end());

    if (m_skySunSpawnTimer.getElapsedTime().asSeconds() >= m_currentSkySunSpawnInterval)
    {
        spawnSunFromSky();
    }

    m_plantManager.update(deltaTime);
    m_projectileManager.update(deltaTime, m_stateManager->getGame()->getWindow());

    std::vector<Plant *> activePlantsForZombies = m_plantManager.getAllActivePlants();
    std::vector<Zombie *> activeZombiesToUpdate = m_zombieManager.getActiveZombies();
    for (Zombie *zombie : activeZombiesToUpdate)
    {
        if (zombie && zombie->isAlive())
        {
            int currentZombieLane = zombie->getLane();
            std::vector<Plant *> plantsInThisSpecificLane;
            if (currentZombieLane != -1)
            {
                for (Plant *p : activePlantsForZombies)
                {
                    if (p && p->isAlive() && p->getRow() == currentZombieLane)
                    {
                        plantsInThisSpecificLane.push_back(p);
                    }
                }
            }
            zombie->update(deltaTime, plantsInThisSpecificLane);
        }
    }
    for (Zombie *zombie : m_zombieManager.getActiveZombies())
    {
        if (zombie && zombie->isAlive() && zombie->getPosition().x < ZOMBIE_REACHED_HOUSE_X)
        {
            std::cout << "Game Over: A zombie reached the house!" << std::endl;
            m_isGameOver = true;
            m_stateManager->changeState(std::make_unique<GameOverState>(m_stateManager));
            return;
        }
    }
    m_zombieManager.update(deltaTime, m_stateManager->getGame()->getWindow());
    m_collisionSystem.update(m_projectileManager, m_zombieManager, m_plantManager);
    m_waveManager.update(deltaTime);
    m_hud.update(deltaTime);

    if (!m_isGameOver &&
        m_waveManager.getCurrentWaveNumber() >= TOTAL_WAVES_TO_WIN &&
        m_waveManager.getCurrentSpawnState() == SpawnState::ALL_WAVES_COMPLETED &&
        m_zombieManager.getActiveZombies().empty())
    {
        std::cout << "Victory! All waves cleared." << std::endl;
        m_isGameOver = true;
        m_stateManager->changeState(std::make_unique<VictoryState>(m_stateManager));
        return;
    }

    std::stringstream ss;
    ss.precision(1);
    ss << std::fixed;
    ss << "Time: " << m_gameTime << "s | FPS: " << static_cast<int>(1.f / deltaTime)
       << " | Mouse: (" << m_mousePixelPos.x << "," << m_mousePixelPos.y << ")"
       << " | Suns: " << m_sunManager.getCurrentSun()
       << " | Entities: S:" << m_activeSuns.size()
       << " P:" << m_projectileManager.getAllProjectiles().size()
       << " Z:" << m_zombieManager.getActiveZombies().size()
       << " | Plants: " << m_plantManager.getAllActivePlants().size()
       << " | " << m_waveManager.getCurrentWaveStatusText();
    m_debugInfoText.setString(ss.str());
}

void GamePlayState::render(sf::RenderWindow &window)
{

    window.draw(m_BackgroundSpite);
    m_grid.render(window);
    m_plantManager.draw(window);
    for (const auto &sun : m_activeSuns)
    {
        sun->draw(window);
    }
    m_projectileManager.draw(window);
    m_zombieManager.draw(window);
    m_hud.draw(window);
    window.draw(m_debugInfoText);
}

void GamePlayState::spawnSunFromSky()
{
    sf::RenderWindow &window = m_stateManager->getGame()->getWindow();
    float spawnX = randomFloat(window.getSize().x * 0.05f, window.getSize().x * 0.95f);
    float spawnY = -30.f;
    float groundMinY = GRID_START_Y + (GRID_ROWS / 2.0f) * GRID_CELL_HEIGHT;
    float groundMaxY = GRID_START_Y + GRID_ROWS * GRID_CELL_HEIGHT - GRID_CELL_HEIGHT * 0.5f;
    groundMaxY = std::min(groundMaxY, static_cast<float>(window.getSize().y - 50.f));
    if (groundMinY >= groundMaxY)
        groundMinY = groundMaxY - 50.f;
    float targetY = randomFloat(groundMinY, groundMaxY);

    m_activeSuns.emplace_back(std::make_unique<Sun>(
        m_stateManager->getGame()->getResourceManager(), m_sunManager,
        sf::Vector2f(spawnX, spawnY), SunSpawnType::FROM_SKY, targetY));

    m_skySunSpawnTimer.restart();
    m_currentSkySunSpawnInterval = randomFloat(m_skySunSpawnIntervalMin, m_skySunSpawnIntervalMax);
}

void GamePlayState::spawnSunFromPlant(Plant *plant)
{
    if (!plant)
        return;
    sf::Vector2f plantPos = plant->getPosition();
    float heightOffset = plant->getGlobalBounds().height * 0.3f;
    if (heightOffset <= 0)
        heightOffset = 20.f;

    sf::Vector2f sunSpawnPos = sf::Vector2f(plantPos.x, plantPos.y - heightOffset);
    m_activeSuns.emplace_back(std::make_unique<Sun>(
        m_stateManager->getGame()->getResourceManager(), m_sunManager,
        sunSpawnPos, SunSpawnType::FROM_PLANT));
}

void GamePlayState::resetLevel()
{
    std::cout << "GamePlayState: Resetting level..." << std::endl;

    m_isGameOver = false;
    m_gameTime = 0.f;

    m_sunManager.reset();
    m_plantManager.clear();
    m_projectileManager.clear();
    m_zombieManager.clear();
    m_waveManager.reset();
    m_waveManager.start();

    m_activeSuns.clear();

    m_skySunSpawnTimer.restart();
    m_currentSkySunSpawnInterval = randomFloat(m_skySunSpawnIntervalMin, m_skySunSpawnIntervalMax);

    std::cout << "GamePlayState: Level reset complete." << std::endl;
}