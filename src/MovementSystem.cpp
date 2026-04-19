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
    Cords direction = Cords::normal(e->get<CDestination>().cords, pos.cords);
    if (m_grid.move(e, direction.x, direction.y))
        return true;

    // single direction fallbacks
    if (direction.x != 0 && m_grid.move(e, direction.x, 0))
        return true;
    if (direction.y != 0 && m_grid.move(e, 0, direction.y))
        return true;

    return false;
}

bool MovementSystem::nextToDestination(std::shared_ptr<Entity> e)
{
    auto &pos = e->get<CPosition>();
    if (e->has<CDestination>())
    {
        auto &dest = e->get<CDestination>();
        return isNextToCord(pos.cords, dest.cords);
    }
    return false;
}

bool MovementSystem::moveAwayFrom(std::shared_ptr<Entity> e, const Cords &threat)
{
    auto &pos = e->get<CPosition>();
    Cords direction = Cords::normal(pos.cords, threat);
    return m_grid.move(e, direction.x, direction.y);
}
