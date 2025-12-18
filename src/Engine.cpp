#include "Engine.h"
#include <iostream>

void Engine::moveRand(std::shared_ptr<Entity> e)
{
    auto &pos = e->get<CPosition>();
    std::uniform_int_distribution<int> dist(-1, 1);
    int x = dist(m_rng);
    int y = dist(m_rng);
    if (m_grid.move(e, x, y))
    {
        std::cout << "X: " << pos.cords.x << " | Y: " << pos.cords.y << "\n";
    }
}

void Engine::hungerSystem(std::shared_ptr<Entity> e)
{
    if (e->has<CHunger>())
    {
        auto &hunger = e->get<CHunger>();
        if (hunger.isStarved())
        {
            e->setAlive(false);
            return;
        }
        hunger.hungerTick();
    }
}

void Engine::simulate()
{
    for (auto e : m_entities.getEntities())
    {
        hungerSystem(e);
        if (e->isAlive())
        {
            moveRand(e);
        }
        else
        {
            m_grid.remove(e);
        }
    }
    m_entities.update();
    m_tick++;
}

Engine::Engine(u_int32_t seed, int width, int height) : m_rng(seed), m_tick(0)
{
    auto npc = m_entities.addEntity(entity_type::npc);
    npc->add<CPosition>(0, 0);
    npc->add<CHunger>(0, 10);
    m_entities.update();
    m_grid = Grid(width, height);
}

u_int32_t Engine::getTick()
{
    return m_tick;
}
