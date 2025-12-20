#pragma once
#include "utility.hpp"
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
};

class CKnowledge : public Component
{
    std::map<Cords, Seen> m_reported_positions;

public:
    CKnowledge() = default;
    CKnowledge(bool exists);
    void update(Cords cords, entity_type type);
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
    int food = 0;
    int maxFood = 10;

public:
    CInventory() = default;
    CInventory(int food);

    const int getFood();
};