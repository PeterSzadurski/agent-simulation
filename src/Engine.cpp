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
            m_grid.remove(e);
            return;
        }
        hunger.hungerTick();
    }
}

void Engine::simulate()
{
    for (auto e : m_entities.getEntities())
    {
        auto &state = e->get<CState>();
        hungerSystem(e);
        if (e->isAlive())
        {
            if (state.exists)
            {
                movementSystem(e);
                if (e->has<CLineOfSight>())
                {
                    m_knowledge.updateLineOfSight(e);
                }
            }
        }
        else
        {
            m_grid.remove(e);
        }
    }
    m_entities.update();
    m_tick++;
}

Engine::Engine(u_int32_t seed, int width, int height) : m_rng(seed), m_tick(0), m_grid(width, height), m_knowledge(m_grid)
{
    {
        auto npc = m_entities.addEntity(entity_type::npc);
        npc->add<CPosition>(0, 0);
        npc->add<CHunger>(0, 10);
        npc->add<CState>(STATE::wander);
        npc->add<CLineOfSight>(3);
        npc->add<CKnowledge>(true);
        m_grid.placeRandom(npc, m_rng);
    }
    {
        auto food = m_entities.addEntity(entity_type::meal);
        food->add<CPosition>(0, 0);
        if (m_grid.placeRandom(food, m_rng))
        {
            std::cout << "Placed food at: " << food->get<CPosition>().cords.x << ", " << food->get<CPosition>().cords.y << "\n";
        }
    }

    m_entities.update();
}

int Engine::getTick()
{
    return m_tick;
}

void Engine::movementSystem(std::shared_ptr<Entity> e)
{
    auto &state = e->get<CState>();
    switch (state.state)
    {
    case STATE::wander:
        moveRand(e);
        break;

    default:
        break;
    }
}
