#include "Zombie.h"
#include "../Entities/Plant.h" // 包含 Plant.h
#include "../Core/ResourceManager.h"
#include "../Utils/Constants.h" // 包含 Constants.h
#include "../Systems/Grid.h"    // <--- 包含 Grid.h
#include <iostream>
#include <limits> // for std::numeric_limits
#include <cmath>  // for std::abs

Zombie::Zombie(ResourceManager &resManager, const std::string &textureKey,
               const sf::Vector2f &spawnPosition,
               int health, float speed, int damage, float attackInterval,
               Grid &grid) // <--- 接收 Grid 引用
    : Entity(resManager.getTexture(textureKey)),
      m_health(health), m_speed(speed), m_damagePerAttack(damage),
      m_attackInterval(attackInterval),
      m_currentState(ZombieState::WALKING), m_stateTimer(0.0f),
      m_currentTargetPlant(nullptr),
      m_gridRef(grid)
{ // <--- 初始化 Grid 引用
    setPosition(spawnPosition);
    sf::FloatRect bounds = getLocalBounds();      // 确保 Entity::getLocalBounds() 已实现
    setOrigin(bounds.width / 2.f, bounds.height); // 原点设为底部中心
    // std::cout << "[Zombie] DEBUG: Constructor - Addr: " << this << " HP: " << m_health
    //           << " Damage: " << m_damagePerAttack << " AttackInterval: " << m_attackInterval << std::endl;
}

void Zombie::update(float dt, const std::vector<Plant *> &plantsInLane)
{
    m_stateTimer += dt;

    switch (m_currentState)
    {
    case ZombieState::WALKING:
        m_currentTargetPlant = findTargetPlant(plantsInLane); // 在已筛选的同路植物中寻找目标
        if (m_currentTargetPlant)
        {                                        // 如果找到植物
            changeState(ZombieState::ATTACKING); // 切换到攻击状态，本帧不再移动
            // 第一次攻击的计时器处理：
            // m_stateTimer = 0.0f; // 从0开始计时，第一次攻击会有完整间隔
            // 或者 m_stateTimer = m_attackInterval; // 如果希望第一次相遇就立即攻击 (不常见)
            std::cout << "[Zombie] DEBUG: Addr: " << this << " at (X:" << getPosition().x << ", Y:" << getPosition().y
                      << ") found target Plant Addr: " << m_currentTargetPlant << " (X:"
                      << m_currentTargetPlant->getPosition().x << ", Y:" << m_currentTargetPlant->getPosition().y
                      << "), switching to ATTACKING." << std::endl;
        }
        else
        {
            moveLeft(dt); // 没有目标，继续前进
        }
        break;

    case ZombieState::ATTACKING:
        if (!m_currentTargetPlant || !m_currentTargetPlant->isAlive())
        { // 如果目标丢失或死亡
            m_currentTargetPlant = nullptr;
            changeState(ZombieState::WALKING); // 返回行走状态
            // std::cout << "[Zombie] DEBUG: Addr: " << this << " 目标丢失/死亡，返回行走状态。" << std::endl;
        }
        else
        {
            // 确认是否仍在攻击位置
            sf::FloatRect zombieBounds = getGlobalBounds();
            sf::FloatRect plantBounds = m_currentTargetPlant->getGlobalBounds();

            // 僵尸的“攻击点”是其最左侧边缘。植物的“被攻击体”是其整个宽度。
            // 条件：僵尸的左边缘已经非常接近或接触到植物的右边缘，并且不能离植物的左边缘太远。
            bool stillInAttackPosition =
                (zombieBounds.left <= (plantBounds.left + plantBounds.width + ZOMBIE_ATTACK_RANGE / 2.f)) && // 僵尸头可以够到植物右边一点
                (zombieBounds.left >= (plantBounds.left - ZOMBIE_ATTACK_RANGE / 2.f));                       // 僵尸头不能离植物左边太远

            if (!stillInAttackPosition)
            {
                // std::cout << "[Zombie] DEBUG: Addr: " << this << " 与目标植物不再接触，尝试重新索敌。" << std::endl;
                m_currentTargetPlant = nullptr; // 清除目标，下一帧会重新索敌
                changeState(ZombieState::WALKING);
                break;
            }

            if (m_stateTimer >= m_attackInterval)
            {                                 // 如果攻击冷却完毕
                attack(m_currentTargetPlant); // 调用 attack 方法
                m_stateTimer = 0.0f;          // 重置攻击计时器
            }
        }
        break;

    case ZombieState::DYING:
        // TODO: 播放死亡动画，动画结束后切换到 DEAD
        if (m_stateTimer >= 0.5f)
        { // 假设死亡动画持续0.5秒 (应使用常量)
            changeState(ZombieState::DEAD);
        }
        break;
    case ZombieState::DEAD:
        // 等待被 ZombieManager 移除
        break;
    }
}

