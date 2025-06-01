#pragma once
#include <SFML/Graphics.hpp>

class Entity
{
public:
    Entity();
    Entity(float x, float y);
    virtual ~Entity() = default;

    // 纯虚函数，子类必须实现
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow &window) = 0;

    // 位置相关
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f &position);
    sf::Vector2f getPosition() const;

    // 网格位置相关
    void setGridPosition(int row, int col);
    sf::Vector2i getGridPosition() const;
    sf::Vector2f getWorldPositionFromGrid(int row, int col) const;

    // 纹理相关
    void setTexture(const sf::Texture &texture);
    sf::Sprite &getSprite();
    const sf::Sprite &getSprite() const;

    // 碰撞检测
    sf::FloatRect getBounds() const;
    bool intersects(const Entity &other) const;

    // 激活状态
    bool isActive() const;
    void setActive(bool active);

protected:
    sf::Sprite m_sprite;
    sf::Vector2f m_position;
    sf::Vector2i m_gridPosition;
    bool m_active;
    bool m_hasTexture;
};