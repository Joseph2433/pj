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
    // 构造函数
    // resManager: 资源管理器引用
    // gridPos: 在网格中的位置 (行, 列)
    // gridSystem: 网格系统引用
    // projectileManager: 子弹管理器引用，用于发射子弹
    // (可选) plantManager: 如果豌豆射手需要与植物管理器交互（例如，检查前方是否有其他植物），可以添加
    Peashooter(ResourceManager &resManager,
               const sf::Vector2i &gridPos,
               Grid &gridSystem,
               PlantManager &plantManager,
               ProjectileManager &projectileManager
               /* PlantManager* ownerManager = nullptr */); // PlantManager 可以是可选的或通过其他方式获取

    // 默认析构函数，因为 Plant 基类应该有虚析构函数
    ~Peashooter() override = default;

    // 更新豌豆射手的状态 (覆盖 Plant::update)
    // dt: 帧间隔时间
    void update(float dt) override;

    // Plant 基类已经有 draw() 方法，如果豌豆射手没有特殊的绘制需求，则不需要重写

private:
    // 私有辅助方法，用于执行射击动作
    void shoot();
    bool checkForZombiesInLane() const; // 检查前方是否有僵尸

    // --- 成员变量 ---
    ProjectileManager &m_projectileManagerRef; // 对子弹管理器的引用，用于发射豌豆
    PlantManager &m_plantManagerRef;
    float m_shootTimer;          // 射击计时器，记录距离下次射击还有多久
    const float m_shootInterval; // 固定的射击间隔时间 (从 Constants.h 初始化)

    // 可选：用于精确控制子弹发射点的偏移量 (相对于植物的 getPosition())
    // sf::Vector2f m_shootOriginOffset;
};
