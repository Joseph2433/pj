#include "Projectile.h"
#include "../Core/ResourceManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include "Zombie.h"

Projectile::Projectile(ResourceManager &resManager, const std::string &textureKey,
                       const sf::Vector2f &startPosition,
                       const sf::Vector2f &direction,
                       float speed, int damage, float lifespan)
    : Entity(resManager.getTexture(textureKey)),
      m_direction(direction),
      m_speed(speed),
      m_damage(damage),
      m_lifespan(lifespan),
      m_hasHit(false)
{

    setPosition(startPosition);
}

void Projectile::update(float dt)
{
    if (m_hasHit)
        return;

    moveProjectile(dt);

    if (m_lifespan > 0.0f)
    {
        m_lifespan -= dt;
        if (m_lifespan <= 0.0f)
        {
            onHit();
        }
    }
}

void Projectile::moveProjectile(float dt)
{
    // 直线移动
    sf::Vector2f movement = m_direction * m_speed * dt;
    move(movement);
}

int Projectile::getDamage() const
{
    return m_damage;
}

bool Projectile::isOutOfValidArea(const sf::RenderWindow &window) const
{
    // 检查是否已被标记为击中
    if (m_hasHit)
    {
        return true;
    }

    sf::FloatRect bounds = getGlobalBounds();
    sf::Vector2u windowSize = window.getSize();
    bool isOut = false;

    // 检查是否完全飞出边界
    if (bounds.left > static_cast<float>(windowSize.x))
    {
        isOut = true;
    }
    else if (bounds.left + bounds.width < 0.f)
    { // 完全飞出左边界
        isOut = true;
    }
    else if (bounds.top > static_cast<float>(windowSize.y))
    {
        isOut = true;
    }
    else if (bounds.top + bounds.height < 0.f)
    {
        isOut = true;
    }

    return isOut;
}

void Projectile::onHit()
{
    if (!m_hasHit)
    {
        m_hasHit = true;
        std::cout << "[Projectile] DEBUG: Addr: " << this << " - onHit() called. m_hasHit is now true." << std::endl;
    }
}

bool Projectile::hasHit() const
{
    return m_hasHit;
}

const sf::Vector2f &Projectile::getDirection() const
{
    return m_direction;
}

void Projectile::setDirection(const sf::Vector2f &dir)
{
    m_direction = dir;
}

void Projectile::applyPrimaryEffect(Zombie *targetZombie)
{
}