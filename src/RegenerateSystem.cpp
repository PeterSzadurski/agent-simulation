#include "RegenerateSystem.h"

RegenerateSystem::RegenerateSystem()
{
}

void RegenerateSystem::update(const int tick, EntityManager &em)
{
    for (auto e : em.getEntities(npc))
    {
        if (e->isAlive() && e->has<CRegenerate>() && e->has<CHunger>())
        {
            auto &regenerate = e->get<CRegenerate>();
            if (!e->get<CHunger>().isHalfway())
            {
                regenerate.decayTick();
                if (regenerate.isDecayed())
                {
                    regenerate.reset();
                    auto &stats = e->get<CStats>();
                    if (stats.hitPoints < stats.maxHitPoints())
                    {
                        ++stats.hitPoints;
                    }
                }
            }
        }
    }
}
