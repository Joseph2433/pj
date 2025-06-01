#include "Entity.h"
#include "../Utils/Constants.h"

Entity::Entity()
    : m_position(0.0f, 0.0f), m_gridPosition(-1, -1), m_active(true), m_hasTexture(false)
{
}

Entity::Entity(float x, float y)
    : m_position(x, y), m_gridPosition(-1, -1), m_active(true), m_hasTexture(false)
{
    m_sprite.setPosition(m_position);
}

void Entity::setPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
    m_sprite.setPosition(m_position);
}

void Entity::setPosition(const sf::Vector2f &position)
{
    m_position = position;
    m_sprite.setPosition(m_position);
}

sf::Vector2f Entity::getPosition() const
{
    return m_position;
}

void Entity::setGridPosition(int row, int col)
{
    if (row >= 0 && row < GRID_ROWS && col >= 0 && col < GRID_COLS)
    {
        m_gridPosition.x = row;
        m_gridPosition.y = col;

        // 根据网格位置设置世界坐标
        sf::Vector2f worldPos = getWorldPositionFromGrid(row, col);
        setPosition(worldPos);
    }
}

sf::Vector2i Entity::getGridPosition() const
{
    return m_gridPosition;
}

sf::Vector2f Entity::getWorldPositionFromGrid(int row, int col) const
{
    float x = GRID_START_X + col * GRID_WIDTH + GRID_WIDTH / 2;
    float y = GRID_START_Y + row * GRID_HEIGHT + GRID_HEIGHT / 2;
    return sf::Vector2f(x, y);
}

void Entity::setTexture(const sf::Texture &texture)
{
    m_sprite.setTexture(texture);
    m_hasTexture = true;

    // 设置精灵的原点为中心
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2, bounds.height / 2);
}

sf::Sprite &Entity::getSprite()
{
    return m_sprite;
}

const sf::Sprite &Entity::getSprite() const
{
    return m_sprite;
}

sf::FloatRect Entity::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

bool Entity::intersects(const Entity &other) const
{
    return getBounds().intersects(other.getBounds());
}

bool Entity::isActive() const
{
    return m_active;
}

void Entity::setActive(bool active)
{
    m_active = active;
}