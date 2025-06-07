#pragma once

#include "Entity.h"
#include <SFML/System.hpp>
#include "Zombie.h"

class ResourceManager;
class Zombie;

class Projectile : public Entity
{
public:
    Projectile(ResourceManager &resManager, const std::string &textureKey,
               const sf::Vector2f &startPosition,
               const sf::Vector2f &direction,
               float speed, int damage, float lifespan = -1.f);
    ~Projectile() override = default;
    void update(float dt) override;
    int getDamage() const;
    virtual bool isOutOfValidArea(const sf::RenderWindow &window) const;
    virtual void onHit();
    virtual void applyPrimaryEffect(Zombie *targetZombie);
    bool hasHit() const;
    const sf::Vector2f &getDirection() const;
    void setDirection(const sf::Vector2f &dir);

protected:
    sf::Vector2f m_direction;
    float m_speed;
    int m_damage;
    float m_lifespan;
    bool m_hasHit;
    virtual void moveProjectile(float dt);
};
