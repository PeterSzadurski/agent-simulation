#pragma once
#include "Grid.h"
enum Action
{
    Eat,
    Cook,
    RefuelCampfire,
    GatherFood,
    CutTree,
    TransferToCampfire,
    PickupCampMeal,
    EatGrass,
    Flee,
    HuntDeer,
    None,
    Wander,
    ButcherDeer,
    PickupMeal,
    PickupWood,
    PickupLoot,
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
    bool hasKnowledgeTree = false;
    bool hasKnowledgeGrass = false;
    bool hasKnowledgeDeer = false;
    bool hasKnowledgeMeal = false;
    bool hasKnowledgeWood = false;
    bool hasKnowledgeLoot = false;
    bool hasKnowledgeDeerCorpse = false;
    bool isAlreadyAtCampfire = false;

    std::optional<Cords> threatPosition;
};

class DecisionSystem
{
    int scoreEat(const EntityState &es);
    int scoreCook(const EntityState &es);
    int scoreRefuel(const EntityState &es);
    int scoreGatherFood(const EntityState &es);
    int scoreGatherWood(const EntityState &es);
    int scorePickupWood(const EntityState &es);
    int scoreTransferToCampfire(const EntityState &es);
    int scorePickupCampMeal(const EntityState &es);
    int scorePickupMeal(const EntityState &es);
    int scoreEatGrass(const EntityState &es);
    int scoreFlee(const EntityState &es);
    int scoreHuntDeer(const EntityState &es);
    int scoreButcherDeer(const EntityState &es);
    int scorePickupLoot(const EntityState &es);

public:
    DecisionSystem();
    Action chooseNpcAction(const EntityState &es);
    Action chooseDeerAction(const EntityState &es);
};
