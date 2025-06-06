#include "WaveManager.h"
#include "ZombieManager.h"
#include "../Core/Game.h"
#include "../Utils/Constants.h" // 包含 GRID_ROWS 等
#include <iostream>
#include <cstdlib>   // For rand(), RAND_MAX
#include <algorithm> // For std::min, std::max, std::random_shuffle
#include <vector>    // For std::vector used in spawnZombiesForNormalWave

// 辅助函数：生成指定范围内的随机浮点数
static float randomFloat(float min, float max)
{
    if (min >= max)
        return min;
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

// 辅助函数：生成指定范围内的随机整数
static int randomInt(int min, int max)
{
    if (min >= max)
        return min;
    return min + (rand() % (max - min + 1));
}

WaveManager::WaveManager(ZombieManager &zombieManager, Game &game)
    : m_zombieManagerRef(zombieManager),
      m_gameRef(game),
      m_currentSpawnState(SpawnState::IDLE),
      m_currentWaveNumber(0),
      m_nextNormalSpawnTime(0.0f),
      // Configurable parameters
      m_initialPeaceDuration(20.0f),    // 开局20秒和平
      m_wavePrepareDuration(3.0f),      // 每波开始前3秒准备
      m_hugeWaveAnnounceDuration(5.0f), // 大波宣告5秒
      m_normalWave_targetZombiesToSpawn(0),
      m_normalWave_zombiesSpawnedThisWave(0),
      m_normalWave_minLanes(1),
      m_normalWave_maxLanes(GRID_ROWS / 2 > 1 ? GRID_ROWS / 2 : 1), // 至少1行，最多一半行
      m_normalWave_minZombiesPerSpawnEvent(1),
      m_normalWave_maxZombiesPerSpawnEvent(2),
      m_normalWave_spawnIntervalMin(7.0f),
      m_normalWave_spawnIntervalMax(12.0f),
      m_hugeWave_zombiesPerLaneMin(3),
      m_hugeWave_zombiesPerLaneMax(5),
      m_hugeWave_spawnedThisCycle(false),
      m_hugeWaveFrequency(4), // 每完成4个普通波后，第5个是“大波”前奏
      m_wavesSinceLastHugeWave(0),
      m_waveCooldownDuration(15.0f),       // 波次结束后15秒冷却
      m_minZombiesOnScreenToEndCooldown(3) // 场上僵尸少于3个时，冷却期可提前结束
{
    // 初始化随机数引擎 m_rng
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 或者使用 std::random_device 获取更高质量的非确定性种子 (如果可用)
    // std::random_device rd;
    // unsigned seed = rd();
    m_rng.seed(seed); // <--- 在构造函数体中播种
}

void WaveManager::start()
{
    reset(); // 确保从干净的状态开始
    std::cout << "WaveManager: Started. Initial peace period for " << m_initialPeaceDuration << "s." << std::endl;
}

void WaveManager::reset()
{
    m_currentSpawnState = SpawnState::IDLE;
    m_currentWaveNumber = 0;
    m_wavesSinceLastHugeWave = 0;
    m_stateTimer.restart();
    // 其他状态相关的重置在 transitionToState 或 prepareNextWaveLogic 中处理
}

void WaveManager::update(float dt)
{
    switch (m_currentSpawnState)
    {
    case SpawnState::IDLE:
        updateIdleState(dt);
        break;
    case SpawnState::PREPARING_WAVE:
        updatePreparingWaveState(dt);
        break;
    case SpawnState::NORMAL_SPAWN:
        updateNormalSpawnState(dt);
        break;
    case SpawnState::HUGE_WAVE_ANNOUNCE:
        updateHugeWaveAnnounceState(dt);
        break;
    case SpawnState::HUGE_WAVE_SPAWN:
        updateHugeWaveSpawnState(dt);
        break;
    case SpawnState::WAVE_COOLDOWN:
        updateWaveCooldownState(dt);
        break;
    }
}

void WaveManager::transitionToState(SpawnState newState)
{
    if (m_currentSpawnState == newState)
        return; // 避免不必要的重复转换

    std::cout << "WaveManager: Transitioning from state " << static_cast<int>(m_currentSpawnState)
              << " to " << static_cast<int>(newState) << std::endl;

    m_currentSpawnState = newState;
    m_stateTimer.restart(); // 重置新状态的计时器

    // 特定状态转换时的初始化逻辑
    if (newState == SpawnState::NORMAL_SPAWN)
    {
        m_spawnIntervalTimer.restart();
        m_nextNormalSpawnTime = randomFloat(m_normalWave_spawnIntervalMin, m_normalWave_spawnIntervalMax);
        m_normalWave_zombiesSpawnedThisWave = 0;
        // m_normalWave_targetZombiesToSpawn 根据波数等在 prepareNextWaveLogic 中设置
    }
    else if (newState == SpawnState::HUGE_WAVE_SPAWN)
    {
        m_hugeWave_spawnedThisCycle = false; // 确保每次进入都尝试生成一次
    }
}

void WaveManager::updateIdleState(float dt)
{
    if (m_stateTimer.getElapsedTime().asSeconds() >= m_initialPeaceDuration)
    {
        prepareNextWaveLogic(); // 和平期结束，准备第一波
    }
}

void WaveManager::prepareNextWaveLogic()
{
    m_currentWaveNumber++;
    m_normalWave_targetZombiesToSpawn = 5 + m_currentWaveNumber * 2; // 示例：每波增加2个目标僵尸
    // 更多难度调整可以在这里进行，例如增加 m_normalWave_maxLanes 等

    std::cout << "WaveManager: Preparing Wave " << m_currentWaveNumber << std::endl;
    // TODO: 可以在此通知UI显示 "Wave X Starting Soon"
    transitionToState(SpawnState::PREPARING_WAVE);
}

void WaveManager::updatePreparingWaveState(float dt)
{
    if (m_stateTimer.getElapsedTime().asSeconds() >= m_wavePrepareDuration)
    {
        // 准备期结束，判断是进入普通波还是大波宣告
        if (m_wavesSinceLastHugeWave >= m_hugeWaveFrequency)
        {
            transitionToState(SpawnState::HUGE_WAVE_ANNOUNCE);
        }
        else
        {
            transitionToState(SpawnState::NORMAL_SPAWN);
        }
    }
}

void WaveManager::updateNormalSpawnState(float dt)
{
    // 检查是否已生成足够僵尸
    if (m_normalWave_zombiesSpawnedThisWave >= m_normalWave_targetZombiesToSpawn)
    {
        std::cout << "WaveManager: Normal Wave " << m_currentWaveNumber << " completed (target zombies spawned)." << std::endl;
        m_wavesSinceLastHugeWave++;
        transitionToState(SpawnState::WAVE_COOLDOWN);
        return;
    }

    // 按间隔生成僵尸
    if (m_spawnIntervalTimer.getElapsedTime().asSeconds() >= m_nextNormalSpawnTime)
    {
        spawnZombiesForNormalWave();
        m_spawnIntervalTimer.restart();
        m_nextNormalSpawnTime = randomFloat(m_normalWave_spawnIntervalMin, m_normalWave_spawnIntervalMax);
    }
}

void WaveManager::spawnZombiesForNormalWave()
{
    int lanesToSpawnIn = randomInt(m_normalWave_minLanes, m_normalWave_maxLanes);
    lanesToSpawnIn = std::min(lanesToSpawnIn, GRID_ROWS); // 不超过最大行数

    std::vector<int> availableLanes(GRID_ROWS);
    for (int i = 0; i < GRID_ROWS; ++i)
        availableLanes[i] = i;
    std::shuffle(availableLanes.begin(), availableLanes.end(), m_rng);

    int zombiesActuallySpawnedThisEvent = 0;
    for (int i = 0; i < lanesToSpawnIn; ++i)
    {
        if (m_normalWave_zombiesSpawnedThisWave >= m_normalWave_targetZombiesToSpawn)
            break; // 已达本波总数

        int lane = availableLanes[i];
        int zombiesInThisLane = randomInt(m_normalWave_minZombiesPerSpawnEvent, m_normalWave_maxZombiesPerSpawnEvent);

        for (int z = 0; z < zombiesInThisLane; ++z)
        {
            if (m_normalWave_zombiesSpawnedThisWave >= m_normalWave_targetZombiesToSpawn)
                break;

            m_zombieManagerRef.spawnZombie(lane, ZombieType::BASIC); // 假设 ZombieManager 有此接口
            m_normalWave_zombiesSpawnedThisWave++;
            zombiesActuallySpawnedThisEvent++;
        }
    }
    if (zombiesActuallySpawnedThisEvent > 0)
    {
        std::cout << "WaveManager: Spawned " << zombiesActuallySpawnedThisEvent << " normal zombies for Wave "
                  << m_currentWaveNumber << ". (Total this wave: " << m_normalWave_zombiesSpawnedThisWave << "/" << m_normalWave_targetZombiesToSpawn << ")" << std::endl;
    }
}

void WaveManager::updateHugeWaveAnnounceState(float dt)
{
    // TODO: 通知UI显示 "A HUGE WAVE IS APPROACHING!"
    std::cout << "WaveManager: ANNOUNCING HUGE WAVE for Wave " << m_currentWaveNumber << "!" << std::endl;
    if (m_stateTimer.getElapsedTime().asSeconds() >= m_hugeWaveAnnounceDuration)
    {
        transitionToState(SpawnState::HUGE_WAVE_SPAWN);
    }
}

void WaveManager::updateHugeWaveSpawnState(float dt)
{
    if (!m_hugeWave_spawnedThisCycle)
    {
        spawnZombiesForHugeWave();
        m_hugeWave_spawnedThisCycle = true;
        // TODO: UI显示 "THEY ARE COMING!"
    }

    // 大规模波次生成后，进入冷却期（等待僵尸被清理）
    // 这里的条件可以更复杂，比如等待场上僵尸数量减少
    // 为简单起见，一旦生成完毕就立即进入WAVE_COOLDOWN
    // 或者可以设置一个HUGE_WAVE_SPAWN状态的固定持续时间，期间僵尸不断冲击
    // 目前设计为一次性生成
    if (m_hugeWave_spawnedThisCycle)
    { // 确认已生成
        std::cout << "WaveManager: Huge Wave " << m_currentWaveNumber << " deployed. Transitioning to cooldown." << std::endl;
        m_wavesSinceLastHugeWave = 0; // 重置计数器
        transitionToState(SpawnState::WAVE_COOLDOWN);
    }
}

void WaveManager::spawnZombiesForHugeWave()
{
    std::cout << "WaveManager: Spawning HUGE WAVE zombies for Wave " << m_currentWaveNumber << std::endl;
    int totalSpawned = 0;
    for (int lane = 0; lane < GRID_ROWS; ++lane)
    {
        int numZombiesInLane = randomInt(m_hugeWave_zombiesPerLaneMin, m_hugeWave_zombiesPerLaneMax);
        for (int j = 0; j < numZombiesInLane; ++j)
        {
            m_zombieManagerRef.spawnZombie(lane, ZombieType::BASIC); // 将来可以有更强的僵尸
            totalSpawned++;
        }
    }
    std::cout << "WaveManager: Spawned " << totalSpawned << " zombies for the huge wave." << std::endl;
}

void WaveManager::updateWaveCooldownState(float dt)
{
    bool canEndCooldownEarly = (m_zombieManagerRef.getActiveZombies().size() <= m_minZombiesOnScreenToEndCooldown);
    bool cooldownTimeElapsed = (m_stateTimer.getElapsedTime().asSeconds() >= m_waveCooldownDuration);

    if (cooldownTimeElapsed || canEndCooldownEarly)
    {
        if (canEndCooldownEarly && !cooldownTimeElapsed)
        {
            std::cout << "WaveManager: Cooldown for Wave " << m_currentWaveNumber << " ending early (few zombies left)." << std::endl;
        }
        else
        {
            std::cout << "WaveManager: Cooldown for Wave " << m_currentWaveNumber << " finished." << std::endl;
        }
        prepareNextWaveLogic(); // 准备下一波
    }
}

// --- Getters ---
int WaveManager::getCurrentWaveNumber() const
{
    return m_currentWaveNumber;
}

SpawnState WaveManager::getCurrentSpawnState() const
{
    return m_currentSpawnState;
}

bool WaveManager::isGameInPeacefulPeriod() const
{
    return m_currentSpawnState == SpawnState::IDLE ||
           (m_currentSpawnState == SpawnState::WAVE_COOLDOWN && m_zombieManagerRef.getActiveZombies().empty());
}

std::string WaveManager::getCurrentWaveStatusText() const
{
    std::string status = "Wave: " + std::to_string(m_currentWaveNumber);
    switch (m_currentSpawnState)
    {
    case SpawnState::IDLE:
        status += (m_currentWaveNumber == 0 ? " (Starting Soon)" : " (Peace Time)");
        break;
    case SpawnState::PREPARING_WAVE:
        status += " (Get Ready!)";
        break;
    case SpawnState::NORMAL_SPAWN:
        status += " (Incoming)";
        break;
    case SpawnState::HUGE_WAVE_ANNOUNCE:
        status += " (HUGE WAVE INCOMING!)";
        break;
    case SpawnState::HUGE_WAVE_SPAWN:
        status += " (THEY ARE HERE!)";
        break;
    case SpawnState::WAVE_COOLDOWN:
        status += " (Clearing...)";
        break;
    }
    float timeInState = m_stateTimer.getElapsedTime().asSeconds();
    status += " T-" + std::to_string(static_cast<int>(timeInState)) + "s";
    return status;
}

float WaveManager::getCurrentWaveProgress() const
{
    float progress = 0.0f;
    float elapsedTime = m_stateTimer.getElapsedTime().asSeconds();

    switch (m_currentSpawnState)
    {
    case SpawnState::IDLE:
        if (m_currentWaveNumber == 0 && m_initialPeaceDuration > 0)
        {
            progress = elapsedTime / m_initialPeaceDuration;
        }
        else
        {
            progress = 0.0f; // 或1.0f如果表示间歇完成
        }
        break;
    case SpawnState::PREPARING_WAVE:
        if (m_wavePrepareDuration > 0)
        {
            progress = elapsedTime / m_wavePrepareDuration;
        }
        break;
    case SpawnState::NORMAL_SPAWN:
        if (m_normalWave_targetZombiesToSpawn > 0)
        {
            progress = static_cast<float>(m_normalWave_zombiesSpawnedThisWave) / m_normalWave_targetZombiesToSpawn;
        }
        break;
    case SpawnState::HUGE_WAVE_ANNOUNCE:
        if (m_hugeWaveAnnounceDuration > 0)
        {
            progress = elapsedTime / m_hugeWaveAnnounceDuration;
        }
        break;
    case SpawnState::HUGE_WAVE_SPAWN:
        progress = m_hugeWave_spawnedThisCycle ? 1.0f : 0.0f; // 大波一旦生成，进度条满
        break;
    case SpawnState::WAVE_COOLDOWN:
        // 冷却期进度可以是从0到1（时间流逝），或者从1到0（僵尸被清理）
        // 这里我们用时间流逝
        if (m_waveCooldownDuration > 0)
        {
            progress = elapsedTime / m_waveCooldownDuration;
            // 如果希望基于僵尸数量
            // float initialZombies = ... (需要记录或估算冷却开始时的僵尸数)
            // float currentZombies = m_zombieManagerRef.getActiveZombies().size();
            // progress = 1.0f - (currentZombies / initialZombies);
        }
        break;
    default:
        progress = 0.0f;
        break;
    }
#if __cplusplus >= 201703L
    return std::clamp(progress, 0.0f, 1.0f);
#else
    return std::max(0.0f, std::min(progress, 1.0f));
#endif
}

std::string WaveManager::getWaveProgressLabel() const
{
    // 这个方法可以根据当前状态返回更具体的文本
    // 例如，在NORMAL_SPAWN时显示 "Zombies: X/Y"
    // 为了简单，我们先用 getCurrentWaveStatusText() 的一部分
    std::string baseStatus = "Wave: " + std::to_string(m_currentWaveNumber);
    switch (m_currentSpawnState)
    {
    case SpawnState::IDLE:
        return (m_currentWaveNumber == 0 ? "Starting Soon" : "Peace Time");
    case SpawnState::PREPARING_WAVE:
        return "Get Ready!";
    case SpawnState::NORMAL_SPAWN:
        return baseStatus + " (" + std::to_string(m_normalWave_zombiesSpawnedThisWave) + "/" + std::to_string(m_normalWave_targetZombiesToSpawn) + ")";
    case SpawnState::HUGE_WAVE_ANNOUNCE:
        return "HUGE WAVE INCOMING!";
    case SpawnState::HUGE_WAVE_SPAWN:
        return "THEY ARE HERE!";
    case SpawnState::WAVE_COOLDOWN:
        return "Clearing...";
    default:
        return baseStatus;
    }
}