#pragma once
#include "utility.hpp"
#include <optional>
#include <map>

class Component
{
public:
    bool exists = false;
    Component();
};

struct CPosition : public Component
{
    Cords cords;
    CPosition() = default;
    CPosition(int x, int y);
};

struct CDesination : public Component
{
    Cords cords;
    CDesination() = default;
    CDesination(int x, int y);
};

struct CStats : public Component
{
    int hitPoints, strength, speed = 1;
    CStats() = default;
};

class CHunger : public Component
{
    int m_currentHunger = 0;
    int m_hungerTick = 10;

public:
    bool isHungry();
    void hungerTick();
    bool isStarved();
    CHunger();
    CHunger(int hunger, int hungerTick);
    const int getHunger() const;
    void reset();
};

class CKnowledge : public Component
{

public:
    std::map<Cords, Seen> m_reported_positions;
    std::optional<Cords> m_closetst_food;
    CKnowledge() = default;
    CKnowledge(bool exists);
};

struct CState : public Component
{
    STATE state;
    CState() = default;
    CState(STATE state);
};

class CLineOfSight : public Component
{
    int m_sight = 0;

public:
    CLineOfSight() = default;
    CLineOfSight(int sight);
    const int sight() const;
};

class CInventory : public Component
{
    int m_food = 0;
    int m_maxFood = 10;
    int m_maxMeal = 0;
    int m_meal = 0;

public:
    CInventory() = default;
    CInventory(int maxMeal);

    const int mealCount();
    const int maxMeal();
    bool adjustMeal(int value);
};