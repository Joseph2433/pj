#include "States/GamePlayState.h"
#include "States/MenuState.h"     // 为了实现按 ESC 返回主菜单
#include "Core/StateManager.h"    // 状态管理器
#include "Core/Game.h"            // Game 类，用于获取 ResourceManager 和 Window
#include "Core/ResourceManager.h" // 资源管理器
#include "../Utils/Constants.h"   // 游戏常量
#include "../Entities/Plant.h"    // Plant 类 (spawnSunFromPlant 参数需要)
#include <iostream>               // 用于调试输出
#include <sstream>                // 用于构建调试信息字符串
#include <cstdlib>                // 为了 rand(), srand()
#include <ctime>                  // 为了 std::time() (用于 srand 初始化)
#include <algorithm>              // 为了 std::remove_if

// GamePlayState 构造函数
GamePlayState::GamePlayState(StateManager *stateManager)
    : GameState(stateManager), // 初始化基类
      m_fontsLoaded(false),    // 初始时字体未加载
      m_grid(),                // Grid 使用默认构造函数 (它会从 Constants.h 读取参数)
      // 初始化 PlantManager 时传递 *this (当前 GamePlayState 实例的引用)
      m_plantManager(stateManager->getGame()->getResourceManager(), m_grid, *this),
      m_sunManager(INITIAL_SUN_AMOUNT), // 初始化阳光管理器，使用常量中的初始阳光值
      // HUD 的构造需要 ResourceManager, SunManager, 以及字体。
      // 字体在 loadAssets() 中加载，因此这里传递的是将要被填充的字体对象的引用。
      m_hud(stateManager->getGame()->getResourceManager(), m_sunManager, m_primaryGameFont, m_secondaryGameFont),
      m_gameTime(0.0f), // 初始化游戏时间
      // 初始化天空阳光生成的参数 (可以考虑也从 Constants.h 读取这些间隔值)
      m_skySunSpawnIntervalMin(5.0f),
      m_skySunSpawnIntervalMax(12.0f),
      m_currentSkySunSpawnInterval(0.0f)
{ // 初始间隔将在 enter() 或构造时首次设定

    std::cout << "GamePlayState 正在构造..." << std::endl;
    loadAssets(); // 调用辅助函数加载此状态所需的资源

    // 初始化天空阳光生成计时器和第一次的随机间隔
    // std::srand 已在 Game::Game() 中全局调用过一次
    m_currentSkySunSpawnInterval = m_skySunSpawnIntervalMin +
                                   static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (m_skySunSpawnIntervalMax - m_skySunSpawnIntervalMin)));
    m_skySunSpawnTimer.restart(); // 启动计时器

    std::cout << "GamePlayState 构造完毕。" << std::endl;
}

