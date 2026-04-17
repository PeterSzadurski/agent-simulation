#pragma once
#include "EntityManager.h"
#include "DecisionSystem.h"
#include "MovementSystem.h"

class ActionSystem
{
private:
    void gatherResource(int tick, std::shared_ptr<Entity> e, std::optional<Cords> &knowledgeTarget, entity_type resourceType, const std::string &logName);
    void useNoticeBoard(EntityManager &em, CKnowledge &knowledge);
    MovementSystem m_movement;
    DecisionSystem m_decision;
    Grid m_grid;

public:
    ActionSystem(DecisionSystem &decision,
                 MovementSystem &movement,
                 Grid &grid);
    void update(int tick, EntityManager &em);
};
