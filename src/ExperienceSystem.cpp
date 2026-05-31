#include "ExperienceSystem.h"
#include "EngineLog.hpp"

float ExperienceSystem::calcNextLevel(float rate, int level)
{
    return rate * std::powf(level, 1.5f);
}

ExperienceSystem::ExperienceSystem()
{
}

void ExperienceSystem::update(int tick, EntityManager &em, Statistics &statistics)
{
    for (auto e : em.getEntities(npc))
    {
        if (e->isAlive())
        {
            auto &exp = e->get<CExperience>();
            auto &stats = e->get<CStats>();

            if (exp.hpCap() > stats.maxHitPoints() && exp.hpExp >= calcNextLevel(exp.rate(), stats.maxHitPoints()))
            {
                stats.setMaxHitPoints(stats.maxHitPoints() + 1);
                exp.hpExp = 0;
                EngineLog::levelUp(tick, e->id(), stats.maxHitPoints(), "Max HP");
            }

            if (exp.strCap() > stats.strength && exp.strExp >= (calcNextLevel(exp.rate(), stats.strength)))
            {
                ++stats.strength;
                exp.strExp = 0;
                EngineLog::levelUp(tick, e->id(), stats.strength, "Strength");
            }

            if (exp.spdCap() > stats.speed && exp.spdExp >= (calcNextLevel(exp.rate(), stats.speed)))
            {
                ++stats.speed;
                exp.strExp = 0;
                EngineLog::levelUp(tick, e->id(), stats.speed, "Speed");
            }
        }
    }
}
