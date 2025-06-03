#pragma once // 防止头文件被多次包含

#include "Core/GameState.h"               // 游戏状态基类
#include "../Systems/Grid.h"              // 网格系统
#include "../Systems/ProjectileManager.h" //子弹管理器
#include "../Systems/PlantManager.h"      // 植物管理器
#include "../Systems/ZombieManager.h"     //僵尸管理器
#include "../UI/HUD.h"                    // HUD (内部包含SeedManager)
#include "../Systems/SunManager.h"        // SunManager (之前是GameLogic/，根据你的结构改为Systems/)
#include "../Entities/Sun.h"              // <--- 新增: 包含阳光实体头文件
#include <SFML/Graphics.hpp>              // SFML 图形库
#include <vector>                         // <--- 新增: 用于 std::vector
#include <memory>                         // <--- 新增: 用于 std::unique_ptr

// 前向声明
class StateManager;
class Plant; // <--- 新增: 前向声明 Plant 类，因为 spawnSunFromPlant 方法参数需要

// 游戏进行状态类，处理核心的游戏逻辑，如植物种植、僵尸交互、UI显示等
class GamePlayState : public GameState
{
public:
    // 构造函数，接收一个指向状态管理器的指针
    GamePlayState(StateManager *stateManager);
    // 默认析构函数
    ~GamePlayState() override = default;

    // --- GameState 接口方法实现 ---
    void enter() override;                             // 进入此状态时调用
    void exit() override;                              // 退出此状态时调用
    void handleEvent(const sf::Event &event) override; // 处理事件
    void update(float deltaTime) override;             // 更新游戏逻辑
    void render(sf::RenderWindow &window) override;    // 渲染游戏画面

    // --- 公共方法，供 PlantManager 调用 ---
    // 当植物（如向日葵）请求产生阳光时，由 PlantManager 调用此方法
    void spawnSunFromPlant(Plant *plant);

private:
    // 私有辅助方法，用于加载此状态所需的特定资源（主要是字体）
    void loadAssets();
    // 私有辅助方法，用于从天空随机产生阳光
    void spawnSunFromSky();
    // 测试僵尸的生成
    void spawnInitialZombiesForTesting();

    // --- 字体资源 ---
    sf::Font m_primaryGameFont;   // 主要游戏字体
    sf::Font m_secondaryGameFont; // 次要游戏字体
    bool m_fontsLoaded;           // 标记字体是否已成功加载

    // --- 基础UI元素 ---
    sf::RectangleShape m_backgroundShape; // 游戏背景
    sf::Text m_debugInfoText;             // 显示调试信息

    // --- 核心游戏系统与逻辑对象 ---
    Grid m_grid;                           // 游戏场地网格
    ProjectileManager m_projectileManager; // 子弹管理器实例
    PlantManager m_plantManager;           // 植物管理器实例 (构造时会接收 GamePlayState&)
    SunManager m_sunManager;               // 阳光管理器实例 (管理总阳光数)
    ZombieManager m_zombieManager;         // 僵尸管理器实例
    HUD m_hud;                             // 游戏主界面 (包含种子包栏和阳光显示)

    // --- 阳光实体管理 ---
    std::vector<std::unique_ptr<Sun>> m_activeSuns; // <--- 新增: 存储所有激活的阳光实体

    // --- 天空阳光生成相关 ---
    sf::Clock m_skySunSpawnTimer;       // <--- 新增: 天空阳光生成计时器
    float m_skySunSpawnIntervalMin;     // <--- 新增: 最小生成间隔 (秒)
    float m_skySunSpawnIntervalMax;     // <--- 新增: 最大生成间隔 (秒)
    float m_currentSkySunSpawnInterval; // <--- 新增: 当前的随机生成间隔

    // 简单按时间生成僵尸的计时器
    sf::Clock m_zombieSpawnTestTimer;
    float m_zombieSpawnTestInterval = 8.0f; // 每8秒生成一个测试僵尸

    // --- 状态内部变量 ---
    float m_gameTime;             // 游戏进行时间
    sf::Vector2i m_mousePixelPos; // 最近的鼠标像素坐标
};
