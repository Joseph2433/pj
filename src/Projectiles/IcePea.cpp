#include "IcePea.h"
#include "../Core/ResourceManager.h"
#include "../Utils/Constants.h" // 需要 ZOMBIE_SLOW_DURATION, ZOMBIE_SLOW_FACTOR
#include "../Entities/Zombie.h" // 需要完整的 Zombie 定义来调用 applySlow
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
    // 可选：设置寒冰子弹的特殊外观，如颜色
    // m_sprite.setColor(sf::Color(173, 216, 230)); // Light blue
    centerOrigin();
    std::cout << "IcePea created." << std::endl;
}

void IcePea::applyPrimaryEffect(Zombie *hitZombie)
{
    Projectile::applyPrimaryEffect(hitZombie); // 调用基类方法（如果它有通用逻辑）

    if (hitZombie && hitZombie->isAlive())
    { // 确保僵尸有效且存活
        std::cout << "IcePea applying SLOW effect to Zombie Addr: " << hitZombie << std::endl;
        hitZombie->applySlow(ZOMBIE_SLOW_DURATION, ZOMBIE_SLOW_FACTOR); // 调用僵尸的减速方法
    }
}
