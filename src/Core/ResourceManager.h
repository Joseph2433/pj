#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>

class ResourceManager
{
public:
    static ResourceManager &getInstance();

    // 纹理管理
    bool loadTexture(const std::string &name, const std::string &filename);
    sf::Texture &getTexture(const std::string &name);

    // 字体管理
    bool loadFont(const std::string &name, const std::string &filename);
    sf::Font &getFont(const std::string &name);

    // 音效管理
    bool loadSoundBuffer(const std::string &name, const std::string &filename);
    sf::SoundBuffer &getSoundBuffer(const std::string &name);

    // 清理资源
    void clear();

private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;

    std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::unique_ptr<sf::Font>> m_fonts;
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;
};