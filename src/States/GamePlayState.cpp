#include "States/GamePlayState.h"
#include "States/MenuState.h"
#include "Core/StateManager.h"
#include "Core/Game.h"
#include "Core/ResourceManager.h"
#include "../Utils/Constants.h"
#include "../Entities/Plant.h"
#include "../Systems/ProjectileManager.h"
#include "../Systems/ZombieManager.h"
#include "../Systems/CollisionSystem.h"
#include "../Entities/Zombie.h"
#include "../Entities/Projectile.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

GamePlayState::GamePlayState(StateManager *stateManager)
    : GameState(stateManager),          // 初始化基类
      m_fontsLoaded(false),             // 初始时字体未加载
      m_grid(),                         // Grid 使用默认构造函数
      m_sunManager(INITIAL_SUN_AMOUNT), // 初始化阳光管理器
      m_projectileManager(stateManager->getGame()->getResourceManager()),
      m_zombieManager(stateManager->getGame()->getResourceManager(), m_grid),
      // 初始化 PlantManager (它需要 GamePlayState& 和 ProjectileManager&)
      m_plantManager(stateManager->getGame()->getResourceManager(), m_grid, *this, m_projectileManager, m_zombieManager),
      // 初始化 HUD (它需要 ResourceManager, SunManager, 和字体)
      m_hud(stateManager->getGame()->getResourceManager(), m_sunManager, m_primaryGameFont, m_secondaryGameFont),
      m_gameTime(0.0f), // 初始化游戏时间
      m_skySunSpawnIntervalMin(5.0f),
      m_skySunSpawnIntervalMax(12.0f),
      m_currentSkySunSpawnInterval(0.0f),
      m_collisionSystem(),
      m_zombieSpawnTestInterval(8.0f)
{

    std::cout << "GamePlayState 正在构造..." << std::endl;
    loadAssets(); // 调用辅助函数加载此状态所需的资源

    // 初始化天空阳光生成计时器和第一次的随机间隔
    // std::srand 已在 Game::Game() 中全局调用过一次，此处无需重复
    m_currentSkySunSpawnInterval = m_skySunSpawnIntervalMin +
                                   static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (m_skySunSpawnIntervalMax - m_skySunSpawnIntervalMin)));
    m_skySunSpawnTimer.restart(); // 启动计时器

    m_zombieSpawnTestTimer.restart(); // 初始化测试僵尸生成计时器

    std::cout << "GamePlayState 构造完毕。" << std::endl;
}

