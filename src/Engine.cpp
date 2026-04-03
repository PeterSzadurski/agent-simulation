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

            if (e->has<CDesination>() && m_movement.nextToDestination(e))
            {
                bool roomForFood = inventory.foodCount() < inventory.maxFood();
                bool roomForMeal = inventory.mealCount() < inventory.maxMeal();
                auto &dest = e->get<CDesination>();
                auto &dE = m_grid.at(dest.cords.x, dest.cords.y);
                switch (dE->type())
                {
                case raw_meat:
                    if (roomForFood)
                    {
                        inventory.adjustFood(1);
                        dE->setAlive(false);
                        knowledge.m_reported_positions[dest.cords] = Seen(empty, m_tick);
                        if (knowledge.m_closest_food.has_value() && knowledge.m_closest_food.value() == dest.cords)
                        {
                            knowledge.m_closest_food.reset();
                        }
                        e->remove<CDesination>();
                    }
                    break;
                case campfire:
                    if (roomForMeal && inventory.foodCount() > 0)
                    {
                        spdlog::info("[Tick: {:08d}] ID:{:08d} Cooking food.", m_tick, e->id());
                        inventory.adjustFood(-1);
                        inventory.adjustMeal(1);
                    }
                    else
                    {
                        e->remove<CDesination>();
                        state = wander;
                    }
                    break;
                }
            }

            if (hunger.isHungry())
            {
                if (inventory.mealCount() > 0)
                {
                    hunger.reset();
                    inventory.adjustMeal(-1);
                }
                else if (inventory.foodCount() > 0)
                {
                    // head for campfire
                    e->add<CDesination>(knowledge.m_campfire.x, knowledge.m_campfire.y);
                    state = STATE::walking_to;
                }
                else if (m_knowledge.findNearestEntityType(e, entity_type::raw_meat))
                {
                    state = STATE::walking_to;
                }
                else
                {
                    state = STATE::wander;
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