// 在已筛选的、同一行的植物中寻找攻击目标
Plant *Zombie::findTargetPlant(const std::vector<Plant *> &plantsInLane)
{
    // std::cout << "[Zombie] DEBUG: findTargetPlant for Zombie Addr: " << this
    //           << " at X: " << getPosition().x
    //           << ". Plants in lane passed: " << plantsInLane.size() << std::endl;

    Plant *target = nullptr;
    // 对于向左移动的僵尸，它会攻击它前方（X坐标比它小，但不能太小）中，X坐标最大的那个（最先遇到的）
    float maxPlantXInReach = -std::numeric_limits<float>::max();

    sf::FloatRect zombieBounds = getGlobalBounds();
    float zombieAttackPointX = zombieBounds.left; // 僵尸的攻击点（最左端）

    for (Plant *plant : plantsInLane)
    { // plantsInLane 已经是过滤筛选过的、与当前僵尸同一行的植物
        // if (!plant || !plant->isAlive()) continue; // GamePlayState在构建plantsInLane时应该已经过滤了isAlive

        sf::FloatRect plantBounds = plant->getGlobalBounds();
        float plantBodyRightEdge = plantBounds.left + plantBounds.width;
        float plantBodyLeftEdge = plantBounds.left;

        // std::cout << "[Zombie] DEBUG:   Checking Plant Addr: " << plant
        //           << " at X_left: " << plantBodyLeftEdge << ", X_right: " << plantBodyRightEdge << std::endl;

        // 条件1: 植物在僵尸的前方或僵尸已经接触到它。
        // 僵尸的攻击点 (zombieAttackPointX) 应该在植物身体的某个范围内。
        // 僵尸的左边缘 <= 植物的右边缘 (还没完全走过植物)
        // 并且 植物的左边缘 < 僵尸的左边缘 + 攻击范围 (植物在僵尸的攻击范围内)
        bool isPlantInFrontAndReachable =
            (zombieAttackPointX <= plantBodyRightEdge + ZOMBIE_ATTACK_RANGE / 2.f) && // 僵尸左边缘可以够到植物右边缘一点
            (plantBodyLeftEdge < zombieAttackPointX + ZOMBIE_ATTACK_RANGE);           // 植物的左边缘在僵尸攻击范围内

        if (isPlantInFrontAndReachable)
        {
            // std::cout << "[Zombie] DEBUG:     Plant Addr: " << plant << " is potentially reachable." << std::endl;
            // 在所有可攻击的植物中，选择X坐标最大的那个（即僵尸最先遇到的）
            if (plantBodyLeftEdge > maxPlantXInReach)
            {
                maxPlantXInReach = plantBodyLeftEdge;
                target = plant;
                // std::cout << "[Zombie] DEBUG:       New closest target set: Plant Addr: " << plant << std::endl;
            }
        }
    }

    // if (target) {
    //     std::cout << "[Zombie] DEBUG: Zombie Addr: " << this << " FINALLY TARGETED Plant Addr: " << target
    //               << " at X: " << target->getGlobalBounds().left << std::endl;
    // } else {
    //     // std::cout << "[Zombie] DEBUG: Zombie Addr: " << this << " found NO target plant." << std::endl;
    // }
    return target;
}

void Zombie::attack(Plant *targetPlant)
{
    if (targetPlant && targetPlant->isAlive())
    {
        // std::cout << "[Zombie] DEBUG: Addr: " << this << " 攻击植物 (Addr: " << targetPlant
        //           << ", HP before: " << targetPlant->getHealth() << ") 造成 " << m_damagePerAttack << " 伤害。" << std::endl;
        targetPlant->takeDamage(m_damagePerAttack);
    }
}

void Zombie::moveLeft(float dt)
{
    move(-m_speed * dt, 0.f);
}

void Zombie::takeDamage(int amount)
{
    if (m_currentState == ZombieState::DYING || m_currentState == ZombieState::DEAD)
        return;
    m_health -= amount;
    // std::cout << "[Zombie] DEBUG: Addr: " << this << " 受到 " << amount << " 点伤害, 剩余HP: " << m_health << std::endl;
    if (m_health <= 0)
    {
        m_health = 0;
        changeState(ZombieState::DYING);
        // std::cout << "[Zombie] DEBUG: Addr: " << this << " 已死亡，进入 DYING 状态。" << std::endl;
    }
}

bool Zombie::isAlive() const { return m_currentState != ZombieState::DEAD; }
bool Zombie::isReadyToBeRemoved() const { return m_currentState == ZombieState::DEAD; }
ZombieState Zombie::getCurrentState() const { return m_currentState; }

void Zombie::changeState(ZombieState newState)
{
    if (m_currentState != newState)
    {
        // std::cout << "[Zombie] DEBUG: Addr: " << this
        //           << " at (X:" << getPosition().x << ", Y:" << getPosition().y << ")"
        //           << " STATE CHANGE: "
        //           << static_cast<int>(m_currentState) << " -> " << static_cast<int>(newState) << std::endl;
        m_currentState = newState;
        m_stateTimer = 0.0f;
    }
}

// 使用 m_gridRef 来获取网格参数
int Zombie::getLane() const
{
    const sf::Vector2f cellSize = m_gridRef.getCellSize();
    const sf::Vector2f gridStart = m_gridRef.getGridStartPosition();

    if (cellSize.y <= 0.f)
        return -1;

    float zombieFeetY = getPosition().y; // 原点在底部中心
    if (zombieFeetY < gridStart.y)
        return -1; // 在网格上方

    // 调整一点以避免边界问题
    int lane = static_cast<int>((zombieFeetY - gridStart.y - cellSize.y * 0.1f) / cellSize.y);

    if (lane >= 0 && lane < m_gridRef.getRows())
    { // 使用 m_gridRef.getRows()
        return lane;
    }
    return -1; // 不在有效行内
}
