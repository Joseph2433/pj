#pragma once

#include <vector>
#include <memory>
#include <SFML/System.hpp>

// 前向声明
namespace sf
{
    class RenderWindow;
}
class Projectile;
class ResourceManager;
// class Zombie;

class ProjectileManager
{
public:
    ProjectileManager(ResourceManager &resManager);
    ~ProjectileManager();
    void addProjectile(std::unique_ptr<Projectile> projectile);
    // void firePea(const sf::Vector2f &startPosition, const sf::Vector2f &direction = sf::Vector2f(1.f, 0.f));
    void update(float dt, const sf::RenderWindow &window);
    void draw(sf::RenderWindow &window);
    void clear();
    std::vector<Projectile *> getAllActiveProjectiles();

    const std::vector<std::unique_ptr<Projectile>> &getAllProjectiles() const;

private:
    std::vector<std::unique_ptr<Projectile>> m_projectiles;
    ResourceManager &m_resourceManagerRef;
};
