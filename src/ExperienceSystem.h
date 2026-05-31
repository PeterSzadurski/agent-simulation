#pragma once
#include "EntityManager.h"
class ExperienceSystem
{
    float calcNextLevel(float rate, int level);

public:
    ExperienceSystem();
    void update(int tick, EntityManager &em, Statistics &statistics);
};
