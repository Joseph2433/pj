#include "Pea.h"
#include "../Core/ResourceManager.h"
#include "../Utils/Constants.h"
#include <iostream>

Pea::Pea(ResourceManager &resManager, const sf::Vector2f &startPosition, const sf::Vector2f &direction)
    : Projectile(resManager,
                 PEA_TEXTURE_KEY,
                 startPosition,
                 direction,
                 PEA_SPEED,
                 PEA_DAMAGE,
                 -1.f)
{
}
