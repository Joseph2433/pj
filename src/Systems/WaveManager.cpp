#include "WaveManager.h"
#include "ZombieManager.h"      // For ZombieManager aclass nd ZombieType enum
#include "../Core/Game.h"       // For Game class (if needed, e.g. for global game time or other systems)
#include "../Utils/Constants.h" // For TOTAL_WAVES_TO_WIN, GRID_ROWS, etc.
#include <iostream>             // For std::cout, std::cerr
#include <cstdlib>              // For rand(), RAND_MAX
#include <algorithm>            // For std::min, std::max, std::shuffle
#include <vector>               // For std::vector
#include <sstream>              // For std::stringstream, std::to_string
#include <iomanip>              // For std::fixed, std::setprecision (used in getWaveProgressLabel if needed)
// <random> and <chrono> are included via WaveManager.h for std::mt19937

// Anonymous namespace for helper functions (if not already in a utility file)
namespace
{
    float randomFloat(float min, float max)
    {
        if (min >= max)
            return min;
        // Prevent division by zero if max - min is extremely small or zero
        if ((max - min) < 0.00001f && (max - min) > -0.00001f)
            return min;
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }

    int randomInt(int min, int max)
    {
        if (min > max)
            std::swap(min, max); // Ensure min <= max
        if (min == max)
            return min;
        return min + (rand() % (max - min + 1));
    }
} // end anonymous namespace

WaveManager::WaveManager(ZombieManager &zombieManager, Game &game)
    : m_zombieManagerRef(zombieManager),
      m_gameRef(game),
      m_currentSpawnState(SpawnState::IDLE),
      m_currentWaveNumber(0),
      m_nextNormalSpawnTime(0.0f), // Will be properly set when transitioning to NORMAL_SPAWN
      // Configurable parameters from Constants.h or defaults
      m_initialPeaceDuration(20.0f), // Example: Initial peace duration
      m_wavePrepareDuration(3.0f),
      m_hugeWaveAnnounceDuration(5.0f),
      m_normalWave_targetZombiesToSpawn(0), // Calculated per wave
      m_normalWave_zombiesSpawnedThisWave(0),
      m_normalWave_minLanes(1),
      m_normalWave_maxLanes(GRID_ROWS / 2 > 1 ? GRID_ROWS / 2 : 1),
      m_normalWave_minZombiesPerSpawnEvent(1),
      m_normalWave_maxZombiesPerSpawnEvent(2),
      m_normalWave_spawnIntervalMin(7.0f),
      m_normalWave_spawnIntervalMax(12.0f),
      m_hugeWave_zombiesPerLaneMin(3),
      m_hugeWave_zombiesPerLaneMax(5),
      m_hugeWave_spawnedThisCycle(false),
      m_hugeWaveFrequency(4), // Huge wave after every 4 normal waves (so on wave 5, 10, etc. if starting from 1)
      m_wavesSinceLastHugeWave(0),
      m_waveCooldownDuration(15.0f),
      m_minZombiesOnScreenToEndCooldown(3)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    m_rng.seed(seed);
    std::cout << "WaveManager constructed. TOTAL_WAVES_TO_WIN is " << TOTAL_WAVES_TO_WIN << std::endl;
}

void WaveManager::start()
{
    reset();
    std::cout << "WaveManager: Started. Initial peace period for " << m_initialPeaceDuration << "s." << std::endl;
}

void WaveManager::reset()
{
    m_currentSpawnState = SpawnState::IDLE;
    m_currentWaveNumber = 0; // Start before wave 1
    m_wavesSinceLastHugeWave = 0;
    m_normalWave_zombiesSpawnedThisWave = 0;
    m_hugeWave_spawnedThisCycle = false;
    m_stateTimer.restart();
    m_spawnIntervalTimer.restart(); // Also reset spawn timer
    std::cout << "WaveManager: Reset to initial state." << std::endl;
}

void WaveManager::update(float dt)
{
    // If all waves are completed, WaveManager's primary job is done.
    if (m_currentSpawnState == SpawnState::ALL_WAVES_COMPLETED)
    {
        updateAllWavesCompletedState(dt); // This might just be an empty function or log
        return;
    }

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
    case SpawnState::ALL_WAVES_COMPLETED: /* updateAllWavesCompletedState(dt); already handled above */
        break;
    }
}

void WaveManager::transitionToState(SpawnState newState)
{
    // Allow re-entering IDLE or ALL_WAVES_COMPLETED if needed (e.g., from reset)
    if (m_currentSpawnState == newState &&
        newState != SpawnState::IDLE &&
        newState != SpawnState::ALL_WAVES_COMPLETED)
    {
        return;
    }

    std::cout << "WaveManager: Transitioning from state "
              << static_cast<int>(m_currentSpawnState)
              << " to " << static_cast<int>(newState) << std::endl;

    m_currentSpawnState = newState;
    m_stateTimer.restart(); // Reset timer for the new state

    if (newState == SpawnState::NORMAL_SPAWN)
    {
        m_spawnIntervalTimer.restart();
        m_nextNormalSpawnTime = randomFloat(m_normalWave_spawnIntervalMin, m_normalWave_spawnIntervalMax);
        m_normalWave_zombiesSpawnedThisWave = 0; // Reset count for the new normal wave
    }
    else if (newState == SpawnState::HUGE_WAVE_SPAWN)
    {
        m_hugeWave_spawnedThisCycle = false; // Reset flag for the new huge wave
    }
}

