#pragma once
#include <SFML/System/Clock.hpp>
#include <string>
#include <vector>
#include <random>
#include <chrono>

// 前向声明
class ZombieManager;
class Game; // WaveManager 将通过 Game 引用访问 ZombieManager 和其他需要的信息

enum class SpawnState
{
    IDLE,               // 无僵尸生成 (游戏初期或波数间歇)
    PREPARING_WAVE,     // (新增) 波次开始前的短暂准备/提示阶段
    NORMAL_SPAWN,       // 普通生成状态
    HUGE_WAVE_ANNOUNCE, // (新增) 大规模波次宣告阶段
    HUGE_WAVE_SPAWN,    // 大规模生成状态 ("一大波僵尸")
    WAVE_COOLDOWN       // (新增) 当前波次僵尸已生成完毕，等待清理或下一波开始的间歇
};

class WaveManager
{
public:
    WaveManager(ZombieManager &zombieManager, Game &game);

    void update(float dt);
    void start(); // 在游戏开始或状态进入时调用
    void reset(); // 重置为初始状态 (例如，游戏重新开始)

    int getCurrentWaveNumber() const;
    SpawnState getCurrentSpawnState() const;
    std::string getCurrentWaveStatusText() const; // 获取当前波数状态文本 (用于UI/调试)
    bool isGameInPeacefulPeriod() const;
    float getCurrentWaveProgress() const;
    std::string getWaveProgressLabel() const; // 判断是否处于开局和平期或波数完成后的安全期

private:
    void transitionToState(SpawnState newState);
    void updateIdleState(float dt);
    void updatePreparingWaveState(float dt);
    void updateNormalSpawnState(float dt);
    void updateHugeWaveAnnounceState(float dt);
    void updateHugeWaveSpawnState(float dt);
    void updateWaveCooldownState(float dt);

    void prepareNextWaveLogic(); // 决定下一波是普通还是巨大，并设置参数
    void spawnZombiesForNormalWave();
    void spawnZombiesForHugeWave();

    ZombieManager &m_zombieManagerRef;
    Game &m_gameRef; // 用于获取GRID_ROWS等，或未来获取总游戏时间

    SpawnState m_currentSpawnState;
    int m_currentWaveNumber;

    sf::Clock m_stateTimer;         // 当前状态持续时间计时器
    sf::Clock m_spawnIntervalTimer; // 普通生成状态下，僵尸生成的间隔计时器
    float m_nextNormalSpawnTime;    // 下一次普通僵尸生成的具体时间点(基于m_spawnIntervalTimer)

    // 初始和平期
    float m_initialPeaceDuration;

    // 波次准备期/宣告期
    float m_wavePrepareDuration;      // 开始一波前的短暂倒计时/提示
    float m_hugeWaveAnnounceDuration; // “一大波僵尸”宣告的持续时间

    // 普通波次参数
    int m_normalWave_targetZombiesToSpawn; // 这一普通波次计划生成的僵尸总数
    int m_normalWave_zombiesSpawnedThisWave;
    int m_normalWave_minLanes;
    int m_normalWave_maxLanes;
    int m_normalWave_minZombiesPerSpawnEvent; // 每次生成事件，每行最少僵尸
    int m_normalWave_maxZombiesPerSpawnEvent; // 每次生成事件，每行最多僵尸
    float m_normalWave_spawnIntervalMin;
    float m_normalWave_spawnIntervalMax;

    // 大规模波次参数
    int m_hugeWave_zombiesPerLaneMin;
    int m_hugeWave_zombiesPerLaneMax;
    bool m_hugeWave_spawnedThisCycle; // 标记大规模僵尸是否已生成 (防止重复)

    // 波数触发大规模攻击的逻辑
    int m_hugeWaveFrequency;      // 每隔多少“普通”波次触发一次大规模攻击
    int m_wavesSinceLastHugeWave; // 自上次大规模攻击以来经过的普通波数

    // 波次冷却/间歇期
    float m_waveCooldownDuration;
    float m_minZombiesOnScreenToEndCooldown; // 当屏幕上僵尸少于此数量时，冷却期可能提前结束

    std::mt19937 m_rng; // 随机数引擎
};
