// src/UI/SeedManager.cpp
#include "SeedManager.h"
#include "../Core/ResourceManager.h"
#include "../Systems/SunManager.h"
#include "../Utils/Constants.h"
#include <SFML/Window/Event.hpp>
#include <iostream>

// 构造函数名更改
SeedManager::SeedManager(ResourceManager &resManager, SunManager &sunManager,
                         sf::Font &primaryFont, sf::Font &secondaryFont)
    : m_hasActiveSelection(false),
      m_resourceManagerRef(resManager),
      m_sunManagerRef(sunManager),
      m_primaryFontRef(primaryFont),
      m_secondaryFontRef(secondaryFont),
      // 从 Constants.h 读取种子包栏的布局常量
      m_uiPosition(SEED_PACKET_UI_START_X, SEED_PACKET_UI_START_Y),
      m_packetSize(SEED_PACKET_WIDTH, SEED_PACKET_HEIGHT),
      m_packetSpacing(SEED_PACKET_SPACING)
{

    initializeSeedPackets();
}

void SeedManager::initializeSeedPackets()
{ // 方法名可以保持或更改
    m_seedPackets.clear();
    // 使用 Constants.h 中定义的纹理键、花费和冷却时间
    addSeedPacket(PlantType::SUNFLOWER, SUNFLOWER_ICON_TEXTURE_KEY, SUNFLOWER_COST, SUNFLOWER_COOLDOWN_TIME);
    addSeedPacket(PlantType::PEASHOOTER, PEASHOOTER_ICON_TEXTURE_KEY, PEASHOOTER_COST, PEASHOOTER_COOLDOWN_TIME);
    // ... add other plants
}

// addSeedPacket, selectSeedPacket, deselectAllPackets, handleEvent, update, draw,
// getSelectedPlantType, getSelectedPlantCost, notifyPlantPlaced 方法的主体逻辑不变,
// 只需要确保它们的定义是 SeedManager::methodName
// 例如:
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

void SeedManager::handleEvent(const sf::Event &event, const sf::Vector2f &mousePosInView)
{
    // (逻辑同之前的 PlantCardUI::handleEvent)
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            bool clickedOnPacket = false;
            for (const auto &packet : m_seedPackets)
            {
                if (packet.handleClick(mousePosInView))
                {
                    selectSeedPacket(packet.getPlantType());
                    clickedOnPacket = true;
                    break;
                }
            }
        }
    }
}

void SeedManager::update(float dt)
{
    // (逻辑同之前的 PlantCardUI::update)
    int currentSun = m_sunManagerRef.getCurrentSun();
    for (auto &packet : m_seedPackets)
    {
        bool isThisPacketSelected = m_hasActiveSelection && (packet.getPlantType() == m_selectedPlantType);
        packet.update(dt, currentSun, isThisPacketSelected);
    }
}

void SeedManager::draw(sf::RenderWindow &window) const
{
    // (逻辑同之前的 PlantCardUI::draw)
    for (const auto &packet : m_seedPackets)
    {
        packet.draw(window);
    }
}

PlantType SeedManager::getSelectedPlantType(bool &isValidSelection) const
{
    // (逻辑同之前的 PlantCardUI::getSelectedPlantType)
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
    // (逻辑同之前的 PlantCardUI::getSelectedPlantCost)
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
    // (逻辑同之前的 PlantCardUI::notifyPlantPlaced)
    for (auto &packet : m_seedPackets)
    {
        if (packet.getPlantType() == plantType)
        {
            packet.startCooldown();
            break;
        }
    }
}
// ... 实现 selectSeedPacket 和 deselectAllPackets ...
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
            else
            {
                // m_hasActiveSelection = false; // 可选
            }
            return;
        }
    }
}

void SeedManager::deselectAllPackets()
{
    m_hasActiveSelection = false;
}