void WaveManager::updateIdleState(float dt)
{
    // Only trigger the first wave from the initial IDLE state
    if (m_currentWaveNumber == 0 && m_stateTimer.getElapsedTime().asSeconds() >= m_initialPeaceDuration)
    {
        prepareNextWaveLogic();
    }
    // If it's IDLE after ALL_WAVES_COMPLETED, do nothing further.
}

void WaveManager::prepareNextWaveLogic()
{
    // This function should only be called if m_currentWaveNumber < TOTAL_WAVES_TO_WIN
    // The check is primarily in updateWaveCooldownState
    if (m_currentWaveNumber >= TOTAL_WAVES_TO_WIN)
    {
        // This should ideally not be reached if updateWaveCooldownState is correct
        std::cout << "WaveManager Error: prepareNextWaveLogic called when all waves ("
                  << m_currentWaveNumber << "/" << TOTAL_WAVES_TO_WIN << ") are already done. Forcing ALL_WAVES_COMPLETED." << std::endl;
        transitionToState(SpawnState::ALL_WAVES_COMPLETED);
        return;
    }

    m_currentWaveNumber++;
    // Example: Difficulty scaling for normal waves
    m_normalWave_targetZombiesToSpawn = 5 + m_currentWaveNumber * 2; // Base + (2 zombies per wave number)
    // m_normalWave_maxLanes = std::min(GRID_ROWS, 1 + m_currentWaveNumber / 2); // Gradually increase lanes

    std::cout << "WaveManager: Preparing Wave " << m_currentWaveNumber << "/" << TOTAL_WAVES_TO_WIN << std::endl;
    transitionToState(SpawnState::PREPARING_WAVE);
}

void WaveManager::updatePreparingWaveState(float dt)
{
    if (m_stateTimer.getElapsedTime().asSeconds() >= m_wavePrepareDuration)
    {
        // Check if it's time for a huge wave (only if not past total waves)
        if (m_wavesSinceLastHugeWave >= m_hugeWaveFrequency && m_currentWaveNumber <= TOTAL_WAVES_TO_WIN)
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
    if (m_normalWave_zombiesSpawnedThisWave >= m_normalWave_targetZombiesToSpawn)
    {
        std::cout << "WaveManager: Normal Wave " << m_currentWaveNumber << " completed (target "
                  << m_normalWave_targetZombiesToSpawn << " zombies spawned)." << std::endl;
        m_wavesSinceLastHugeWave++;
        transitionToState(SpawnState::WAVE_COOLDOWN);
        return;
    }

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
    lanesToSpawnIn = std::min(lanesToSpawnIn, GRID_ROWS);

    std::vector<int> availableLanes(GRID_ROWS);
    for (int i = 0; i < GRID_ROWS; ++i)
        availableLanes[i] = i;
    std::shuffle(availableLanes.begin(), availableLanes.end(), m_rng);

    int zombiesActuallySpawnedThisEvent = 0;
    for (int i = 0; i < lanesToSpawnIn; ++i)
    {
        if (m_normalWave_zombiesSpawnedThisWave >= m_normalWave_targetZombiesToSpawn)
            break;

        int lane = availableLanes[i];
        int zombiesInThisLane = randomInt(m_normalWave_minZombiesPerSpawnEvent, m_normalWave_maxZombiesPerSpawnEvent);

        for (int z = 0; z < zombiesInThisLane; ++z)
        {
            if (m_normalWave_zombiesSpawnedThisWave >= m_normalWave_targetZombiesToSpawn)
                break;
            m_zombieManagerRef.spawnZombie(lane, ZombieType::BASIC);
            m_normalWave_zombiesSpawnedThisWave++;
            zombiesActuallySpawnedThisEvent++;
        }
    }
    if (zombiesActuallySpawnedThisEvent > 0)
    {
        std::cout << "WaveManager: Spawned " << zombiesActuallySpawnedThisEvent << " normal zombies for Wave "
                  << m_currentWaveNumber << ". (Total this wave: "
                  << m_normalWave_zombiesSpawnedThisWave << "/" << m_normalWave_targetZombiesToSpawn << ")" << std::endl;
    }
}

void WaveManager::updateHugeWaveAnnounceState(float dt)
{
    std::cout << "WaveManager: ANNOUNCING HUGE WAVE for Wave " << m_currentWaveNumber << "!" << std::endl;
    // TODO: Notify UI to display "A HUGE WAVE IS APPROACHING!"
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
        // TODO: Notify UI to display "THEY ARE COMING!"
    }

    if (m_hugeWave_spawnedThisCycle)
    {
        std::cout << "WaveManager: Huge Wave " << m_currentWaveNumber << " deployed. Transitioning to cooldown." << std::endl;
        m_wavesSinceLastHugeWave = 0; // Reset counter after a huge wave
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
            m_zombieManagerRef.spawnZombie(lane, ZombieType::BASIC);
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
        if (m_currentWaveNumber >= TOTAL_WAVES_TO_WIN)
        { // Check against the constant
            std::cout << "WaveManager: Final wave's cooldown finished (Wave " << m_currentWaveNumber
                      << "/" << TOTAL_WAVES_TO_WIN << "). Transitioning to ALL_WAVES_COMPLETED." << std::endl;
            transitionToState(SpawnState::ALL_WAVES_COMPLETED);
        }
        else
        {
            if (canEndCooldownEarly && !cooldownTimeElapsed)
            {
                std::cout << "WaveManager: Cooldown for Wave " << m_currentWaveNumber << " ending early (few zombies left)." << std::endl;
            }
            else
            {
                std::cout << "WaveManager: Cooldown for Wave " << m_currentWaveNumber << " finished." << std::endl;
            }
            prepareNextWaveLogic();
        }
    }
}

