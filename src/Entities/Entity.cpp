#include "Entity.h"

// 构造函数
Entity::Entity(const sf::Texture &texture)
{
    m_sprite.setTexture(texture);
    // 默认情况下，精灵图的原点(origin)是其左上角 (0,0)。
    // 你可能希望将其设置为纹理的中心，以便旋转和定位更直观。
    // 例如: centerOrigin(); // 可以在构造后由派生类决定是否调用
}

// 虚更新函数 (默认实现为空)
void Entity::update(float dt)
{
    // 基类实体没有特定的更新逻辑
    // 派生类将重写此方法
    (void)dt; // 如果函数体为空，用于抑制未使用参数的警告
}

// 绘制实体的精灵图
void Entity::draw(sf::RenderWindow &window) const
{
    window.draw(m_sprite);
}

// 位置相关函数
void Entity::setPosition(float x, float y)
{
    m_sprite.setPosition(x, y);
}

void Entity::setPosition(const sf::Vector2f &position)
{
    m_sprite.setPosition(position);
}

const sf::Vector2f &Entity::getPosition() const
{
    return m_sprite.getPosition();
}

// 中心点 (Origin) 相关函数
void Entity::setOrigin(float x, float y)
{
    m_sprite.setOrigin(x, y);
}

void Entity::setOrigin(const sf::Vector2f &origin)
{
    m_sprite.setOrigin(origin);
}

const sf::Vector2f &Entity::getOrigin() const
{
    return m_sprite.getOrigin();
}

void Entity::centerOrigin()
{
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

// 缩放相关函数
void Entity::setScale(float factorX, float factorY)
{
    m_sprite.setScale(factorX, factorY);
}

void Entity::setScale(const sf::Vector2f &factors)
{
    m_sprite.setScale(factors);
}

const sf::Vector2f &Entity::getScale() const
{
    return m_sprite.getScale();
}

// 旋转相关函数
void Entity::setRotation(float angle)
{
    m_sprite.setRotation(angle);
}

float Entity::getRotation() const
{
    return m_sprite.getRotation();
}

// 获取全局包围盒
sf::FloatRect Entity::getGlobalBounds() const
{
    return m_sprite.getGlobalBounds();
}

void Entity::move(float offsetX, float offsetY)
{
    m_sprite.move(offsetX, offsetY);
}

void Entity::move(const sf::Vector2f &offset)
{
    m_sprite.move(offset);
}

// 可选：直接访问精灵图
// sf::Sprite& Entity::getSprite() {
//     return m_sprite;
// }
//
// const sf::Sprite& Entity::getSprite() const {
//     return m_sprite;
// }
