#pragma once

#include "Entity.h"
#include <SFML/System/Clock.hpp>

class ResourceManager;
class SunManager;

enum class SunSpawnType
{
    FROM_SKY,
    FROM_PLANT
};

class Sun : public Entity
{
public:
    Sun(ResourceManager &resManager, SunManager &sunManager,
        const sf::Vector2f &spawnPosition, // 统一叫 spawnPosition
        SunSpawnType type,
        // 对于天空阳光，targetY 是其最终落地点Y；对于植物阳光，这可能是其运动的相对目标Y或不用
        float skySunTargetYGround = -1.f);

    ~Sun() override = default;

    void update(float dt) override;

    bool handleClick(const sf::Vector2f &mousePos);
    bool isExpired() const;
    int getValue() const;

    bool isCollected() const { return m_collected; }
    void collect();

private:
    void updateSkySun(float dt);   // 天空阳光的更新逻辑
    void updatePlantSun(float dt); // 植物阳光的更新逻辑

    SunManager &m_sunManagerRef;
    int m_value;
    SunSpawnType m_spawnType;

    float m_lifespanTimer; // 对于天空阳光，是在地上的计时；对于植物阳光，是总计时
    bool m_collected;

    // 特定于天空阳光的属性
    bool m_isFallingSkySun;
    float m_skySunTargetY;

    // 特定于植物阳光的属性
    sf::Vector2f m_velocity;          // 植物阳光的当前速度 (x, y)
    sf::Vector2f m_plantSunTargetPos; // 植物阳光的目标停留位置 (相对于初始产生位置)
    bool m_plantSunReachedTarget;     // 植物阳光是否已到达其目标位置
};
