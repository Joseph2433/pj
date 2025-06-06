#pragma once

#include <SFML/Graphics.hpp>
#include "SeedManager.h"
#include "ProgressBar.h"
// #include "../UI/Button.h" // 如果铲子也用Button类实现

// Forward declarations
class ResourceManager;
class SunManager;
class WaveManager;

// 新增一个枚举或布尔值来表示HUD的交互模式
enum class HUDInteractionMode
{
    NORMAL,         // 正常选择种子包
    SHOVEL_SELECTED // 铲子已被选中，等待点击植物
};

class HUD
{
public:
    HUD(ResourceManager &resManager, SunManager &sunManager, WaveManager &waveManager,
        sf::Font &primaryFont, sf::Font &secondaryFont);

    bool handleEvent(const sf::Event &event, const sf::Vector2f &mousePosInView);
    void update(float dt);
    void draw(sf::RenderWindow &window);

    PlantType getSelectedPlantTypeFromSeedManager(bool &isValidSelection) const;
    void notifyPlantPlacedToSeedManager(PlantType plantType);
    int getSelectedPlantCostFromSeedManager() const;

    HUDInteractionMode getCurrentInteractionMode() const; // <--- 获取当前交互模式
    void resetInteractionMode();                          // <--- 重置交互模式 (例如，在种植或铲除后)

private:
    void setupShovelUI(); // 初始化铲子UI

    SeedManager m_seedManager;
    sf::Text m_sunDisplayText;
    ProgressBar m_waveProgressBar;
    sf::Text m_totalWavesText;

    // --- 铲子相关 ---
    sf::Sprite m_shovelSprite;             // 铲子图标
    sf::RectangleShape m_shovelButtonArea; // 铲子图标的可点击区域 (或者使用 Button 类)
    HUDInteractionMode m_currentMode;      // <--- 当前HUD的交互模式
    sf::Sprite m_mouseCursorShovel;        // <--- (可选) 鼠标选中铲子后的光标精灵

    SunManager &m_sunManagerRef;
    WaveManager &m_waveManagerRef;
    ResourceManager &m_resourceManagerRef_forHUD; // <--- HUD 可能需要自己的 ResourceManager 引用
    sf::Font &m_primaryFontRef;
};
