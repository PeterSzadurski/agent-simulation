#include "KnowledgeSystem.h"
KnowledgeSystem::KnowledgeSystem(const Grid &grid) : m_grid(grid)
{
}

void KnowledgeSystem::updateEntityKnowledge(CKnowledge &knowledge, const int currentTick, Cords cords, entity_type type)
{
    knowledge.m_reported_positions[cords] = Seen(type, currentTick);
}

bool KnowledgeSystem::updateLineOfSight(std::shared_ptr<Entity> entity, const int currentTick)
{
    bool foundSomething = false;
    auto &pos = entity->get<CPosition>();
    auto &knowledge = entity->get<CKnowledge>();
    int lineOfSight = entity->get<CLineOfSight>().sight();
    for (int x = pos.cords.x - lineOfSight; x < pos.cords.x + lineOfSight; ++x)
    {
        for (int y = pos.cords.y - lineOfSight; y < pos.cords.y + lineOfSight; ++y)
        {
            if ((x != pos.cords.x || y != pos.cords.y) && m_grid.inBounds(x, y))
            {
                Cords cords = Cords(x, y);
                if (m_grid.at(x, y) == nullptr && knowledge.m_reported_positions.contains(cords))
                {
                    updateEntityKnowledge(knowledge, currentTick, cords, entity_type::empty);
                }
                else if (m_grid.at(x, y) != nullptr)
                {
                    auto seenEntity = m_grid.at(x, y);
                    updateEntityKnowledge(knowledge, currentTick, cords, seenEntity->type());
                    foundSomething = true;
                }
            }
        }
    }

    return foundSomething;
}

bool KnowledgeSystem::findNearestMeal(std::shared_ptr<Entity> entity)
{
    auto &pos = entity->get<CPosition>();
    auto &knowledge = entity->get<CKnowledge>();

    for (const auto &[key, value] : knowledge.m_reported_positions)
    {
        if (knowledge.m_reported_positions[key].type == entity_type::meal)
        {
            pos.destination = key;
            return true;
        }
    }
    return false;
}
