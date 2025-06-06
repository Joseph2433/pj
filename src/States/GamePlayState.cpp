#include "States/GamePlayState.h"
#include "States/MenuState.h"
#include "Core/StateManager.h"
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

// 确保 randomFloat 辅助函数可用
namespace
{
    float randomFloat(float min, float max)
    {
        if (min >= max)
            return min;
        // 修正可能的除零错误，如果 min == max，则 RAND_MAX / 0 会出问题
        if ((max - min) == 0.0f)
            return min; // 或者抛出异常，或者返回一个固定值
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
      m_hud(stateManager->getGame()->getResourceManager(), m_sunManager, m_waveManager, m_primaryGameFont, m_secondaryGameFont), // <--- 将 m_waveManager 传递给 HUD
      m_gameTime(0.0f),
      m_skySunSpawnIntervalMin(5.0f),
      m_skySunSpawnIntervalMax(12.0f),
      m_currentSkySunSpawnInterval(0.0f),
      m_collisionSystem()

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
    std::string gameplayBgTexturePath = "../../assets/images/gameplay_background.jpg"; // 或 .png

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

    // 字体加载 (填充你省略的部分)
    if (resMan.hasFont(FONT_ID_PRIMARY))
    {
        m_primaryGameFont = resMan.getFont(FONT_ID_PRIMARY);
        m_fontsLoaded = true;
    }
    else
    {
        std::cerr << "GamePlayState:主要字体 (ID: " << FONT_ID_PRIMARY << ") 未从 ResourceManager 加载!" << std::endl;
        if (m_primaryGameFont.loadFromFile(FONT_PATH_ARIAL))
        { // 使用常量
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
        { // 使用另一个常量或路径
            std::cout << "GamePlayState: Loaded fallback secondary font: " << FONT_PATH_VERDANA << std::endl;
        }
        else
        {
            std::cerr << "GamePlayState:备用次要字体也加载失败, 将使用主要字体。" << std::endl;
            m_secondaryGameFont = m_primaryGameFont; // 使用已加载的主字体（如果有）
        }
    }
    if (!m_fontsLoaded && m_primaryGameFont.getInfo().family.empty())
    {
        std::cerr << "GamePlayState:警告 - 没有有效的字体被加载!UI文本可能无法显示。" << std::endl;
    }

    // 其他资源加载 (填充你省略的部分)
    if (!resMan.hasTexture(BASIC_ZOMBIE_TEXTURE_KEY))
    {
        if (!resMan.loadTexture(BASIC_ZOMBIE_TEXTURE_KEY, "assets/images/zombies/basic_zombie.png"))
        {
            std::cerr << "GamePlayState: 普通僵尸纹理 (" << BASIC_ZOMBIE_TEXTURE_KEY << ") 加载失败！" << std::endl;
        }
    }
    if (!resMan.hasTexture(SUNFLOWER_TEXTURE_KEY))
    {
        resMan.loadTexture(SUNFLOWER_TEXTURE_KEY, "assets/images/plants/sunflower.png");
    }
    if (!resMan.hasTexture(PEASHOOTER_TEXTURE_KEY))
    {
        resMan.loadTexture(PEASHOOTER_TEXTURE_KEY, "assets/images/plants/peashooter.png");
    }
    if (!resMan.hasTexture(SUNFLOWER_ICON_TEXTURE_KEY))
    {
        resMan.loadTexture(SUNFLOWER_ICON_TEXTURE_KEY, "assets/images/plants/sunflower_icon.png");
    }
    if (!resMan.hasTexture(PEASHOOTER_ICON_TEXTURE_KEY))
    {
        resMan.loadTexture(PEASHOOTER_ICON_TEXTURE_KEY, "assets/images/plants/peashooter_icon.png");
    }
    if (!resMan.hasTexture(SUN_TEXTURE_KEY))
    {
        resMan.loadTexture(SUN_TEXTURE_KEY, "assets/images/ui/sun.png");
    }
    if (!resMan.hasTexture(PEA_TEXTURE_KEY))
    {
        if (!resMan.loadTexture(PEA_TEXTURE_KEY, "assets/images/projectiles/pea.png"))
        {
            std::cerr << "GamePlayState: 豌豆子弹纹理 (" << PEA_TEXTURE_KEY << ") 加载失败!" << std::endl;
        }
    }
    std::cout << "GamePlayState:资源加载尝试完毕。" << std::endl;
}

void GamePlayState::enter()
{
    std::cout << "GamePlayState 进入。" << std::endl;
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
        // 可以尝试使用 ResourceManager 的默认字体，如果 ResourceManager 有此机制
        // m_debugInfoText.setFont(resManager.getDefaultFont());
    }
    m_debugInfoText.setCharacterSize(14);
    m_debugInfoText.setFillColor(sf::Color::White);
    m_debugInfoText.setPosition(10, WINDOW_HEIGHT - 50); // 调整Y位置以防遮挡

    m_grid.initialize();
    m_sunManager.reset();
    m_plantManager.clear();
    m_projectileManager.clear();
    m_zombieManager.clear();
    m_activeSuns.clear();
    // 如果 HUD 或 SeedManager 需要在进入时重置，应有相应方法
    // m_hud.resetState(); // 示例

    m_skySunSpawnTimer.restart();
    m_currentSkySunSpawnInterval = randomFloat(m_skySunSpawnIntervalMin, m_skySunSpawnIntervalMax);

    m_waveManager.start(); // <--- 启动或重置 WaveManager

    m_gameTime = 0.f;
    std::cout << "GamePlayState enter 逻辑执行完毕。" << std::endl;
}

void GamePlayState::exit()
{
    std::cout << "GamePlayState exit。" << std::endl;
    m_plantManager.clear();
    m_projectileManager.clear();
    m_zombieManager.clear();
    m_activeSuns.clear();
    m_waveManager.reset(); // <--- 推荐在退出时重置 WaveManager
}

void GamePlayState::handleEvent(const sf::Event &event)
{
    sf::RenderWindow &window = m_stateManager->getGame()->getWindow();
    sf::Vector2f currentEventMousePosView;

    if (event.type == sf::Event::MouseMoved)
    {
        m_mousePixelPos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
    }

    // 确保 currentEventMousePosView 总是有最新的映射值
    if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::MouseMoved)
    {
        currentEventMousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
    }
    else
    { // 其他事件类型，也获取当前鼠标位置（如果需要的话）
        currentEventMousePosView = window.mapPixelToCoords(m_mousePixelPos, window.getView());
    }

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            m_stateManager->changeState(std::make_unique<MenuState>(m_stateManager));
            return; // 切换状态后，当前状态的事件处理应结束
        }
        if (event.key.code == sf::Keyboard::F1)
        {
            m_sunManager.addSun(100);
            std::cout << "调试:阳光增加到 " << m_sunManager.getCurrentSun() << std::endl;
        }
    }

    m_hud.handleEvent(event, currentEventMousePosView); // HUD 事件处理

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            // 注意：clickPosView 应该使用事件发生时的精确坐标
            sf::Vector2f clickPosView = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), window.getView());

            bool sunCollectedThisClick = false;
            for (auto it = m_activeSuns.rbegin(); it != m_activeSuns.rend(); ++it)
            {
                if (!(*it)->isCollected() && (*it)->handleClick(clickPosView))
                {
                    (*it)->collect();
                    sunCollectedThisClick = true;
                    break;
                }
            }
            if (sunCollectedThisClick)
                return; // 如果收集了阳光，则不进行后续的种植尝试

            // 尝试种植植物
            bool clickedOnGridArea = clickPosView.y > (SEED_PACKET_UI_START_Y + SEED_PACKET_HEIGHT + SEED_PACKET_SPACING); // 简单判断
            if (clickedOnGridArea)
            {
                sf::Vector2i gridCoords = m_grid.getGridPosition(clickPosView);
                bool isValidPlantSelection = false;
                PlantType selectedPlant = m_hud.getSelectedPlantTypeFromSeedManager(isValidPlantSelection);

                if (m_grid.isValidGridPosition(gridCoords) && isValidPlantSelection)
                {
                    // 检查格子是否被占用 (依赖 PlantManager 正确更新 Grid 状态，或 Grid 自身维护)
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
                        std::cout << "GamePlayState: Clicked on invalid grid position." << std::endl;
                    if (!isValidPlantSelection)
                        std::cout << "GamePlayState: No valid plant selected from HUD." << std::endl;
                }
            }
        }
    }
}

