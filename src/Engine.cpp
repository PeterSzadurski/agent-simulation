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
        auto campfire = m_entities.addEntity(entity_type::campfire);
        campfire->add<CPosition>(width / 2, height / 2);
        m_grid.place(campfire);
    }

    {
        auto npc = m_entities.addEntity(entity_type::npc);
        npc->add<CPosition>(0, 0);
        npc->add<CHunger>(0, 1000);
        npc->add<CState>(STATE::wander);
        npc->add<CLineOfSight>(3);
        npc->add<CKnowledge>(true);
        npc->add<CInventory>(2);
        m_grid.placeRandom(npc, m_rng);
    }
    {
        auto food = m_entities.addEntity(entity_type::raw_meat);
        food->add<CPosition>(0, 0);
        if (m_grid.placeRandom(food, m_rng))
        {
            spdlog::info("[Tick: {:08d}] Placed FOOD at ({:02d}, {:02d})", m_tick, food->get<CPosition>().cords.x, food->get<CPosition>().cords.y);
        }
    }

    {
        auto food = m_entities.addEntity(entity_type::raw_meat);
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
            // spdlog::info("[Tick: {:08d}] ID:{:08d} moved to ({:02d}, {:02d})", m_tick, e->id(), pos.cords.x, pos.cords.y);
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
            auto &inventory = e->get<CInventory>();
            auto &knowledge = e->get<CKnowledge>();
            bool roomForFood = inventory.foodCount() < inventory.maxFood();
            bool roomForMeal = inventory.mealCount() < inventory.maxMeal();

            if (hunger.isHungry())
            {
                if (inventory.mealCount() > 0)
                {
                    // eat inventory meal
                    hunger.reset();
                    if (inventory.adjustMeal(-1))
                    {
                        spdlog::info("[Tick: {:08d}] ID:{:08d} ate (1) from Inventory.", m_tick, e->id());
                    }
                }
            }
            if (!roomForFood)
            {
                e->add<CDesination>(knowledge.m_campfire.x, knowledge.m_campfire.y);
                if (m_movement.nextToDestination(e))
                {
                    spdlog::info("[Tick: {:08d}] ID:{:08d} Next to campfire.", m_tick, e->id());
                    if (inventory.foodCount() > 0)
                    {
                        int count = inventory.foodCount();
                        inventory.adjustFood(-count);
                        inventory.adjustMeal(count);
                    }
                }
                else
                {
                    state = STATE::walking_to;
                }
            }
            else if (roomForFood && roomForMeal)
            {
                entity_type searchEntity = entity_type::meal;

                if (roomForMeal)
                {
                    searchEntity = entity_type::meal;
                }
                if (roomForFood)
                {
                    searchEntity = entity_type::raw_meat;
                }
                // room to add more items to inventory
                if (m_knowledge.findNearestEntityType(e, searchEntity) && e->has<CDesination>())
                {

                    if (m_movement.nextToDestination(e))
                    {
                        // pick up the food
                        auto &dest = e->get<CDesination>();
                        m_grid.at(dest.cords.x, dest.cords.y)->setAlive(false);
                        state = STATE::wander;
                        if (searchEntity == entity_type::meal)
                        {
                            if (inventory.adjustMeal(1))
                            {
                                spdlog::info("[Tick: {:08d}] ID:{:08d} added (1) meal to Inventory.", m_tick, e->id());
                            }
                        }
                        else
                        {
                            if (inventory.adjustFood(1))
                            {
                                spdlog::info("[Tick: {:08d}] ID:{:08d} added (1) raw meat to Inventory.", m_tick, e->id());
                                if (knowledge.m_closest_food = dest.cords)
                                {
                                    knowledge.m_closest_food.reset();
                                    knowledge.m_reported_positions[dest.cords] = Seen(entity_type::empty, m_tick);
                                }
                            }
                        }
                        e->remove<CDesination>();
                    }
                    else
                    {
                        state = STATE::walking_to;
                    }
                }
            }
            // inventory full, return to campfire
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
