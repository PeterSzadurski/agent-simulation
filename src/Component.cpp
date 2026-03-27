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

CInventory::CInventory(int maxMeal) : m_maxMeal(maxMeal)
{
    exists = true;
}

const int CInventory::mealCount()
{
    return m_meal;
}

const int CInventory::maxMeal()
{
    return m_maxMeal;
}

bool CInventory::adjustMeal(int value)
{
    if (m_meal + value > m_maxMeal || m_meal + value < 0)
    {
        return false;
    }
    m_meal += value;
    return true;
}

const int CInventory::foodCount()
{
    return m_food;
}

const int CInventory::maxFood()
{
    return m_maxFood;
}

bool CInventory::adjustFood(int value)
{
    if (m_food + value > m_maxFood || m_food + value < 0)
    {
        return false;
    }
    m_food += value;
    return true;
}
