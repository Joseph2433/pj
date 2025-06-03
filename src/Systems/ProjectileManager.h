#pragma once

#include <vector>
#include <memory>
#include <SFML/System.hpp>

// 前向声明
namespace sf
{
    class RenderWindow;
}
class Projectile; // <--- 使用 Projectile 基类
class ResourceManager;
// class Zombie;

class ProjectileManager
{
public:
    ProjectileManager(ResourceManager &resManager);
    ~ProjectileManager();

    // 创建并发射一个具体类型的子弹 (这里是豌豆)
    // startPosition: 发射位置
    // direction: 发射方向
    void firePea(const sf::Vector2f &startPosition, const sf::Vector2f &direction = sf::Vector2f(1.f, 0.f));

    // (未来可以添加 fireOtherProjectile(params...) 来发射其他类型的子弹)
    // void fireIcePea(const sf::Vector2f& startPosition, const sf::Vector2f& direction);

    void update(float dt, const sf::RenderWindow &window /*, const std::vector<std::unique_ptr<Zombie>>& zombies */);
    void draw(sf::RenderWindow &window);
    void clear();

    // 返回所有活动子弹的列表 (只读访问，用于碰撞检测等)
    const std::vector<std::unique_ptr<Projectile>> &getAllProjectiles() const;

private:
    // 使用基类指针的 vector 来存储所有类型的子弹
    std::vector<std::unique_ptr<Projectile>> m_projectiles; // <--- 修改为 Projectile

    ResourceManager &m_resourceManagerRef;
};
