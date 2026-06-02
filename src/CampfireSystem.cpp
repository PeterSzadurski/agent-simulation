#include "CampfireSystem.h"
#include "EngineLog.hpp"
#include "EntityManager.h"

CampfireSystem::CampfireSystem()
{
}

void CampfireSystem::update(const int tick)
{
    assignLeader(tick);
};

void CampfireSystem::assignLeader(const int tick)
{
    if (m_localEntities.size() > 0)
    {
        std::shared_ptr<Entity> leader;
        if (m_leader.has_value() && m_leader.value()->isAlive())
        {
            leader = m_leader.value();
        }
        else if (m_localEntities[0]->get<CRole>().role == ROLE::Common ||
                 m_localEntities[0]->get<CRole>().role == ROLE::Leader)
        {
            leader = m_localEntities[0];
        }

        for (size_t n = 0; n < m_localEntities.size(); ++n)
        {
            auto &comp = m_localEntities[n];
            auto &role = comp->get<CRole>();
            if (role.role == ROLE::Common || role.role == ROLE::Leader)
            {
                if (leader->get<CStats>().strength < comp->get<CStats>().strength)
                {
                    leader->get<CRole>().role = ROLE::Common;
                    leader = comp;
                }
            }
        }
        leader->get<CRole>().role = ROLE::Leader;
        if (!m_leader.has_value() || m_leader.value()->id() != leader->id())
        {
            EngineLog::promotionLeader(tick, leader->id());
        }
        m_leader = leader;
        m_localEntities.clear();
    }
}

void CampfireSystem::pushToCampfire(std::shared_ptr<Entity> e)
{
    m_localEntities.push_back(e);
}

const std::optional<std::shared_ptr<Entity>> CampfireSystem::leader()
{
    return m_leader;
}

void CampfireSystem::useNoticeBoard(EntityManager &em, CKnowledge &knowledge)
{
    auto &campKnowledge = em.getEntities(campfire).front()->get<CKnowledge>();

    // higher tick overrides
    auto mergeMaps = [](std::unordered_map<Cords, Seen> &dest,
                        const std::unordered_map<Cords, Seen> &src)
    {
        for (const auto &[pos, seen] : src)
        {
            auto it = dest.find(pos);
            if (it == dest.end() || it->second.tick < seen.tick)
            {
                dest[pos] = seen; // Add new or update if newer
            }
        }
    };
    mergeMaps(campKnowledge.m_reported_positions, knowledge.m_reported_positions);
    mergeMaps(knowledge.m_reported_positions, campKnowledge.m_reported_positions);

    for (auto it = knowledge.m_reported_positions.begin(); it != knowledge.m_reported_positions.end();)
    {
        if (it->second.type == entity_type::empty)
            it = knowledge.m_reported_positions.erase(it);
        else
            ++it;
    }

    for (auto it = campKnowledge.m_reported_positions.begin(); it != campKnowledge.m_reported_positions.end();)
    {
        if (it->second.type == entity_type::empty)
            it = campKnowledge.m_reported_positions.erase(it);
        else
            ++it;
    }
}
