#include "States/GamePlayState.h"
#include "States/MenuState.h"
#include "Core/StateManager.h"
#include "Core/Game.h" // 假设 Game 类提供 getResourceManager() 和 getWindow()
#include "../Utils/Constants.h"
#include <iostream>
#include <sstream>

// 构造函数
GamePlayState::GamePlayState(StateManager *stateManager)
    : GameState(stateManager),
      m_useCustomFont(false),
      m_gameTime(0.0f),
      m_mousePos(0, 0),
      // --- 初始化新增成员 ---
      // Grid 的参数需要根据你的 Constants.h 和 Grid 构造函数来定
      // 你的 Grid 构造函数是 Grid()，它内部使用了 Constants.h 的值
      m_grid(), // 如果 Grid 构造函数是 Grid()，并从 Constants.h 读取参数
      // 如果 Grid 构造函数需要参数:
      // m_grid(Constants::GRID_ROWS, Constants::GRID_COLS,
      //        Constants::GRID_CELL_WIDTH, Constants::GRID_CELL_HEIGHT,
      //        sf::Vector2f(Constants::GRID_START_X, Constants::GRID_START_Y)),
      m_plantManager(stateManager->getGame()->getResourceManager(), m_grid), // 传递 ResourceManager 和 Grid
      m_currentPlantToPlace(PlantType::SUNFLOWER)                            // 默认种植向日葵
{
    std::cout << "GamePlayState constructing..." << std::endl;
    // 加载植物纹理 (确保 ResourceManager 已经准备好)
    // 这些键名需要与你在 Sunflower.cpp 和 Peashooter.cpp 中使用的键名一致
    ResourceManager &resMan = stateManager->getGame()->getResourceManager();
    if (!resMan.hasTexture("sunflower"))
    { // 避免重复加载
        if (!resMan.loadTexture("sunflower", "assets/images/plants/sunflower.png"))
        {
            std::cerr << "Failed to load sunflower texture!" << std::endl;
        }
    }
    if (!resMan.hasTexture("peashooter"))
    { // 避免重复加载
        if (!resMan.loadTexture("peashooter", "assets/images/plants/peashooter.png"))
        {
            std::cerr << "Failed to load peashooter texture!" << std::endl;
        }
    }
    // 加载其他植物纹理...

    std::cout << "Plant textures loading attempted." << std::endl;
}

void GamePlayState::enter()
{
    std::cout << "GamePlayState entering..." << std::endl;
    // 尝试加载字体
    bool fontLoaded = false;
    if (m_font.loadFromFile("C:/Windows/Fonts/arial.ttf") ||
        m_font.loadFromFile("C:/Windows/Fonts/msyh.ttc") ||                     // 尝试微软雅黑
        m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) // Linux 常见字体
    {
        fontLoaded = true;
    }
    else
    {
        std::cerr << "Failed to load any system font. Text might not display correctly." << std::endl;
    }
    m_useCustomFont = fontLoaded;

    // 背景
    m_background.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    m_background.setFillColor(sf::Color(34, 139, 34));
    m_background.setPosition(0, 0);

    // 标题
    m_titleText.setString("Game Play - Plants vs Zombies");
    if (m_useCustomFont)
        m_titleText.setFont(m_font);
    m_titleText.setCharacterSize(32);
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setPosition(50, 30);

    // 说明文本 (用于种植提示)
    m_instructionText.setString("Click on grid to plant.\n1: Sunflower | 2: Peashooter\nESC: Menu | R: Restart");
    if (m_useCustomFont)
        m_instructionText.setFont(m_font);
    m_instructionText.setCharacterSize(18);
    m_instructionText.setFillColor(sf::Color::Yellow);
    m_instructionText.setPosition(50, 80);

    // 种植模式文本
    if (m_useCustomFont)
        m_plantingModeText.setFont(m_font);
    m_plantingModeText.setCharacterSize(18);
    m_plantingModeText.setFillColor(sf::Color::Magenta);
    m_plantingModeText.setPosition(WINDOW_WIDTH - 250, 50); // 右上角显示
    updatePlantingModeText();

    // 游戏信息文本 (可以保留或修改)
    if (m_useCustomFont)
        m_gameInfoText.setFont(m_font);
    m_gameInfoText.setCharacterSize(16);
    m_gameInfoText.setFillColor(sf::Color::Cyan);
    m_gameInfoText.setPosition(WINDOW_WIDTH - 250, 80); // 种植模式文本下方

    m_gameTime = 0.0f;

    // 初始化网格 (如果 Grid 类有 initialize 方法)
    m_grid.initialize(); // 确保你的 Grid.cpp 中 Grid::initialize() 实际创建了线条
                         // 并且 Grid 的构造函数设置了正确的起始位置和单元格大小
                         // 从你的 Grid.cpp 看，构造函数已经设置了 m_startPosition，initialize() 调用 createGridLines()

    std::cout << "GamePlayState entered successfully!" << std::endl;
}

void GamePlayState::exit()
{
    m_plantManager.clear(); // 清理植物，以防状态切换时残留
    std::cout << "GamePlayState exited" << std::endl;
}

