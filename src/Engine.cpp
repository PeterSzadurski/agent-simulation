#include "Engine.h"
#include <iostream>
#include "EngineLog.hpp"

void Engine::simulate()
{
    lineOfSightSystem();
    m_action.update(m_tick, m_entities, m_rng, m_pendingDrops, m_statistics);
    movementSystem();
    m_decay.update(m_tick, m_entities);
    cleanGrid();
    spawnSystem();
    featSystem();
    m_entities.update();
    m_tick++;
}

Engine::Engine(u_int32_t seed, int width, int height) : m_rng(seed), m_tick(0),
                                                        m_grid(width, height), m_knowledge(m_grid),
                                                        m_movement(m_grid), m_decision(), m_decay(),
                                                        m_action(m_decision, m_movement, m_grid), m_width(width), m_height(height)
{
    spdlog::info("Init Engine");

    {
        auto campfire = m_entities.addEntity(entity_type::campfire);
        campfire->add<CPosition>(m_width / 2, m_height / 2);
        campfire->add<CInventory>(0);
        campfire->add<CFuel>(0, 250);
        campfire->add<CKnowledge>(m_width, m_height);
        m_grid.place(campfire);
    }
    int npcSpawnCount = randRange(1, 3);
    int deerSpawnCount = randRange(2, 5);
    int grassSpawnCount = randRange(1, 10);
    int treeSpawnCount = randRange(2, 5);

    for (int n = 0; n < npcSpawnCount; ++n)
    {
        spawnNpc();
    }

    for (int n = 0; n < deerSpawnCount; ++n)
    {
        spawnDeer();
    }

    for (int n = 0; n < grassSpawnCount; ++n)
    {
        spawnGrass();
    }

    for (int n = 0; n < treeSpawnCount; ++n)
    {
        spawnTree();
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
        if (!e->isAlive() || !e->has<CState>() || !e->has<CPosition>())
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
            case STATE::feeling_from:
                moved = m_movement.moveAwayFrom(e, e->get<CThreat>().threatPos);
                e->remove<CThreat>();
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
            EngineLog::entityRemoved(m_tick, e->id());
        }
    }
    for (auto &[type, pos] : m_pendingDrops)
    {
        auto &existing = m_grid.at(pos.x, pos.y);
        if (existing != nullptr)
        {
            spdlog::warn("[Tick: {:08d}] Drop conflict at {} — {} already there, dropping {}",
                         m_tick, Cords(pos.x, pos.y).toStringPadded(),
                         entityTypeToString(existing->type()),
                         entityTypeToString(type));
            continue;
        }
        auto drop = m_entities.addEntity(type);
        drop->add<CPosition>(pos);
        m_grid.place(drop);
    }
    m_pendingDrops.clear();
}

int Engine::randRange(int min, int max)
{
    return std::uniform_int_distribution<int>(min, max)(m_rng);
}

void Engine::spawnNpc()
{
    auto npc = m_entities.addEntity(entity_type::npc);
    npc->add<CPosition>(0, 0);
    npc->add<CHunger>(0, 1000);
    npc->add<CState>(STATE::wander);
    npc->add<CLineOfSight>(3);
    npc->add<CKnowledge>(m_width, m_height);
    npc->add<CInventory>(10);
    npc->add<CStats>(randRange(5, 15), randRange(3, 15), randRange(1, 5));
    npc->add<CFeats>(true);
    m_grid.placeRandom(npc, m_rng);
}

void Engine::spawnDeer()
{
    auto deer = m_entities.addEntity(entity_type::deer);
    deer->add<CPosition>(0, 0);
    deer->add<CHunger>(0, 1000);
    deer->add<CState>(STATE::wander);
    deer->add<CLineOfSight>(3);
    deer->add<CKnowledge>(m_width, m_height);
    deer->add<CStats>(randRange(1, 10), randRange(5, 10), randRange(1, 3));
    m_grid.placeRandom(deer, m_rng);
}

void Engine::spawnGrass()
{
    auto grass = m_entities.addEntity(entity_type::grass);
    grass->add<CPosition>(0, 0);
    if (m_grid.placeRandom(grass, m_rng))
    {
        EngineLog::placedAt(m_tick, grass->type(), grass->get<CPosition>().cords);
    }
}

void Engine::spawnTree()
{
    auto tree = m_entities.addEntity(entity_type::tree);
    tree->add<CPosition>(0, 0);
    if (m_grid.placeRandom(tree, m_rng))
    {
        EngineLog::placedAt(m_tick, tree->type(), tree->get<CPosition>().cords);
    }
}

void Engine::spawnSystem()
{
    if (m_tick % m_spawnRate == 0)
    {
        int roll = randRange(1, 100);
        if (roll <= 10)
        {
            spawnNpc();
        }
        else if (roll <= 25)
        {
            spawnDeer();
        }
        else if (roll <= 75)
        {
            spawnGrass();
        }
        else
        {
            spawnTree();
        }
    }
}

void Engine::featSystem()
{
    for (auto e : m_entities.getEntities(npc))
    {
        auto &feats = e->get<CFeats>();
        if (feats.choppedTrees > m_bestLumberjack.second)
        {
            m_bestLumberjack.first = e->id();
            m_bestLumberjack.second = feats.choppedTrees;
        }
        if (feats.foodAte > m_mostHungry.second)
        {
            m_mostHungry.first = e->id();
            m_mostHungry.second = feats.foodAte;
        }
        if (feats.slainDeer > m_bestHunter.second)
        {
            m_bestHunter.first = e->id();
            m_bestHunter.second = feats.slainDeer;
        }
        if (feats.mealsCooked > m_bestCook.second)
        {
            m_bestCook.first = e->id();
            m_bestCook.second = feats.mealsCooked;
        }
    }
}

void Engine::printFeats()
{
    EngineLog::printFeats(
        m_bestHunter.first, m_bestHunter.second,
        m_bestCook.first, m_bestCook.second,
        m_mostHungry.first, m_mostHungry.second,
        m_bestLumberjack.first, m_bestLumberjack.second,
        m_statistics);
}
