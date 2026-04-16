#pragma once
#include "Grid.h"
enum Action
{
    Eat,
    Cook,
    RefuelCampfire,
    GatherFood,
    GatherWood,
    TransferToCampfire,
    PickupMeal,
    None,
    Wander
};

struct EntityState
{
    EntityState(std::shared_ptr<Entity> e, std::shared_ptr<Entity> campfire);
    bool hasRawMeat = false;
    bool hasMeals = false;
    bool hasInventorySpace = false;
    bool isCampfireFueled = false;
    bool hasWood = false;
    bool isHungry = false;
    bool hasCampMeals = false;
    bool hasCampRawMeat = false;
    bool hasKnowledgeRawMeat = false;
    bool isAlreadyAtCampfire = false;
};

class DecisionSystem
{
    int scoreEat(const EntityState &es);
    int scoreCook(const EntityState &es);
    int scoreRefuel(const EntityState &es);
    int scoreGatherFood(const EntityState &es);
    int scoreGatherWood(const EntityState &es);
    int scoreTransferToCampfire(const EntityState &es);
    int scorePickupMeal(const EntityState &es);

public:
    DecisionSystem();
    Action chooseAction(const EntityState &es);
};
