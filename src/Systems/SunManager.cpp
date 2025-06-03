#include "SunManager.h"
// No external dependencies other than its own header for this simple version.

SunManager::SunManager(int initialSun)
    : m_currentSun(initialSun), m_initialSunValue(initialSun)
{
}

int SunManager::getCurrentSun() const
{
    return m_currentSun;
}

void SunManager::addSun(int amount)
{
    if (amount > 0)
    {
        m_currentSun += amount;
    }
    // Optionally, add a cap for sun amount if desired
    // if (m_currentSun > MAX_SUN_CAP) m_currentSun = MAX_SUN_CAP;
}

bool SunManager::trySpendSun(int amount)
{
    if (amount > 0 && m_currentSun >= amount)
    {
        m_currentSun -= amount;
        return true;
    }
    return false;
}

void SunManager::reset()
{
    m_currentSun = m_initialSunValue;
}
