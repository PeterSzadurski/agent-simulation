#include "KnowledgeSystem.h"
KnowledgeSystem::KnowledgeSystem(const Grid &grid) : m_grid(grid)
{
}

bool KnowledgeSystem::updateLineOfSight(std::shared_ptr<Entity> entity)
{
    bool foundSomething = false;
    auto &pos = entity->get<CPosition>();
    auto &knowledge = entity->get<CKnowledge>();
    int lineOfSight = entity->get<CLineOfSight>().sight();
    for (int x = pos.cords.x - lineOfSight; x < pos.cords.x + lineOfSight; ++x)
    {
        for (int y = pos.cords.y - lineOfSight; y < pos.cords.y + lineOfSight; ++y)
        {
            if ((x != pos.cords.x && y != pos.cords.y) && m_grid.inBounds(x, y) && m_grid.at(x, y) != nullptr)
            {
                // something is within line of sight
                auto thing = m_grid.at(x, y);
                entity->get<CKnowledge>().update(thing->get<CPosition>().cords, thing->type());
                foundSomething = true;
            }
        }
    }

    return foundSomething;
}
