#include "EntityManager.h"
#include <random>
#include "Grid.h"
class Engine
{
    std::mt19937 m_rng;
    u_int32_t m_tick;
    EntityManager m_entities;
    Grid m_grid;
    void moveRand(std::shared_ptr<Entity> e);
    void hungerSystem(std::shared_ptr<Entity> e);

public:
    Engine(u_int32_t seed, int width, int height);
    u_int32_t getTick();
    void simulate();
};
