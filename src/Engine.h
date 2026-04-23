#include "EntityManager.h"
#include <random>
#include "Grid.h"
#include "KnowledgeSystem.h"
// #include "MovementSystem.h"
#include "DecisionSystem.h"
#include "DecaySystem.h"
#include "ActionSystem.h"

typedef std::pair<int, int> NpcFeat;

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
    std::vector<EntityPos> m_pendingDrops;
    const int m_width, m_height;
    const int m_spawnRate = 500;

    NpcFeat m_bestHunter;
    NpcFeat m_bestLumberjack;
    NpcFeat m_bestCook;
    NpcFeat m_mostHungry;
    Statistics m_statistics;

    void movementSystem();
    void lineOfSightSystem();
    void cleanGrid();
    void spawnSystem();
    void featSystem();

    int randRange(int min, int max);
    void spawnNpc();
    void spawnDeer();
    void spawnGrass();
    void spawnTree();

public:
    Engine(uint32_t seed, int width, int height);
    int getTick();
    void simulate();
    void printFeats();
};
