#include "Pea.h"
#include "../Core/ResourceManager.h" // 确保包含
#include "../Utils/Constants.h"
#include <iostream>

Pea::Pea(ResourceManager &resManager, const sf::Vector2f &startPosition, const sf::Vector2f &direction)
    : Projectile(resManager,      // 资源管理器
                 PEA_TEXTURE_KEY, // 纹理键名 (来自 Constants.h)
                 startPosition,   // 起始位置
                 direction,       // 飞行方向
                 PEA_SPEED,       // 飞行速度 (来自 Constants.h)
                 PEA_DAMAGE,      // 伤害值 (来自 Constants.h)
                 -1.f)
{
    // 生命周期 (-1 表示无限，直到出界或击中)
    // Pea 特有的初始化代码（如果需要）
    // 例如，如果豌豆有特定的视觉效果或动画，可以在这里设置
    // Projectile 的构造函数已经设置了位置和纹理，Entity 的 centerOrigin() 也可以在那里调用
    // 如果希望豌豆从豌豆射手的“嘴部”精确发出，可以在 Peashooter::shoot() 中调整 startPosition
    // 或者在这里根据豌豆自身的尺寸和原点进行微调。
    // 例如，如果 Projectile 的 setPosition 设置的是中心点，而豌豆图片的原点希望是左侧：
    // sf::FloatRect bounds = getGlobalBounds();
    // setOrigin(0, bounds.height / 2.f); // 将原点设为左中
}

// 如果 Pea 没有特殊的更新逻辑，可以不重写 update，它会自动使用 Projectile::update()
// void Pea::update(float dt) {
//     Projectile::update(dt); // 调用基类更新
//     // Pea 特有的更新逻辑，例如旋转、特殊效果等
// }
