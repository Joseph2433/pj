#pragma once

#include <SFML/Graphics.hpp>
#include "SeedManager.h"
#include "ProgressBar.h"

class ResourceManager;
class SunManager;
class WaveManager;

// HUD的交互模式
enum class HUDInteractionMode
{
    NORMAL,
    SHOVEL_SELECTED
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

    HUDInteractionMode getCurrentInteractionMode() const;
    void resetInteractionMode();

private:
    void setupPacket();
    void setupShovelUI();
    SeedManager m_seedManager;
    sf::Text m_sunDisplayText;
    ProgressBar m_waveProgressBar;
    sf::Text m_totalWavesText;

    // 铲子相关
    sf::Sprite m_shovelSprite;
    sf::RectangleShape m_shovelButtonArea;
    HUDInteractionMode m_currentMode;
    sf::Sprite m_mouseCursorShovel;

    SunManager &m_sunManagerRef;
    WaveManager &m_waveManagerRef;
    ResourceManager &m_resourceManagerRef_forHUD;
    sf::Font &m_primaryFontRef;
};
