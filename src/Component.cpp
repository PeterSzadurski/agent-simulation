#include "Component.h"

Component::Component()
{
    exists = false;
}

CPosition::CPosition(int x, int y)
{
    cords = Cords(x, y);
    exists = true;
}
CPosition::CPosition(const Cords &c)
{
    cords = c;
    exists = true;
}

CDestination::CDestination(int x, int y)
{
    cords = Cords(x, y);
    exists = true;
}
CDestination::CDestination(const Cords &c)
{
    cords = c;
    exists = true;
}

CDecay::CDecay()
{
    exists = false;
}

bool CDecay::isHalfway()
{
    return m_currentDecay > 4;
}

void CDecay::decayTick()
{
    --m_decayTick;
    if (m_decayTick < 0)
    {
        ++m_currentDecay;
        m_decayTick = 10;
    }
}

void CDecay::reset()
{
    m_currentDecay = 0;
    m_decayTick = 10;
}

bool CDecay::isDecayed()
{
    return m_currentDecay > 9;
}

CDecay::CDecay(int decay, int decayTick) : m_currentDecay(decay), m_decayTick(decayTick)
{
    exists = true;
}

const int CDecay::getDecay() const
{
    return m_currentDecay;
}

CKnowledge::CKnowledge(int width, int height)
{
    exists = true;
    m_campfire.x = width / 2;
    m_campfire.y = height / 2;
}

CLineOfSight::CLineOfSight(int sight) : m_sight(sight)
{
    exists = true;
    m_detectedEntities.reserve(sight * sight * 4);
}

const int CLineOfSight::sight() const
{
    return m_sight;
}

CState::CState(STATE state) : state(state)
{
    exists = true;
}

CInventory::CInventory(int maxItems) : m_maxItems(maxItems)
{
    exists = true;
    m_items = {{raw_meat, 0},
               {meal, 0},
               {wood, 0}};
}

bool CInventory::adjustItems(entity_type e_type, int value)
{
    if ((m_maxItems > 0 && m_totalCount + value > m_maxItems) || m_totalCount + value < 0)
        return false;
    m_items[e_type] += value;
    m_totalCount += value;
    return true;
}

const int CInventory::totalCount()
{
    return m_totalCount;
}

const int CInventory::maxItems()
{
    return m_maxItems;
}

const bool CInventory::hasRoom()
{
    return m_maxItems <= 0 || m_totalCount < m_maxItems;
}

const int CInventory::itemCount(entity_type e_type)
{
    return m_items[e_type];
}

CStats::CStats(int hp, int str, int spd) : hitPoints(hp), strength(str), speed(spd)
{
    exists = true;
}

CThreat::CThreat(const Cords &c)
{
    exists = true;
    threatPos = c;
}

CFeats::CFeats(bool exists)
{
    this->exists = exists;
}
