#include "HUD.h"
#include "../Core/ResourceManager.h" // If HUD loads its own resources like sun icon
#include "../Systems/SunManager.h"
#include "../Utils/Constants.h" // For layout constants like SUN_DISPLAY_X/Y
#include <SFML/Window/Event.hpp>
#include <sstream> // For std::to_string

HUD::HUD(ResourceManager &resManager, SunManager &sunManager,
         sf::Font &primaryFont, sf::Font &secondaryFont)
    : m_seedManager(resManager, sunManager, primaryFont, secondaryFont), // Initialize SeedManager
      m_sunManagerRef(sunManager),
      m_primaryFontRef(primaryFont)
{

    // Initialize Sun Display Text
    m_sunDisplayText.setFont(m_primaryFontRef);
    m_sunDisplayText.setCharacterSize(SUN_DISPLAY_FONT_SIZE);
    m_sunDisplayText.setFillColor(sf::Color::Yellow);
    m_sunDisplayText.setOutlineColor(sf::Color::Black);
    m_sunDisplayText.setOutlineThickness(1.f);
    m_sunDisplayText.setPosition(SUN_DISPLAY_X, SUN_DISPLAY_Y);

    // Example: Load sun icon if you have one
    // if (resManager.hasTexture(SUN_ICON_TEXTURE_KEY)) {
    //     m_sunIcon.setTexture(resManager.getTexture(SUN_ICON_TEXTURE_KEY));
    //     m_sunIcon.setPosition(SUN_DISPLAY_X - 40, SUN_DISPLAY_Y); // Adjust position
    // }
}

void HUD::handleEvent(const sf::Event &event, const sf::Vector2f &mousePosInView)
{
    m_seedManager.handleEvent(event, mousePosInView);
    // Handle other HUD interactable elements if any (e.g., shovel button)
}

void HUD::update(float dt)
{
    m_seedManager.update(dt); // Update all seed packets (cooldowns)

    // Update sun display text
    std::stringstream ss;
    ss << m_sunManagerRef.getCurrentSun(); // Just the number, "Sun: " can be static or part of a background
    m_sunDisplayText.setString(ss.str());

    // Update other HUD elements
}

void HUD::draw(sf::RenderWindow &window)
{
    m_seedManager.draw(window); // Draw the seed packet bar
    // window.draw(m_sunIcon);     // Draw sun icon
    window.draw(m_sunDisplayText); // Draw sun amount
    // Draw other HUD elements
}

PlantType HUD::getSelectedPlantTypeFromSeedManager(bool &isValidSelection) const
{
    return m_seedManager.getSelectedPlantType(isValidSelection);
}

void HUD::notifyPlantPlacedToSeedManager(PlantType plantType)
{
    m_seedManager.notifyPlantPlaced(plantType);
}

int HUD::getSelectedPlantCostFromSeedManager() const
{
    return m_seedManager.getSelectedPlantCost();
}
