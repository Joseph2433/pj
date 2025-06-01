#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager() = default;

    // 纹理管理
    bool loadTexture(const std::string &id, const std::string &filename);
    const sf::Texture &getTexture(const std::string &id) const;
    bool hasTexture(const std::string &id) const;

    // 字体管理
    bool loadFont(const std::string &id, const std::string &filename);
    const sf::Font &getFont(const std::string &id) const;
    bool hasFont(const std::string &id) const;

    // 音效管理
    bool loadSoundBuffer(const std::string &id, const std::string &filename);
    const sf::SoundBuffer &getSoundBuffer(const std::string &id) const;
    bool hasSoundBuffer(const std::string &id) const;

    // 创建默认资源（用于测试）
    void createDefaultResources();

private:
    std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::unique_ptr<sf::Font>> m_fonts;
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;
};