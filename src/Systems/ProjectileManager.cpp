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

void ProjectileManager::firePea(const sf::Vector2f &startPosition, const sf::Vector2f &direction)
{
    // 创建一个 Pea 对象，并将其作为 Projectile 指针添加到 m_projectiles 向量中
    m_projectiles.emplace_back(std::make_unique<Pea>(m_resourceManagerRef, startPosition, direction));
    std::cout << "ProjectileManager: fire." << std::endl;
}

// void ProjectileManager::fireIcePea(...) {
//     m_projectiles.emplace_back(std::make_unique<IcePea>(...));
// }

void ProjectileManager::update(float dt, const sf::RenderWindow &window /*, zombies */)
{
    // std::cout << "[ProjectileManager] DEBUG: Update START. Num projectiles: " << m_projectiles.size() << std::endl;

    // 1. 更新所有子弹
    for (size_t i = 0; i < m_projectiles.size(); ++i)
    {
        if (m_projectiles[i])
        { // 确保指针有效
            // std::cout << "[ProjectileManager] DEBUG: Updating projectile Addr: " << m_projectiles[i].get() << std::endl;
            m_projectiles[i]->update(dt);
        }
    }

    // 2. (未来) 碰撞检测：如果碰撞，调用 projectile->onHit();

    // 3. 移除无效的子弹 (飞出屏幕、已击中、生命周期结束)
    // std::cout << "[ProjectileManager] DEBUG: Checking for removal. Current count: " << m_projectiles.size() << std::endl;

    // 使用 erase-remove idiom
    // lambda 表达式现在只依赖 isOutOfValidArea，该方法内部会检查 hasHit
    auto it = std::remove_if(m_projectiles.begin(), m_projectiles.end(),
                             [&window](const std::unique_ptr<Projectile> &proj_ptr)
                             {
                                 if (!proj_ptr)
                                 { // 以防万一有空指针被加入
                                     std::cerr << "[ProjectileManager] CRITICAL: Encountered nullptr in m_projectiles during removal check!" << std::endl;
                                     return true; // 标记为空指针以便移除
                                 }
                                 bool shouldRemove = proj_ptr->isOutOfValidArea(window);
                                 if (shouldRemove)
                                 {
                                     std::cout << "[ProjectileManager] DEBUG: Marking projectile Addr: " << proj_ptr.get()
                                               << " at (" << proj_ptr->getPosition().x << "," << proj_ptr->getPosition().y << ")"
                                               << " for REMOVAL. (isOutOfValidArea returned true)" << std::endl;
                                 }
                                 return shouldRemove;
                             });

    if (it != m_projectiles.end())
    { // 如果确实有元素被标记为移除
        std::cout << "[ProjectileManager] DEBUG: Erasing " << std::distance(it, m_projectiles.end()) << " projectiles." << std::endl;
        m_projectiles.erase(it, m_projectiles.end());
    }
    // std::cout << "[ProjectileManager] DEBUG: Update END. Num projectiles after removal: " << m_projectiles.size() << std::endl;
}

void ProjectileManager::draw(sf::RenderWindow &window)
{
    for (const auto &projectile : m_projectiles)
    {
        projectile->draw(window); // 调用 Projectile (或其派生类) 的 draw 方法
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
        { // 只返回未击中且有效的子弹
            activeProjectiles.push_back(proj_ptr.get());
        }
    }
    return activeProjectiles;
}