// 加载此状态所需的资源
void GamePlayState::loadAssets()
{
    std::cout << "GamePlayState：正在加载资源..." << std::endl;
    ResourceManager &resMan = m_stateManager->getGame()->getResourceManager();

    // 加载字体 (Game::loadGlobalResources 应该已经加载了通过ID引用的字体)
    // 如果 HUD 和 SeedManager/SeedPacket 直接使用这些 sf::Font 对象，
    // 那么 GamePlayState 必须确保它们被有效加载。
    // 如果它们通过 ResourceManager 按ID获取字体，则这里不需要重复加载。
    // 为确保独立性，我们假设 GamePlayState 为其自身和直接传递的UI组件加载/获取字体。
    // (假设 FONT_ID_PRIMARY 和 FONT_ID_SECONDARY 已由 Game::loadGlobalResources 加载到 resMan)
    if (resMan.hasFont(FONT_ID_PRIMARY))
    {
        m_primaryGameFont = resMan.getFont(FONT_ID_PRIMARY);
        m_fontsLoaded = true;
    }
    else
    {
        std::cerr << "GamePlayState：主要字体 (ID: " << FONT_ID_PRIMARY << ") 未从 ResourceManager 加载！" << std::endl;
        // 可以尝试直接加载一个备用字体
        if (m_primaryGameFont.loadFromFile(FONT_PATH_ARIAL))
            m_fontsLoaded = true;
        else
            std::cerr << "GamePlayState：备用主要字体也加载失败！" << std::endl;
    }

    if (resMan.hasFont(FONT_ID_SECONDARY))
    {
        m_secondaryGameFont = resMan.getFont(FONT_ID_SECONDARY);
    }
    else
    {
        std::cerr << "GamePlayState：次要字体 (ID: " << FONT_ID_SECONDARY << ") 未从 ResourceManager 加载，将使用主要字体。" << std::endl;
        m_secondaryGameFont = m_primaryGameFont; // 使用主要字体作为备用
    }
    if (!m_fontsLoaded && m_primaryGameFont.getInfo().family.empty())
    { // 再次检查
        std::cerr << "GamePlayState：警告 - 没有有效的字体被加载！UI文本可能无法显示。" << std::endl;
    }

    // 加载植物主要纹理 (供 PlantManager 中的植物实体使用)
    if (!resMan.hasTexture(SUNFLOWER_TEXTURE_KEY))
    {
        resMan.loadTexture(SUNFLOWER_TEXTURE_KEY, "assets/images/plants/sunflower.png");
    }
    if (!resMan.hasTexture(PEASHOOTER_TEXTURE_KEY))
    {
        resMan.loadTexture(PEASHOOTER_TEXTURE_KEY, "assets/images/plants/peashooter.png");
    }
    // ... 加载其他植物的主要纹理 ...

    // 加载植物图标纹理 (供 SeedPacket 使用)
    // 这些应该已在 Constants.h 中定义键名，Game::loadGlobalResources 或这里加载
    if (!resMan.hasTexture(SUNFLOWER_ICON_TEXTURE_KEY))
    {
        resMan.loadTexture(SUNFLOWER_ICON_TEXTURE_KEY, "assets/images/plants/sunflower_icon.png");
    }
    if (!resMan.hasTexture(PEASHOOTER_ICON_TEXTURE_KEY))
    {
        resMan.loadTexture(PEASHOOTER_ICON_TEXTURE_KEY, "assets/images/plants/peashooter_icon.png");
    }
    // ... 加载其他植物的图标纹理 ...

    // 加载阳光实体纹理 (如果 Game::loadGlobalResources 没加载)
    if (!resMan.hasTexture(SUN_TEXTURE_KEY))
    {
        resMan.loadTexture(SUN_TEXTURE_KEY, "assets/images/ui/sun.png");
    }
    std::cout << "GamePlayState：资源加载尝试完毕。" << std::endl;
}

// 进入状态时的初始化
void GamePlayState::enter()
{
    std::cout << "GamePlayState 进入。" << std::endl;

    // 设置游戏背景
    m_backgroundShape.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    m_backgroundShape.setFillColor(sf::Color(34, 139, 34)); // 森林绿

    // 设置调试信息文本 (如果字体已加载)
    if (m_fontsLoaded)
    {
        m_debugInfoText.setFont(m_primaryGameFont);
    }
    else
    { // 如果字体加载失败，尝试使用SFML默认字体（通常不可见或很小）
        std::cerr << "GamePlayState::enter - 调试文本无法设置字体，因字体未加载。" << std::endl;
    }
    m_debugInfoText.setCharacterSize(14);
    m_debugInfoText.setFillColor(sf::Color::White);
    m_debugInfoText.setPosition(10, WINDOW_HEIGHT - 30); // 调整到底部，避免与HUD重叠太多

    m_grid.initialize();    // 初始化网格线
    m_sunManager.reset();   // 重置阳光数量
    m_plantManager.clear(); // 清空上一局的植物
    m_activeSuns.clear();   // 清空上一局的阳光实体

    // 重置天空阳光生成计时器和间隔
    m_skySunSpawnTimer.restart();
    m_currentSkySunSpawnInterval = m_skySunSpawnIntervalMin +
                                   static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (m_skySunSpawnIntervalMax - m_skySunSpawnIntervalMin)));

    m_gameTime = 0.f; // 重置游戏时间
}

// 退出状态时的清理
void GamePlayState::exit()
{
    std::cout << "GamePlayState 退出。" << std::endl;
    m_plantManager.clear(); // 清理所有植物
    m_activeSuns.clear();   // 清理所有阳光实体
}

