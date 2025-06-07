#include "SoundManager.h"
#include <iostream>
#include <algorithm> // For std::remove_if

SoundManager::SoundManager() : m_globalVolume(70.f), m_currentPlayingMusicId("")
{ // 默认全局音量70%
    std::cout << "SoundManager constructed." << std::endl;
}

SoundManager::~SoundManager()
{
    stopMusic(); // 确保音乐停止
    stopAllSounds();
    m_musicTracks.clear(); // unique_ptr 会自动删除 sf::Music 对象
    m_soundBuffers.clear();
    m_playingSounds.clear();
    std::cout << "SoundManager destructed." << std::endl;
}

// --- 背景音乐 ---
bool SoundManager::loadMusic(const std::string &id, const std::string &filename)
{
    auto music = std::make_unique<sf::Music>();
    if (!music->openFromFile(filename))
    {
        std::cerr << "SoundManager Error: Failed to load music '" << filename << "' with ID '" << id << "'" << std::endl;
        return false;
    }
    m_musicTracks[id] = std::move(music);
    m_musicBaseVolumes[id] = 50.f;
    std::cout << "SoundManager: Loaded music '" << filename << "' as ID '" << id << "'" << std::endl;
    return true;
}

void SoundManager::playMusic(const std::string &id, bool loop, float basevolume)
{
    auto it = m_musicTracks.find(id);
    if (it != m_musicTracks.end() && it->second)
    {
        // 如果有其他音乐正在播放，先停止它
        if (!m_currentPlayingMusicId.empty() && m_currentPlayingMusicId != id)
        {
            auto oldTrackIt = m_musicTracks.find(m_currentPlayingMusicId);
            if (oldTrackIt != m_musicTracks.end() && oldTrackIt->second)
            {
                oldTrackIt->second->stop();
            }
        }
        m_musicBaseVolumes[id] = std::max(0.f, std::min(100.f, basevolume));
        it->second->setLoop(loop);
        it->second->setVolume(basevolume * (m_globalVolume / 100.f)); // 应用全局音量
        it->second->play();
        m_currentPlayingMusicId = id;
        std::cout << "SoundManager: Playing music ID '" << id << "'" << std::endl;
    }
    else
    {
        std::cerr << "SoundManager Error: Music ID '" << id << "' not found or not loaded." << std::endl;
    }
}

void SoundManager::stopMusic()
{
    if (!m_currentPlayingMusicId.empty())
    {
        auto it = m_musicTracks.find(m_currentPlayingMusicId);
        if (it != m_musicTracks.end() && it->second)
        {
            it->second->stop();
            std::cout << "SoundManager: Stopped music ID '" << m_currentPlayingMusicId << "'" << std::endl;
        }
        m_currentPlayingMusicId = ""; // 清除当前播放ID
    }
}

void SoundManager::pauseMusic()
{
    if (!m_currentPlayingMusicId.empty())
    {
        auto it = m_musicTracks.find(m_currentPlayingMusicId);
        if (it != m_musicTracks.end() && it->second && it->second->getStatus() == sf::SoundSource::Playing)
        {
            it->second->pause();
            std::cout << "SoundManager: Paused music ID '" << m_currentPlayingMusicId << "'" << std::endl;
        }
    }
}

void SoundManager::resumeMusic()
{
    if (!m_currentPlayingMusicId.empty())
    {
        auto it = m_musicTracks.find(m_currentPlayingMusicId);
        if (it != m_musicTracks.end() && it->second && it->second->getStatus() == sf::SoundSource::Paused)
        {
            it->second->play(); // play() 会从暂停处继续
            std::cout << "SoundManager: Resumed music ID '" << m_currentPlayingMusicId << "'" << std::endl;
        }
    }
}

void SoundManager::setMusicVolume(float baseVolume)
{
    if (!m_currentPlayingMusicId.empty())
    {
        auto it_track = m_musicTracks.find(m_currentPlayingMusicId);
        if (it_track != m_musicTracks.end() && it_track->second)
        {
            m_musicBaseVolumes[m_currentPlayingMusicId] = std::max(0.f, std::min(100.f, baseVolume));
            it_track->second->setVolume(m_musicBaseVolumes[m_currentPlayingMusicId] * (m_globalVolume / 100.f));
            std::cout << "SoundManager: Set base volume for '" << m_currentPlayingMusicId << "' to " << m_musicBaseVolumes[m_currentPlayingMusicId] << "%" << std::endl;
        }
    }
}