// 加载此状态所需的资源
void GamePlayState::loadAssets()
{
    std::cout << "GamePlayState:正在加载资源..." << std::endl;
    ResourceManager &resMan = m_stateManager->getGame()->getResourceManager();

    // --- 字体加载 ---
    if (resMan.hasFont(FONT_ID_PRIMARY))
    {
        m_primaryGameFont = resMan.getFont(FONT_ID_PRIMARY);
        m_fontsLoaded = true;
    }
    else
    {
        std::cerr << "GamePlayState:主要字体 (ID: " << FONT_ID_PRIMARY << ") 未从 ResourceManager 加载!" << std::endl;
        if (m_primaryGameFont.loadFromFile(FONT_PATH_ARIAL))
            m_fontsLoaded = true; // 尝试备用
        else
            std::cerr << "GamePlayState:备用主要字体也加载失败!" << std::endl;
    }
    if (resMan.hasFont(FONT_ID_SECONDARY))
    {
        m_secondaryGameFont = resMan.getFont(FONT_ID_SECONDARY);
    }
    else
    {
        std::cerr << "GamePlayState:次要字体 (ID: " << FONT_ID_SECONDARY << ") 未从 ResourceManager 加载，将使用主要字体。" << std::endl;
        m_secondaryGameFont = m_primaryGameFont;
    }
    if (!m_fontsLoaded && m_primaryGameFont.getInfo().family.empty())
    {
        std::cerr << "GamePlayState:警告 - 没有有效的字体被加载!UI文本可能无法显示。" << std::endl;
    }

    // --- 僵尸主要纹理加载 ---
    if (!resMan.hasTexture(BASIC_ZOMBIE_TEXTURE_KEY))
    {
        if (!resMan.loadTexture(BASIC_ZOMBIE_TEXTURE_KEY, "assets/images/zombies/basic_zombie.png"))
        { // 确保路径正确
            std::cerr << "GamePlayState: 普通僵尸纹理 (" << BASIC_ZOMBIE_TEXTURE_KEY << ") 加载失败！" << std::endl;
        }
    }

    // --- 植物主要纹理加载 ---
    if (!resMan.hasTexture(SUNFLOWER_TEXTURE_KEY))
    {
        resMan.loadTexture(SUNFLOWER_TEXTURE_KEY, "assets/images/plants/sunflower.png");
    }
    if (!resMan.hasTexture(PEASHOOTER_TEXTURE_KEY))
    {
        resMan.loadTexture(PEASHOOTER_TEXTURE_KEY, "assets/images/plants/peashooter.png");
    }

    // --- 植物图标纹理加载 (供 SeedPacket 使用) ---
    if (!resMan.hasTexture(SUNFLOWER_ICON_TEXTURE_KEY))
    {
        resMan.loadTexture(SUNFLOWER_ICON_TEXTURE_KEY, "assets/images/plants/sunflower_icon.png");
    }
    if (!resMan.hasTexture(PEASHOOTER_ICON_TEXTURE_KEY))
    {
        resMan.loadTexture(PEASHOOTER_ICON_TEXTURE_KEY, "assets/images/plants/peashooter_icon.png");
    }

    // --- 阳光实体纹理加载 ---
    if (!resMan.hasTexture(SUN_TEXTURE_KEY))
    {
        resMan.loadTexture(SUN_TEXTURE_KEY, "assets/images/ui/sun.png");
    }

    // --- 子弹纹理加载 --- <--- 新增/确认
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

    m_backgroundShape.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    m_backgroundShape.setFillColor(sf::Color(34, 139, 34));

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
    m_debugInfoText.setPosition(10, WINDOW_HEIGHT - 30);

    m_grid.initialize();
    m_sunManager.reset();
    m_plantManager.clear();
    m_projectileManager.clear();
    m_zombieManager.clear();
    m_activeSuns.clear();

    m_skySunSpawnTimer.restart();
    m_currentSkySunSpawnInterval = m_skySunSpawnIntervalMin +
                                   static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (m_skySunSpawnIntervalMax - m_skySunSpawnIntervalMin)));
    // 重置测试僵尸生成计时器
    m_zombieSpawnTestTimer.restart();
    spawnInitialZombiesForTesting();
    m_gameTime = 0.f;
    std::cout << "GamePlayState enter。" << std::endl;
}
// 测试用，生成初始僵尸
void GamePlayState::spawnInitialZombiesForTesting()
{
    m_zombieManager.spawnZombie(rand() % GRID_ROWS); // 在随机行生成一个普通僵尸
}

// 退出状态时的清理
void GamePlayState::exit()
{
    std::cout << "GamePlayState exit。" << std::endl;
    m_plantManager.clear();
    m_projectileManager.clear();
    m_zombieManager.clear();
    m_activeSuns.clear();
}

