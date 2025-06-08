#include "HUD.h"
#include "../Core/ResourceManager.h"
#include "../Systems/SunManager.h"
#include "../Systems/WaveManager.h"
#include "../Utils/Constants.h"
#include <SFML/Window/Event.hpp>
#include <sstream>
#include <iomanip>
#include <iostream>

HUD::HUD(ResourceManager &resManager, SunManager &sunManager, WaveManager &waveManager,
         sf::Font &primaryFont, sf::Font &secondaryFont)
    : m_seedManager(resManager, sunManager, primaryFont, secondaryFont),
      m_sunManagerRef(sunManager),
      m_waveManagerRef(waveManager),
      m_resourceManagerRef_forHUD(resManager),
      m_primaryFontRef(primaryFont),
      m_waveProgressBar(
          sf::Vector2f(WINDOW_WIDTH - 220.f, SEED_PACKET_UI_START_Y + 10.f),
          sf::Vector2f(200.f, 20.f),
          sf::Color(70, 70, 70, 200),
          sf::Color(200, 50, 50, 220)),
      m_currentMode(HUDInteractionMode::NORMAL)
{
    std::cout << "HUD constructing..." << std::endl;

    // 1. 初始化阳光数量显示文本
    m_sunDisplayText.setFont(m_primaryFontRef);
    m_sunDisplayText.setCharacterSize(SUN_DISPLAY_FONT_SIZE);
    m_sunDisplayText.setFillColor(sf::Color::Yellow);
    m_sunDisplayText.setOutlineColor(sf::Color::Black);
    m_sunDisplayText.setOutlineThickness(1.f);
    m_sunDisplayText.setPosition(SUN_DISPLAY_X, SUN_DISPLAY_Y);

    // 2. 初始化进度条的文本属性
    m_waveProgressBar.setFont(m_primaryFontRef);
    m_waveProgressBar.setCharacterSize(12);
    m_waveProgressBar.setTextColor(sf::Color::White);

    // 3. 初始化总波数显示文本
    m_totalWavesText.setFont(m_primaryFontRef);
    m_totalWavesText.setCharacterSize(14);
    m_totalWavesText.setFillColor(sf::Color(220, 220, 220));
    m_totalWavesText.setString("Total: " + std::to_string(TOTAL_WAVES_TO_WIN));

    // 定位总波数文本
    const sf::Vector2f progressBarDefinedPos = sf::Vector2f(WINDOW_WIDTH - 220.f, SEED_PACKET_UI_START_Y + 10.f);
    const sf::Vector2f progressBarDefinedSize = sf::Vector2f(200.f, 20.f);
    float totalWavesTextX = progressBarDefinedPos.x;
    float totalWavesTextY = progressBarDefinedPos.y + progressBarDefinedSize.y + 5.f;
    m_totalWavesText.setPosition(totalWavesTextX, totalWavesTextY);

    // 4. 初始化铲子UI
    setupShovelUI();
    std::cout << "HUD constructed." << std::endl;
}

void HUD::setupShovelUI()
{
    std::cout << "HUD: Setting up shovel UI..." << std::endl;
    const sf::Texture &shovelTex = m_resourceManagerRef_forHUD.getTexture(SHOVEL_TEXTURE_KEY);

    if (shovelTex.getSize().x == 0 || shovelTex.getSize().y == 0)
    {
        std::cerr << "HUD Warning: Shovel texture (Key: " << SHOVEL_TEXTURE_KEY << ") not found or is invalid from ResourceManager." << std::endl;
    }
    m_shovelSprite.setTexture(shovelTex);

    // 设置铲子图标的位置 (在种子包栏的右边)
    float shovelX = SEED_PACKET_UI_START_X + 10.f;
    float shovelY = SEED_PACKET_UI_START_Y + SEED_PACKET_HEIGHT + 20.f;
    m_shovelSprite.setPosition(shovelX, shovelY);

    // 设置铲子图标的可点击区域
    sf::FloatRect shovelBounds = m_shovelSprite.getGlobalBounds();
    m_shovelButtonArea.setPosition(shovelBounds.left, shovelBounds.top);
    m_shovelButtonArea.setSize(sf::Vector2f(shovelBounds.width, shovelBounds.height));

    // 加载铲子鼠标光标纹理
    if (!m_resourceManagerRef_forHUD.hasTexture(SHOVEL_CURSOR_TEXTURE_KEY))
    {
        m_resourceManagerRef_forHUD.loadTexture(SHOVEL_CURSOR_TEXTURE_KEY, "../../assets/images/shovel.png");
    }
    m_mouseCursorShovel.setTexture(m_resourceManagerRef_forHUD.getTexture(SHOVEL_CURSOR_TEXTURE_KEY));
    // 将光标原点设为其“尖端”
    m_mouseCursorShovel.setOrigin(0, 0);
    std::cout << "HUD: Shovel UI setup complete. Position: (" << shovelX << "," << shovelY << ")" << std::endl;
}

