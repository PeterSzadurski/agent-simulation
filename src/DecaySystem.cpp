#include "DecaySystem.h"
DecaySystem::DecaySystem()
{
}

void DecaySystem::update(EntityManager &em, int tick)
{
    for (auto e : em.getEntities())
    {
        if (e->has<CHunger>())
        {
            auto &hunger = e->get<CHunger>();
            if (hunger.isDecayed())
            {
                e->setAlive(false);
                continue;
            }
            systemProcess(tick, e, hunger, "hunger");
        }
        if (e->has<CFuel>())
        {
            auto &fuel = e->get<CFuel>();
            if (!fuel.isDecayed())
            {
                systemProcess(tick, e, fuel, "fuel");
            }
            else
            {
                auto &inv = e->get<CInventory>();
                if (inv.itemCount(wood) > 0)
                {
                    inv.adjustItems(wood, -1);
                    fuel.reset();
                }
            }
        }
    }
}

void DecaySystem::systemProcess(int tick, std::shared_ptr<Entity> e, CDecay &decay, std::string debugStr)
{
    auto &pos = e->get<CPosition>();
    int oldDecayValue = decay.getDecay();
    decay.decayTick();
    if (oldDecayValue != decay.getDecay())
    {
        spdlog::info("[Tick: {:08d}] ID:{:08d} has ({:02d}) {} at pos {}", tick, e->id(), decay.getDecay(),
                     debugStr, pos.cords.toStringPadded());
    }
}
