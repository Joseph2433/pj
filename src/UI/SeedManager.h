#pragma once

#include "SeedPacket.h"
#include <vector>
#include <SFML/System.hpp>

// 前向声明
class ResourceManager;
class SunManager;
namespace sf
{
    class Font;
    class RenderWindow;
    class Event;
}

class SeedManager
{ // <--- 类名已更改
public:
    SeedManager(ResourceManager &resManager, SunManager &sunManager,
                sf::Font &primaryFont, sf::Font &secondaryFont);

    void handleEvent(const sf::Event &event, const sf::Vector2f &mousePosInView);
    void update(float dt);
    void draw(sf::RenderWindow &window) const;

    PlantType getSelectedPlantType(bool &isValidSelection) const;
    void notifyPlantPlaced(PlantType plantType);
    int getSelectedPlantCost() const;

private:
    void initializeSeedPackets(); // 方法名也可以调整以匹配
    void addSeedPacket(PlantType type, const std::string &textureKey, int cost, float cooldownTime);
    void selectSeedPacket(PlantType type);
    void deselectAllPackets();

    std::vector<SeedPacket> m_seedPackets;
    PlantType m_selectedPlantType;
    bool m_hasActiveSelection;

    ResourceManager &m_resourceManagerRef;
    SunManager &m_sunManagerRef;
    sf::Font &m_primaryFontRef;
    sf::Font &m_secondaryFontRef;

    sf::Vector2f m_uiPosition;
    sf::Vector2f m_packetSize;
    float m_packetSpacing;
};
