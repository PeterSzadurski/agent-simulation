#include "Engine.h"
#include <iostream>

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
        if (hunger.isHungry())
        {
            if (!e->get<CPosition>().destination.has_value())
            {
                if (m_knowledge.findNearestMeal(e))
                {
                    e->get<CState>().state = STATE::walking_to;
                }
            }
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
    }

    for (auto e : m_entities.getEntities())
    {
        auto &state = e->get<CState>();
        if (e->isAlive())
        {
            if (state.exists)
            {
                if (e->has<CLineOfSight>())
                {
                    m_knowledge.updateLineOfSight(e, m_tick);
                }
            }
        }
    }

    for (auto e : m_entities.getEntities())
    {
        auto &state = e->get<CState>();
        if (e->isAlive())
        {
            if (state.exists)
            {
                movementSystem(e);
            }
        }
        else
        {
            m_grid.remove(e);
        }
    }

    for (auto e : m_entities.getEntities())
    {
        auto &state = e->get<CState>();
        if (e->isAlive())
        {
            if (state.exists)
            {
            }
        }
    }
    m_entities.update();
    m_tick++;
}

Engine::Engine(u_int32_t seed, int width, int height) : m_rng(seed), m_tick(0), m_grid(width, height), m_knowledge(m_grid), m_movement(m_grid)
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
        // moveRand(e);
        m_movement.moveRand(e, m_rng);
        break;
    case STATE::walking_to:
        if (!m_movement.nextToDestination(e))
        {
            m_movement.moveTo(e);
        }
        else
        {
            // eat food reset hunger
            auto &pos = e->get<CPosition>();
            m_grid.at(pos.destination.value().x, pos.destination.value().y)->setAlive(false);
            e->get<CHunger>().reset();
            pos.destination.reset();
            state.state = STATE::wander;
        }
        break;
    default:
        break;
    }
}
