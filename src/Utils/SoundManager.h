#pragma once
#include <SFML/Audio.hpp> // 包含 SFML Audio 模块
#include <string>
#include <map>
#include <memory> // For std::unique_ptr if storing sf::Sound

// 推荐将音效和音乐的ID定义在 Constants.h 或一个专门的 AudioConstants.h 中
// 例如: const std::string BGM_GAMEPLAY = "bgm_gameplay";
//       const std::string SFX_PEASHOOT = "sfx_peashoot";

class SoundManager
{
public:
    SoundManager();
    ~SoundManager(); // 需要正确停止音乐和释放资源

    // --- 背景音乐 (sf::Music) ---
    bool loadMusic(const std::string &id, const std::string &filename);
    void playMusic(const std::string &id, bool loop = true, float volume = 50.f); // 默认音量50%
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    void setMusicVolume(float volume); // 0 to 100
    bool isMusicPlaying() const;
    sf::SoundSource::Status getMusicStatus() const;

    // --- 音效 (sf::SoundBuffer 和 sf::Sound) ---
    bool loadSoundBuffer(const std::string &id, const std::string &filename);
    void playSound(const std::string &id, float volume = 100.f, float pitch = 1.f, bool loop = false);
    void stopAllSounds(); // 停止所有播放中的音效

    // 全局音量控制
    void setGlobalVolume(float volume); // 0 to 100 (会影响所有之后播放的音效和音乐)
    float getGlobalVolume() const;

    const std::string &getCurrentPlayingMusicId() const;

private:
    // 背景音乐 (通常只有一个同时播放，但可以支持多个加载)
    // sf::Music m_currentMusic; // 如果只支持一个背景音乐实例
    std::map<std::string, std::unique_ptr<sf::Music>> m_musicTracks; // 支持加载多个音乐文件
    std::map<std::string, float> m_musicBaseVolumes;
    std::string m_currentPlayingMusicId; // 追踪当前播放的音乐ID

    // 音效缓冲 (加载到内存)
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;

    // 播放中的音效实例 (sf::Sound 需要 SoundBuffer)
    // 由于 sf::Sound 是轻量级的，通常在需要时创建，或者用一个池来管理
    std::vector<sf::Sound> m_playingSounds; // 管理播放中的音效实例，以便可以停止或修改它们

    float m_globalVolume; // 0.0f to 100.0f
};
