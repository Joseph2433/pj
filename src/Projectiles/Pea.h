#pragma once

#include "../Entities/Projectile.h" // <--- 继承自 Projectile
#include "../Utils/Constants.h"     // 为了 PEA_SPEED, PEA_DAMAGE, PEA_TEXTURE_KEY

class ResourceManager;

// 豌豆类，是 Projectile 的一个具体实现
class Pea : public Projectile
{
public:
    // 构造函数
    // startPosition: 发射位置
    // direction: 发射方向 (通常是 {1,0} 即向右)
    Pea(ResourceManager &resManager, const sf::Vector2f &startPosition, const sf::Vector2f &direction = sf::Vector2f(1.f, 0.f));
    ~Pea() override = default;

    // Pea 的 update 和 draw 可以直接使用 Projectile 基类的实现，
    // 除非 Pea 有特殊的更新或绘制逻辑。
    // void update(float dt) override; // 如果需要特殊更新逻辑，则重写
    // void draw(sf::RenderWindow& window) const override; // 如果需要特殊绘制，则重写

    // 可以添加 Pea 特有的方法，如果需要的话
};
