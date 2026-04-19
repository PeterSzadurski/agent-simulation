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
    auto &lineOfSight = entity->get<CLineOfSight>();

    lineOfSight.m_detectedEntities.clear();

    for (int x = pos.cords.x - lineOfSight.sight(); x < pos.cords.x + lineOfSight.sight(); ++x)
    {
        for (int y = pos.cords.y - lineOfSight.sight(); y < pos.cords.y + lineOfSight.sight(); ++y)
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
                    lineOfSight.m_detectedEntities.push_back(EntityPos(seenEntity->type(), pos.cords));
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
    knowledge.m_closest_deer.reset();

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
        if (seen.type == deer)
        {
            updateIfNearer(cords, knowledge.m_closest_deer);
        }
    }
}
