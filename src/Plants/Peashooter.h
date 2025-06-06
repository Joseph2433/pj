#pragma once // 防止头文件被多次包含

#include "../Entities/Plant.h"  // 继承自植物基类
#include "../Utils/Constants.h" // 为了访问豌豆射手相关的常量，如射击间隔

// 前向声明，以避免在头文件中包含完整的定义，减少编译依赖
class ResourceManager;   // 用于加载纹理
class Grid;              // 用于网格逻辑
class PlantManager;      // 植物管理器，向日葵可能通过它请求产生阳光 (豌豆射手可能不需要直接交互)
class ProjectileManager; // 子弹管理器，豌豆射手通过它发射子弹

// 豌豆射手类，继承自 Plant
class Peashooter : public Plant
{
public:
    Peashooter(ResourceManager &resManager,
               const sf::Vector2i &gridPos,
               Grid &gridSystem,
               PlantManager &plantManager,
               ProjectileManager &projectileManager);
    ~Peashooter() override = default;
    void update(float dt) override;

private:
    void shoot();
    bool checkForZombiesInLane() const;
    ProjectileManager &m_projectileManagerRef;
    PlantManager &m_plantManagerRef;
    float m_shootTimer;
    const float m_shootInterval;
};
