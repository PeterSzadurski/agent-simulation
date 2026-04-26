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

Engine::Engine(uint32_t seed, int spawnRate, int width, int height) : m_rng(seed), m_tick(0),
                                                                      m_grid(width, height), m_knowledge(m_grid),
                                                                      m_movement(m_grid), m_decision(), m_decay(),
                                                                      m_action(m_decision, m_movement,
                                                                               m_grid),
                                                                      m_width(width), m_height(height), m_spawnRate(spawnRate)
{
    spdlog_info("Init Engine");

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
                //   spdlog_info("[Tick: {:08d}] ID:{:08d} moved to ({:02d}, {:02d})", m_tick, e->id(), pos.cords.x, pos.cords.y);
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
            spdlog_warn("[Tick: {:08d}] Drop conflict at {} — {} already there, dropping {}",
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
        if (roll <= 15)
        {
            if (m_entities.getEntities(npc).size() < m_maxNpcs)
                spawnNpc();
        }
        else if (roll <= 35)
        {
            if (m_entities.getEntities(deer).size() < m_maxNpcs)
                spawnDeer();
        }
        else if (roll <= 75)
        {
            if (m_entities.getEntities(grass).size() < m_maxNpcs)
                spawnGrass();
        }
        else
        {
            if (m_entities.getEntities(tree).size() < m_maxNpcs)
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

EntityInspectData Engine::getEntityAt(int x, int y)
{
    EntityInspectData data;
    auto &e = m_grid.at(x, y);
    if (!e)
        return data;

    data.exists = true;
    data.x = x;
    data.y = y;
    data.type = static_cast<int>(e->type());

    if (e->has<CStats>())
    {
        auto &s = e->get<CStats>();
        data.hasStats = true;
        data.hitPoints = s.hitPoints;
        data.strength = s.strength;
        data.speed = s.speed;
    }
    if (e->has<CHunger>())
    {
        data.hasHunger = true;
        data.hunger = e->get<CHunger>().getDecay();
    }
    if (e->has<CFuel>())
    {
        data.hasFuel = true;
        data.fuel = e->get<CFuel>().getDecay();
    }
    if (e->has<CState>())
    {
        data.hasState = true;
        data.state = static_cast<int>(e->get<CState>().state);
    }
    if (e->has<CDestination>())
    {
        data.hasDestination = true;
        data.destX = e->get<CDestination>().cords.x;
        data.destY = e->get<CDestination>().cords.y;
    }
    if (e->has<CInventory>())
    {
        auto &inv = e->get<CInventory>();
        data.hasInventory = true;
        data.rawMeat = inv.itemCount(raw_meat);
        data.meals = inv.itemCount(meal);
        data.wood = inv.itemCount(wood);
    }
    if (e->has<CTarget>())
    {
        data.hasTarget = true;
        auto &t = e->get<CTarget>().target->get<CPosition>();
        data.targetX = t.cords.x;
        data.targetY = t.cords.y;
    }
    if (e->has<CFeats>())
    {
        auto &f = e->get<CFeats>();
        data.hasFeats = true;
        data.slainDeer = f.slainDeer;
        data.choppedTrees = f.choppedTrees;
        data.foodAte = f.foodAte;
        data.mealsCooked = f.mealsCooked;
    }
    if (e->has<CKnowledge>())
    {
        auto &k = e->get<CKnowledge>();
        data.hasKnowledge = true;
        if (k.m_closest_food.has_value())
        {
            data.hasClosestFood = true;
            data.closestFoodX = k.m_closest_food->x;
            data.closestFoodY = k.m_closest_food->y;
        }
        if (k.m_closest_tree.has_value())
        {
            data.hasClosestTree = true;
            data.closestTreeX = k.m_closest_tree->x;
            data.closestTreeY = k.m_closest_tree->y;
        }
        if (k.m_closest_deer.has_value())
        {
            data.hasClosestDeer = true;
            data.closestDeerX = k.m_closest_deer->x;
            data.closestDeerY = k.m_closest_deer->y;
        }
        if (k.m_closest_grass.has_value())
        {
            data.hasClosestGrass = true;
            data.closestGrassX = k.m_closest_grass->x;
            data.closestGrassY = k.m_closest_grass->y;
        }
    }
    return data;
}

const Statistics &Engine::getStatistics()
{
    return m_statistics;
}

std::vector<int> Engine::getGridSnapshot()
{
    std::vector<int> snapshot(m_width * m_height, 0);
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            auto &e = m_grid.at(x, y);
            snapshot[y * m_width + x] = e ? static_cast<int>(e->type()) : 0;
        }
    }
    return snapshot;
}
