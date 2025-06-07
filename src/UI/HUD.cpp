#include "HUD.h"
#include "../Core/ResourceManager.h" // HUD 可能需要直接加载自己的图标等
#include "../Systems/SunManager.h"   // 需要访问阳光数量
#include "../Systems/WaveManager.h"  // 需要访问波数和状态信息
#include "../Utils/Constants.h"      // 包含UI布局、字体大小、纹理键名等常量
#include <SFML/Window/Event.hpp>     // 用于事件处理 (虽然大部分事件传递给 SeedManager)
#include <sstream>                   // 用于将数字转换为字符串
#include <iomanip>                   // 用于格式化输出 (例如，std::fixed, std::setprecision)
#include <iostream>                  // 用于调试输出

// 假设铲子图标的纹理键在 Constants.h 中定义或在这里定义
// const std::string SHOVEL_TEXTURE_KEY = "shovel_icon"; // 示例
// const std::string SHOVEL_CURSOR_TEXTURE_KEY = "shovel_cursor"; // 示例

HUD::HUD(ResourceManager &resManager, SunManager &sunManager, WaveManager &waveManager,
         sf::Font &primaryFont, sf::Font &secondaryFont)
    : m_seedManager(resManager, sunManager, primaryFont, secondaryFont), // 初始化 SeedManager
      m_sunManagerRef(sunManager),
      m_waveManagerRef(waveManager),
      m_resourceManagerRef_forHUD(resManager), // HUD 也需要 ResourceManager 来加载自己的资源
      m_primaryFontRef(primaryFont),
      m_waveProgressBar(                                                     // 初始化 ProgressBar
          sf::Vector2f(WINDOW_WIDTH - 220.f, SEED_PACKET_UI_START_Y + 10.f), // 位置：右上角
          sf::Vector2f(200.f, 20.f),                                         // 大小
          sf::Color(70, 70, 70, 200),                                        // 背景色 (带透明度)
          sf::Color(200, 50, 50, 220)                                        // 默认填充色 (红色，带透明度)
          ),
      m_currentMode(HUDInteractionMode::NORMAL) // 初始交互模式
{
    std::cout << "HUD constructing..." << std::endl;

    // 1. 初始化阳光数量显示文本
    m_sunDisplayText.setFont(m_primaryFontRef);
    m_sunDisplayText.setCharacterSize(SUN_DISPLAY_FONT_SIZE);
    m_sunDisplayText.setFillColor(sf::Color::Yellow);
    m_sunDisplayText.setOutlineColor(sf::Color::Black);
    m_sunDisplayText.setOutlineThickness(1.f);
    m_sunDisplayText.setPosition(SUN_DISPLAY_X, SUN_DISPLAY_Y); // 位置来自 Constants.h

    // 2. 初始化进度条的文本属性
    m_waveProgressBar.setFont(m_primaryFontRef); // 进度条上的文本也用主字体
    m_waveProgressBar.setCharacterSize(12);      // 进度条上的文本字号可以小一些
    m_waveProgressBar.setTextColor(sf::Color::White);

    // 3. 初始化总波数显示文本
    m_totalWavesText.setFont(m_primaryFontRef);
    m_totalWavesText.setCharacterSize(14);                                      // 比进度条文本略大或一样
    m_totalWavesText.setFillColor(sf::Color(220, 220, 220));                    // 亮灰色
    m_totalWavesText.setString("Total: " + std::to_string(TOTAL_WAVES_TO_WIN)); // 从 Constants.h 获取

    // 定位总波数文本 (例如，在进度条正下方)
    // 使用 ProgressBar 的初始化参数来定位，而不是直接访问其私有成员
    const sf::Vector2f progressBarDefinedPos = sf::Vector2f(WINDOW_WIDTH - 220.f, SEED_PACKET_UI_START_Y + 10.f);
    const sf::Vector2f progressBarDefinedSize = sf::Vector2f(200.f, 20.f);
    float totalWavesTextX = progressBarDefinedPos.x;                                  // 与进度条左对齐
    float totalWavesTextY = progressBarDefinedPos.y + progressBarDefinedSize.y + 5.f; // 进度条Y + 进度条高 + 5像素间距
    m_totalWavesText.setPosition(totalWavesTextX, totalWavesTextY);

    // 4. 初始化铲子UI
    setupShovelUI();
    std::cout << "HUD constructed." << std::endl;
}

