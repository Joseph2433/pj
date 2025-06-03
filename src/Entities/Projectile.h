#pragma once

#include "Entity.h" // 继承自实体基类
#include <SFML/System.hpp>

class ResourceManager; // 前向声明

// 子弹基类，定义所有子弹的共同行为和属性
class Projectile : public Entity
{
public:
    // 构造函数
    // textureKey: 在 ResourceManager 中的纹理键名
    // startPosition: 子弹的初始发射位置
    // direction: 子弹的初始飞行方向 (单位向量)
    // speed: 子弹的飞行速度
    // damage: 子弹造成的伤害
    // lifespan: 子弹的生命周期 (可选，例如飞行一定时间后消失)
    Projectile(ResourceManager &resManager, const std::string &textureKey,
               const sf::Vector2f &startPosition,
               const sf::Vector2f &direction,
               float speed, int damage, float lifespan = -1.f); // lifespan < 0 表示无限生命周期直到出界或碰撞

    ~Projectile() override = default; // 虚析构函数由 Entity 继承而来，这里可以 default

    // 更新子弹状态 (主要是移动)
    // dt: 帧间隔时间
    void update(float dt) override;
    // Entity 基类已有 draw()

    // 获取子弹造成的伤害值
    int getDamage() const;

    // 检查子弹是否飞出屏幕或生命周期结束
    // window: 当前渲染窗口，用于获取边界
    virtual bool isOutOfValidArea(const sf::RenderWindow &window) const;

    // 标记子弹已击中目标或因其他原因需要被移除
    virtual void onHit();
    bool hasHit() const; // 检查是否已击中

    // 获取飞行方向 (主要用于调试或特殊逻辑)
    const sf::Vector2f &getDirection() const;
    void setDirection(const sf::Vector2f &dir); // 允许改变方向 (例如追踪子弹)

protected:                    // 使用 protected 以便派生类可以访问和修改
    sf::Vector2f m_direction; // 子弹的当前飞行方向 (通常是单位向量)
    float m_speed;            // 子弹的飞行速度 (像素/秒)
    int m_damage;             // 子弹造成的伤害值
    float m_lifespan;         // 剩余生命周期 (秒)，如果为负则表示无限
    bool m_hasHit;            // 标记子弹是否已击中目标

    // 辅助方法，由派生类重写以定义其特定的移动逻辑（如果需要）
    virtual void moveProjectile(float dt);
};
