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
        const sf::Vector2f &spawnPosition,
        SunSpawnType type,
        float skySunTargetYGround = -1.f);

    ~Sun() override = default;

    void update(float dt) override;

    bool handleClick(const sf::Vector2f &mousePos);
    bool isExpired() const;
    int getValue() const;

    bool isCollected() const { return m_collected; }
    void collect();

private:
    void updateSkySun(float dt);
    void updatePlantSun(float dt);

    SunManager &m_sunManagerRef;
    int m_value;
    SunSpawnType m_spawnType;

    float m_lifespanTimer;
    bool m_collected;

    // 天空阳光
    bool m_isFallingSkySun;
    float m_skySunTargetY;

    // 植物阳光
    sf::Vector2f m_velocity;
    sf::Vector2f m_plantSunTargetPos;
    bool m_plantSunReachedTarget;
};
