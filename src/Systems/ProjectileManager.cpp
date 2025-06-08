#include "ProjectileManager.h"
#include "../Entities/Projectile.h" // 包含 Projectile.h
#include "../Projectiles/Pea.h"     // 包含 Pea.h，因为我们要创建 Pea 实例
#include "../Core/ResourceManager.h"
#include "../Utils/Constants.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>

ProjectileManager::ProjectileManager(ResourceManager &resManager)
    : m_resourceManagerRef(resManager)
{
}

ProjectileManager::~ProjectileManager() = default;

void ProjectileManager::addProjectile(std::unique_ptr<Projectile> projectile)
{
    if (projectile)
    {
        m_projectiles.emplace_back(std::move(projectile));
        std::cout << "ProjectileManager: Added a projectile. Total: " << m_projectiles.size() << std::endl;
    }
}

void ProjectileManager::update(float dt, const sf::RenderWindow &window)
{

    for (size_t i = 0; i < m_projectiles.size(); ++i)
    {
        if (m_projectiles[i])
        {
            m_projectiles[i]->update(dt);
        }
    }

    auto it = std::remove_if(m_projectiles.begin(), m_projectiles.end(),
                             [&window](const std::unique_ptr<Projectile> &proj_ptr)
                             {
                                 if (!proj_ptr)
                                 {
                                     std::cerr << "[ProjectileManager] CRITICAL: Encountered nullptr in m_projectiles during removal check!" << std::endl;
                                     return true;
                                 }
                                 return proj_ptr->isOutOfValidArea(window);
                             });

    if (it != m_projectiles.end())
    {
        m_projectiles.erase(it, m_projectiles.end());
    }
}
void ProjectileManager::draw(sf::RenderWindow &window)
{
    for (const auto &projectile : m_projectiles)
    {
        projectile->draw(window);
    }
}

void ProjectileManager::clear()
{
    m_projectiles.clear();
}

const std::vector<std::unique_ptr<Projectile>> &ProjectileManager::getAllProjectiles() const
{
    return m_projectiles;
}

std::vector<Projectile *> ProjectileManager::getAllActiveProjectiles()
{
    std::vector<Projectile *> activeProjectiles;
    for (const auto &proj_ptr : m_projectiles)
    {
        if (proj_ptr && !proj_ptr->hasHit())
        {
            activeProjectiles.push_back(proj_ptr.get());
        }
    }
    return activeProjectiles;
}
