#pragma once

#include "Entity.h"
#include <string>
#include <vector>
#include <SFML/System/Clock.hpp>

class ResourceManager;
class Plant;
class Grid;

enum class ZombieState
{
    WALKING,
    ATTACKING,
    DYING,
    DEAD
};

class Zombie : public Entity
{
public:
    Zombie(ResourceManager &resManager, const std::string &textureKey,
           const sf::Vector2f &spawnPosition,
           int health, float speed, int damage,
           float attackInterval,
           Grid &grid);

    ~Zombie() override = default;
    virtual void update(float dt, const std::vector<Plant *> &plantsInLane);
    virtual void takeDamage(int amount);

    // --- 状态查询 ---
    bool isAlive() const;
    bool isReadyToBeRemoved() const;
    ZombieState getCurrentState() const;

    // --- 状态管理 ---
    virtual void changeState(ZombieState newState);

    // --- 位置---
    int getLane() const;
    int getHealth() const { return m_health; }

    void applySlow(float duration, float slowFactor);
    bool isSlowed() const;

protected:
    int m_health;
    float m_originalSpeed;
    float m_currentSpeed;
    int m_damagePerAttack;
    float m_attackInterval;

    ZombieState m_currentState;
    float m_stateTimer;
    Plant *m_currentTargetPlant;
    Grid &m_gridRef; // <--- 新增: 对 Grid 的引用，用于 getLane

    // --- 行为辅助方法 ---
    virtual void moveLeft(float dt);
    Plant *findTargetPlant(const std::vector<Plant *> &plantsInLane);
    virtual void attack(Plant *targetPlant);
    bool m_isSlowed;
    float m_slowDurationRemaining;
};
