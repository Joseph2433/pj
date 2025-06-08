#include "IcePea.h"
#include "../Core/ResourceManager.h"
#include "../Utils/Constants.h"
#include "../Entities/Zombie.h"
#include <iostream>

IcePea::IcePea(ResourceManager &resManager,
               const sf::Vector2f &startPosition,
               const sf::Vector2f &direction)
    : Projectile(resManager,
                 ICE_PEA_TEXTURE_KEY,
                 startPosition,
                 direction,
                 ICE_PEA_SPEED,
                 ICE_PEA_DAMAGE,
                 ICE_PEA_LIFESPAN_SECONDS)
{
    centerOrigin();
    std::cout << "IcePea created." << std::endl;
}

void IcePea::applyPrimaryEffect(Zombie *hitZombie)
{
    Projectile::applyPrimaryEffect(hitZombie);

    if (hitZombie && hitZombie->isAlive())
    {
        std::cout << "IcePea applying SLOW effect to Zombie Addr: " << hitZombie << std::endl;
        hitZombie->applySlow(ZOMBIE_SLOW_DURATION, ZOMBIE_SLOW_FACTOR);
    }
}
