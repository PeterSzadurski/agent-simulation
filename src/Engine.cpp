#include "Engine.h"
#include <iostream>

void Engine::hungerSystem()
{
    for (auto e : m_entities.getEntities())
    {

        if (e->has<CHunger>())
        {
            auto &hunger = e->get<CHunger>();
            int oldHungerValue = hunger.getHunger();
            if (hunger.isStarved())
            {
                e->setAlive(false);
                return;
            }
            hunger.hungerTick();
            if (oldHungerValue != hunger.getHunger())
                spdlog::info("[Tick: {:08d}] ID:{:08d} has ({:02d}) hunger", m_tick, e->id(), hunger.getHunger());
        }
    }
}

void Engine::simulate()
{

    lineOfSightSystem();
    actionSystem();
    movementSystem();
    hungerSystem();
    cleanGrid();
    m_entities.update();
    m_tick++;
}

Engine::Engine(u_int32_t seed, int width, int height) : m_rng(seed), m_tick(0), m_grid(width, height), m_knowledge(m_grid), m_movement(m_grid)
{
    spdlog::info("Init Engine");
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
            spdlog::info("[Tick: {:08d}] Placed FOOD at ({:02d}, {:02d})", m_tick, food->get<CPosition>().cords.x, food->get<CPosition>().cords.y);
        }
    }

    m_entities.update();
}

int Engine::getTick()
{
    return m_tick;
}

void Engine::movementSystem()
{
    bool moved = false;
    for (auto e : m_entities.getEntities())
    {
        if (!e->has<CState>() || !e->has<CPosition>())
        {
            continue;
        }
        auto &state = e->get<CState>();
        auto &pos = e->get<CPosition>();

        switch (state.state)
        {
        case STATE::wander:
            // moveRand(e);
            moved = m_movement.moveRand(e, m_rng);
            break;
        case STATE::walking_to:
            if (!m_movement.nextToDestination(e))
            {
                moved = m_movement.moveTo(e);
            }
            break;
        default:
            break;
        }
        if (moved)
        {
            spdlog::info("[Tick: {:08d}] ID:{:08d} moved to ({:02d}, {:02d})", m_tick, e->id(), pos.cords.x, pos.cords.y);
        }
    }
}

void Engine::actionSystem()
{
    for (auto e : m_entities.getEntities())
    {
        if (e->has<CState>() && e->isAlive())
        {
            auto &state = e->get<CState>();
            auto &hunger = e->get<CHunger>();
            if (hunger.isHungry())
            {
                if (m_knowledge.findNearestMeal(e) && e->has<CDesination>())
                {
                    if (m_movement.nextToDestination(e))
                    {
                        // eat food
                        auto &dest = e->get<CDesination>();
                        m_grid.at(dest.cords.x, dest.cords.y)->setAlive(false);
                        hunger.reset();
                        state = STATE::wander;
                        e->remove<CDesination>();
                    }
                    else
                    {
                        state = STATE::walking_to;
                    }
                }
            }
        }
    }
}

void Engine::lineOfSightSystem()
{
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
}

void Engine::cleanGrid()
{
    for (auto e : m_entities.getEntities())
    {
        if (!e->isAlive())
        {
            m_grid.remove(e);
            spdlog::info("[Tick: {:08d}] ID:{:08d} removed.", m_tick, e->id());
        }
    }
}
