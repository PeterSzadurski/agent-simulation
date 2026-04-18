#include "KnowledgeSystem.h"
KnowledgeSystem::KnowledgeSystem(const Grid &grid) : m_grid(grid)
{
}

void KnowledgeSystem::updateEntityKnowledge(CPosition &pos, CKnowledge &knowledge, const int currentTick, Cords cords, entity_type type)
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
    recalculateClosest(entity);
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

void KnowledgeSystem::recalculateClosest(std::shared_ptr<Entity> entity)
{

    auto &knowledge = entity->get<CKnowledge>();
    auto &pos = entity->get<CPosition>();

    if (knowledge.m_lastRecalcPosition == pos.cords)
    {
        return;
    }

    auto updateIfNearer = [&pos](const Cords &candidate, std::optional<Cords> &closest)
    {
        if (!closest.has_value())
        {
            closest = candidate;
        }
        else
        {
            float currentDist = Distance2(pos.cords, closest.value());
            float candidateDist = Distance2(pos.cords, candidate);
            if (candidateDist < currentDist)
            {
                closest = candidate;
            }
        }
    };

    knowledge.m_lastRecalcPosition = pos.cords;
    knowledge.m_closest_food.reset();
    knowledge.m_closest_tree.reset();
    knowledge.m_closest_grass.reset();

    for (const auto &[cords, seen] : knowledge.m_reported_positions)
    {

        if (seen.type == raw_meat)
        {
            updateIfNearer(cords, knowledge.m_closest_food);
        }
        if (seen.type == tree)
        {
            updateIfNearer(cords, knowledge.m_closest_tree);
        }
        if (seen.type == grass)
        {
            updateIfNearer(cords, knowledge.m_closest_grass);
        }
    }
}
