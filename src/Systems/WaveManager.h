#pragma once

#include "ZombieManager.h"
#include <SFML/System/Clock.hpp>
#include <string>
#include <vector>
#include <random>
#include <chrono>

class ZombieManager;
class Game;

enum class SpawnState
{
    IDLE,
    PREPARING_WAVE,
    NORMAL_SPAWN,
    HUGE_WAVE_ANNOUNCE,
    HUGE_WAVE_SPAWN,
    WAVE_COOLDOWN,
    ALL_WAVES_COMPLETED
};

class WaveManager
{
public:
    WaveManager(ZombieManager &zombieManager, Game &game);

    void update(float dt);
    void start();
    void reset();

    int getCurrentWaveNumber() const;
    SpawnState getCurrentSpawnState() const;
    std::string getCurrentWaveStatusText() const;
    bool isGameInPeacefulPeriod() const;
    float getCurrentWaveProgress() const;
    std::string getWaveProgressLabel() const;

private:
    void transitionToState(SpawnState newState);
    void updateIdleState(float dt);
    void updatePreparingWaveState(float dt);
    void updateNormalSpawnState(float dt);
    void updateHugeWaveAnnounceState(float dt);
    void updateHugeWaveSpawnState(float dt);
    void updateWaveCooldownState(float dt);
    void updateAllWavesCompletedState(float dt);

    void prepareNextWaveLogic();
    void spawnZombiesForNormalWave();
    void spawnZombiesForHugeWave();

    ZombieManager &m_zombieManagerRef;
    Game &m_gameRef;

    SpawnState m_currentSpawnState;
    int m_currentWaveNumber;

    sf::Clock m_stateTimer;
    sf::Clock m_spawnIntervalTimer;
    float m_nextNormalSpawnTime;

    // 初始和平期
    float m_initialPeaceDuration;

    // 波次准备期/宣告期
    float m_wavePrepareDuration;
    float m_hugeWaveAnnounceDuration;

    // 普通波次
    int m_normalWave_targetZombiesToSpawn;
    int m_normalWave_zombiesSpawnedThisWave;
    int m_normalWave_minLanes;
    int m_normalWave_maxLanes;
    int m_normalWave_minZombiesPerSpawnEvent;
    int m_normalWave_maxZombiesPerSpawnEvent;
    float m_normalWave_spawnIntervalMin;
    float m_normalWave_spawnIntervalMax;

    // 大规模波次
    int m_hugeWave_zombiesPerLaneMin;
    int m_hugeWave_zombiesPerLaneMax;
    bool m_hugeWave_spawnedThisCycle;
    int m_hugeWaveFrequency;
    int m_wavesSinceLastHugeWave;

    // 波次冷却/间歇期
    float m_waveCooldownDuration;
    float m_minZombiesOnScreenToEndCooldown;

    // 随机数
    ZombieType getRandomZombieTypeForCurrentWave() const;
    std::mt19937 m_rng;
};