bool SoundManager::isMusicPlaying() const
{
    if (!m_currentPlayingMusicId.empty())
    {
        auto it = m_musicTracks.find(m_currentPlayingMusicId);
        if (it != m_musicTracks.end() && it->second)
        {
            return it->second->getStatus() == sf::SoundSource::Playing;
        }
    }
    return false;
}

sf::SoundSource::Status SoundManager::getMusicStatus() const
{
    if (!m_currentPlayingMusicId.empty())
    {
        auto it = m_musicTracks.find(m_currentPlayingMusicId);
        if (it != m_musicTracks.end() && it->second)
        {
            return it->second->getStatus();
        }
    }
    return sf::SoundSource::Stopped;
}

// --- 音效 ---
bool SoundManager::loadSoundBuffer(const std::string &id, const std::string &filename)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename))
    {
        std::cerr << "SoundManager Error: Failed to load sound buffer '" << filename << "' with ID '" << id << "'" << std::endl;
        return false;
    }
    m_soundBuffers[id] = buffer;
    std::cout << "SoundManager: Loaded sound buffer '" << filename << "' as ID '" << id << "'" << std::endl;
    return true;
}

void SoundManager::playSound(const std::string &id, float volume, float pitch, bool loop)
{
    // 清理已停止播放的音效，以避免 m_playingSounds 无限增长
    m_playingSounds.erase(
        std::remove_if(m_playingSounds.begin(), m_playingSounds.end(),
                       [](const sf::Sound &s)
                       { return s.getStatus() == sf::SoundSource::Stopped; }),
        m_playingSounds.end());

    auto it = m_soundBuffers.find(id);
    if (it != m_soundBuffers.end())
    {
        // 创建一个新的 sf::Sound 实例来播放
        // sf::Sound 是轻量级的，可以按需创建
        m_playingSounds.emplace_back(it->second); // 使用已加载的 buffer
        sf::Sound &sound = m_playingSounds.back();
        sound.setVolume(volume * (m_globalVolume / 100.f));
        sound.setPitch(pitch);
        sound.setLoop(loop);
        sound.play();
        // std::cout << "SoundManager: Playing sound ID '" << id << "'" << std::endl;
    }
    else
    {
        std::cerr << "SoundManager Error: SoundBuffer ID '" << id << "' not found or not loaded." << std::endl;
    }
}

void SoundManager::stopAllSounds()
{
    for (sf::Sound &sound : m_playingSounds)
    {
        sound.stop();
    }
    m_playingSounds.clear(); // 清空列表
    std::cout << "SoundManager: All sounds stopped." << std::endl;
}

void SoundManager::setGlobalVolume(float volume)
{
    m_globalVolume = std::max(0.f, std::min(100.f, volume)); // Clamp 0-100
    std::cout << "SoundManager: Global volume set to " << m_globalVolume << "%" << std::endl;

    // 更新当前正在播放的音乐的实际音量
    if (!m_currentPlayingMusicId.empty())
    {
        auto it_track = m_musicTracks.find(m_currentPlayingMusicId);
        auto it_base_vol = m_musicBaseVolumes.find(m_currentPlayingMusicId);

        if (it_track != m_musicTracks.end() && it_track->second &&
            it_base_vol != m_musicBaseVolumes.end())
        {
            it_track->second->setVolume(it_base_vol->second * (m_globalVolume / 100.f));
            std::cout << "SoundManager: Updated currently playing music '" << m_currentPlayingMusicId
                      << "' volume based on new global volume." << std::endl;
        }
    }
}
float SoundManager::getGlobalVolume() const
{
    return m_globalVolume;
}

const std::string &SoundManager::getCurrentPlayingMusicId() const
{
    return m_currentPlayingMusicId;
}