#include "ActionSystem.h"
ActionSystem::ActionSystem(DecisionSystem &decision,
                           MovementSystem &movement,
                           Grid &grid) : m_decision(decision), m_movement(movement), m_grid(grid)
{
}

void ActionSystem::update(const int tick, EntityManager &em)
{
    auto &campInv = em.getEntities(campfire).front()->get<CInventory>();
    auto &campFuel = em.getEntities(campfire).front()->get<CFuel>();
    auto &campfireEntity = em.getEntities(campfire).front();

    for (auto e : em.getEntities(npc))
    {
        if (e->has<CState>() && e->isAlive())
        {
            auto &state = e->get<CState>();
            auto &hunger = e->get<CHunger>();
            auto &inventory = e->get<CInventory>();
            auto &knowledge = e->get<CKnowledge>();
            auto &pos = e->get<CPosition>();

            EntityState es = EntityState(e, campfireEntity);
            switch (m_decision.chooseNpcAction(es))
            {
            case Eat:
                hunger.reset();
                inventory.adjustItems(meal, -1);
                spdlog::info("[Tick: {:08d}] ID:{:08d} ate food.", tick, e->id());
                break;
            case Cook:
                e->add<CDestination>(knowledge.m_campfire);
                state = STATE::walking_to;
                // spdlog::info("[Tick: {:08d}] ID:{:08d} set destination to campfire.", m_tick, e->id());
                if (m_movement.nextToDestination(e))
                {
                    spdlog::info("[Tick: {:08d}] ID:{:08d} Cooking food.", tick, e->id());
                    inventory.adjustItems(raw_meat, -1);
                    campInv.adjustItems(meal, 1);
                    e->remove<CDestination>();
                    useNoticeBoard(em, knowledge);
                }
                break;
            case PickupMeal:
                e->add<CDestination>(knowledge.m_campfire);
                state = STATE::walking_to;
                if (m_movement.nextToDestination(e))
                {
                    spdlog::info("[Tick: {:08d}] ID:{:08d} picked up meal.", tick, e->id());
                    campInv.adjustItems(meal, -1);
                    inventory.adjustItems(meal, 1);
                    e->remove<CDestination>();
                    useNoticeBoard(em, knowledge);
                }
                break;
            case RefuelCampfire:
                e->add<CDestination>(knowledge.m_campfire);
                state = STATE::walking_to;
                if (m_movement.nextToDestination(e))
                {
                    spdlog::info("[Tick: {:08d}] ID:{:08d} refueling.", tick, e->id());
                    inventory.adjustItems(wood, -1);
                    campInv.adjustItems(wood, 1);
                    e->remove<CDestination>();
                    useNoticeBoard(em, knowledge);
                }
                break;
            case GatherFood:
                gatherResource(tick, e, knowledge.m_closest_food, raw_meat, "meat");
                break;
            case GatherWood:
                gatherResource(tick, e, knowledge.m_closest_tree, wood, "wood");
                break;
            default:
                e->remove<CDestination>();
                state = STATE::wander;
                break;
            }
        }
    }

    for (auto e : em.getEntities(deer))
    {
        if (e->has<CState>() && e->isAlive())
        {
            auto &state = e->get<CState>();
            auto &hunger = e->get<CHunger>();
            auto &knowledge = e->get<CKnowledge>();
            auto &pos = e->get<CPosition>();

            EntityState es = EntityState(e, campfireEntity);
            switch (m_decision.chooseDeerAction(es))
            {
            case EatGrass:
                e->add<CDestination>(knowledge.m_closest_grass.value());
                state = STATE::walking_to;
                if (m_movement.nextToDestination(e))
                {
                    auto &dest = e->get<CDestination>();
                    auto &dE = m_grid.at(dest.cords.x, dest.cords.y);
                    if (dE)
                    {
                        spdlog::info("[Tick: {:08d}] ID:{:08d} {} ate grass.", tick, e->id(), entityTypeToString(e->type()));
                        dE->setAlive(false);
                        knowledge.m_reported_positions[dest.cords] = Seen(empty, tick);
                        hunger.reset();
                    }
                    e->remove<CDestination>();
                }
                break;
            default:
                e->remove<CDestination>();
                state = STATE::wander;
                break;
            }
        }
    }
}

void ActionSystem::gatherResource(int tick, std::shared_ptr<Entity> e, std::optional<Cords> &knowledgeTarget, entity_type resourceType, const std::string &logName)
{
    auto &state = e->get<CState>();
    auto &hunger = e->get<CHunger>();
    auto &inventory = e->get<CInventory>();
    auto &knowledge = e->get<CKnowledge>();

    e->add<CDestination>(knowledgeTarget.value());
    state = walking_to;
    spdlog::info("[Tick: {:08d}] ID:{:08d} set destination to {}.", tick, e->id(), logName);
    if (m_movement.nextToDestination(e))
    {
        auto &dest = e->get<CDestination>();
        auto &dE = m_grid.at(dest.cords.x, dest.cords.y);
        if (dE)
        {
            spdlog::info("[Tick: {:08d}] ID:{:08d} picked up {}.", tick, e->id(), logName);
            inventory.adjustItems(resourceType, 1);

            dE->setAlive(false);
            knowledge.m_reported_positions[dest.cords] = Seen(empty, tick);
            if (knowledgeTarget.has_value() && knowledgeTarget.value() == dest.cords)
            {
                knowledgeTarget.reset();
            }
        }
        e->remove<CDestination>();
    }
}

void ActionSystem::useNoticeBoard(EntityManager &em, CKnowledge &knowledge)
{
    auto &campKnowledge = em.getEntities(campfire).front()->get<CKnowledge>();

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