// 处理事件
void GamePlayState::handleEvent(const sf::Event &event)
{
    sf::RenderWindow &window = m_stateManager->getGame()->getWindow();
    sf::Vector2f currentEventMousePosView; // 在事件循环中声明，避免每次都重新计算

    // 更新鼠标的原始像素位置 (在事件循环开始时做一次，或按需)
    if (event.type == sf::Event::MouseMoved)
    {
        m_mousePixelPos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
    }
    // 确保 currentEventMousePosView 使用最新的鼠标位置
    if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::MouseMoved)
    {
        currentEventMousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
    }
    else
    { // 对于其他事件类型，如果需要鼠标位置，则使用上一次记录的 m_mousePixelPos
        currentEventMousePosView = window.mapPixelToCoords(m_mousePixelPos, window.getView());
    }

    // 优先处理高优先级事件，如退出
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            m_stateManager->changeState(std::make_unique<MenuState>(m_stateManager));
            return;
        }
        if (event.key.code == sf::Keyboard::F1)
        {
            m_sunManager.addSun(100);
            std::cout << "调试:阳光增加到 " << m_sunManager.getCurrentSun() << std::endl;
        }
    }

    // 将事件传递给 HUD 处理
    m_hud.handleEvent(event, currentEventMousePosView);

    // 处理鼠标左键点击事件 (用于收集阳光和种植植物)
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            // clickPosView 应该使用事件发生时的精确位置
            sf::Vector2f clickPosView = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), window.getView());

            // 1. 尝试收集阳光
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
                return; // 如果收集了阳光，则不进行种植

            // 2. 尝试种植植物
            bool clickedOnGridArea = clickPosView.y > (SEED_PACKET_UI_START_Y + SEED_PACKET_HEIGHT + SEED_PACKET_SPACING);
            if (clickedOnGridArea)
            {
                sf::Vector2i gridCoords = m_grid.getGridPosition(clickPosView);
                bool isValidPlantSelection = false;
                PlantType selectedPlant = m_hud.getSelectedPlantTypeFromSeedManager(isValidPlantSelection);

                if (m_grid.isValidGridPosition(gridCoords) && isValidPlantSelection)
                {
                    int cost = m_hud.getSelectedPlantCostFromSeedManager();
                    if (m_sunManager.getCurrentSun() >= cost)
                    {
                        if (m_plantManager.tryAddPlant(selectedPlant, gridCoords))
                        {
                            m_sunManager.trySpendSun(cost);
                            m_hud.notifyPlantPlacedToSeedManager(selectedPlant);
                            // (日志输出)
                        }
                        else
                        { /* 种植失败日志 */
                            std::cout << "Plant faild" << std::endl;
                        }
                    }
                    else
                    { /* 阳光不足日志 */
                        std::cout << "Sun is not enough" << std::endl;
                    }
                }
                else
                { /* 无效选择或位置日志 */
                    std::cout << "Illegal position" << std::endl;
                }
            }
        }
    }
}

// 更新游戏逻辑
void GamePlayState::update(float deltaTime)
{
    m_gameTime += deltaTime;

    // 1. 更新阳光实体
    for (auto &sun : m_activeSuns)
    {
        sun->update(deltaTime);
    }
    m_activeSuns.erase(
        std::remove_if(m_activeSuns.begin(), m_activeSuns.end(),
                       [](const std::unique_ptr<Sun> &s)
                       { return s->isExpired(); }),
        m_activeSuns.end());

    // 2. 天空阳光生成
    if (m_skySunSpawnTimer.getElapsedTime().asSeconds() >= m_currentSkySunSpawnInterval)
    {
        spawnSunFromSky();
    }

    // 3. 更新植物
    m_plantManager.update(deltaTime);

    // 4. 更新子弹 <--- 确保这行存在且被调用

    m_projectileManager.update(deltaTime, m_stateManager->getGame()->getWindow());

    // 5.更新僵尸 (僵尸的索敌和攻击植物)
    std::vector<Plant *> activePlantsForZombies = m_plantManager.getAllActivePlants(); // 获取所有存活植物
    std::vector<Zombie *> activeZombiesToUpdate = m_zombieManager.getActiveZombies();  // 获取所有存活僵尸

    for (Zombie *zombie : activeZombiesToUpdate)
    {
        if (zombie && zombie->isAlive())
        {
            int currentZombieLane = zombie->getLane();
            std::vector<Plant *> plantsInThisSpecificLane;
            if (currentZombieLane != -1)
            { // 确保僵尸在有效行内
                for (Plant *p : activePlantsForZombies)
                {
                    if (p && p->isAlive() && p->getRow() == currentZombieLane)
                    {
                        plantsInThisSpecificLane.push_back(p);
                    }
                }
            }
            else
            {
                std::cout << "[GamePlayState] DEBUG: Zombie Addr: " << zombie << " is not in a valid lane, passing empty plant list." << std::endl;
            }
            zombie->update(deltaTime, plantsInThisSpecificLane);
        }
    }
    // ZombieManager 自身的更新 (例如，移除已死亡僵尸，检查是否到达房子)
    m_zombieManager.update(deltaTime, m_stateManager->getGame()->getWindow());

    // 执行碰撞检测
    m_collisionSystem.update(m_projectileManager, m_zombieManager, m_plantManager);
    // 6. 测试：按时间间隔生成僵尸 (简单版波次)
    if (m_zombieSpawnTestTimer.getElapsedTime().asSeconds() >= m_zombieSpawnTestInterval)
    {
        int randomRow = rand() % GRID_ROWS;     // 在随机一行生成
        m_zombieManager.spawnZombie(randomRow); // 调用 ZombieManager 生成僵尸
        std::cout << "测试：在第 " << randomRow << " 行生成了一个僵尸。" << std::endl;
        m_zombieSpawnTestTimer.restart(); // 重置计时器，为下一次生成做准备
        // (可选) 更新下一次生成的间隔时间，使其随机化
        // m_zombieSpawnTestInterval = MIN_ZOMBIE_SPAWN_INTERVAL +
        //                           static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (MAX_ZOMBIE_SPAWN_INTERVAL - MIN_ZOMBIE_SPAWN_INTERVAL)));
    }

    // 7. 更新 HUD
    m_hud.update(deltaTime);

    // 更新调试信息文本
    std::stringstream ss;
    ss << "Time: " << static_cast<int>(m_gameTime) << "s | FPS: " << static_cast<int>(1.f / deltaTime)
       << " | Mouse: (" << m_mousePixelPos.x << "," << m_mousePixelPos.y << ")"
       << " | Suns: " << m_sunManager.getCurrentSun()
       << " | Existing Suns: " << m_activeSuns.size()
       << " | Projectiles: " << m_projectileManager.getAllProjectiles().size()
       << " | Zombies: " << m_zombieManager.getActiveZombies().size();
    m_debugInfoText.setString(ss.str());
}

