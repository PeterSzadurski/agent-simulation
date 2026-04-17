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

struct CDestination : public Component
{
    Cords cords;
    CDestination() = default;
    CDestination(int x, int y);
    CDestination(const Cords &c);
};

struct CStats : public Component
{
    int hitPoints, strength, speed = 1;
    CStats() = default;
    CStats(int hp, int str, int spd);
};

class CDecay : public Component
{
protected:
    int m_currentDecay = 0;
    int m_decayTick = 10;

public:
    bool isHalfway();
    void decayTick();
    bool isDecayed();
    CDecay();
    CDecay(int decay, int decayTick);
    const int getDecay() const;
    void reset();
};
class CHunger : public CDecay
{
public:
    using CDecay::CDecay;
};

class CFuel : public CDecay
{
public:
    using CDecay::CDecay;
};

class CKnowledge : public Component
{

public:
    std::map<Cords, Seen> m_reported_positions;
    std::optional<Cords> m_closest_meal;
    std::optional<Cords> m_closest_tree;
    std::optional<Cords> m_closest_food;
    Cords m_campfire;
    CKnowledge() = default;
    CKnowledge(int width, int height);
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