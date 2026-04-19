#pragma once
#include "Grid.h"

class MovementSystem
{
    Grid &m_grid;

public:
    MovementSystem(Grid &grid);
    bool moveRand(std::shared_ptr<Entity> e, std::mt19937 &rng);
    bool moveTo(std::shared_ptr<Entity> e);
    bool nextToDestination(std::shared_ptr<Entity> e);
    bool moveAwayFrom(std::shared_ptr<Entity> e, const Cords &threat);
};
