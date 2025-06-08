#pragma once

#include <vector>
#include <memory>

class Projectile;
class Zombie;
class Plant;
class ProjectileManager;
class ZombieManager;
class PlantManager;

class CollisionSystem
{
public:
    CollisionSystem();
    ~CollisionSystem() = default;
    void update(ProjectileManager &projectileManager,
                ZombieManager &zombieManager,
                PlantManager &plantManager);

private:
    void checkProjectileZombieCollisions(std::vector<Projectile *> &projectiles,
                                         std::vector<Zombie *> &zombies);

    void checkZombiePlantCollisions(std::vector<Zombie *> &zombies,
                                    std::vector<Plant *> &plants,
                                    float dt);
};
