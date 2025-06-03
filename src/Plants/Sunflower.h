#pragma once
#include "../Entities/Plant.h"
#include "../Utils/Constants.h"

class PlantManager; // 前向声明

class Sunflower : public Plant
{
public:
    Sunflower(ResourceManager &resManager, const sf::Vector2i &gridPos, Grid &gridSystem, PlantManager &plantManager);
    ~Sunflower() override = default;
    void update(float dt) override;

private:
    float m_produceSunTimer;
    PlantManager &m_plantManagerRef;
};
