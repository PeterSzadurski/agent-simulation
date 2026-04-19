#pragma once
#include "utility.hpp"
#include <optional>
#include <map>
#include <unordered_map>
#include <memory>
#include <vector>

class Entity;
typedef std::pair<entity_type, Cords> EntityPos;

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
    CPosition(const Cords &c);
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
    std::optional<Cords> m_closest_grass;
    std::optional<Cords> m_closest_deer;
    Cords m_lastRecalcPosition;
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
    std::vector<EntityPos> m_detectedEntities;
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

struct CTarget : public Component
{
    std::shared_ptr<Entity> target;

    CTarget() = default;
    CTarget(std::shared_ptr<Entity> t) : target(t) { exists = true; }
};
struct CThreat : public Component
{
    Cords threatPos;
    CThreat() = default;
    CThreat(const Cords &c);
};