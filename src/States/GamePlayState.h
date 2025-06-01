#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H

#include "../Core/GameState.h"
#include "../Systems/Grid.h"
#include "../Systems/PlantManager.h"
#include "../Systems/ZombieManager.h"
#include "../Systems/WaveManager.h"
#include "../UI/HUD.h"
#include <SFML/Graphics.hpp>
#include <memory>

// GamePlayState是游戏的核心状态，管理实际的游戏玩法
class GamePlayState : public GameState
{
public:
    GamePlayState(Game &game);
    ~GamePlayState();

    // 实现基类的纯虚函数
    void handleEvent(const sf::Event &event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

    void onEnter() override;
    void onExit() override;

    // 游戏状态访问器
    int getSunCount() const { return m_sunCount; }
    void addSun(int amount) { m_sunCount += amount; }
    bool spendSun(int amount);

    Grid &getGrid() { return *m_grid; }

private:
    // 游戏系统组件
    std::unique_ptr<Grid> m_grid;                   // 网格系统
    std::unique_ptr<PlantManager> m_plantManager;   // 植物管理器
    std::unique_ptr<ZombieManager> m_zombieManager; // 僵尸管理器
    std::unique_ptr<WaveManager> m_waveManager;     // 波次管理器
    std::unique_ptr<HUD> m_hud;                     // 用户界面

    // 游戏状态
    int m_sunCount;  // 当前阳光数量
    bool m_gameWon;  // 是否获胜
    bool m_gameLost; // 是否失败

    // 背景渲染
    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;

    // 选中的植物类型（用于种植）
    int m_selectedPlantType;

    // 私有方法
    void initializeBackground();
    void handleMouseClick(const sf::Event &event);
    void checkGameConditions();
};

#endif