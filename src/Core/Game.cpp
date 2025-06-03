#include "Game.h"             // Game 类的头文件
#include "States/MenuState.h" // 假设你的初始状态是主菜单
// #include "States/GamePlayState.h" // 如果想直接测试游戏状态，可以取消注释这个
#include "../Utils/Constants.h" // 包含窗口标题、尺寸等常量
#include <iostream>             // 用于调试输出

Game::Game()
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), // 使用常量设置窗口模式
               WINDOW_TITLE,                               // 使用常量设置窗口标题
               sf::Style::Default),                        // 使用默认窗口样式
      m_resourceManager(),                                 // ResourceManager 的默认构造函数会被调用
      m_stateManager(this)
{ // 将 Game 实例 (this) 传递给 StateManager 的构造函数

    m_window.setFramerateLimit(static_cast<unsigned int>(TARGET_FPS)); // 限制帧率
    std::cout << "Game 对象已构造。" << std::endl;

    // 在游戏开始时加载全局/通用资源，特别是UI和状态可能提前需要的字体
    loadGlobalResources();

    // 推入初始状态 (例如，主菜单状态)
    m_stateManager.pushState(std::make_unique<MenuState>(&m_stateManager));
    // 如果想直接测试 GamePlayState:
    // m_stateManager.pushState(std::make_unique<GamePlayState>(&m_stateManager));

    if (m_stateManager.isEmpty())
    { // 检查状态是否成功推入
        std::cerr << "严重错误:没有初始状态被推入 StateManager！" << std::endl;
        m_window.close(); // 如果没有状态，游戏无法运行
    }
}

// 新增:加载全局资源的方法
void Game::loadGlobalResources()
{
    std::cout << "Game:正在加载全局资源..." << std::endl;
    // 示例:加载主要字体，如果 Constants.h 中定义了 FONT_ID_PRIMARY 和 FONT_PATH_MAIN
    if (!m_resourceManager.hasFont(FONT_ID_PRIMARY))
    {
        if (!m_resourceManager.loadFont(FONT_ID_PRIMARY, FONT_PATH_ARIAL))
        {
            // 如果主要字体（例如项目自带字体）加载失败，尝试加载一个系统字体作为备用
            if (!m_resourceManager.loadFont(FONT_ID_PRIMARY, FONT_PATH_ARIAL))
            { // 使用常量定义的Arial路径
                std::cerr << "Game:严重 - 全局主要字体加载失败。" << std::endl;
            }
        }
    }
    // 加载次要字体，同理
    if (!m_resourceManager.hasFont(FONT_ID_SECONDARY))
    {
        // 假设 FONT_PATH_ARIAL 也可作为次要字体路径，或定义另一个路径常量
        if (!m_resourceManager.loadFont(FONT_ID_SECONDARY, FONT_PATH_ARIAL))
        {
            std::cerr << "Game:全局次要字体加载失败。某些UI可能会使用主要字体作为备用。" << std::endl;
        }
    }

    if (!m_resourceManager.hasTexture(PEA_TEXTURE_KEY))
    {
        if (!m_resourceManager.loadTexture(PEA_TEXTURE_KEY, "assets/images/projectiles/pea.png"))
        { // 确保路径正确
            std::cerr << "Game: 豌豆子弹纹理 (" << PEA_TEXTURE_KEY << ") 加载失败！" << std::endl;
        }
    }
    std::cout << "Game:全局资源加载尝试完毕。" << std::endl;
}

void Game::run()
{
    sf::Clock clock;                               // 用于计算每帧消耗的时间
    sf::Time timeSinceLastUpdate = sf::Time::Zero; // 累积自上次更新以来的时间，用于固定时间步长

    // 主游戏循环
    while (m_window.isOpen())
    {
        sf::Time elapsedTime = clock.restart(); // 获取自上次调用 clock.restart() 以来经过的时间
        timeSinceLastUpdate += elapsedTime;     // 累加到时间池

        // 固定时间步长更新循环:
        // 只要累积的时间足够执行一次或多次固定时长的更新，就执行更新。
        // 这能确保游戏逻辑的更新频率是稳定的，不受渲染帧率波动的影响。
        while (timeSinceLastUpdate > TIME_PER_FRAME)
        {
            timeSinceLastUpdate -= TIME_PER_FRAME; // 从时间池中消耗一个时间步长

            processEvents();        // 处理窗口事件和用户输入
            update(TIME_PER_FRAME); // 以固定的时间间隔 (TIME_PER_FRAME) 更新游戏逻辑
        }

        // 渲染，其频率会尽可能快（但会受到垂直同步或 setFramerateLimit 的限制）
        render();

        // 如果状态管理器变空了 (例如，所有状态都已退出)，则关闭游戏窗口
        if (m_stateManager.isEmpty())
        {
            m_window.close();
        }
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    { // 处理所有在事件队列中待处理的事件
        // 首先处理窗口关闭事件，这是最高优先级的
        if (event.type == sf::Event::Closed)
        {
            m_window.close(); // 关闭窗口
        }
        // 将其他事件传递给当前活动的状态进行处理
        m_stateManager.handleEvent(event);
    }
}

void Game::update(sf::Time deltaTime)
{
    // 更新当前活动的状态
    // deltaTime.asSeconds() 将 sf::Time 对象转换为浮点数表示的秒
    m_stateManager.update(deltaTime.asSeconds());
}

void Game::render()
{
    m_window.clear(sf::Color(50, 50, 50)); // 用一个深灰色清屏，你可以选择其他背景色

    // 渲染当前活动的状态
    m_stateManager.render(m_window);

    m_window.display(); // 将后台缓冲区绘制的所有内容显示到屏幕上
}

// --- 公共访问器方法 ---
ResourceManager &Game::getResourceManager()
{
    return m_resourceManager;
}

StateManager &Game::getStateManager()
{
    return m_stateManager;
}

sf::RenderWindow &Game::getWindow()
{
    return m_window;
}