void GamePlayState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::Escape:
            m_stateManager->changeState(std::make_unique<MenuState>(m_stateManager));
            break;
        case sf::Keyboard::R:
            m_gameTime = 0.0f;
            m_plantManager.clear(); // 重启时也清理植物
            std::cout << "Game restarted!" << std::endl;
            break;
        case sf::Keyboard::Num1: // 按键 1 选择向日葵
        case sf::Keyboard::Numpad1:
            m_currentPlantToPlace = PlantType::SUNFLOWER;
            updatePlantingModeText();
            std::cout << "Selected Sunflower to plant." << std::endl;
            break;
        case sf::Keyboard::Num2: // 按键 2 选择豌豆射手
        case sf::Keyboard::Numpad2:
            m_currentPlantToPlace = PlantType::PEASHOOTER;
            updatePlantingModeText();
            std::cout << "Selected Peashooter to plant." << std::endl;
            break;
        default:
            break;
        }
    }

    if (event.type == sf::Event::MouseMoved)
    {
        m_mousePos.x = event.mouseMove.x;
        m_mousePos.y = event.mouseMove.y;
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            // 将屏幕像素坐标转换为网格坐标 (行, 列)
            // 你的 Grid::getGridPosition(const sf::Vector2f&) 返回 (row, col)
            sf::Vector2i gridPos = m_grid.getGridPosition(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));

            // 尝试种植植物
            // gridPos.x 是行, gridPos.y 是列
            if (m_grid.isValidGridPosition(gridPos))
            { // PlantManager 内部也会检查，但这里预先检查可以给用户反馈
                if (m_plantManager.tryAddPlant(m_currentPlantToPlace, gridPos))
                {
                    std::cout << "Successfully planted at grid (" << gridPos.x << "r, " << gridPos.y << "c)" << std::endl;
                }
                else
                {
                    std::cout << "Failed to plant at grid (" << gridPos.x << "r, " << gridPos.y << "c). Cell might be occupied or invalid." << std::endl;
                }
            }
            else
            {
                std::cout << "Clicked outside valid grid area." << std::endl;
            }
        }
    }
}

void GamePlayState::update(float deltaTime)
{
    m_gameTime += deltaTime;
    m_plantManager.update(deltaTime); // 更新植物管理器

    // 更新游戏信息显示
    std::stringstream ss;
    ss << "Game Time: " << static_cast<int>(m_gameTime) << "s\n";
    ss << "Mouse: (" << m_mousePos.x << ", " << m_mousePos.y << ")\n";
    ss << "FPS: " << static_cast<int>(1.0f / deltaTime) << "\n";
    // ss << "Plants: " << m_plantManager.getAllPlants().size() << "\n"; // 显示植物数量

    m_gameInfoText.setString(ss.str());
}

void GamePlayState::render(sf::RenderWindow &window)
{
    window.clear(); // 通常在 Game::run 或 StateManager 的 render 中调用 clear

    // 渲染背景
    window.draw(m_background);

    // 渲染网格 (由 Grid 类负责)
    m_grid.render(window);

    // 渲染植物 (由 PlantManager 负责)
    m_plantManager.draw(window);

    // 渲染UI文本
    window.draw(m_titleText);
    window.draw(m_instructionText);
    window.draw(m_plantingModeText);
    window.draw(m_gameInfoText);

    // 优化后的鼠标位置指示器 (如果你还想保留它)
    // 确保 Constants.h 中有 GRID_START_X, GRID_START_Y, GRID_CELL_WIDTH, GRID_CELL_HEIGHT
    // 但现在 Grid 类内部有 m_startPosition，最好从 Grid 实例获取
    if (m_mousePos.x >= 0 && m_mousePos.y >= 0)
    {
        sf::Vector2f gridDrawStart = m_grid.getGridStartPosition(); // 获取Grid实际绘制的左上角
        sf::Vector2f cellSize = m_grid.getCellSize();

        // 计算鼠标在哪个网格单元格内（用于高亮或其他反馈）
        sf::Vector2i hoverGridPos = m_grid.getGridPosition(sf::Vector2f(m_mousePos.x, m_mousePos.y));

        // 如果希望高亮鼠标悬停的格子
        if (m_grid.isValidGridPosition(hoverGridPos))
        {
            sf::RectangleShape hoverCellHighlight;
            hoverCellHighlight.setSize(cellSize - sf::Vector2f(2, 2));   // 比格子小一点
            hoverCellHighlight.setFillColor(sf::Color(255, 255, 0, 70)); // 黄色半透明
            // 你需要 Grid 类提供一个 getWorldPosition(row, col) 来获取单元格左上角
            // 或者使用 cellCenter 然后调整。你的 Grid::getWorldPosition 返回中心。
            // Plant.cpp 使用了 grid.getWorldPosition(row, col) 来获取中心。
            // 我们需要左上角来画高亮框。
            // 假设 Grid 有一个 getCellTopLeftPosition(row, col)
            // sf::Vector2f cellTopLeft = m_grid.getCellTopLeftPosition(hoverGridPos.x, hoverGridPos.y);
            // hoverCellHighlight.setPosition(cellTopLeft + sf::Vector2f(1,1));
            // window.draw(hoverCellHighlight);

            // 或者简单地画一个鼠标指针标记
            sf::CircleShape mouseIndicator(8);
            mouseIndicator.setFillColor(sf::Color::Transparent);
            mouseIndicator.setOutlineColor(sf::Color::Red);
            mouseIndicator.setOutlineThickness(2);
            mouseIndicator.setPosition(m_mousePos.x - mouseIndicator.getRadius(), m_mousePos.y - mouseIndicator.getRadius());
            window.draw(mouseIndicator);
        }
    }
}

void GamePlayState::updatePlantingModeText()
{
    std::string mode = "Planting: ";
    switch (m_currentPlantToPlace)
    {
    case PlantType::SUNFLOWER:
        mode += "Sunflower (1)";
        break;
    case PlantType::PEASHOOTER:
        mode += "Peashooter (2)";
        break;
    default:
        mode += "None";
        break;
    }
    m_plantingModeText.setString(mode);
}
