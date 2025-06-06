#include "HUD.h"
#include "../Core/ResourceManager.h" // If HUD loads its own resources like sun icon
#include "../Systems/SunManager.h"
#include "../Utils/Constants.h" // For layout constants like SUN_DISPLAY_X/Y
#include <SFML/Window/Event.hpp>
#include "../Systems/WaveManager.h"
#include <sstream>
#include <iomanip>

HUD::HUD(ResourceManager &resManager, SunManager &sunManager, WaveManager &waveManager,
         sf::Font &primaryFont, sf::Font &secondaryFont)
    : m_seedManager(resManager, sunManager, primaryFont, secondaryFont), // Initialize SeedManager
      m_sunManagerRef(sunManager),
      m_waveManagerRef(waveManager),
      m_primaryFontRef(primaryFont),
      m_waveProgressBar(
          sf::Vector2f(WINDOW_WIDTH - 220.f, SEED_PACKET_UI_START_Y + 10.f), // 示例位置：右上角
          sf::Vector2f(200.f, 20.f),                                         // 示例大小
          sf::Color(70, 70, 70),                                             // 背景色
          sf::Color(200, 50, 50)                                             // 填充色 (红色，表示僵尸进度)
      )
{

    // Initialize Sun Display Text
    m_sunDisplayText.setFont(m_primaryFontRef);
    m_sunDisplayText.setCharacterSize(SUN_DISPLAY_FONT_SIZE);
    m_sunDisplayText.setFillColor(sf::Color::Yellow);
    m_sunDisplayText.setOutlineColor(sf::Color::Black);
    m_sunDisplayText.setOutlineThickness(1.f);
    m_sunDisplayText.setPosition(SUN_DISPLAY_X, SUN_DISPLAY_Y);

    m_waveProgressBar.setFont(m_primaryFontRef);
    m_waveProgressBar.setCharacterSize(12);
    m_waveProgressBar.setTextColor(sf::Color::White);
}

void HUD::handleEvent(const sf::Event &event, const sf::Vector2f &mousePosInView)
{
    m_seedManager.handleEvent(event, mousePosInView);
    // Handle other HUD interactable elements if any (e.g., shovel button)
}

void HUD::update(float dt)
{
    m_seedManager.update(dt); // Update all seed packets (cooldowns)

    // Update sun display text
    std::stringstream ss;
    ss << m_sunManagerRef.getCurrentSun(); // Just the number, "Sun: " can be static or part of a background
    m_sunDisplayText.setString(ss.str());

    // --- 更新进度条 ---
    float progress = m_waveManagerRef.getCurrentWaveProgress();
    std::string progressText = m_waveManagerRef.getWaveProgressLabel();

    m_waveProgressBar.setProgress(progress);
    m_waveProgressBar.setText(progressText);

    SpawnState currentState = m_waveManagerRef.getCurrentSpawnState();
    sf::Color barFillColor = sf::Color(200, 50, 50); // 默认是红色 (用于进攻波次)

    switch (currentState)
    {
    case SpawnState::IDLE:
    case SpawnState::WAVE_COOLDOWN:
        barFillColor = sf::Color(50, 150, 200); // 和平/冷却期用蓝色
        break;
    case SpawnState::PREPARING_WAVE:
        barFillColor = sf::Color(200, 150, 50); // 准备期用橙色
        break;
    case SpawnState::HUGE_WAVE_ANNOUNCE:
    case SpawnState::HUGE_WAVE_SPAWN:
        barFillColor = sf::Color(255, 0, 0, 220); // 大波时用更鲜艳的红色
        break;
    case SpawnState::NORMAL_SPAWN:
        // 对于普通波，保持默认的进攻色 (红色)
        break;
    }
}

void HUD::draw(sf::RenderWindow &window)
{
    m_seedManager.draw(window);
    window.draw(m_sunDisplayText);
    m_waveProgressBar.draw(window);
}

PlantType HUD::getSelectedPlantTypeFromSeedManager(bool &isValidSelection) const
{
    return m_seedManager.getSelectedPlantType(isValidSelection);
}

void HUD::notifyPlantPlacedToSeedManager(PlantType plantType)
{
    m_seedManager.notifyPlantPlaced(plantType);
}

int HUD::getSelectedPlantCostFromSeedManager() const
{
    return m_seedManager.getSelectedPlantCost();
}
