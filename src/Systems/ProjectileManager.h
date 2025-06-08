#pragma once

#include <vector>
#include <memory>
#include <SFML/System.hpp>

namespace sf
{
    class RenderWindow;
}
class Projectile;
class ResourceManager;

class ProjectileManager
{
public:
    ProjectileManager(ResourceManager &resManager);
    ~ProjectileManager();
    void addProjectile(std::unique_ptr<Projectile> projectile);
    void update(float dt, const sf::RenderWindow &window);
    void draw(sf::RenderWindow &window);
    void clear();
    std::vector<Projectile *> getAllActiveProjectiles();

    const std::vector<std::unique_ptr<Projectile>> &getAllProjectiles() const;

private:
    std::vector<std::unique_ptr<Projectile>> m_projectiles;
    ResourceManager &m_resourceManagerRef;
};
