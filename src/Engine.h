#include "EntityManager.h"
#include <random>
#include "Grid.h"
#include "KnowledgeSystem.h"
// #include "MovementSystem.h"
#include "DecisionSystem.h"
#include "DecaySystem.h"
#include "ActionSystem.h"

typedef std::pair<int, int> NpcFeat;

struct EntityInspectData
{
    int x = 0;
    int y = 0;
    int type = 0;
    bool exists = false;

    bool hasStats = false;
    int hitPoints = 0;
    int strength = 0;
    int speed = 0;

    bool hasHunger = false;
    int hunger = 0;
    bool hasFuel = false;
    int fuel = 0;

    bool hasState = false;
    int state = 0;

    bool hasDestination = false;
    int destX = 0;
    int destY = 0;

    bool hasInventory = false;
    int rawMeat = 0;
    int meals = 0;
    int wood = 0;

    bool hasTarget = false;
    int targetX = 0;
    int targetY = 0;

    bool hasFeats = false;
    int slainDeer = 0;
    int choppedTrees = 0;
    int foodAte = 0;
    int mealsCooked = 0;

    bool hasKnowledge = false;
    bool hasClosestFood = false;
    int closestFoodX = 0;
    int closestFoodY = 0;
    bool hasClosestTree = false;
    int closestTreeX = 0;
    int closestTreeY = 0;
    bool hasClosestDeer = false;
    int closestDeerX = 0;
    int closestDeerY = 0;
    bool hasClosestGrass = false;
    int closestGrassX = 0;
    int closestGrassY = 0;
};

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

    const size_t m_maxNpcs = 5;
    const size_t m_maxDeer = 15;
    const size_t m_maxGrass = 25;
    const size_t m_maxTrees = 15;

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
    Engine(uint32_t seed, int spawnRate, int width, int height);
    int getTick();
    void simulate();
    void printFeats();
    EntityInspectData getEntityAt(int x, int y);
    const Statistics &getStatistics();
    std::vector<int> getGridSnapshot();
};
