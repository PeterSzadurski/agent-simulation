#include "EntityManager.h"
#include <random>
class Engine
{
    std::mt19937 rng;
    u_int32_t tick;
    EntityManager m_entities;
    void moveRand(std::shared_ptr<Entity> e);

public:
    Engine(u_int32_t seed);
    u_int32_t getTick();
    void simulate();
};
