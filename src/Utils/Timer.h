#ifndef TIMER_H
#define TIMER_H

// 简单的计时器类，用于管理各种游戏中的时间相关逻辑
class Timer
{
public:
    Timer(float duration = 0.0f);

    // 更新计时器
    void update(float deltaTime);

    // 检查是否到时间
    bool isExpired() const { return m_elapsed >= m_duration; }

    // 重置计时器
    void reset();
    void reset(float newDuration);

    // 获取进度 (0.0 到 1.0)
    float getProgress() const;

    // 获取剩余时间
    float getRemainingTime() const;

    // 设置持续时间
    void setDuration(float duration) { m_duration = duration; }
    float getDuration() const { return m_duration; }

private:
    float m_duration; // 计时器持续时间
    float m_elapsed;  // 已经过的时间
};

#endif