// 渲染游戏画面
void GamePlayState::render(sf::RenderWindow &window)
{
    window.draw(m_backgroundShape);
    m_grid.render(window);

    // 绘制植物
    m_plantManager.draw(window);

    // 绘制阳光实体
    for (const auto &sun : m_activeSuns)
    {
        sun->draw(window);
    }

    // 绘制子弹
    m_projectileManager.draw(window);

    // 绘制僵尸
    m_zombieManager.draw(window);

    m_hud.draw(window);
    window.draw(m_debugInfoText);
}

void GamePlayState::spawnSunFromSky()
{
    sf::RenderWindow &window = m_stateManager->getGame()->getWindow();
    float spawnX = static_cast<float>(rand() % (static_cast<int>(window.getSize().x * 0.9f))) + window.getSize().x * 0.05f;
    float spawnY = -30.f;
    float groundMinY = GRID_START_Y + (GRID_ROWS / 2.f) * GRID_CELL_HEIGHT;
    float groundMaxY = GRID_START_Y + GRID_ROWS * GRID_CELL_HEIGHT + GRID_CELL_HEIGHT * 0.5f;
    groundMaxY = std::min(groundMaxY, static_cast<float>(window.getSize().y - 50.f));
    if (groundMinY >= groundMaxY)
        groundMinY = groundMaxY - 50.f;
    float targetY = groundMinY + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (groundMaxY - groundMinY)));

    m_activeSuns.emplace_back(std::make_unique<Sun>(
        m_stateManager->getGame()->getResourceManager(), m_sunManager,
        sf::Vector2f(spawnX, spawnY), SunSpawnType::FROM_SKY, targetY));
    m_skySunSpawnTimer.restart();
    m_currentSkySunSpawnInterval = m_skySunSpawnIntervalMin +
                                   static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (m_skySunSpawnIntervalMax - m_skySunSpawnIntervalMin)));
}

void GamePlayState::spawnSunFromPlant(Plant *plant)
{
    if (!plant)
        return;
    sf::Vector2f plantPos = plant->getPosition();
    sf::Vector2f sunSpawnPos = sf::Vector2f(plantPos.x, plantPos.y - plant->getGlobalBounds().height * 0.3f);
    m_activeSuns.emplace_back(std::make_unique<Sun>(
        m_stateManager->getGame()->getResourceManager(), m_sunManager,
        sunSpawnPos, SunSpawnType::FROM_PLANT));
}