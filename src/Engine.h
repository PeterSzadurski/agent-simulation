#include "EntityManager.h"
#include <random>
#include "Grid.h"
#include "KnowledgeSystem.h"
class Engine
{
    std::mt19937 m_rng;
    int m_tick;
    EntityManager m_entities;
    Grid m_grid;
    KnowledgeSystem m_knowledge;
    void moveRand(std::shared_ptr<Entity> e);
    void hungerSystem(std::shared_ptr<Entity> e);
    void movementSystem(std::shared_ptr<Entity> e);

public:
    Engine(u_int32_t seed, int width, int height);
    int getTick();
    void simulate();
};
