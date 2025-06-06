#include "SunManager.h"

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
