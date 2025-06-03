#pragma once

class SunManager
{
public:
    SunManager(int initialSun);
    ~SunManager() = default;

    int getCurrentSun() const;
    void addSun(int amount);
    bool trySpendSun(int amount); // Returns true if sun was spent, false otherwise
    void reset();                 // Resets sun to initial value

private:
    int m_currentSun;
    const int m_initialSunValue; // Store initial value for reset
};
