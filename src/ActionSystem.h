#pragma once
#include "EntityManager.h"
#include "DecisionSystem.h"
#include "MovementSystem.h"

class ActionSystem
{
private:
    void gatherResource(int tick, std::shared_ptr<Entity> e, std::optional<Cords> &knowledgeTarget, entity_type resourceType, const std::string &logName);
    void useNoticeBoard(EntityManager &em, CKnowledge &knowledge);
    MovementSystem &m_movement;
    DecisionSystem &m_decision;
    Grid &m_grid;
    void combat(int const tick, std::mt19937 &rng, std::shared_ptr<Entity> entity_a, std::shared_ptr<Entity> &entity_b);
    void combatOutcome(int const tick, std::shared_ptr<Entity> &attacker, std::shared_ptr<Entity> &defender, std::vector<EntityPos> &pendingDrops);

public:
    ActionSystem(DecisionSystem &decision,
                 MovementSystem &movement,
                 Grid &grid);
    void update(int tick, EntityManager &em, std::mt19937 &rng, std::vector<EntityPos> &pendingDrops);
};
