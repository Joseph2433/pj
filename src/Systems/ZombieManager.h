#pragma once

#include <vector>
#include <memory> // For std::unique_ptr
#include <SFML/System.hpp>

// 前向声明
namespace sf
{
    class RenderWindow;
}
class Zombie; // 僵尸基类
class ResourceManager;
class Grid; // ZombieManager 可能需要 Grid 来确定僵尸的行或生成位置

// 僵尸类型枚举 (如果需要区分生成不同类型的僵尸)
enum class ZombieType
{
    BASIC
    // CONEHEAD, BUCKETHEAD, etc.
};

class ZombieManager
{
public:
    // 构造函数可能需要 ResourceManager (创建僵尸时传递) 和 Grid (确定生成位置/行)
    ZombieManager(ResourceManager &resManager, Grid &grid);
    ~ZombieManager();

    // 在指定行生成一个指定类型的僵尸 (当前只实现普通僵尸)
    // row: 要在哪一行生成僵尸 (0 到 GRID_ROWS-1)
    // type: 要生成的僵尸类型 (当前只支持 BASIC)
    void spawnZombie(int row, ZombieType type = ZombieType::BASIC);

    // 更新所有活动僵尸的状态
    // dt: 帧间隔时间
    // window: 用于边界检测 (例如，僵尸是否到达屏幕左侧)
    void update(float dt, const sf::RenderWindow &window);

    // 绘制所有活动僵尸
    void draw(sf::RenderWindow &window);

    // 清除所有僵尸 (例如，用于游戏重置或关卡结束)
    void clear();

    // 获取所有活动僵尸的列表 (主要用于碰撞检测)
    // 返回原始指针的向量，因为碰撞检测方通常不拥有这些僵尸
    std::vector<Zombie *> getActiveZombies();
    // 或者返回常量引用，如果外部只需要读取信息
    // const std::vector<std::unique_ptr<Zombie>>& getAllZombies() const;

private:
    std::vector<std::unique_ptr<Zombie>> m_zombies; // 存储所有活动僵尸

    ResourceManager &m_resourceManagerRef; // 资源管理器引用
    Grid &m_gridRef;                       // 网格系统引用
};
