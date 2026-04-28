class EntityStats {
    id = 0;
    x = 0;
    y = 0;
    type = 0;
    alive = false;
    exists = false;

    hasStats = false;
    hitPoints = 0;
    strength = 0;
    speed = 0;

    hasHunger = false;
    hunger = 0;

    hasFuel = false;
    fuel = 0;

    hasState = false;
    state = 0;

    hasDestination = false;
    destX = 0;
    destY = 0;

    hasInventory = false;
    rawMeat = 0;
    meals = 0;
    wood = 0;

    hasTarget = false;
    targetX = 0;
    targetY = 0;

    hasFeats = false;
    slainDeer = 0;
    choppedTrees = 0;
    foodAte = 0;
    mealsCooked = 0;

    hasKnowledge = false;

    hasClosestFood = false;
    closestFoodX = 0;
    closestFoodY = 0;

    hasClosestTree = false;
    closestTreeX = 0;
    closestTreeY = 0;

    hasClosestDeer = false;
    closestDeerX = 0;
    closestDeerY = 0;

    hasClosestGrass = false;
    closestGrassX = 0;
    closestGrassY = 0;
}
export default EntityStats;
