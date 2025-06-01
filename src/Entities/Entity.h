#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

// Entity是所有游戏对象的基类
// 它提供了位置、纹理、生命值等基本属性和方法
class Entity
{
public:
    Entity();
    virtual ~Entity() = default;

    // 纯虚函数，子类必须实现
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow &window) = 0;

    // 位置相关方法
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f &position);
    sf::Vector2f getPosition() const;

    // 纹理相关方法
    void setTexture(const sf::Texture &texture);
    sf::Sprite &getSprite() { return m_sprite; }
    const sf::Sprite &getSprite() const { return m_sprite; }

    // 碰撞检测
    sf::FloatRect getBounds() const;
    bool intersects(const Entity &other) const;

    // 生命值系统
    void setHealth(int health) { m_health = health; }
    int getHealth() const { return m_health; }
    void takeDamage(int damage);
    bool isAlive() const { return m_health > 0; }

    // 激活状态
    void setActive(bool active) { m_active = active; }
    bool isActive() const { return m_active; }

protected:
    sf::Sprite m_sprite; // 精灵对象，用于渲染
    int m_health;        // 生命值
    bool m_active;       // 是否激活状态
};

#endif