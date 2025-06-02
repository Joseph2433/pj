#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

class Entity
{
public:
    // 构造函数：接收一个对纹理的常量引用
    Entity(const sf::Texture &texture);

    // 虚析构函数，确保派生类的析构函数被正确调用
    virtual ~Entity() = default;

    // 虚更新函数，由派生类重写
    // dt: دلتا时间，即自上一帧以来经过的时间
    virtual void update(float dt);

    // 将实体绘制到渲染窗口
    // 标记为 const 是因为绘制操作通常不应改变实体的状态
    virtual void draw(sf::RenderWindow &window) const;

    // 位置相关函数
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f &position);
    const sf::Vector2f &getPosition() const;

    // 中心点 (Origin) 相关函数 (用于围绕某点旋转和缩放)
    void setOrigin(float x, float y);
    void setOrigin(const sf::Vector2f &origin);
    const sf::Vector2f &getOrigin() const;
    void centerOrigin(); // 辅助函数，将中心点设置到精灵图的中心

    // 缩放相关函数
    void setScale(float factorX, float factorY);
    void setScale(const sf::Vector2f &factors);
    const sf::Vector2f &getScale() const;

    // 旋转相关函数
    void setRotation(float angle);
    float getRotation() const;

    // 获取实体精灵图的全局包围盒 (常用于碰撞检测)
    sf::FloatRect getGlobalBounds() const;

    // 可选：如果派生类需要更复杂的精灵图操作，可以提供对精灵图的直接访问
    // sf::Sprite& getSprite();
    // const sf::Sprite& getSprite() const;

protected:
    sf::Sprite m_sprite; // 实体的主要视觉表现

    // 实体可能还会有生命值、速度等通用属性，
    // 但为了保持基类的简洁性，这些可以添加到派生类中，
    // 或者如果它们确实是所有实体都通用的，则添加到这里。
};
