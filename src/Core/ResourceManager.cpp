#include "ResourceManager.h"
#include <iostream>

ResourceManager::ResourceManager()
{
    createDefaultResources();
}

bool ResourceManager::loadTexture(const std::string &id, const std::string &filename)
{
    auto texture = std::make_unique<sf::Texture>();
    if (texture->loadFromFile(filename))
    {
        m_textures[id] = std::move(texture);
        return true;
    }
    std::cerr << "Failed to load texture: " << filename << std::endl;
    return false;
}

const sf::Texture &ResourceManager::getTexture(const std::string &id) const
{
    auto it = m_textures.find(id);
    if (it != m_textures.end())
    {
        return *(it->second);
    }

    // 返回默认纹理
    static sf::Texture defaultTexture;
    return defaultTexture;
}

bool ResourceManager::hasTexture(const std::string &id) const
{
    return m_textures.find(id) != m_textures.end();
}

bool ResourceManager::loadFont(const std::string &id, const std::string &filename)
{
    auto font = std::make_unique<sf::Font>();
    if (font->loadFromFile(filename))
    {
        m_fonts[id] = std::move(font);
        return true;
    }
    std::cerr << "Failed to load font: " << filename << std::endl;
    return false;
}

const sf::Font &ResourceManager::getFont(const std::string &id) const
{
    auto it = m_fonts.find(id);
    if (it != m_fonts.end())
    {
        return *(it->second);
    }

    // 返回默认字体
    static sf::Font defaultFont;
    return defaultFont;
}

bool ResourceManager::hasFont(const std::string &id) const
{
    return m_fonts.find(id) != m_fonts.end();
}

bool ResourceManager::loadSoundBuffer(const std::string &id, const std::string &filename)
{
    auto buffer = std::make_unique<sf::SoundBuffer>();
    if (buffer->loadFromFile(filename))
    {
        m_soundBuffers[id] = std::move(buffer);
        return true;
    }
    std::cerr << "Failed to load sound: " << filename << std::endl;
    return false;
}

const sf::SoundBuffer &ResourceManager::getSoundBuffer(const std::string &id) const
{
    auto it = m_soundBuffers.find(id);
    if (it != m_soundBuffers.end())
    {
        return *(it->second);
    }

    // 返回默认音效缓冲区
    static sf::SoundBuffer defaultBuffer;
    return defaultBuffer;
}

bool ResourceManager::hasSoundBuffer(const std::string &id) const
{
    return m_soundBuffers.find(id) != m_soundBuffers.end();
}

void ResourceManager::createDefaultResources()
{
    // 创建默认纹理（用于测试）
    auto defaultTexture = std::make_unique<sf::Texture>();
    defaultTexture->create(64, 64);

    // 创建一个简单的彩色图案
    sf::Uint8 *pixels = new sf::Uint8[64 * 64 * 4];
    for (int i = 0; i < 64 * 64 * 4; i += 4)
    {
        pixels[i] = 100;     // Red
        pixels[i + 1] = 200; // Green
        pixels[i + 2] = 100; // Blue
        pixels[i + 3] = 255; // Alpha
    }
    defaultTexture->update(pixels);
    delete[] pixels;

    m_textures["default"] = std::move(defaultTexture);

    // 尝试加载系统默认字体
    auto defaultFont = std::make_unique<sf::Font>();
// 大多数系统都有这个字体文件
#ifdef _WIN32
    if (defaultFont->loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        m_fonts["default"] = std::move(defaultFont);
    }
#else
    if (defaultFont->loadFromFile("/System/Library/Fonts/Arial.ttf") ||
        defaultFont->loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"))
    {
        m_fonts["default"] = std::move(defaultFont);
    }
#endif
}