void WaveManager::updateAllWavesCompletedState(float dt)
{
    // WaveManager has finished its primary task of spawning waves.
    // It will remain in this state. GamePlayState will check this state
    // and the zombie count to trigger victory.
    // Optional: Log once or periodically if desired for debugging.
    // static bool loggedOnce = false;
    // if (!loggedOnce) {
    //     std::cout << "WaveManager: All waves completed. Waiting for GamePlayState to confirm victory." << std::endl;
    //     loggedOnce = true;
    // }
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
           m_currentSpawnState == SpawnState::ALL_WAVES_COMPLETED || // ALL_WAVES_COMPLETED is peaceful
           (m_currentSpawnState == SpawnState::WAVE_COOLDOWN && m_zombieManagerRef.getActiveZombies().empty());
}

// getCurrentWaveProgress() - using the version from your provided code
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
            progress = 0.0f;
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
        progress = m_hugeWave_spawnedThisCycle ? 1.0f : 0.0f;
        break;
    case SpawnState::WAVE_COOLDOWN:
        if (m_waveCooldownDuration > 0)
        {
            progress = elapsedTime / m_waveCooldownDuration;
        }
        break;
    case SpawnState::ALL_WAVES_COMPLETED:
        progress = 1.0f; // All waves done, progress is full
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

// getWaveProgressLabel() - using the version from your provided code
std::string WaveManager::getWaveProgressLabel() const
{
    std::string waveNumStr = std::to_string(m_currentWaveNumber) + "/" + std::to_string(TOTAL_WAVES_TO_WIN);
    std::string baseStatus = "Wave: " + waveNumStr;

    if (m_currentSpawnState == SpawnState::ALL_WAVES_COMPLETED)
        return "All Waves Processed!";

    if (m_currentWaveNumber == 0 && m_currentSpawnState == SpawnState::IDLE)
        return "Starting Soon...";
    switch (m_currentSpawnState)
    {
    case SpawnState::IDLE:
        return baseStatus + (m_currentWaveNumber == 0 ? " (Starting)" : " (Peace)");
    case SpawnState::PREPARING_WAVE:
        return baseStatus + " (Get Ready!)";
    case SpawnState::NORMAL_SPAWN:
        return baseStatus + " (" + std::to_string(m_normalWave_zombiesSpawnedThisWave) + "/" + std::to_string(m_normalWave_targetZombiesToSpawn) + ")";
    case SpawnState::HUGE_WAVE_ANNOUNCE:
        return baseStatus + " (HUGE WAVE!)";
    case SpawnState::HUGE_WAVE_SPAWN:
        return baseStatus + " (ATTACK!)";
    case SpawnState::WAVE_COOLDOWN:
        return baseStatus + " (Clearing...)";
    case SpawnState::ALL_WAVES_COMPLETED:
        return "All Waves Done!"; // Should be caught by the check above
    default:
        return baseStatus;
    }
}

// getCurrentWaveStatusText() - using the version from your provided code
std::string WaveManager::getCurrentWaveStatusText() const
{
    std::string status = "Wave: " + std::to_string(m_currentWaveNumber) + "/" + std::to_string(TOTAL_WAVES_TO_WIN);

    if (m_currentSpawnState == SpawnState::ALL_WAVES_COMPLETED)
    {
        status += " (ALL WAVES COMPLETED)";
    }
    else
    {
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
        default:
            break;
        }
    }
    // Don't add time suffix if all waves are completed, unless desired
    if (m_currentSpawnState != SpawnState::ALL_WAVES_COMPLETED)
    {
        float timeInState = m_stateTimer.getElapsedTime().asSeconds();
        std::stringstream ssTime;
        ssTime.precision(1);
        ssTime << std::fixed << timeInState;
        status += " T-" + ssTime.str() + "s";
    }
    return status;
}