void HUD::setupShovelUI()
{
    std::cout << "HUD: Setting up shovel UI..." << std::endl;
    // 加载铲子图标纹理
    if (!m_resourceManagerRef_forHUD.hasTexture(SHOVEL_TEXTURE_KEY))
    {
        // 确保 "assets/images/ui/shovel.png" 图片存在
        if (!m_resourceManagerRef_forHUD.loadTexture(SHOVEL_TEXTURE_KEY, "assets/images/ui/shovel.png"))
        {
            std::cerr << "HUD Error: Failed to load shovel icon texture (Key: " << SHOVEL_TEXTURE_KEY << ")!" << std::endl;
        }
        else
        {
            std::cout << "HUD: Shovel icon texture loaded." << std::endl;
        }
    }
    else
    {
        std::cout << "HUD: Shovel icon texture already loaded." << std::endl;
    }
    m_shovelSprite.setTexture(m_resourceManagerRef_forHUD.getTexture(SHOVEL_TEXTURE_KEY));

    // 设置铲子图标的位置 (例如，在种子包栏的右边，阳光显示的下方)
    // float shovelX = SUN_DISPLAY_X + m_sunDisplayText.getGlobalBounds().width + 20.f; // 另一种定位方式
    float shovelX = SEED_PACKET_UI_START_X + 10.f;                      // 放在种子包左侧一点试试
    float shovelY = SEED_PACKET_UI_START_Y + SEED_PACKET_HEIGHT + 20.f; // 种子包栏下方
    m_shovelSprite.setPosition(shovelX, shovelY);

    // 根据需要调整铲子图标大小
    // m_shovelSprite.setScale(0.8f, 0.8f);

    // 设置铲子图标的可点击区域（重要：在设置完位置和缩放后获取包围盒）
    sf::FloatRect shovelBounds = m_shovelSprite.getGlobalBounds();
    m_shovelButtonArea.setPosition(shovelBounds.left, shovelBounds.top);
    m_shovelButtonArea.setSize(sf::Vector2f(shovelBounds.width, shovelBounds.height));
    // m_shovelButtonArea.setFillColor(sf::Color(0,0,255,100)); // 调试时可见

    //(可选) 加载铲子鼠标光标纹理
    if (!m_resourceManagerRef_forHUD.hasTexture(SHOVEL_CURSOR_TEXTURE_KEY))
    {
        if (!m_resourceManagerRef_forHUD.loadTexture(SHOVEL_CURSOR_TEXTURE_KEY, "assets/images/ui/shovel.png"))
        {
            // std::cerr << "HUD Warning: Failed to load shovel cursor texture." << std::endl; // 这个不是关键错误
        }
    }
    m_mouseCursorShovel.setTexture(m_resourceManagerRef_forHUD.getTexture(SHOVEL_CURSOR_TEXTURE_KEY));
    // 通常将光标原点设为其“尖端”，例如左上角或特定偏移
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
                return true; // <--- 事件被铲子图标消耗
            }
        }
    }

    // 如果当前不是铲子模式（或者点击的不是铲子图标），则将事件传递给种子包管理器
    if (m_currentMode == HUDInteractionMode::NORMAL)
    {
        if (m_seedManager.handleEvent(event, mousePosInView))
        {                // <--- 假设 SeedManager::handleEvent 也返回 bool
            return true; // 事件被种子包消耗
        }
    }
    else if (m_currentMode == HUDInteractionMode::SHOVEL_SELECTED)
    {
        // 在铲子模式下，HUD本身可能也需要处理一些取消事件
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
        {
            resetInteractionMode(); // 右键取消铲子
            return true;            // 事件被消耗
        }
        // ESC键取消铲子的逻辑已移至 GamePlayState
    }
    return false; // 事件未被HUD消耗
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
    case SpawnState::ALL_WAVES_COMPLETED: // ALL_WAVES_COMPLETED 也用和平色
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

    // 总波数文本的显示逻辑
    if (m_waveManagerRef.getCurrentSpawnState() == SpawnState::ALL_WAVES_COMPLETED)
    {
        // GamePlayState 会处理胜利切换，这里 HUD 可以显示一个通用的完成信息
        // 或者，如果 WaveManager 有 hasPlayerWon() 并且它考虑了僵尸清场，可以用那个
        // 但更简单的是，如果 WaveManager 到了 ALL_WAVES_COMPLETED，
        // GamePlayState 会在僵尸清零后切换到 VictoryState，HUD 会随之销毁或不再更新。
        // 所以，HUD 这里可能不需要特别处理胜利文本，除非 VictoryState 也用这个 HUD。
        // 让我们假设当 ALL_WAVES_COMPLETED 时，总波数文本可以显示一个通用完成信息。
        m_totalWavesText.setString("All waves done!"); // 或者 "Final Wave!"
        // (重新定位文本如果需要)
    }
    else
    {
        m_totalWavesText.setString("Total: " + std::to_string(TOTAL_WAVES_TO_WIN));
        // 确保位置正确
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
    window.draw(m_totalWavesText); // 绘制总波数文本
    window.draw(m_shovelSprite);   // 绘制铲子图标

    // (可选) 绘制自定义铲子鼠标光标
    // if (m_currentMode == HUDInteractionMode::SHOVEL_SELECTED) {
    //     // 确保 GamePlayState 在 update 中设置了 window.setMouseCursorVisible(false);
    //     // 并且更新了 m_mouseCursorShovel 的位置
    //     // window.draw(m_mouseCursorShovel);
    // }
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
    m_seedManager.notifyPlantPlaced(plantType); // SeedManager 内部会取消选中
    // m_currentMode = HUDInteractionMode::NORMAL; // 种植后也应该回到普通模式，如果之前是种子选择模式
}

int HUD::getSelectedPlantCostFromSeedManager() const
{
    return m_seedManager.getSelectedPlantCost();
}
