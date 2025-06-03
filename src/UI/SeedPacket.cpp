#include "SeedPacket.h"
#include "../Core/ResourceManager.h"
#include "../Utils/Constants.h"
#include <iomanip>  // for std::fixed, std::setprecision
#include <sstream>  // for std::stringstream
#include <iostream> // For debugging

SeedPacket::SeedPacket(PlantType type, int cost, float cooldownTime,
                       ResourceManager &resManager,
                       const std::string &plantIconTextureKey,
                       sf::Font &primaryFont, sf::Font &secondaryFont,
                       const sf::Vector2f &position, const sf::Vector2f &size)
    : m_plantType(type), m_cost(cost), m_cooldownTimeTotal(cooldownTime), m_currentCooldown(0.0f),
      m_isExternallySelected(false),
      m_resManagerRef(resManager), m_primaryFontRef(primaryFont), m_secondaryFontRef(secondaryFont),
      m_position(position), m_size(size)
{

    // Background
    m_background.setPosition(m_position);
    m_background.setSize(m_size);
    m_background.setOutlineThickness(1.5f); // Slightly thicker outline

    // Plant Icon
    m_plantIconSprite.setTexture(m_resManagerRef.getTexture(plantIconTextureKey));
    sf::FloatRect iconLocalBounds = m_plantIconSprite.getLocalBounds();
    float iconScaleX = (m_size.x * SEED_PACKET_ICON_SCALE_FACTOR) / iconLocalBounds.width;
    float iconScaleY = (m_size.y * SEED_PACKET_ICON_SCALE_FACTOR * 0.7f) / iconLocalBounds.height; // Leave space for cost
    float finalIconScale = std::min(iconScaleX, iconScaleY);
    m_plantIconSprite.setScale(finalIconScale, finalIconScale);
    m_plantIconSprite.setOrigin(iconLocalBounds.left + iconLocalBounds.width / 2.f,
                                iconLocalBounds.top + iconLocalBounds.height / 2.f);
    m_plantIconSprite.setPosition(m_position.x + m_size.x / 2.f,
                                  m_position.y + m_size.y * 0.40f); // Position icon in upper part

    // Cost Text
    m_costText.setFont(m_primaryFontRef);
    m_costText.setString(std::to_string(m_cost));
    m_costText.setCharacterSize(SEED_PACKET_COST_FONT_SIZE);
    sf::FloatRect costTextBounds = m_costText.getLocalBounds();
    m_costText.setOrigin(costTextBounds.left + costTextBounds.width / 2.f,
                         costTextBounds.top + costTextBounds.height / 2.f);
    m_costText.setPosition(m_position.x + m_size.x / 2.f,
                           m_position.y + m_size.y * 0.85f); // Position cost at the bottom

    // Cooldown Overlay
    m_cooldownOverlay.setPosition(m_position);
    m_cooldownOverlay.setFillColor(sf::Color(30, 30, 30, static_cast<sf::Uint8>(SEED_PACKET_COOLDOWN_OVERLAY_ALPHA)));
    m_cooldownOverlay.setSize(sf::Vector2f(m_size.x, 0)); // Initially no overlay

    // Cooldown Text (Optional)
    m_cooldownText.setFont(m_secondaryFontRef); // Use secondary font for cooldown
    m_cooldownText.setCharacterSize(SEED_PACKET_COOLDOWN_FONT_SIZE);
    m_cooldownText.setFillColor(sf::Color::White);
    // Positioning for cooldown text will be done in update based on overlay

    updateAppearance(0); // Initial appearance based on 0 sun (likely unaffordable)
}

