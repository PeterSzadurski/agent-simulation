#include "EntityManager.h"
#include <random>
#include "Grid.h"
#include "KnowledgeSystem.h"
// #include "MovementSystem.h"
#include "DecisionSystem.h"
#include "DecaySystem.h"
#include "ActionSystem.h"

class Engine
{
    std::mt19937 m_rng;
    int m_tick;
    EntityManager m_entities;
    Grid m_grid;
    KnowledgeSystem m_knowledge;
    MovementSystem m_movement;
    DecisionSystem m_decision;
    DecaySystem m_decay;
    ActionSystem m_action;
    std::vector<std::pair<entity_type, Cords>> m_pendingDrops;
    void movementSystem();
    void lineOfSightSystem();
    void cleanGrid();

public:
    Engine(u_int32_t seed, int width, int height);
    int getTick();
    void simulate();
};
