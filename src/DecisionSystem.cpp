#include "DecisionSystem.h"
#include <algorithm>

EntityState::EntityState(std::shared_ptr<Entity> e, std::shared_ptr<Entity> campfire)
{
    auto &inv = e->get<CInventory>();
    auto &campInv = campfire->get<CInventory>();
    auto &pos = e->get<CPosition>();
    auto &knowledge = e->get<CKnowledge>();

    if (e->has<CInventory>())
    {
        hasInventorySpace = inv.hasRoom();
        hasMeals = inv.itemCount(meal) > 0;
        hasRawMeat = inv.itemCount(raw_meat) > 0;
        hasWood = inv.itemCount(wood) > 0;
    }
    isHungry = e->get<CHunger>().isHalfway();
    hasCampMeals = campInv.itemCount(meal);
    // /hasCampWood = campInv.itemCount(wood);
    hasCampRawMeat = campInv.itemCount(raw_meat);

    isCampfireFueled = !campfire->get<CFuel>().isDecayed();

    hasKnowledgeRawMeat = knowledge.m_closest_food.has_value();
    hasKnowledgeTree = knowledge.m_closest_tree.has_value();
    hasKnowledgeGrass = knowledge.m_closest_grass.has_value();
    isAlreadyAtCampfire = isNextToCord(pos.cords, knowledge.m_campfire);
}

DecisionSystem::DecisionSystem()
{
}

int DecisionSystem::scoreEat(const EntityState &es)
{
    if (es.hasMeals && es.isHungry)
        return 100;
    return 0;
}

int DecisionSystem::scoreCook(const EntityState &es)
{
    int baseScore = 75;
    if ((es.hasRawMeat || es.hasCampRawMeat) && es.isCampfireFueled)
    {
        if (es.isAlreadyAtCampfire)
        {
            return baseScore + 50;
        }
        return baseScore;
    }
    return 0;
}

int DecisionSystem::scoreRefuel(const EntityState &es)
{
    int baseScore = 90;
    if (es.hasWood && !es.isCampfireFueled)
    {
        if (es.isAlreadyAtCampfire)
            return baseScore + 50;
        return baseScore;
    }
    return 0;
}

int DecisionSystem::scoreGatherFood(const EntityState &es)
{
    if (es.hasKnowledgeRawMeat && es.hasInventorySpace)
        return 80;
    return 0;
}

int DecisionSystem::scoreGatherWood(const EntityState &es)
{
    if (es.hasKnowledgeTree && es.hasInventorySpace && !es.hasWood && !es.isCampfireFueled)
        return 85;
    return 0;
}

int DecisionSystem::scoreTransferToCampfire(const EntityState &es)
{
    if (!es.hasInventorySpace)
        return 95;
    return 0;
}

int DecisionSystem::scorePickupMeal(const EntityState &es)
{
    int baseScore = 95;
    if (es.hasCampMeals && !es.hasMeals)
    {
        if (es.isAlreadyAtCampfire)
        {
            return baseScore + 50;
        }
        return baseScore;
    }
    return 0;
}

int DecisionSystem::scoreEatGrass(const EntityState &es)
{
    if (es.hasKnowledgeGrass && es.isHungry)
    {
        return 100;
    }
    return 0;
}

Action DecisionSystem::chooseNpcAction(const EntityState &es)
{
    std::vector<std::pair<Action, int>> scores = {
        {Eat, scoreEat(es)},
        {Cook, scoreCook(es)},
        {GatherWood, scoreGatherWood(es)},
        {GatherFood, scoreGatherFood(es)},
        {TransferToCampfire, scoreTransferToCampfire(es)},
        {RefuelCampfire, scoreRefuel(es)},
        {PickupMeal, scorePickupMeal(es)},
        {Action::Wander, 1}};
    return std::max_element(scores.begin(), scores.end(), [](auto &a, auto &b)
                            { return a.second < b.second; })
        ->first;
}

Action DecisionSystem::chooseDeerAction(const EntityState &es)
{
    std::vector<std::pair<Action, int>> scores = {
        {EatGrass, scoreEatGrass(es)},
        {Action::Wander, 1}};
    return std::max_element(scores.begin(), scores.end(), [](auto &a, auto &b)
                            { return a.second < b.second; })
        ->first;
}
