#pragma once
#include "Cords.h"
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

class CInventory : public Component
{
    int food = 0;
    int maxFood = 10;

public:
    CInventory() = default;
    CInventory(int food);

    const int getFood();
};