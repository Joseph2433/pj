#pragma once

#include <SFML/Graphics.hpp>
#include "SeedManager.h" // Your SeedManager class

// Forward declarations
class ResourceManager;
class SunManager;

class HUD
{
public:
    HUD(ResourceManager &resManager, SunManager &sunManager,
        sf::Font &primaryFont, sf::Font &secondaryFont); // Pass fonts needed by SeedManager and HUD itself

    void handleEvent(const sf::Event &event, const sf::Vector2f &mousePosInView);
    void update(float dt); // dt for SeedManager's SeedPacket cooldowns
    void draw(sf::RenderWindow &window);

    // Interface for GamePlayState to interact with the seed selection
    PlantType getSelectedPlantTypeFromSeedManager(bool &isValidSelection) const;
    void notifyPlantPlacedToSeedManager(PlantType plantType);
    int getSelectedPlantCostFromSeedManager() const;

private:
    SeedManager m_seedManager; // Manages the seed packets bar

    // HUD's own UI elements, like sun display
    sf::Text m_sunDisplayText;
    // sf::Sprite m_sunIcon; // Optional sun icon next to the display

    // References for its own elements if needed, or passed down
    SunManager &m_sunManagerRef;
    sf::Font &m_primaryFontRef; // Font for sun display, etc.
};
