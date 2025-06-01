#include "Core/ResourceManager.h"
#include <iostream>

ResourceManager &ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}

bool ResourceManager::loadTexture(const std::string &name, const std::string &filename)
{
    auto texture = std::make_unique<sf::Texture>();
    if (texture->loadFromFile(filename))
    {
        m_textures[name] = std::move(texture);
        std::cout << "Loaded texture: " << name << " from " << filename << std::endl;
        return true;
    }
    std::cerr << "Failed to load texture: " << filename << std::endl;
    return false;
}

sf::Texture &ResourceManager::getTexture(const std::string &name)
{
    auto it = m_textures.find(name);
    if (it != m_textures.end())
    {
        return *it->second;
    }

    // 如果找不到纹理，返回一个默认的空纹理
    static sf::Texture defaultTexture;
    std::cerr << "Texture not found: " << name << std::endl;
    return defaultTexture;
}

bool ResourceManager::loadFont(const std::string &name, const std::string &filename)
{
    auto font = std::make_unique<sf::Font>();
    if (font->loadFromFile(filename))
    {
        m_fonts[name] = std::move(font);
        std::cout << "Loaded font: " << name << " from " << filename << std::endl;
        return true;
    }
    std::cerr << "Failed to load font: " << filename << std::endl;
    return false;
}

sf::Font &ResourceManager::getFont(const std::string &name)
{
    auto it = m_fonts.find(name);
    if (it != m_fonts.end())
    {
        return *it->second;
    }

    // 如果找不到字体，返回默认字体
    static sf::Font defaultFont;
    std::cerr << "Font not found: " << name << std::endl;
    return defaultFont;
}

bool ResourceManager::loadSoundBuffer(const std::string &name, const std::string &filename)
{
    auto soundBuffer = std::make_unique<sf::SoundBuffer>();
    if (soundBuffer->loadFromFile(filename))
    {
        m_soundBuffers[name] = std::move(soundBuffer);
        std::cout << "Loaded sound: " << name << " from " << filename << std::endl;
        return true;
    }
    std::cerr << "Failed to load sound: " << filename << std::endl;
    return false;
}

sf::SoundBuffer &ResourceManager::getSoundBuffer(const std::string &name)
{
    auto it = m_soundBuffers.find(name);
    if (it != m_soundBuffers.end())
    {
        return *it->second;
    }

    // 如果找不到音效，返回默认的空音效缓冲区
    static sf::SoundBuffer defaultSoundBuffer;
    std::cerr << "Sound buffer not found: " << name << std::endl;
    return defaultSoundBuffer;
}

void ResourceManager::clear()
{
    m_textures.clear();
    m_fonts.clear();
    m_soundBuffers.clear();
    std::cout << "All resources cleared" << std::endl;
}