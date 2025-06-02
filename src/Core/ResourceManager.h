#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>
#include <map>
#include <memory>

class ResourceManager
{
public:
    ResourceManager(); // 构造函数可以创建默认资源

    bool loadTexture(const std::string &id, const std::string &filename);
    const sf::Texture &getTexture(const std::string &id) const;
    bool hasTexture(const std::string &id) const;

    bool loadFont(const std::string &id, const std::string &filename);
    const sf::Font &getFont(const std::string &id) const;
    bool hasFont(const std::string &id) const;

    // (如果你有音效)
    // bool loadSoundBuffer(const std::string& id, const std::string& filename);
    // const sf::SoundBuffer& getSoundBuffer(const std::string& id) const;
    // bool hasSoundBuffer(const std::string& id) const;

private:
    void createDefaultResources(); // 用于创建无法加载时的备用资源

    std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::unique_ptr<sf::Font>> m_fonts;
    // std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;

    // 备用资源 (当请求的资源找不到时返回)
    sf::Texture m_defaultTexture;
    sf::Font m_defaultFont;
    // sf::SoundBuffer m_defaultSoundBuffer;
};
