#include "Engine.h"
#include <iostream>

void Engine::decaySystem()
{
    for (auto e : m_entities.getEntities())
    {
        if (e->has<CHunger>())
        {
            auto &hunger = e->get<CHunger>();
            if (hunger.isDecayed())
            {
                e->setAlive(false);
                continue;
            }
            decaySystemProcess(e, hunger, "hunger");
        }
        if (e->has<CFuel>())
        {
            auto &fuel = e->get<CFuel>();
            if (!fuel.isDecayed())
            {
                decaySystemProcess(e, fuel, "fuel");
            }
        }
    }
};

void Engine::decaySystemProcess(std::shared_ptr<Entity> e, CDecay &decay, std::string debugStr)
{
    auto &pos = e->get<CPosition>();
    int oldDecayValue = decay.getDecay();
    decay.decayTick();
    if (oldDecayValue != decay.getDecay())
    {
        spdlog::info("[Tick: {:08d}] ID:{:08d} has ({:02d}) {} at pos ({:02d},{:02d})", m_tick, e->id(), decay.getDecay(),
                     debugStr, pos.cords.x, pos.cords.y);
    }
};

void Engine::simulate()
{

    lineOfSightSystem();
    actionSystem();
    movementSystem();
    decaySystem();
    cleanGrid();
    m_entities.update();
    m_tick++;
}

