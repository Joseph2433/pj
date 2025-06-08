#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Entity
{
public:
    Entity(const sf::Texture &texture);
    virtual ~Entity() = default;
    virtual void update(float dt);

    // 将实体绘制到渲染窗口
    virtual void draw(sf::RenderWindow &window) const;

    // 位置相关函数
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f &position);
    const sf::Vector2f &getPosition() const;

    // 中心点 (Origin) 相关函数 用于围绕某点旋转和缩放
    void setOrigin(float x, float y);
    void setOrigin(const sf::Vector2f &origin);
    const sf::Vector2f &getOrigin() const;
    void centerOrigin();

    // 缩放相关函数
    void setScale(float factorX, float factorY);
    void setScale(const sf::Vector2f &factors);
    const sf::Vector2f &getScale() const;

    // 旋转相关函数
    void setRotation(float angle);
    float getRotation() const;

    // 获取实体精灵图的全局包围盒用于碰撞检测
    sf::FloatRect getGlobalBounds() const;
    sf::FloatRect getLocalBounds() const;

    void move(float offsetX, float offsetY);
    void move(const sf::Vector2f &offset);

protected:
    sf::Sprite m_sprite;
};
