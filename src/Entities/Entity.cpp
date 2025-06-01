#include "Entity.h"

Entity::Entity()
    : m_health(100), m_active(true)
{
}

void Entity::setPosition(float x, float y)
{
    m_sprite.setPosition(x, y);
}

void Entity::setPosition(const sf::Vector2f &position)
{
    m_sprite.setPosition(position);
}

sf::Vector2f Entity::getPosition() const
{
    return m_sprite.getPosition();
}

void Entity::setTexture(const sf::Texture &texture)
{
    m_sprite.setTexture(texture);
}

sf::FloatRect Entity::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

bool Entity::intersects(const Entity &other) const
{
    return getBounds().intersects(other.getBounds());
}

void Entity::takeDamage(int damage)
{
    m_health -= damage;
    if (m_health < 0)
    {
        m_health = 0;
    }
}