Engine::Engine(u_int32_t seed, int width, int height) : m_rng(seed), m_tick(0), m_grid(width, height), m_knowledge(m_grid), m_movement(m_grid), m_decision()
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
        m_grid.placeRandom(npc, m_rng);
    }
    {
        auto npc = m_entities.addEntity(entity_type::npc);
        npc->add<CPosition>(0, 0);
        npc->add<CHunger>(0, 1000);
        npc->add<CState>(STATE::wander);
        npc->add<CLineOfSight>(3);
        npc->add<CKnowledge>(width, height);
        npc->add<CInventory>(10);
        m_grid.placeRandom(npc, m_rng);
    }
    for (int n = 0; n < 10; ++n)
    {
        auto food = m_entities.addEntity(entity_type::raw_meat);
        food->add<CPosition>(0, 0);
        if (m_grid.placeRandom(food, m_rng))
        {
            spdlog::info("[Tick: {:08d}] Placed FOOD at ({:02d}, {:02d})", m_tick, food->get<CPosition>().cords.x, food->get<CPosition>().cords.y);
        }
    }

    for (int n = 0; n < 5; ++n)
    {
        auto tree = m_entities.addEntity(entity_type::tree);
        tree->add<CPosition>(0, 0);
        if (m_grid.placeRandom(tree, m_rng))
        {
            spdlog::info("[Tick: {:08d}] Placed TREE at ({:02d}, {:02d})", m_tick, tree->get<CPosition>().cords.x, tree->get<CPosition>().cords.y);
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
    auto &campInv = m_entities.getEntities(campfire).front()->get<CInventory>();
    auto &campFuel = m_entities.getEntities(campfire).front()->get<CFuel>();
    auto &campfireEntity = m_entities.getEntities(campfire).front();

    for (auto e : m_entities.getEntities())
    {
        if (e->has<CState>() && e->isAlive())
        {
            auto &state = e->get<CState>();
            auto &hunger = e->get<CHunger>();
            auto &inventory = e->get<CInventory>();
            auto &knowledge = e->get<CKnowledge>();
            auto &pos = e->get<CPosition>();

            // refactor
            EntityState es = EntityState(e, campfireEntity);
            switch (m_decision.chooseAction(es))
            {
            case Eat:
                hunger.reset();
                inventory.adjustItems(meal, -1);
                spdlog::info("[Tick: {:08d}] ID:{:08d} ate food.", m_tick, e->id());
                break;
            case Cook:
                e->add<CDestination>(knowledge.m_campfire);
                state = STATE::walking_to;
                // spdlog::info("[Tick: {:08d}] ID:{:08d} set destination to campfire.", m_tick, e->id());
                if (m_movement.nextToDestination(e))
                {
                    spdlog::info("[Tick: {:08d}] ID:{:08d} Cooking food.", m_tick, e->id());
                    inventory.adjustItems(raw_meat, -1);
                    campInv.adjustItems(meal, 1);
                    e->remove<CDestination>();
                    useNoticeBoard(knowledge);
                    useNoticeBoard(knowledge);
                }
                break;
            case PickupMeal:
                e->add<CDestination>(knowledge.m_campfire);
                state = STATE::walking_to;
                if (m_movement.nextToDestination(e))
                {
                    spdlog::info("[Tick: {:08d}] ID:{:08d} picked up meal.", m_tick, e->id());
                    campInv.adjustItems(meal, -1);
                    inventory.adjustItems(meal, 1);
                    e->remove<CDestination>();
                    useNoticeBoard(knowledge);
                }
                break;
            case RefuelCampfire:
                e->add<CDestination>(knowledge.m_campfire);
                state = STATE::walking_to;
                if (m_movement.nextToDestination(e))
                {
                    spdlog::info("[Tick: {:08d}] ID:{:08d} refueling.", m_tick, e->id());
                    inventory.adjustItems(wood, -1);
                    campInv.adjustItems(wood, 1);
                    campFuel.reset();
                    e->remove<CDestination>();
                    useNoticeBoard(knowledge);
                }
                break;
            case GatherFood:
                gatherResource(e, knowledge.m_closest_food, raw_meat, "meat");
                break;
            case GatherWood:
                gatherResource(e, knowledge.m_closest_tree, wood, "wood");
                break;
            default:
                e->remove<CDestination>();
                state = STATE::wander;
                break;
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

void Engine::gatherResource(std::shared_ptr<Entity> e, std::optional<Cords> &knowledgeTarget, entity_type resourceType, const std::string &logName)
{

    auto &state = e->get<CState>();
    auto &hunger = e->get<CHunger>();
    auto &inventory = e->get<CInventory>();
    auto &knowledge = e->get<CKnowledge>();

    e->add<CDestination>(knowledgeTarget.value());
    state = walking_to;
    spdlog::info("[Tick: {:08d}] ID:{:08d} set destination to {}.", m_tick, e->id(), logName);
    if (m_movement.nextToDestination(e))
    {
        auto &dest = e->get<CDestination>();
        auto &dE = m_grid.at(dest.cords.x, dest.cords.y);
        spdlog::info("[Tick: {:08d}] ID:{:08d} picked up {}.", m_tick, e->id(), logName);
        inventory.adjustItems(resourceType, 1);

        dE->setAlive(false);
        knowledge.m_reported_positions[dest.cords] = Seen(empty, m_tick);
        if (knowledgeTarget.has_value() && knowledgeTarget.value() == dest.cords)
        {
            knowledgeTarget.reset();
        }
        e->remove<CDestination>();
    }
}

void Engine::useNoticeBoard(CKnowledge &knowledge)
{
    auto &campKnowledge = m_entities.getEntities(campfire).front()->get<CKnowledge>();

    // higher tick overrides
    auto mergeMaps = [](std::map<Cords, Seen> &dest,
                        const std::map<Cords, Seen> &src)
    {
        for (const auto &[pos, seen] : src)
        {
            auto it = dest.find(pos);
            if (it == dest.end() || it->second.tick < seen.tick)
            {
                dest[pos] = seen; // Add new or update if newer
            }
        }
    };
    mergeMaps(campKnowledge.m_reported_positions, knowledge.m_reported_positions);
    mergeMaps(knowledge.m_reported_positions, campKnowledge.m_reported_positions);

    std::erase_if(knowledge.m_reported_positions,
                  [](const auto &pair)
                  { return pair.second.type == entity_type::empty; });

    std::erase_if(campKnowledge.m_reported_positions,
                  [](const auto &pair)
                  { return pair.second.type == entity_type::empty; });
}
