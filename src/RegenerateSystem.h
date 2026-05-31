#pragma once
#include "EntityManager.h"
class RegenerateSystem
{
public:
    RegenerateSystem();
    void update(const int tick, EntityManager &em);
};
