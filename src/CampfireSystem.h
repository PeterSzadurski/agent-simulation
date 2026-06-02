#pragma once
#include "Entity.hpp"

class CampfireSystem
{
private:
private:
    std::vector<std::shared_ptr<Entity>> m_localEntities;
    std::optional<std::shared_ptr<Entity>> m_leader;
    void assignLeader(const int tick);

public:
    CampfireSystem();
    void update(const int tick);
    void pushToCampfire(std::shared_ptr<Entity>);
    const std::optional<std::shared_ptr<Entity>> leader();
    void useNoticeBoard(EntityManager &em, CKnowledge &knowledge);
};
