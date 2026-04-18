#include "Engine.h"
#include <iostream>

void Engine::simulate()
{

    lineOfSightSystem();
    m_action.update(m_tick, m_entities);
    movementSystem();
    m_decay.update(m_tick, m_entities);
    cleanGrid();
    m_entities.update();
    m_tick++;
}

Engine::Engine(u_int32_t seed, int width, int height) : m_rng(seed), m_tick(0),
                                                        m_grid(width, height), m_knowledge(m_grid),
                                                        m_movement(m_grid), m_decision(), m_decay(),
                                                        m_action(m_decision, m_movement, m_grid)
{
    spdlog::info("Init Engine");

    {
        auto campfire = m_entities.addEntity(entity_type::campfire);
        campfire->add<CPosition>(width / 2, height / 2);
        campfire->add<CInventory>(0);
        campfire->add<CFuel>(0, 250);
        campfire->add<CKnowledge>(width, height);
        m_grid.place(campfire);
    }

    {
        auto npc = m_entities.addEntity(entity_type::npc);
        npc->add<CPosition>(0, 0);
        npc->add<CHunger>(0, 1000);
        npc->add<CState>(STATE::wander);
        npc->add<CLineOfSight>(3);
        npc->add<CKnowledge>(width, height);
        npc->add<CInventory>(10);
        npc->add<CStats>(10, 5, 1);
        m_grid.placeRandom(npc, m_rng);
    }
    // {
    //     auto npc = m_entities.addEntity(entity_type::npc);
    //     npc->add<CPosition>(0, 0);
    //     npc->add<CHunger>(0, 1000);
    //     npc->add<CState>(STATE::wander);
    //     npc->add<CLineOfSight>(3);
    //     npc->add<CKnowledge>(width, height);
    //     npc->add<CInventory>(10);
    //     m_grid.placeRandom(npc, m_rng);
    // }

    {
        auto deer = m_entities.addEntity(entity_type::deer);
        deer->add<CPosition>(0, 0);
        deer->add<CHunger>(0, 1000);
        deer->add<CState>(STATE::wander);
        deer->add<CLineOfSight>(3);
        deer->add<CKnowledge>(width, height);
        // deer->add<CInventory>(10);
        deer->add<CStats>(10, 2, 1);
        m_grid.placeRandom(deer, m_rng);
    }

    for (int n = 0; n < 2; ++n)
    {
        auto food = m_entities.addEntity(entity_type::raw_meat);
        food->add<CPosition>(0, 0);
        if (m_grid.placeRandom(food, m_rng))
        {
            spdlog::info("[Tick: {:08d}] Placed meat at {}", m_tick, food->get<CPosition>().cords.toStringPadded());
        }
    }

    for (int n = 0; n < 10; ++n)
    {
        auto grass = m_entities.addEntity(entity_type::grass);
        grass->add<CPosition>(0, 0);
        if (m_grid.placeRandom(grass, m_rng))
        {
            spdlog::info("[Tick: {:08d}] Placed grass at {}", m_tick, grass->get<CPosition>().cords.toStringPadded());
        }
    }

    for (int n = 0; n < 5; ++n)
    {
        auto tree = m_entities.addEntity(entity_type::tree);
        tree->add<CPosition>(0, 0);
        if (m_grid.placeRandom(tree, m_rng))
        {
            spdlog::info("[Tick: {:08d}] Placed Tree at ({:02d}, {:02d})", m_tick, tree->get<CPosition>().cords.x, tree->get<CPosition>().cords.y);
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
        const int speed = e->get<CStats>().speed;
        for (int moveCount = 0; moveCount < speed; ++moveCount)
        {

            switch (state.state)
            {
            case STATE::wander:
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
                //   spdlog::info("[Tick: {:08d}] ID:{:08d} moved to ({:02d}, {:02d})", m_tick, e->id(), pos.cords.x, pos.cords.y);
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
