#include "KnowledgeSystem.h"
KnowledgeSystem::KnowledgeSystem(const Grid &grid) : m_grid(grid)
{
}

void KnowledgeSystem::updateEntityKnowledge(CPosition &pos, CKnowledge &knowledge, const int currentTick, Cords cords, entity_type type)
{
    knowledge.m_reported_positions[cords] = Seen(type, currentTick);
    if (type == entity_type::raw_meat)
    {
        if (!knowledge.m_closest_food.has_value())
        {
            knowledge.m_closest_food = cords;
        }
        else
        {
            float distA = Distance2(pos.cords, knowledge.m_closest_food.value());
            float distB = Distance2(pos.cords, cords);

            if (distA > distB)
            {
                knowledge.m_closest_food = cords;
            }
        }
    }
    else if (type == entity_type::tree)
    {

        if (!knowledge.m_closest_tree.has_value())
        {
            knowledge.m_closest_tree = cords;
        }
        else
        {
            float distA = Distance2(pos.cords, knowledge.m_closest_tree.value());
            float distB = Distance2(pos.cords, cords);

            if (distA > distB)
            {
                knowledge.m_closest_tree = cords;
            }
        }
    }
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
                    updateEntityKnowledge(pos, knowledge, currentTick, cords, entity_type::empty);
                }
                else if (m_grid.at(x, y) != nullptr)
                {
                    auto seenEntity = m_grid.at(x, y);
                    updateEntityKnowledge(pos, knowledge, currentTick, cords, seenEntity->type());
                    foundSomething = true;
                }
            }
        }
    }

    return foundSomething;
}

bool KnowledgeSystem::findNearestEntityType(std::shared_ptr<Entity> entity, entity_type e_type)
{
    auto &pos = entity->get<CPosition>();
    auto &knowledge = entity->get<CKnowledge>();

    for (const auto &[key, value] : knowledge.m_reported_positions)
    {
        if (knowledge.m_reported_positions[key].type == e_type)
        {
            entity->add<CDestination>(key.x, key.y);

            return true;
        }
    }
    return false;
}