bool HUD::handleEvent(const sf::Event &event, const sf::Vector2f &mousePosInView)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            // 检查是否点击了铲子图标
            if (m_shovelButtonArea.getGlobalBounds().contains(mousePosInView))
            {
                if (m_currentMode == HUDInteractionMode::SHOVEL_SELECTED)
                {
                    m_currentMode = HUDInteractionMode::NORMAL;
                    std::cout << "HUD: Shovel deselected by clicking icon again." << std::endl;
                }
                else
                {
                    m_currentMode = HUDInteractionMode::SHOVEL_SELECTED;
                    m_seedManager.deselectAllPackets();
                    std::cout << "HUD: Shovel selected." << std::endl;
                }
                return true;
            }
        }
    }

    if (m_currentMode == HUDInteractionMode::NORMAL)
    {
        if (m_seedManager.handleEvent(event, mousePosInView))
        {
            return true;
        }
    }
    else if (m_currentMode == HUDInteractionMode::SHOVEL_SELECTED)
    {

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
        {
            resetInteractionMode();
            return true;
        }
    }
    return false;
}

void HUD::update(float dt)
{
    m_seedManager.update(dt);

    std::stringstream ssSun;
    ssSun << m_sunManagerRef.getCurrentSun();
    m_sunDisplayText.setString(ssSun.str());

    float progress = m_waveManagerRef.getCurrentWaveProgress();
    std::string progressLabel = m_waveManagerRef.getWaveProgressLabel();

    m_waveProgressBar.setProgress(progress);
    m_waveProgressBar.setText(progressLabel);

    SpawnState currentState = m_waveManagerRef.getCurrentSpawnState();
    sf::Color barFillColor = sf::Color(200, 50, 50, 220);
    switch (currentState)
    {
    case SpawnState::IDLE:
    case SpawnState::WAVE_COOLDOWN:
    case SpawnState::ALL_WAVES_COMPLETED:
        barFillColor = sf::Color(50, 150, 200, 220);
        break;
    case SpawnState::PREPARING_WAVE:
        barFillColor = sf::Color(200, 150, 50, 220);
        break;
    case SpawnState::HUGE_WAVE_ANNOUNCE:
    case SpawnState::HUGE_WAVE_SPAWN:
        barFillColor = sf::Color(255, 0, 0, 240);
        break;
    case SpawnState::NORMAL_SPAWN:
        break;
    }
    m_waveProgressBar.setFillColor(barFillColor);

    if (m_waveManagerRef.getCurrentSpawnState() == SpawnState::ALL_WAVES_COMPLETED)
    {

        m_totalWavesText.setString("All waves done!");
    }
    else
    {
        m_totalWavesText.setString("Total: " + std::to_string(TOTAL_WAVES_TO_WIN));
        const sf::Vector2f progressBarDefinedPos = sf::Vector2f(WINDOW_WIDTH - 220.f, SEED_PACKET_UI_START_Y + 10.f);
        const sf::Vector2f progressBarDefinedSize = sf::Vector2f(200.f, 20.f);
        float totalWavesTextX = progressBarDefinedPos.x;
        float totalWavesTextY = progressBarDefinedPos.y + progressBarDefinedSize.y + 5.f;
        m_totalWavesText.setPosition(totalWavesTextX, totalWavesTextY);
    }

    // 铲子图标颜色更新
    if (m_currentMode == HUDInteractionMode::SHOVEL_SELECTED)
    {
        m_shovelSprite.setColor(sf::Color(255, 255, 150, 255));
    }
    else
    {
        m_shovelSprite.setColor(sf::Color::White);
    }
}

void HUD::draw(sf::RenderWindow &window)
{
    m_seedManager.draw(window);
    window.draw(m_sunDisplayText);
    m_waveProgressBar.draw(window);
    window.draw(m_totalWavesText);
    window.draw(m_shovelSprite);
}

HUDInteractionMode HUD::getCurrentInteractionMode() const
{
    return m_currentMode;
}

void HUD::resetInteractionMode()
{
    if (m_currentMode != HUDInteractionMode::NORMAL)
    {
        m_currentMode = HUDInteractionMode::NORMAL;
        std::cout << "HUD: Interaction mode reset to NORMAL." << std::endl;
    }
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
