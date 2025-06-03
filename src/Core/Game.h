#pragma once // 防止头文件被多次包含

#include <SFML/Graphics.hpp> // 包含 SFML 图形库，用于窗口和时间等
#include "StateManager.h"    // 包含状态管理器头文件
#include "ResourceManager.h" // 包含资源管理器头文件
// #include "../Utils/Constants.h" // Constants.h 通常被 .cpp 文件包含，或按需被其他 .h 包含

class Game
{
public:
    // 构造函数：初始化游戏窗口、资源管理器、状态管理器等
    Game();
    // 默认析构函数：对于这个类，默认析构通常足够，
    // 因为 m_window, m_resourceManager, m_stateManager 都是栈上对象或能自我管理的对象。
    ~Game() = default;

    // 运行游戏主循环
    void run();

    // --- 公共访问器 (Getter) ---
    // 获取资源管理器的引用，以便其他部分（如状态）可以加载和获取资源
    ResourceManager &getResourceManager();
    // 获取状态管理器的引用（虽然通常状态是通过构造函数接收 StateManager*）
    StateManager &getStateManager();
    // 获取渲染窗口的引用，例如状态需要知道窗口尺寸或进行自定义绘制
    sf::RenderWindow &getWindow();

private:
    // --- 私有辅助方法 ---
    // 处理窗口事件（如关闭、键盘、鼠标）
    void processEvents();
    // 更新游戏逻辑，参数为每帧的时间差 (delta time)
    void update(sf::Time deltaTime);
    // 渲染游戏画面
    void render();
    // 加载全局共享资源，例如游戏启动时就需要的主字体等
    void loadGlobalResources();

    // --- 私有成员变量 ---
    sf::RenderWindow m_window;         // SFML 渲染窗口对象
    ResourceManager m_resourceManager; // 游戏资源管理器实例
    StateManager m_stateManager;       // 游戏状态管理器实例

    // 控制游戏循环的固定时间步长 (例如，每秒更新60次)
    // TIME_PER_FRAME 应该在 Constants.h 中定义
    // const sf::Time TimePerFrame = sf::seconds(1.f / 60.f); // 或者直接在此定义
};