void GamePlayState::update(float deltaTime)
{
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
        spawnSunFromSky(); // spawnSunFromSky 内部会重置计时器和间隔
    }

    m_plantManager.update(deltaTime);
    m_projectileManager.update(deltaTime, m_stateManager->getGame()->getWindow());

    std::vector<Plant *> activePlantsForZombies = m_plantManager.getAllActivePlants();
    std::vector<Zombie *> activeZombiesToUpdate = m_zombieManager.getActiveZombies();
    for (Zombie *zombie : activeZombiesToUpdate)
    {
        if (zombie && zombie->isAlive())
        { // 再次确认存活
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
    m_zombieManager.update(deltaTime, m_stateManager->getGame()->getWindow());

    m_collisionSystem.update(m_projectileManager, m_zombieManager, m_plantManager);

    m_waveManager.update(deltaTime); // <--- 更新 WaveManager

    m_hud.update(deltaTime);

    std::stringstream ss;
    ss.precision(1);
    ss << std::fixed;
    ss << "Time: " << m_gameTime << "s | FPS: " << static_cast<int>(1.f / deltaTime)
       << " | Mouse: (" << m_mousePixelPos.x << "," << m_mousePixelPos.y << ")"
       << " | Suns: " << m_sunManager.getCurrentSun()
       << " | Entities: S:" << m_activeSuns.size()
       << " P:" << m_projectileManager.getAllProjectiles().size() // 假设 ProjectileManager 有此方法
       << " Z:" << m_zombieManager.getActiveZombies().size()
       << " | Plants: " << m_plantManager.getAllActivePlants().size() // 假设 PlantManager 有此方法
       << " | " << m_waveManager.getCurrentWaveStatusText();          // <--- 添加波数状态
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
    float groundMinY = GRID_START_Y + (GRID_ROWS / 2.0f) * GRID_CELL_HEIGHT;                  // Example logic
    float groundMaxY = GRID_START_Y + GRID_ROWS * GRID_CELL_HEIGHT - GRID_CELL_HEIGHT * 0.5f; // Example logic
    groundMaxY = std::min(groundMaxY, static_cast<float>(window.getSize().y - 50.f));
    if (groundMinY >= groundMaxY)
        groundMinY = groundMaxY - 50.f; // Basic safety
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
    // Make sure plant->getGlobalBounds() is valid if sprite is not yet drawn or texture not fully loaded
    float heightOffset = plant->getGlobalBounds().height * 0.3f;
    if (heightOffset <= 0)
        heightOffset = 20.f; // Fallback if bounds are weird

    sf::Vector2f sunSpawnPos = sf::Vector2f(plantPos.x, plantPos.y - heightOffset);
    m_activeSuns.emplace_back(std::make_unique<Sun>(
        m_stateManager->getGame()->getResourceManager(), m_sunManager,
        sunSpawnPos, SunSpawnType::FROM_PLANT));
}
