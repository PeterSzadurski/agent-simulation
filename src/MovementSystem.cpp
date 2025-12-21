#include "MovementSystem.h"

MovementSystem::MovementSystem(Grid &grid) : m_grid(grid)
{
}

void MovementSystem::moveRand(std::shared_ptr<Entity> e, std::mt19937 &rng)
{
    auto &pos = e->get<CPosition>();
    std::uniform_int_distribution<int> dist(-1, 1);
    int x = dist(rng);
    int y = dist(rng);
    m_grid.move(e, x, y);
}

void MovementSystem::moveTo(std::shared_ptr<Entity> e)
{
    auto &pos = e->get<CPosition>();
    Cords direction = Cords::normal(pos.destination.value(), pos.cords);
    m_grid.move(e, direction.x, direction.y);
}

bool MovementSystem::nextToDestination(std::shared_ptr<Entity> e)
{
    auto &pos = e->get<CPosition>();
    int x = pos.cords.x - pos.destination.value().x;
    int y = pos.cords.y - pos.destination.value().y;
    return x <= 1 && y <= 1;
}
