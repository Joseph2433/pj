#include "Sun.h"
#include "../Core/ResourceManager.h"
#include "../Systems/SunManager.h"
#include "../Utils/Constants.h"
#include <iostream>
#include <cmath> // For std::abs for plant sun movement

Sun::Sun(ResourceManager &resManager, SunManager &sunManager,
         const sf::Vector2f &spawnPosition,
         SunSpawnType type, float skySunTargetYGround)
    : Entity(resManager.getTexture(SUN_TEXTURE_KEY)),
      m_sunManagerRef(sunManager),
      m_value(SUN_VALUE_DEFAULT), // 默认价值
      m_spawnType(type),
      m_lifespanTimer(0.f), // 根据类型初始化
      m_collected(false),
      m_isFallingSkySun(false), m_skySunTargetY(0.f),      // 天空阳光属性
      m_velocity(0.f, 0.f), m_plantSunReachedTarget(false) // 植物阳光属性
{
    setPosition(spawnPosition);
    centerOrigin();

    if (m_spawnType == SunSpawnType::FROM_SKY)
    {
        m_isFallingSkySun = true;
        m_lifespanTimer = SKY_SUN_LIFESPAN_ON_GROUND; // 这个计时器在落地后启动
        if (skySunTargetYGround != -1.f)
        {
            m_skySunTargetY = skySunTargetYGround;
        }
        else
        {
            // 默认掉落到屏幕下方某个安全位置，你需要根据游戏窗口调整
            m_skySunTargetY = getPosition().y + 300.f; // 示例
        }
        // 确保目标Y在起始Y下方
        if (m_skySunTargetY <= getPosition().y)
            m_skySunTargetY = getPosition().y + 50.f;
        std::cout << "天空阳光创建于 Y=" << getPosition().y << ", 目标Y=" << m_skySunTargetY << std::endl;
    }
    else
    {                                         // FROM_PLANT
        m_lifespanTimer = PLANT_SUN_LIFESPAN; // 植物阳光的总生命周期
        m_isFallingSkySun = false;            // 不是天空下落模式

        // 初始化植物阳光的运动参数 (例如，向上弹跳然后落到植物旁边)
        m_velocity.y = PLANT_SUN_SPAWN_VELOCITY_Y;
        // 水平速度可以给个小的随机偏移
        float xOffset = (static_cast<float>(rand()) / RAND_MAX) * 2.f - 1.f; // -1 to 1
        m_velocity.x = xOffset * PLANT_SUN_SPAWN_VELOCITY_X_MAX_OFFSET;

        // 设定一个相对于初始位置的目标点
        m_plantSunTargetPos = sf::Vector2f(
            getPosition().x + m_velocity.x * 0.5f,      // X方向稍微移动一点
            getPosition().y + PLANT_SUN_TARGET_Y_OFFSET // Y方向停在初始位置下方一点
        );
        m_plantSunReachedTarget = false;
        std::cout << "植物阳光创建于 (" << getPosition().x << "," << getPosition().y
                  << "), 目标 (" << m_plantSunTargetPos.x << "," << m_plantSunTargetPos.y << ")" << std::endl;
    }
}

void Sun::update(float dt)
{
    if (m_collected)
        return; // 已收集，不更新

    if (m_spawnType == SunSpawnType::FROM_SKY)
    {
        updateSkySun(dt);
    }
    else
    { // FROM_PLANT
        updatePlantSun(dt);
    }

    // 所有类型的阳光在未被收集时，生命周期都会减少
    // 对于天空阳光，这个计时是在落地后才有效；对于植物阳光，是总生命周期
    if ((m_spawnType == SunSpawnType::FROM_SKY && !m_isFallingSkySun) ||
        (m_spawnType == SunSpawnType::FROM_PLANT && m_plantSunReachedTarget))
    {
        // 只有当天空阳光落地后，或者植物阳光到达目标后，才开始减少其特定的生命周期计时
    }
    // 或者，植物阳光的总生命周期从一开始就计时，天空阳光的落地后计时是独立的
    if (m_spawnType == SunSpawnType::FROM_PLANT)
    {
        m_lifespanTimer -= dt; // 植物阳光总生命周期从产生就开始计时
    }

    if (m_lifespanTimer <= 0.f)
    {
        // 过期逻辑已移至 isExpired()，这里只更新计时器
    }

    // 可选：旋转动画
    // m_sprite.rotate(30.f * dt);
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
            m_isFallingSkySun = false; // 到达地面，停止下落
            // 天空阳光的生命周期计时器在这里才真正开始计时
            m_lifespanTimer = SKY_SUN_LIFESPAN_ON_GROUND;
            std::cout << "天空阳光降落。" << std::endl;
        }
        setPosition(currentPos);
    }
    else if (!m_collected)
    {                          // 如果在地面上且未被收集
        m_lifespanTimer -= dt; // 减少地面停留时间
    }
}

void Sun::updatePlantSun(float dt)
{
    if (!m_plantSunReachedTarget && !m_collected)
    {
        // 模拟抛物线运动
        m_velocity.y += PLANT_SUN_GRAVITY * dt; // 应用重力
        move(m_velocity * dt);                  // 根据速度移动

        // 简单判断是否接近或越过目标点 (更精确可以用向量判断)
        // 这里用Y坐标作为主要判断依据，假设它会先向上再向下
        if (m_velocity.y > 0 && getPosition().y >= m_plantSunTargetPos.y)
        {
            setPosition(m_plantSunTargetPos); // 精确设置到目标位置
            m_plantSunReachedTarget = true;
            m_velocity = sf::Vector2f(0, 0); // 停止移动
            std::cout << "植物阳光到达目标位置。" << std::endl;
            // 植物阳光的生命周期计时器是从一开始就计算的，所以这里不需要重置
        }
        // 如果X方向也有限制，也需要判断
        // if (std::abs(getPosition().x - m_plantSunTargetPos.x) < 1.0f && ...)
    }
    // 植物阳光的总生命周期 m_lifespanTimer 是在主 update 中处理的
}

bool Sun::handleClick(const sf::Vector2f &mousePos)
{
    if (m_collected || isExpired())
        return false; // 已收集或过期则不能点击

    // 使用精灵的全局包围盒进行点击检测
    // 如果精灵有旋转或不规则形状，可能需要更精确的检测
    if (getGlobalBounds().contains(mousePos))
    {
        // 确保不是在冷却覆盖层上 (如果阳光本身有类似机制的话，目前没有)
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
        std::cout << "阳光 (" << m_value << ") 已收集。总阳光: " << m_sunManagerRef.getCurrentSun() << std::endl;
        // 使其消失 (实际移除由管理器负责)
        setScale(0, 0);
    }
}

bool Sun::isExpired() const
{
    // 对于天空阳光，是落地后的生命周期结束
    // 对于植物阳光，是总生命周期结束
    if (m_collected)
        return true;

    if (m_spawnType == SunSpawnType::FROM_SKY)
    {
        return !m_isFallingSkySun && m_lifespanTimer <= 0.f;
    }
    else
    { // FROM_PLANT
        return m_lifespanTimer <= 0.f;
    }
}

int Sun::getValue() const
{
    return m_value;
}
