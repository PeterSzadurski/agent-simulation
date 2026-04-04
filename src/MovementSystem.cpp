#include "MovementSystem.h"

MovementSystem::MovementSystem(Grid &grid) : m_grid(grid)
{
}

bool MovementSystem::moveRand(std::shared_ptr<Entity> e, std::mt19937 &rng)
{
    auto &pos = e->get<CPosition>();
    std::uniform_int_distribution<int> dist(-1, 1);
    int x = dist(rng);
    int y = dist(rng);
    return m_grid.move(e, x, y);
}

bool MovementSystem::moveTo(std::shared_ptr<Entity> e)
{
    auto &pos = e->get<CPosition>();
    Cords direction = Cords::normal(e->get<CDesination>().cords, pos.cords);
    return m_grid.move(e, direction.x, direction.y);
}

bool MovementSystem::nextToDestination(std::shared_ptr<Entity> e)
{
    auto &pos = e->get<CPosition>();
    if (e->has<CDesination>())
    {
        auto &dest = e->get<CDesination>();
        int x = pos.cords.x - dest.cords.x;
        int y = pos.cords.y - dest.cords.y;
        return abs(x) <= 1 && abs(y) <= 1;
    }
    return false;
}
