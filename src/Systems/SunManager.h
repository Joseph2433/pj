#pragma once

class SunManager
{
public:
    SunManager(int initialSun);
    ~SunManager() = default;

    int getCurrentSun() const;
    void addSun(int amount);
    bool trySpendSun(int amount);
    void reset();

private:
    int m_currentSun;
    const int m_initialSunValue;
};
