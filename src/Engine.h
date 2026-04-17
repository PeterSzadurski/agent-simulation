#include "EntityManager.h"
#include <random>
#include "Grid.h"
#include "KnowledgeSystem.h"
#include "MovementSystem.h"
#include "DecisionSystem.h"
#include "DecaySystem.h"
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

    void movementSystem();
    void decisionSystem();
    void actionSystem();
    void lineOfSightSystem();
    void cleanGrid();
    void gatherResource(std::shared_ptr<Entity> e, std::optional<Cords> &knowledgeTarget,
                        entity_type resourceType, const std::string &logName);
    void useNoticeBoard(CKnowledge &knowledge);

public:
    Engine(u_int32_t seed, int width, int height);
    int getTick();
    void simulate();
};
