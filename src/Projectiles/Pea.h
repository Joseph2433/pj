#pragma once

#include "../Entities/Projectile.h"
#include "../Utils/Constants.h"

class ResourceManager;

class Pea : public Projectile
{
public:
    Pea(ResourceManager &resManager, const sf::Vector2f &startPosition, const sf::Vector2f &direction = sf::Vector2f(1.f, 0.f));
    ~Pea() override = default;
};
