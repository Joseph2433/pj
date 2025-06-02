#include "ResourceManager.h" // 假设在 Core/ 目录下
#include <iostream>

ResourceManager::ResourceManager()
{
    createDefaultResources();
}

void ResourceManager::createDefaultResources()
{
    // 创建一个简单的默认纹理
    if (m_defaultTexture.create(32, 32))
    {
        sf::Uint8 pixels[32 * 32 * 4];
        for (int i = 0; i < 32 * 32 * 4; i += 4)
        {
            pixels[i] = 255;     // R
            pixels[i + 1] = 0;   // G
            pixels[i + 2] = 255; // B (Magenta)
            pixels[i + 3] = 255; // A
        }
        m_defaultTexture.update(pixels);
    }
    else
    {
        std::cerr << "Failed to create default texture." << std::endl;
    }

    // 尝试加载一个系统字体作为默认字体
    // 注意: 这里的路径是硬编码的，更好的做法是提供一个通用的字体文件或错误处理
    if (!m_defaultFont.loadFromFile("C:/Windows/Fonts/arial.ttf") &&
        !m_defaultFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        std::cerr << "Failed to load default font. Text rendering might be affected." << std::endl;
        // 如果字体加载失败，m_defaultFont 会是一个无效的字体对象
    }
}

bool ResourceManager::loadTexture(const std::string &id, const std::string &filename)
{
    auto texture = std::make_unique<sf::Texture>();
    if (texture->loadFromFile(filename))
    {
        m_textures[id] = std::move(texture);
        std::cout << "Loaded texture: " << filename << " as ID: " << id << std::endl;
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
    // std::cerr << "Warning: Texture with ID '" << id << "' not found. Returning default texture." << std::endl;
    return m_defaultTexture; // 返回预先创建的默认纹理
}

bool ResourceManager::hasTexture(const std::string &id) const
{
    return m_textures.count(id) > 0;
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
    // std::cerr << "Warning: Font with ID '" << id << "' not found. Returning default font." << std::endl;
    return m_defaultFont; // 返回预先创建的默认字体
}

bool ResourceManager::hasFont(const std::string &id) const
{
    return m_fonts.count(id) > 0;
}
