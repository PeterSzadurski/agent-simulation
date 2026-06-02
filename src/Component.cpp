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
               {wood, 0},
               {pelt, 0}};
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

bool CInventory::transferTo(CInventory &other)
{
    for (auto &[type, count] : m_items)
    {
        if (count > 0)
        {
            if (!other.adjustItems(type, count))
                return false;
            m_totalCount -= count;
            count = 0;
        }
    }
    m_totalCount = 0;
    return true;
}

entity_type CInventory::randomItem(std::mt19937 &rng)
{
    std::vector<entity_type> available;
    for (auto &[type, count] : m_items)
    {
        if (count > 0)
            available.push_back(type);
    }
    std::uniform_int_distribution<int> dist(0, available.size() - 1);
    return available[dist(rng)];
}

CStats::CStats(int hp, int str, int spd) : hitPoints(hp), strength(str), speed(spd), m_maxHitPoints(hp)
{
    exists = true;
}

const int CStats::maxHitPoints()
{
    return m_maxHitPoints;
}

void CStats::setMaxHitPoints(int maxHitPoints)
{
    m_maxHitPoints = maxHitPoints;
}

CExperience::CExperience(float fastRate, float slowRate, int hpCap, int strCap, int spdCap) : m_fastRate(fastRate), m_slowRate(slowRate), m_hpCap(hpCap), m_strCap(strCap), m_spdCap(spdCap)
{
    exists = true;
    hpExp = 0.f;
    strExp = 0.f;
    spdExp = 0.f;
}

const int CExperience::hpCap()
{
    return m_hpCap;
}

const int CExperience::strCap()
{
    return m_strCap;
}

const int CExperience::spdCap()
{
    return m_spdCap;
}

const float CExperience::fastRate()
{
    return m_fastRate;
}
const float CExperience::slowRate()
{
    return m_slowRate;
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

CRole::CRole(ROLE role) : role(role)
{
    exists = true;
}
