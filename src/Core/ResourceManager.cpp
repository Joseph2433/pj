#include "ResourceManager.h"
#include <iostream>
#include "../Utils/Constants.h"

ResourceManager::ResourceManager()
{
    createDefaultResources();
}

void ResourceManager::createDefaultResources()
{

    sf::Image image;
    image.create(32, 32, sf::Color::Magenta);
    if (!m_defaultTexture.loadFromImage(image))
    {
        std::cerr << "Failed to create default texture." << std::endl;
    }

    if (!m_defaultFont.loadFromFile(FONT_PATH_ARIAL))
    {
        std::cerr << "ResourceManager: Failed to load default font. Text may not render." << std::endl;
    }
}

bool ResourceManager::loadTexture(const std::string &id, const std::string &filename)
{
    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(filename))
    {
        std::cerr << "ResourceManager: Failed to load texture '" << filename << "' for ID '" << id << "'." << std::endl;
        return false;
    }
    m_textures[id] = std::move(texture);
    std::cout << "ResourceManager: Loaded texture '" << filename << "' as ID '" << id << "'." << std::endl;
    return true;
}

const sf::Texture &ResourceManager::getTexture(const std::string &id) const
{
    auto it = m_textures.find(id);
    if (it != m_textures.end())
    {
        return *(it->second);
    }

    return m_defaultTexture;
}

bool ResourceManager::hasTexture(const std::string &id) const
{
    return m_textures.count(id);
}

// Font loading
bool ResourceManager::loadFont(const std::string &id, const std::string &filename)
{
    auto font = std::make_unique<sf::Font>();
    if (!font->loadFromFile(filename))
    {
        std::cerr << "ResourceManager: Failed to load font '" << filename << "' for ID '" << id << "'." << std::endl;
        return false;
    }
    m_fonts[id] = std::move(font);
    std::cout << "ResourceManager: Loaded font '" << filename << "' as ID '" << id << "'." << std::endl;
    return true;
}

const sf::Font &ResourceManager::getFont(const std::string &id) const
{
    auto it = m_fonts.find(id);
    if (it != m_fonts.end())
    {
        return *(it->second);
    }

    return m_defaultFont;
}

bool ResourceManager::hasFont(const std::string &id) const
{
    return m_fonts.count(id);
}
