#include "EntityManager.h"
#include <algorithm>

EntityManager::EntityManager::EntityManager()
{
}

EntityManager::EntityManager::~EntityManager()
{
}

void EntityManager::EntityManager::update()
{
    for (auto e : m_toAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->type()].push_back(e);
    }
    m_toAdd.clear();

    for (int n = m_entities.size() - 1; n >= 0; --n)
    {
        auto e = m_entities[n];
        if (!e->isAlive())
        {
            auto &map = m_entityMap[e->type()];
            auto map_iterator = std::find(map.begin(), map.end(), e);

            if (map_iterator != map.end())
            {
                map.erase(map_iterator);
            }

            m_entities.erase(m_entities.begin() + n);
        }
    }
}

std::shared_ptr<Entity> EntityManager::addEntity(entity_type type)
{
    auto e = std::shared_ptr<Entity>(new Entity(type, m_totalEntities++));
    m_toAdd.push_back(e);
    return e;
}

EntityVec &EntityManager::EntityManager::getEntities()
{
    return m_entities;
}

EntityVec &EntityManager::EntityManager::getEntities(entity_type type)
{
    return m_entityMap[type];
}