void SeedPacket::update(float dt, int currentSun, bool isExternallySelected)
{
    m_isExternallySelected = isExternallySelected;

    if (m_currentCooldown > 0.0f)
    {
        m_currentCooldown -= dt;
        if (m_currentCooldown < 0.0f)
        {
            m_currentCooldown = 0.0f;
        }
        // Update overlay height
        float heightRatio = m_currentCooldown / m_cooldownTimeTotal;
        m_cooldownOverlay.setSize(sf::Vector2f(m_size.x, m_size.y * heightRatio));
        // Overlay typically grows from top to bottom, so position it at the top of the card.
        m_cooldownOverlay.setPosition(m_position.x, m_position.y);

        // Update cooldown text (optional)
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << m_currentCooldown;
        m_cooldownText.setString(ss.str());
        sf::FloatRect cdTextBounds = m_cooldownText.getLocalBounds();
        m_cooldownText.setOrigin(cdTextBounds.left + cdTextBounds.width / 2.f,
                                 cdTextBounds.top + cdTextBounds.height / 2.f);
        m_cooldownText.setPosition(m_position.x + m_size.x / 2.f,
                                   m_position.y + m_size.y / 2.f); // Center of the card
    }
    else
    {
        m_cooldownOverlay.setSize(sf::Vector2f(0, 0)); // Hide overlay
        m_cooldownText.setString("");
    }

    updateAppearance(currentSun);
}

void SeedPacket::updateAppearance(int currentSun)
{
    bool affordable = isAffordable(currentSun);
    bool onCD = isOnCooldown();

    sf::Color bgColor, outlineColor, iconColor, costTextColor;

    if (onCD)
    {
        bgColor = sf::Color(70, 70, 70, 200); // Dark grey when on cooldown
        outlineColor = sf::Color(100, 100, 100);
        iconColor = sf::Color(120, 120, 120, 180); // Dim icon
        costTextColor = sf::Color(150, 150, 150);
    }
    else if (!affordable)
    {
        bgColor = sf::Color(120, 70, 70, 200); // Reddish tint when unaffordable
        outlineColor = sf::Color(150, 100, 100);
        iconColor = sf::Color(180, 120, 120, 180); // Dim icon, reddish
        costTextColor = sf::Color(200, 150, 150);
    }
    else if (m_isExternallySelected)
    {
        bgColor = sf::Color(100, 220, 100, 220); // Bright green when selected and available
        outlineColor = sf::Color::Yellow;
        iconColor = sf::Color::White;
        costTextColor = sf::Color::Black;
    }
    else
    {                                            // Available but not selected
        bgColor = sf::Color(100, 150, 100, 200); // Normal green/grey available state
        outlineColor = sf::Color(150, 200, 150);
        iconColor = sf::Color::White;
        costTextColor = sf::Color::Black;
    }

    m_background.setFillColor(bgColor);
    m_background.setOutlineColor(outlineColor);
    m_plantIconSprite.setColor(iconColor);
    m_costText.setFillColor(costTextColor);
}

void SeedPacket::draw(sf::RenderWindow &window) const
{
    window.draw(m_background);
    window.draw(m_plantIconSprite);
    window.draw(m_costText);

    if (isOnCooldown())
    {
        window.draw(m_cooldownOverlay);
        if (!m_cooldownText.getString().isEmpty())
        { // Only draw if there's text
            window.draw(m_cooldownText);
        }
    }
}

bool SeedPacket::handleClick(const sf::Vector2f &mousePos) const
{
    return m_background.getGlobalBounds().contains(mousePos);
}

void SeedPacket::startCooldown()
{
    if (m_cooldownTimeTotal > 0.0f)
    {
        m_currentCooldown = m_cooldownTimeTotal;
        std::cout << "SeedPacket for type " << static_cast<int>(m_plantType) << " starting cooldown: " << m_currentCooldown << "s" << std::endl;
    }
}

PlantType SeedPacket::getPlantType() const { return m_plantType; }
int SeedPacket::getCost() const { return m_cost; }
bool SeedPacket::isAffordable(int currentSun) const { return currentSun >= m_cost; }
bool SeedPacket::isOnCooldown() const { return m_currentCooldown > 0.001f; } // Use small epsilon
bool SeedPacket::isSelected() const { return m_isExternallySelected; }
