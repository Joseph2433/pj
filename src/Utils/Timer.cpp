#include "Timer.h"
#include <algorithm>

Timer::Timer(float duration)
    : m_duration(duration), m_elapsed(0.0f)
{
}

void Timer::update(float deltaTime)
{
    m_elapsed += deltaTime;
}

void Timer::reset()
{
    m_elapsed = 0.0f;
}

void Timer::reset(float newDuration)
{
    m_duration = newDuration;
    m_elapsed = 0.0f;
}

float Timer::getProgress() const
{
    if (m_duration <= 0.0f)
        return 1.0f;

    return std::min(m_elapsed / m_duration, 1.0f);
}

float Timer::getRemainingTime() const
{
    return std::max(0.0f, m_duration - m_elapsed);
}
