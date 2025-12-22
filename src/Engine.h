#include "EntityManager.h"
#include <random>
#include "Grid.h"
#include "KnowledgeSystem.h"
#include "MovementSystem.h"
class Engine
{
    std::mt19937 m_rng;
    int m_tick;
    EntityManager m_entities;
    Grid m_grid;
    KnowledgeSystem m_knowledge;
    MovementSystem m_movement;
    void hungerSystem();
    void movementSystem();
    void actionSystem();
    void lineOfSightSystem();
    void cleanGrid();

public:
    Engine(u_int32_t seed, int width, int height);
    int getTick();
    void simulate();
};
