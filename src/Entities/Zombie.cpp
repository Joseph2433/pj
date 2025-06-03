#include "Zombie.h"
#include "../Core/ResourceManager.h"
#include "../Utils/Constants.h" // 可能需要一些通用常量
#include <iostream>             // 用于调试

Zombie::Zombie(ResourceManager &resManager, const std::string &textureKey,
               const sf::Vector2f &spawnPosition,
               int health, float speed, int damage)
    : Entity(resManager.getTexture(textureKey)), // 调用 Entity 构造函数
      m_health(health),
      m_speed(speed), // 存储为正值，方向在 moveLeft 中处理
      m_damagePerAttack(damage),
      m_currentState(ZombieState::WALKING), // 初始状态为行走
      m_stateTimer(0.0f)
{

    setPosition(spawnPosition); // 设置初始位置
    // 通常僵尸图片的原点可以设在底部中心，方便与地面和植物对齐
    // centerOrigin(); // 或者更具体的原点设置
    sf::FloatRect bounds = getLocalBounds();
    setOrigin(bounds.width / 2.f, bounds.height); // 示例：原点设为底部中心
}

void Zombie::update(float dt /*, const std::vector<Plant*>& plantsInLane */)
{
    // Entity::update(dt); // 如果 Entity 基类有通用更新逻辑

    m_stateTimer += dt;

    switch (m_currentState)
    {
    case ZombieState::WALKING:
        moveLeft(dt);
        // TODO: 在这里加入索敌逻辑：
        // 检查前方是否有植物 (plantsInLane)
        // 如果有，且距离足够近，则 changeState(ZombieState::ATTACKING);
        // 并重置 m_stateTimer (用于攻击计时)
        break;
    case ZombieState::ATTACKING:
        // TODO: 实现攻击逻辑
        // if (m_stateTimer >= BASIC_ZOMBIE_ATTACK_INTERVAL) {
        //     // attack(targetPlant);
        //     m_stateTimer = 0.0f;
        // }
        // 如果目标植物死亡或移开，则 changeState(ZombieState::WALKING);
        break;
    case ZombieState::DYING:
        // TODO: 播放死亡动画
        // if (m_stateTimer >= DEATH_ANIMATION_DURATION) {
        //     changeState(ZombieState::DEAD);
        // }
        // 当前简单处理：直接标记为 DEAD (如果死亡动画时间为0)
        if (m_stateTimer >= 0.1f)
        { // 假设一个极短的“死亡中”时间
            changeState(ZombieState::DEAD);
        }
        break;
    case ZombieState::DEAD:
        // 已死亡，等待 ZombieManager 移除
        break;
    }
}

void Zombie::moveLeft(float dt)
{
    // 向左移动，所以 X 坐标减少
    move(-m_speed * dt, 0.f); // Entity::move()
}

void Zombie::takeDamage(int amount)
{
    if (m_currentState == ZombieState::DYING || m_currentState == ZombieState::DEAD)
    {
        return; // 已经在死亡或已死，不再受伤害
    }

    m_health -= amount;
    std::cout << "Zombie (Position: " << this << ") suffer " << amount << " DP, Now HP: " << m_health << std::endl;
    if (m_health <= 0)
    {
        m_health = 0;
        changeState(ZombieState::DYING); // 进入死亡状态
        m_stateTimer = 0.0f;             // 重置状态计时器，用于死亡动画
        std::cout << "zombie (position: " << this << ") died, change to DYING status。" << std::endl;
    }
}

bool Zombie::isAlive() const
{
    // 只要不是明确的 DEAD 状态并且生命值大于0 (或者正在DYING也可以认为逻辑上还"活着"直到动画结束)
    return m_currentState != ZombieState::DEAD;
    // 更严格的定义可能是： return m_health > 0 && m_currentState != ZombieState::DYING && m_currentState != ZombieState::DEAD;
}

bool Zombie::isReadyToBeRemoved() const
{
    // 当状态为 DEAD 时，可以被 ZombieManager 移除
    return m_currentState == ZombieState::DEAD;
}

ZombieState Zombie::getCurrentState() const
{
    return m_currentState;
}

void Zombie::changeState(ZombieState newState)
{
    if (m_currentState != newState)
    {
        // std::cout << "僵尸 (地址: " << this << ") 状态从 " << static_cast<int>(m_currentState)
        //           << " 变为 " << static_cast<int>(newState) << std::endl;
        m_currentState = newState;
        m_stateTimer = 0.0f; // 切换状态时重置计时器
    }
}

int Zombie::getLane(float gridCellHeight, float gridStartY) const
{
    if (gridCellHeight <= 0)
        return -1; // 防止除以0
    // 僵尸的Y坐标通常是其脚底（如果原点设在底部中心）
    // 我们需要的是僵尸“身体”主要所在的行
    float zombieCenterY = getPosition().y - getLocalBounds().height / 2.f; // 获取僵尸视觉中心Y
    if (zombieCenterY < gridStartY)
        return -1; // 在网格上方

    int lane = static_cast<int>((zombieCenterY - gridStartY) / gridCellHeight);
    return lane; // 需要确保这个 lane 在有效范围内 (0 到 GRID_ROWS-1)
}