// 处理事件
void GamePlayState::handleEvent(const sf::Event &event)
{
    sf::RenderWindow &window = m_stateManager->getGame()->getWindow();
    // 获取当前鼠标在视图中的坐标 (注意：m_mousePixelPos 可能不是最新的，最好在事件发生时获取)
    // sf::Vector2f mousePosView = window.mapPixelToCoords(m_mousePixelPos, window.getView());

    // 优先处理高优先级事件，如退出
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            m_stateManager->changeState(std::make_unique<MenuState>(m_stateManager));
            return; // 切换状态后，不应再处理此帧的其他事件
        }
        if (event.key.code == sf::Keyboard::F1)
        { // 调试：按F1增加阳光
            m_sunManager.addSun(100);
            std::cout << "调试：阳光增加到 " << m_sunManager.getCurrentSun() << std::endl;
        }
    }

    // 更新鼠标的原始像素位置
    if (event.type == sf::Event::MouseMoved)
    {
        m_mousePixelPos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
    }

    // 将事件传递给 HUD 处理 (HUD 内部会传递给 SeedManager)
    // 需要事件发生时的鼠标位置
    sf::Vector2f currentEventMousePosView;
    if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::MouseMoved)
    {
        currentEventMousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
    }
    else
    {
        currentEventMousePosView = window.mapPixelToCoords(m_mousePixelPos, window.getView()); // 使用最近的已知位置
    }
    m_hud.handleEvent(event, currentEventMousePosView);

    // 处理鼠标左键点击事件 (用于收集阳光和种植植物)
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f clickPosView = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), window.getView());

            // 1. 尝试收集阳光 (优先级高于种植)
            bool sunCollectedThisClick = false;
            for (auto it = m_activeSuns.rbegin(); it != m_activeSuns.rend(); ++it)
            { // 从后往前遍历，方便移除
                if (!(*it)->isCollected() && (*it)->handleClick(clickPosView))
                {
                    (*it)->collect(); // 收集阳光，内部会调用 sunManager.addSun()
                    sunCollectedThisClick = true;
                    break; // 一次点击只收集一个阳光
                }
            }

            if (sunCollectedThisClick)
            {
                return; // 如果点击收集了阳光，则不进行后续的种植尝试
            }

            // 2. 尝试种植植物 (如果没点击到阳光，并且点击在网格区域)
            // 通过检查Y坐标判断是否在可种植区域 (简单判断，避免在UI栏上种植)
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
                            std::cout << "种植植物 类型 " << static_cast<int>(selectedPlant)
                                      << " 于 (" << gridCoords.x << "行, " << gridCoords.y << "列). "
                                      << "剩余阳光: " << m_sunManager.getCurrentSun() << std::endl;
                        }
                        else
                        {
                            std::cout << "种植失败 (单元格已被占用或其他 PlantManager 问题)。" << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "阳光不足以种植 (需要: " << cost << ", 当前: " << m_sunManager.getCurrentSun() << ")。" << std::endl;
                    }
                }
                else
                {
                    if (!isValidPlantSelection)
                        std::cout << "没有有效选中的植物，或选中的植物正在冷却/阳光不足。" << std::endl;
                    // if (!m_grid.isValidGridPosition(gridCoords)) std::cout << "点击位置不在有效网格内。" << std::endl; // 这个判断可以省略，因为上面有 clickedOnGridArea
                }
            }
        }
    }
}

// 更新游戏逻辑
void GamePlayState::update(float deltaTime)
{
    m_gameTime += deltaTime;

    // 1. 更新所有激活的阳光实体
    for (auto &sun : m_activeSuns)
    {
        sun->update(deltaTime);
    }

    // 2. 移除已过期或已收集的阳光
    m_activeSuns.erase(
        std::remove_if(m_activeSuns.begin(), m_activeSuns.end(),
                       [](const std::unique_ptr<Sun> &s)
                       { return s->isExpired(); }),
        m_activeSuns.end());

    // 3. 定时从天上掉落阳光
    if (m_skySunSpawnTimer.getElapsedTime().asSeconds() >= m_currentSkySunSpawnInterval)
    {
        spawnSunFromSky(); // 调用产生阳光的方法，内部会重置计时器和下次间隔
    }

    // 4. 更新所有场上的植物
    m_plantManager.update(deltaTime); // 植物的 update 内部可能会请求产生阳光

    // 5. 更新 HUD (HUD 会更新 SeedManager，SeedManager 会更新所有 SeedPacket 的冷却等)
    m_hud.update(deltaTime);

    // 更新调试信息文本
    std::stringstream ss;
    ss << "时间: " << static_cast<int>(m_gameTime) << "s | FPS: " << static_cast<int>(1.f / deltaTime)
       << " | 鼠标: (" << m_mousePixelPos.x << "," << m_mousePixelPos.y << ")"
       << " | 阳光: " << m_sunManager.getCurrentSun()
       << " | 存活阳光实体: " << m_activeSuns.size();
    m_debugInfoText.setString(ss.str());
}

