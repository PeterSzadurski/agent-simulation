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

CDesination::CDesination(int x, int y)
{
    cords = Cords(x, y);
    exists = true;
}

CHunger::CHunger()
{
    exists = false;
}

bool CHunger::isHungry()
{
    return m_currentHunger > 4;
}

void CHunger::hungerTick()
{
    --m_hungerTick;
    if (m_hungerTick < 0)
    {
        ++m_currentHunger;
        m_hungerTick = 10;
    }
}

void CHunger::reset()
{
    m_currentHunger = 0;
    m_hungerTick = 10;
}

bool CHunger::isStarved()
{
    return m_currentHunger > 9;
}

CHunger::CHunger(int hunger, int hungerTick) : m_currentHunger(hunger), m_hungerTick(hungerTick)
{
    exists = true;
}

const int CHunger::getHunger() const
{
    return m_currentHunger;
}

CKnowledge::CKnowledge(bool doesExist)
{
    exists = doesExist;
}

CLineOfSight::CLineOfSight(int sight) : m_sight(sight)
{
    exists = true;
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
               {meal, 0}};
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
