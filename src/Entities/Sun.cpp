#include "Sun.h"
#include "../Core/ResourceManager.h"
#include "../Systems/SunManager.h"
#include "../Utils/Constants.h"
#include <iostream>
#include <cmath>

Sun::Sun(ResourceManager &resManager, SunManager &sunManager,
         const sf::Vector2f &spawnPosition,
         SunSpawnType type, float skySunTargetYGround)
    : Entity(resManager.getTexture(SUN_TEXTURE_KEY)),
      m_sunManagerRef(sunManager),
      m_value(SUN_VALUE_DEFAULT),
      m_spawnType(type),
      m_lifespanTimer(0.f),
      m_collected(false),
      m_isFallingSkySun(false), m_skySunTargetY(0.f),
      m_velocity(0.f, 0.f), m_plantSunReachedTarget(false)
{
    setPosition(spawnPosition);
    centerOrigin();

    if (m_spawnType == SunSpawnType::FROM_SKY)
    {
        m_isFallingSkySun = true;
        m_lifespanTimer = SKY_SUN_LIFESPAN_ON_GROUND;
        if (skySunTargetYGround != -1.f)
        {
            m_skySunTargetY = skySunTargetYGround;
        }
        else
        {

            m_skySunTargetY = getPosition().y + 300.f;
        }

        if (m_skySunTargetY <= getPosition().y)
            m_skySunTargetY = getPosition().y + 50.f;
        std::cout << "Skysun Y=" << getPosition().y << ", targetY=" << m_skySunTargetY << std::endl;
    }
    else
    {
        m_lifespanTimer = PLANT_SUN_LIFESPAN;
        m_isFallingSkySun = false;

        m_velocity.y = PLANT_SUN_SPAWN_VELOCITY_Y;

        float xOffset = (static_cast<float>(rand()) / RAND_MAX) * 2.f - 1.f;
        m_velocity.x = xOffset * PLANT_SUN_SPAWN_VELOCITY_X_MAX_OFFSET;

        m_plantSunTargetPos = sf::Vector2f(
            getPosition().x + m_velocity.x * 0.5f,
            getPosition().y + PLANT_SUN_TARGET_Y_OFFSET);
        m_plantSunReachedTarget = false;
        std::cout << "Plantsun (" << getPosition().x << "," << getPosition().y
                  << "), target (" << m_plantSunTargetPos.x << "," << m_plantSunTargetPos.y << ")" << std::endl;
    }
}

void Sun::update(float dt)
{
    if (m_collected)
        return;

    if (m_spawnType == SunSpawnType::FROM_SKY)
    {
        updateSkySun(dt);
    }
    else
    {
        updatePlantSun(dt);
    }

    if (m_spawnType == SunSpawnType::FROM_PLANT)
    {
        m_lifespanTimer -= dt;
    }
}

void Sun::updateSkySun(float dt)
{
    if (m_isFallingSkySun)
    {
        sf::Vector2f currentPos = getPosition();
        currentPos.y += SKY_SUN_FALL_SPEED * dt;
        if (currentPos.y >= m_skySunTargetY)
        {
            currentPos.y = m_skySunTargetY;
            m_isFallingSkySun = false;
            m_lifespanTimer = SKY_SUN_LIFESPAN_ON_GROUND;
            std::cout << "Skysun fall。" << std::endl;
        }
        setPosition(currentPos);
    }
    else if (!m_collected)
    {
        m_lifespanTimer -= dt;
    }
}

void Sun::updatePlantSun(float dt)
{
    if (!m_plantSunReachedTarget && !m_collected)
    {
        // 模拟抛物线运动
        m_velocity.y += PLANT_SUN_GRAVITY * dt;
        move(m_velocity * dt);

        if (m_velocity.y > 0 && getPosition().y >= m_plantSunTargetPos.y)
        {
            setPosition(m_plantSunTargetPos);
            m_plantSunReachedTarget = true;
            m_velocity = sf::Vector2f(0, 0);
            std::cout << "Plantsun arrived target site" << std::endl;
        }
    }
}

bool Sun::handleClick(const sf::Vector2f &mousePos)
{
    if (m_collected || isExpired())
        return false;

    if (getGlobalBounds().contains(mousePos))
    {
        return true;
    }
    return false;
}

void Sun::collect()
{
    if (!m_collected)
    {
        m_collected = true;
        m_sunManagerRef.addSun(m_value);
        std::cout << "sun (" << m_value << ") total suns: " << m_sunManagerRef.getCurrentSun() << std::endl;
        // 消失
        setScale(0, 0);
    }
}

bool Sun::isExpired() const
{

    if (m_collected)
        return true;

    if (m_spawnType == SunSpawnType::FROM_SKY)
    {
        return !m_isFallingSkySun && m_lifespanTimer <= 0.f;
    }
    else
    {
        return m_lifespanTimer <= 0.f;
    }
}

int Sun::getValue() const
{
    return m_value;
}
