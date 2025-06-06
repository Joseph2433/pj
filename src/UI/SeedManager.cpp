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
    addSeedPacket(PlantType::WALLNUT, WALLNUT_ICON_TEXTURE_KEY, WALLNUT_COST, WALLNUT_COOLDOWN_TIME);
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

bool SeedManager::handleEvent(const sf::Event &event, const sf::Vector2f &mousePosInView)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            for (const auto &packet : m_seedPackets)
            {
                // 假设 SeedPacket::handleClick(const sf::Vector2f&) const 只是检查是否点中，不改变状态
                if (packet.handleClick(mousePosInView))
                {
                    selectSeedPacket(packet.getPlantType()); // selectSeedPacket 会更新 SeedManager 的状态
                    std::cout << "SeedManager: Clicked and selected packet type " << static_cast<int>(packet.getPlantType()) << std::endl;
                    return true; // 事件被此种子包消耗
                }
            }
            // 如果点击了种子包栏的空白区域，也可以考虑取消选择并消耗事件
            // sf::FloatRect seedBarBounds = ... ;
            // if (seedBarBounds.contains(mousePosInView)) {
            //     deselectAllPackets();
            //     return true;
            // }
        }
        else if (event.mouseButton.button == sf::Mouse::Right)
        {
            // 如果希望右键点击种子包栏的任何地方都取消选择
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
    deselectAllPackets();
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
