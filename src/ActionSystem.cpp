#include "ActionSystem.h"
ActionSystem::ActionSystem(DecisionSystem &decision,
                           MovementSystem &movement,
                           Grid &grid) : m_decision(decision), m_movement(movement), m_grid(grid)
{
}

void ActionSystem::update(const int tick, EntityManager &em, std::mt19937 &rng, std::vector<EntityPos> &pendingDrops, Statistics &statistics)
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
            auto &feats = e->get<CFeats>();

            EntityState es = EntityState(e, campfireEntity);
            switch (m_decision.chooseNpcAction(es))
            {
            case Eat:
                hunger.reset();
                inventory.adjustItems(meal, -1);
                ++feats.foodAte;
                ++statistics.totalMealsEaten;
                spdlog::info("[Tick: {:08d}] ID:{:08d} ate food.", tick, e->id());
                break;
            case Cook:
                e->add<CDestination>(knowledge.m_campfire);
                state = STATE::walking_to;
                // spdlog::info("[Tick: {:08d}] ID:{:08d} set destination to campfire.", m_tick, e->id());
                if (m_movement.nextToDestination(e))
                {
                    spdlog::info("[Tick: {:08d}] ID:{:08d} Cooking food.", tick, e->id());
                    ++feats.mealsCooked;
                    ++statistics.totalMealsCooked;
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
                gatherResource(tick, e, knowledge.m_closest_food, raw_meat, "meat", statistics);
                break;
            case GatherWood:
                gatherResource(tick, e, knowledge.m_closest_tree, wood, "wood", statistics);
                break;
            case HuntDeer:
                if (e->has<CTarget>())
                {
                    e->add<CDestination>(e->get<CTarget>().target->get<CPosition>().cords);
                }
                else
                {
                    e->add<CDestination>(knowledge.m_closest_deer.value());
                }
                state = STATE::walking_to;
                if (m_movement.nextToDestination(e))
                {

                    auto &dest = e->get<CDestination>();
                    auto &dE = m_grid.at(dest.cords.x, dest.cords.y);
                    if (dE && dE->type() == deer)
                    {
                        e->add<CTarget>(dE);
                        combat(tick, rng, e, dE);
                        combatOutcome(tick, e, dE, pendingDrops, statistics);
                    }
                    else
                    {
                        // state data
                        e->remove<CTarget>();
                        e->remove<CDestination>();
                        knowledge.m_closest_deer.reset();
                    }
                }
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
            case Flee:
                e->add<CThreat>(es.threatPosition.value());
                state = STATE::feeling_from;
                break;
            default:
                e->remove<CDestination>();
                state = STATE::wander;
                break;
            }
        }
    }
}

void ActionSystem::gatherResource(int tick, std::shared_ptr<Entity> e, std::optional<Cords> &knowledgeTarget, entity_type resourceType, const std::string &logName, Statistics &statistics)
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
            if (resourceType == wood)
            {
                ++e->get<CFeats>().choppedTrees;
                ++statistics.totalTreesChopped;
            }
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

void ActionSystem::combat(int const tick, std::mt19937 &rng, std::shared_ptr<Entity> entity_a, std::shared_ptr<Entity> &entity_b)
{
    auto &stats_a = entity_a->get<CStats>();
    auto &stats_b = entity_b->get<CStats>();

    std::uniform_int_distribution<int> distA(0, stats_a.strength);
    std::uniform_int_distribution<int> distB(0, stats_b.strength);

    int damA = distA(rng);
    int damB = distB(rng);

    if (damA > damB)
    {
        int damage = damA - damB;
        spdlog::info("[Tick: {:08d}] {}, ID:{:08d} dealt {} damage to {}, ID:{:08d}",
                     tick, entityTypeToString(entity_a->type()), entity_a->id(), damage,
                     entityTypeToString(entity_b->type()), entity_b->id());
        stats_b.hitPoints -= damage;
        if (stats_b.hitPoints <= 0)
        {
            entity_b->setAlive(false);
        }
    }
    else if (damA < damB)
    {
        int damage = damB - damA;
        spdlog::info("[Tick: {:08d}] {}, ID:{:08d} dealt {} damage to {}, ID:{:08d}",
                     tick, entityTypeToString(entity_b->type()), entity_b->id(),
                     damage, entityTypeToString(entity_a->type()), entity_a->id());
        stats_a.hitPoints -= damage;
        if (stats_a.hitPoints <= 0)
        {
            entity_a->setAlive(false);
        }
    }
    else
    {
        spdlog::info("[Tick: {:08d}] {}, ID:{:08d} blocks an attack from {}, ID:{:08d}",
                     tick, entityTypeToString(entity_b->type()), entity_b->id(),
                     entityTypeToString(entity_a->type()), entity_a->id());
    }
}

void ActionSystem::combatOutcome(int const tick, std::shared_ptr<Entity> &attacker, std::shared_ptr<Entity> &defender, std::vector<std::pair<entity_type, Cords>> &pendingDrops, Statistics &statistics)
{
    if (!defender->isAlive())
    {
        attacker->remove<CDestination>();
        attacker->remove<CTarget>();
        if (defender->type() == deer)
        {
            ++attacker->get<CFeats>().slainDeer;
            ++statistics.totalDeersSlain;
            pendingDrops.push_back({raw_meat, defender->get<CPosition>().cords});
            spdlog::info("[Tick: {:08d}] {}, ID:{:08d} slain {}, ID:{:08d}",
                         tick, entityTypeToString(attacker->type()), attacker->id(),
                         entityTypeToString(defender->type()), defender->id());
        }
    }
    if (!attacker->isAlive())
    {
        spdlog::info("[Tick: {:08d}] {}, ID:{:08d} slain {}, ID:{:08d}",
                     tick, entityTypeToString(defender->type()), defender->id(),
                     entityTypeToString(attacker->type()), attacker->id());

        // todo inventory drop
    }
}
