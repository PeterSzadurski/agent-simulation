#pragma once
#include "Grid.h"

class KnowledgeSystem
{
private:
    const Grid &m_grid;
    void updateEntityKnowledge(CPosition &pos, CKnowledge &knowledge, const int currentTick, Cords cords, entity_type type);

public:
    KnowledgeSystem(const Grid &grid);
    bool updateLineOfSight(std::shared_ptr<Entity> entity, const int currentTick);
    bool findNearestEntityType(std::shared_ptr<Entity> entity, entity_type e_type);
};
