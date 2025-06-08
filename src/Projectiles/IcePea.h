#pragma once
#include "../Entities/Projectile.h"

class ResourceManager;
class Zombie;

class IcePea : public Projectile
{
public:
    IcePea(ResourceManager &resManager,
           const sf::Vector2f &startPosition,
           const sf::Vector2f &direction);
    ~IcePea() override = default;

    void applyPrimaryEffect(Zombie *targetZombie) override;
};
