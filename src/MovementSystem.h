#pragma once
#include "Grid.h"

class MovementSystem
{
    Grid &m_grid;

public:
    MovementSystem(Grid &grid);
    void moveRand(std::shared_ptr<Entity> e, std::mt19937 &rng);
    void moveTo(std::shared_ptr<Entity> e);
    bool nextToDestination(std::shared_ptr<Entity> e);
};
