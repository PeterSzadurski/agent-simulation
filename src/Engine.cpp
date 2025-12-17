#include "Engine.h"
#include <iostream>

void Engine::moveRand(std::shared_ptr<Entity> e)
{
    auto &pos = e->get<CPosition>();
    std::uniform_int_distribution<int> dist(-1, 1);
    pos.cords.x += dist(rng);
    pos.cords.y += dist(rng);
}

void Engine::simulate()
{
    for (auto e : m_entities.getEntities())
    {
        moveRand(e);
        auto &pos = e->get<CPosition>();
        std::cout << "X: " << pos.cords.x << " | Y: " << pos.cords.y << "\n";
    }
    tick++;
}

Engine::Engine(u_int32_t seed) : rng(seed), tick(0)
{
    auto npc = m_entities.addEntity(entity_type::npc);
    npc->add<CPosition>(0, 0);
    m_entities.update();
}

u_int32_t Engine::getTick()
{
    return tick;
}
