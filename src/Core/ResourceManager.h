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
    ResourceManager();

    ~ResourceManager() = default;

    // --- 纹理管理 ---
    bool loadTexture(const std::string &id, const std::string &filename);
    const sf::Texture &getTexture(const std::string &id) const;
    bool hasTexture(const std::string &id) const;

    // --- 字体管理 ---
    bool loadFont(const std::string &id, const std::string &filename);
    const sf::Font &getFont(const std::string &id) const;
    bool hasFont(const std::string &id) const;

private:
    void createDefaultResources();

    // --- 资源存储容器 ---
    std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::unique_ptr<sf::Font>> m_fonts;

    sf::Texture m_defaultTexture;
    sf::Font m_defaultFont;
};
