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
