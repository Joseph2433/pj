#pragma once

#include "../Entities/Zombie.h" // 继承自僵尸基类
#include "../Utils/Constants.h"

class ResourceManager;

// 普通僵尸类，是 Zombie 的一个具体实现
class BasicZombie : public Zombie
{
public:
    // 构造函数
    // resManager: 资源管理器引用
    // spawnPosition: 生成位置
    BasicZombie(ResourceManager &resManager, const sf::Vector2f &spawnPosition, Grid &grid); // <--- 添加 Grid& grid 参数
    ~BasicZombie() override = default;
    // BasicZombie 的 update 和 draw 可以直接使用 Zombie 基类的实现，
    // 除非它有非常特殊的行为需要重写。
    // void update(float dt /*, const std::vector<Plant*>& plantsInLane */) override;
};
