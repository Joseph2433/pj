#pragma once
#include <SFML/Graphics.hpp>
#include <functional> // 用于回调函数

class Button
{
public:
    // 构造函数：初始化按钮位置、尺寸、文本和字体
    Button(const sf::Vector2f &position, const sf::Vector2f &size,
           const std::string &text, const sf::Font &font);

    // 渲染按钮（调用时会自动更新外观）
    void render(sf::RenderWindow &window);

    // 处理鼠标移动事件（更新悬停状态）
    void handleMouseMove(const sf::Vector2f &mousePosition);

    // 处理鼠标点击事件（触发回调）
    void handleMouseClick(const sf::Vector2f &mousePosition);

    // 设置按钮点击回调函数
    void setCallback(std::function<void()> callback);

    // 设置按钮启用状态（禁用时按钮变灰）
    void setEnabled(bool enabled);

    // 获取按钮是否启用
    bool isEnabled() const;

private:
    // 更新按钮外观（根据启用状态和悬停状态）
    void updateAppearance();

    // 按钮图形
    sf::RectangleShape m_buttonShape;
    // 按钮文本
    sf::Text m_text;
    // 回调函数
    std::function<void()> m_callback;
    // 状态变量
    bool m_enabled = true;
    bool m_hovered = false;

    // 颜色定义
    static const sf::Color NORMAL_COLOR;        // 正常状态颜色
    static const sf::Color HOVER_COLOR;         // 悬停状态颜色
    static const sf::Color DISABLED_COLOR;      // 禁用状态颜色
    static const sf::Color TEXT_COLOR;          // 正常文本颜色
    static const sf::Color HOVER_TEXT_COLOR;    // 悬停文本颜色
    static const sf::Color DISABLED_TEXT_COLOR; // 禁用文本颜色
};