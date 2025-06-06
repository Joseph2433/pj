#pragma once
#include "Core/GameState.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "../UI/Button.h" // 使用你的 Button 类

class StateManager;
class ResourceManager;

class PauseState : public GameState
{
public:
    PauseState(StateManager *stateManager);
    ~PauseState() override = default;

    void enter() override;
    void exit() override; // 可能不需要特别的逻辑
    void handleEvent(const sf::Event &event) override;
    void update(float deltaTime) override; // 通常暂停状态的 update 是空的或只更新UI动画
    void render(sf::RenderWindow &window) override;

private:
    void setupUI();
    void executeAction(const std::string &action);

    sf::Font m_font; // 同样，字体最好通过 ResourceManager 管理
    bool m_fontLoaded;

    sf::Text m_pauseText;
    std::vector<Button> m_buttons;
    sf::Vector2f m_mousePosition;

    sf::RectangleShape m_backgroundOverlay; // 半透明背景遮罩
};
