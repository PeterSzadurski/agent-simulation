#pragma once
#include "Cords.h"
class Component
{
public:
    bool exists = false;
    Component();
};

struct CPosition : Component
{
    Cords cords;
    CPosition() = default;
    CPosition(int x, int y);
};

struct CStats : Component
{
    int hitPoints, strength, speed = 1;
    CStats() = default;
};
