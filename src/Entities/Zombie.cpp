#include "Zombie.h"
#include "../Entities/Plant.h"
#include "../Core/ResourceManager.h"
#include "../Utils/Constants.h"
#include "../Systems/Grid.h"
#include <iostream>
#include <limits>
#include <cmath>

Zombie::Zombie(ResourceManager &resManager, const std::string &textureKey,
               const sf::Vector2f &spawnPosition,
               int health, float speed, int damage, float attackInterval,
               Grid &grid)
    : Entity(resManager.getTexture(textureKey)),
      m_health(health),
      m_originalSpeed(speed),
      m_currentSpeed(speed),
      m_damagePerAttack(damage),
      m_attackInterval(attackInterval),
      m_currentState(ZombieState::WALKING), m_stateTimer(0.0f),
      m_currentTargetPlant(nullptr),
      m_gridRef(grid),
      m_isSlowed(false),
      m_slowDurationRemaining(0.0f)
{
    setPosition(spawnPosition);
    sf::FloatRect bounds = getLocalBounds();
    setOrigin(bounds.width / 2.f, bounds.height);
}

void Zombie::update(float dt, const std::vector<Plant *> &plantsInLane)
{
    m_stateTimer += dt;

    if (m_isSlowed)
    {
        m_slowDurationRemaining -= dt;
        if (m_slowDurationRemaining <= 0.f)
        {
            m_isSlowed = false;
            m_currentSpeed = m_originalSpeed;
            m_slowDurationRemaining = 0.f;
            m_sprite.setColor(sf::Color::White);
            std::cout << "Zombie Addr: " << this << " slow effect wore off." << std::endl;
        }
    }

    switch (m_currentState)
    {
    case ZombieState::WALKING:
        m_currentTargetPlant = findTargetPlant(plantsInLane);
        if (m_currentTargetPlant)
        {
            changeState(ZombieState::ATTACKING);

            std::cout << "[Zombie] DEBUG: Addr: " << this << " at (X:" << getPosition().x << ", Y:" << getPosition().y
                      << ") found target Plant Addr: " << m_currentTargetPlant << " (X:"
                      << m_currentTargetPlant->getPosition().x << ", Y:" << m_currentTargetPlant->getPosition().y
                      << "), switching to ATTACKING." << std::endl;
        }
        else
        {
            moveLeft(dt);
        }
        break;

    case ZombieState::ATTACKING:
        if (!m_currentTargetPlant || !m_currentTargetPlant->isAlive())
        {
            m_currentTargetPlant = nullptr;
            changeState(ZombieState::WALKING);
        }
        else
        {
            sf::FloatRect zombieBounds = getGlobalBounds();
            sf::FloatRect plantBounds = m_currentTargetPlant->getGlobalBounds();

            bool stillInAttackPosition =
                (zombieBounds.left <= (plantBounds.left + plantBounds.width + ZOMBIE_ATTACK_RANGE / 2.f)) &&
                (zombieBounds.left >= (plantBounds.left - ZOMBIE_ATTACK_RANGE / 2.f));
            if (!stillInAttackPosition)
            {

                m_currentTargetPlant = nullptr;
                changeState(ZombieState::WALKING);
                break;
            }

            if (m_stateTimer >= m_attackInterval)
            {
                attack(m_currentTargetPlant);
                m_stateTimer = 0.0f;
            }
        }
        break;

    case ZombieState::DYING:
        if (m_stateTimer >= 0.5f)
        {
            changeState(ZombieState::DEAD);
        }
        break;
    case ZombieState::DEAD:

        break;
    }
}

Plant *Zombie::findTargetPlant(const std::vector<Plant *> &plantsInLane)
{

    Plant *target = nullptr;

    float maxPlantXInReach = -std::numeric_limits<float>::max();

    sf::FloatRect zombieBounds = getGlobalBounds();
    float zombieAttackPointX = zombieBounds.left;

    for (Plant *plant : plantsInLane)
    {

        sf::FloatRect plantBounds = plant->getGlobalBounds();
        float plantBodyRightEdge = plantBounds.left + plantBounds.width;
        float plantBodyLeftEdge = plantBounds.left;
        bool isPlantInFrontAndReachable =
            (zombieAttackPointX <= plantBodyRightEdge + ZOMBIE_ATTACK_RANGE / 2.f) &&
            (plantBodyLeftEdge < zombieAttackPointX + ZOMBIE_ATTACK_RANGE);
        if (isPlantInFrontAndReachable)
        {

            if (plantBodyLeftEdge > maxPlantXInReach)
            {
                maxPlantXInReach = plantBodyLeftEdge;
                target = plant;
            }
        }
    }

    return target;
}

void Zombie::attack(Plant *targetPlant)
{
    if (targetPlant && targetPlant->isAlive())
    {
        targetPlant->takeDamage(m_damagePerAttack);
    }
}

void Zombie::moveLeft(float dt)
{
    move(-m_currentSpeed * dt, 0.f);
}

void Zombie::takeDamage(int amount)
{
    if (m_currentState == ZombieState::DYING || m_currentState == ZombieState::DEAD)
        return;
    m_health -= amount;

    if (m_health <= 0)
    {
        m_health = 0;
        changeState(ZombieState::DYING);
    }
}

bool Zombie::isAlive() const { return m_currentState != ZombieState::DEAD; }
bool Zombie::isReadyToBeRemoved() const { return m_currentState == ZombieState::DEAD; }
ZombieState Zombie::getCurrentState() const { return m_currentState; }

void Zombie::changeState(ZombieState newState)
{
    if (m_currentState != newState)
    {
        m_currentState = newState;
        m_stateTimer = 0.0f;
    }
}

int Zombie::getLane() const
{
    const sf::Vector2f cellSize = m_gridRef.getCellSize();
    const sf::Vector2f gridStart = m_gridRef.getGridStartPosition();

    if (cellSize.y <= 0.f)
        return -1;

    float zombieFeetY = getPosition().y;
    if (zombieFeetY < gridStart.y)
        return -1;

    int lane = static_cast<int>((zombieFeetY - gridStart.y - cellSize.y * 0.1f) / cellSize.y);

    if (lane >= 0 && lane < m_gridRef.getRows())
    {
        return lane;
    }
    return -1;
}

void Zombie::applySlow(float duration, float slowFactor)
{
    if (!isAlive())
        return;

    m_isSlowed = true;

    m_slowDurationRemaining = std::max(m_slowDurationRemaining, duration);
    m_currentSpeed = m_originalSpeed * slowFactor;
    m_sprite.setColor(sf::Color(100, 100, 255, 200));

    std::cout << "Zombie Addr: " << this << " slowed. New speed: " << m_currentSpeed
              << ", Duration: " << m_slowDurationRemaining << "s" << std::endl;
}

bool Zombie::isSlowed() const
{
    return m_isSlowed;
}