// 渲染游戏画面
void GamePlayState::render(sf::RenderWindow &window)
{
    window.draw(m_backgroundShape); // 绘制背景
    m_grid.render(window);          // 绘制网格线
    m_plantManager.draw(window);    // 绘制所有场上的植物

    // 绘制所有激活的阳光实体
    for (const auto &sun : m_activeSuns)
    {
        sun->draw(window);
    }

    m_hud.draw(window);           // 绘制 HUD (包括种子包栏和阳光显示)
    window.draw(m_debugInfoText); // 绘制调试信息
}

// --- 私有辅助方法实现 ---

// 从天空产生阳光
void GamePlayState::spawnSunFromSky()
{
    sf::RenderWindow &window = m_stateManager->getGame()->getWindow();
    // 随机X坐标，确保在屏幕可见范围内，并留有边距
    float spawnX = static_cast<float>(rand() % (static_cast<int>(window.getSize().x * 0.9f))) + window.getSize().x * 0.05f;
    float spawnY = -30.f; // 从屏幕顶部稍上方开始

    // 随机一个地面目标Y坐标 (示例：在网格区域的下半部分或稍下方)
    float groundMinY = GRID_START_Y + (GRID_ROWS / 2.f) * GRID_CELL_HEIGHT;
    float groundMaxY = GRID_START_Y + GRID_ROWS * GRID_CELL_HEIGHT + GRID_CELL_HEIGHT * 0.5f;
    groundMaxY = std::min(groundMaxY, static_cast<float>(window.getSize().y - 50.f)); // 确保不超出屏幕底部太多
    if (groundMinY >= groundMaxY)
        groundMinY = groundMaxY - 50.f; // 保证最小范围
    float targetY = groundMinY + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (groundMaxY - groundMinY)));

    m_activeSuns.emplace_back(std::make_unique<Sun>(
        m_stateManager->getGame()->getResourceManager(),
        m_sunManager,
        sf::Vector2f(spawnX, spawnY),
        SunSpawnType::FROM_SKY,
        targetY));
    // std::cout << "从天空产生了一个阳光。" << std::endl;

    // 重置下一次掉落的计时器和随机间隔
    m_skySunSpawnTimer.restart();
    m_currentSkySunSpawnInterval = m_skySunSpawnIntervalMin +
                                   static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (m_skySunSpawnIntervalMax - m_skySunSpawnIntervalMin)));
}

// 当植物（如向日葵）请求时，在其位置产生阳光
void GamePlayState::spawnSunFromPlant(Plant *plant)
{
    if (!plant)
        return; // 安全检查

    sf::Vector2f plantPos = plant->getPosition(); // 获取植物的中心位置
    // 阳光可以出现在植物的中心，或者稍微偏移一点以模拟从植物"弹出"
    // 这里的 sunSpawnPos 应该是 Sun 对象开始其内部动画（如抛物线）的起始点
    sf::Vector2f sunSpawnPos = sf::Vector2f(plantPos.x, plantPos.y - plant->getGlobalBounds().height * 0.3f); // 示例：在植物上方一点

    m_activeSuns.emplace_back(std::make_unique<Sun>(
        m_stateManager->getGame()->getResourceManager(),
        m_sunManager,
        sunSpawnPos,
        SunSpawnType::FROM_PLANT
        // 对于 FROM_PLANT 类型的阳光，Sun 类的构造函数和 updatePlantSun 会处理其特定运动
        ));
    // std::cout << "植物在 (" << plantPos.x << "," << plantPos.y << ") 处生成了阳光。" << std::endl;
}
