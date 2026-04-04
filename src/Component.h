#pragma once
#include "utility.hpp"
#include <optional>
#include <map>
#include <unordered_map>

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
    std::optional<Cords> m_closest_meal;
    std::optional<Cords> m_closest_food;
    Cords m_campfire = Cords(25 / 2, 25 / 2);
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
    int m_maxItems = 0;
    int m_totalCount = 0;
    std::unordered_map<entity_type, int> m_items;

public:
    CInventory() = default;
    CInventory(int maxItems);

    const int totalCount();
    const int maxItems();
    const bool hasRoom();
    const int itemCount(entity_type e_type);

    bool adjustItems(entity_type e_type, int value);
};