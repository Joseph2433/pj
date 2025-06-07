
#include "SeedManager.h"
#include "../Core/ResourceManager.h"
#include "../Systems/SunManager.h"
#include "../Utils/Constants.h"
#include <SFML/Window/Event.hpp>
#include <iostream>

SeedManager::SeedManager(ResourceManager &resManager, SunManager &sunManager,
                         sf::Font &primaryFont, sf::Font &secondaryFont)
    : m_hasActiveSelection(false),
      m_resourceManagerRef(resManager),
      m_sunManagerRef(sunManager),
      m_primaryFontRef(primaryFont),
      m_secondaryFontRef(secondaryFont),
      m_uiPosition(SEED_PACKET_UI_START_X, SEED_PACKET_UI_START_Y),
      m_packetSize(SEED_PACKET_WIDTH, SEED_PACKET_HEIGHT),
      m_packetSpacing(SEED_PACKET_SPACING)
{

    initializeSeedPackets();
}

void SeedManager::initializeSeedPackets()
{
    m_seedPackets.clear();

    addSeedPacket(PlantType::SUNFLOWER, SUNFLOWER_ICON_TEXTURE_KEY, SUNFLOWER_COST, SUNFLOWER_COOLDOWN_TIME);
    addSeedPacket(PlantType::PEASHOOTER, PEASHOOTER_ICON_TEXTURE_KEY, PEASHOOTER_COST, PEASHOOTER_COOLDOWN_TIME);
    addSeedPacket(PlantType::WALLNUT, WALLNUT_ICON_TEXTURE_KEY, WALLNUT_COST, WALLNUT_COOLDOWN_TIME);
    addSeedPacket(PlantType::ICEPEASHOOTER, ICE_PEASHOOTER_ICON_TEXTURE_KEY, ICE_PEASHOOTER_COST, ICE_PEASHOOTER_COOLDOWN_TIME);
}

void SeedManager::addSeedPacket(PlantType type, const std::string &textureKey, int cost, float cooldownTime)
{
    sf::Vector2f packetPosition(
        m_uiPosition.x + m_seedPackets.size() * (m_packetSize.x + m_packetSpacing),
        m_uiPosition.y);

    m_seedPackets.emplace_back(type, cost, cooldownTime,
                               m_resourceManagerRef, textureKey,
                               m_primaryFontRef, m_secondaryFontRef,
                               packetPosition, m_packetSize);
}

bool SeedManager::handleEvent(const sf::Event &event, const sf::Vector2f &mousePosInView)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            for (const auto &packet : m_seedPackets)
            {

                if (packet.handleClick(mousePosInView))
                {
                    selectSeedPacket(packet.getPlantType());
                    std::cout << "SeedManager: Clicked and selected packet type " << static_cast<int>(packet.getPlantType()) << std::endl;
                    return true;
                }
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Right)
        {

            sf::FloatRect seedBarBounds(m_uiPosition.x, m_uiPosition.y,
                                        m_seedPackets.size() * (m_packetSize.x + m_packetSpacing) - (m_seedPackets.empty() ? 0 : m_packetSpacing),
                                        m_packetSize.y);
            if (seedBarBounds.contains(mousePosInView))
            {
                deselectAllPackets();
                return true;
            }
        }
    }
    return false;
}

void SeedManager::update(float dt)
{

    int currentSun = m_sunManagerRef.getCurrentSun();
    for (auto &packet : m_seedPackets)
    {
        bool isThisPacketSelected = m_hasActiveSelection && (packet.getPlantType() == m_selectedPlantType);
        packet.update(dt, currentSun, isThisPacketSelected);
    }
}

void SeedManager::draw(sf::RenderWindow &window) const
{
    for (const auto &packet : m_seedPackets)
    {
        packet.draw(window);
    }
}

PlantType SeedManager::getSelectedPlantType(bool &isValidSelection) const
{

    isValidSelection = false;
    if (m_hasActiveSelection)
    {
        for (const auto &packet : m_seedPackets)
        {
            if (packet.getPlantType() == m_selectedPlantType)
            {
                if (packet.isAffordable(m_sunManagerRef.getCurrentSun()) && !packet.isOnCooldown())
                {
                    isValidSelection = true;
                }
                break;
            }
        }
        return m_selectedPlantType;
    }
    return PlantType::SUNFLOWER;
}

int SeedManager::getSelectedPlantCost() const
{

    if (m_hasActiveSelection)
    {
        for (const auto &packet : m_seedPackets)
        {
            if (packet.getPlantType() == m_selectedPlantType)
            {
                return packet.getCost();
            }
        }
    }
    return 0;
}

void SeedManager::notifyPlantPlaced(PlantType plantType)
{

    for (auto &packet : m_seedPackets)
    {
        if (packet.getPlantType() == plantType)
        {
            packet.startCooldown();
            break;
        }
    }
    deselectAllPackets();
}

void SeedManager::selectSeedPacket(PlantType type)
{
    for (const auto &packet : m_seedPackets)
    {
        if (packet.getPlantType() == type)
        {
            if (!packet.isOnCooldown() && packet.isAffordable(m_sunManagerRef.getCurrentSun()))
            {
                m_selectedPlantType = type;
                m_hasActiveSelection = true;
            }
            return;
        }
    }
}

void SeedManager::deselectAllPackets()
{
    if (m_hasActiveSelection)
    {
        m_hasActiveSelection = false;
        std::cout << "SeedManager: Deselected all packets." << std::endl;
    }
}
