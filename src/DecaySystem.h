#pragma once
#include "EntityManager.h"

class DecaySystem
{
private:
    void systemProcess(int tick, std::shared_ptr<Entity> e, CDecay &decay, std::string debugStr);

public:
    DecaySystem();
    void update(int tick, EntityManager &em);
};
