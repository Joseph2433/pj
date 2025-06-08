#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <memory>

class SoundManager
{
public:
    SoundManager();
    ~SoundManager();

    // 背景音乐
    bool loadMusic(const std::string &id, const std::string &filename);
    void playMusic(const std::string &id, bool loop = true, float volume = 50.f);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    void setMusicVolume(float volume);
    bool isMusicPlaying() const;
    sf::SoundSource::Status getMusicStatus() const;

    // 音效
    bool loadSoundBuffer(const std::string &id, const std::string &filename);
    void playSound(const std::string &id, float volume = 100.f, float pitch = 1.f, bool loop = false);
    void stopAllSounds();

    // 全局音量控制
    void setGlobalVolume(float volume);
    float getGlobalVolume() const;

    const std::string &getCurrentPlayingMusicId() const;

private:
    std::map<std::string, std::unique_ptr<sf::Music>> m_musicTracks;
    std::map<std::string, float> m_musicBaseVolumes;
    std::string m_currentPlayingMusicId;

    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::vector<sf::Sound> m_playingSounds;

    float m_globalVolume;
};
