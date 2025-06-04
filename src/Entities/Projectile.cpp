#include "Projectile.h"
#include "../Core/ResourceManager.h"      // 包含资源管理器
#include <SFML/Graphics/RenderWindow.hpp> // 为了 isOutOfValidArea
#include <iostream>                       // 用于调试

Projectile::Projectile(ResourceManager &resManager, const std::string &textureKey,
                       const sf::Vector2f &startPosition,
                       const sf::Vector2f &direction,
                       float speed, int damage, float lifespan)
    : Entity(resManager.getTexture(textureKey)), // 调用 Entity 基类构造函数
      m_direction(direction),
      m_speed(speed),
      m_damage(damage),
      m_lifespan(lifespan),
      m_hasHit(false)
{

    setPosition(startPosition); // 设置初始位置
    // 可选：根据子弹纹理调整原点，例如设置为中心
    // centerOrigin(); // Entity 基类中应有此方法
}

void Projectile::update(float dt)
{
    // Entity::update(dt); // 如果 Entity 基类有通用更新逻辑

    if (m_hasHit)
        return; // 如果已击中，则不进行移动
    // std::cout << "[Projectile] DEBUG: Update START - Pos_before: (" << getPosition().x << "," << getPosition().y << "), dt:" << dt << std::endl;
    moveProjectile(dt); // 调用受保护的移动方法
    // std::cout << "[Projectile] DEBUG: Update END - Pos_after: (" << getPosition().x << "," << getPosition().y << ")" << std::endl;

    if (m_lifespan > 0.0f)
    { // 如果有生命周期限制
        m_lifespan -= dt;
        if (m_lifespan <= 0.0f)
        {
            onHit(); // 生命周期结束，视为已“击中”（以便被移除）
            // std::cout << "子弹生命周期结束." << std::endl;
        }
    }
}

void Projectile::moveProjectile(float dt)
{
    // 默认的直线移动逻辑
    sf::Vector2f movement = m_direction * m_speed * dt;
    move(movement); // 调用 Entity 基类提供的 move 方法
    // std::cout << "[Projectile] DEBUG: moveProjectile - Current Pos: (" << getPosition().x << "," << getPosition().y << "), Calculated Movement: (" << movement.x << "," << movement.y << ")" << std::endl;
}

int Projectile::getDamage() const
{
    return m_damage;
}

bool Projectile::isOutOfValidArea(const sf::RenderWindow &window) const
{
    // 首先检查是否已被标记为击中 (包括生命周期结束的情况，因为它们会调用 onHit)
    if (m_hasHit)
    {
        // std::cout << "[Projectile] DEBUG: Addr: " << this << " - isOutOfValidArea: true (because m_hasHit is true)" << std::endl;
        return true;
    }

    sf::FloatRect bounds = getGlobalBounds();
    sf::Vector2u windowSize = window.getSize();
    bool isOut = false;

    // 检查是否完全飞出窗口边界
    if (bounds.left > static_cast<float>(windowSize.x))
    { // 完全飞出右边界
        isOut = true;
        // std::cout << "[Projectile] DEBUG: Addr: " << this << " - OutOfValidArea: YES (Right boundary). BoundsLeft: " << bounds.left << ", WinWidth: " << windowSize.x << std::endl;
    }
    else if (bounds.left + bounds.width < 0.f)
    { // 完全飞出左边界
        isOut = true;
        // std::cout << "[Projectile] DEBUG: Addr: " << this << " - OutOfValidArea: YES (Left boundary). BoundsLeft+Width: " << bounds.left + bounds.width << std::endl;
    }
    else if (bounds.top > static_cast<float>(windowSize.y))
    { // 完全飞出下边界
        isOut = true;
        // std::cout << "[Projectile] DEBUG: Addr: " << this << " - OutOfValidArea: YES (Bottom boundary). BoundsTop: " << bounds.top << ", WinHeight: " << windowSize.y << std::endl;
    }
    else if (bounds.top + bounds.height < 0.f)
    { // 完全飞出上边界
        isOut = true;
        // std::cout << "[Projectile] DEBUG: Addr: " << this << " - OutOfValidArea: YES (Top boundary). BoundsTop+Height: " << bounds.top + bounds.height << std::endl;
    }

    // 如果出界了，也应该标记为 m_hasHit，以便统一处理移除逻辑
    // 但是 isOutOfValidArea 是 const 方法，不能修改 m_hasHit。
    // 所以，如果出界，ProjectileManager 应该负责调用 onHit() 或者直接移除。
    // 或者，让 isOutOfValidArea 非 const，并在内部设置 m_hasHit，但这不常见。
    // 目前的设计是 ProjectileManager 依赖 isOutOfValidArea() 返回 true 来移除。

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
    // 可能需要归一化方向向量
    // m_direction = normalized(dir);
    m_direction = dir;
}
