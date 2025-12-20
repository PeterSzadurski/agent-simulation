#pragma once
#include "Grid.h"

class KnowledgeSystem
{
private:
    Grid m_grid;

public:
    KnowledgeSystem(const Grid &grid);
    bool updateLineOfSight(std::shared_ptr<Entity> entity);
};
