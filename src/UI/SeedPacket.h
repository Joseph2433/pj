#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "../Systems/PlantManager.h"

class ResourceManager;

class SeedPacket
{
public:
    SeedPacket(PlantType type, int cost, float cooldownTime,
               ResourceManager &resManager,
               const std::string &plantIconTextureKey,
               sf::Font &primaryFont,   // For cost text
               sf::Font &secondaryFont, // For cooldown text (optional)
               const sf::Vector2f &position,
               const sf::Vector2f &size);

    void update(float dt, int currentSun, bool isExternallySelected);
    void draw(sf::RenderWindow &window) const;

    bool handleClick(const sf::Vector2f &mousePos) const; // const because it doesn't change state, just checks
    void startCooldown();

    PlantType getPlantType() const;
    int getCost() const;
    bool isAffordable(int currentSun) const;
    bool isOnCooldown() const;
    bool isSelected() const; // Reflects if SeedManager has marked it as selected

private:
    void updateAppearance(int currentSun); // Visual update based on state

    PlantType m_plantType;
    int m_cost;
    float m_cooldownTimeTotal;
    float m_currentCooldown;

    bool m_isExternallySelected; // Set by SeedManager

    // Visual components
    sf::RectangleShape m_background;
    sf::Sprite m_plantIconSprite;
    sf::Text m_costText;
    sf::RectangleShape m_cooldownOverlay;
    sf::Text m_cooldownText; // Optional: displays cooldown numbers

    // References (not owned)
    ResourceManager &m_resManagerRef;
    sf::Font &m_primaryFontRef;
    sf::Font &m_secondaryFontRef;

    sf::Vector2f m_position;
    sf::Vector2f m_size;
};
