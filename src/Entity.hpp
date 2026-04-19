#pragma once
#include "Component.h"
#include "tuple"
#include <string>

class EntityManager;
using ComponentTuple = std::tuple<CPosition, CStats, CDecay, CHunger, CFuel,
                                  CLineOfSight, CState, CKnowledge, CDestination,
                                  CInventory, CTarget, CThreat, CFeats>;

class Entity
{
    friend class EntityManager;
    entity_type m_type;
    int m_alive;
    ComponentTuple m_components;
    size_t m_id;

    Entity(const entity_type tag, size_t id)
    {
        m_id = id;
        m_type = tag;
        m_alive = true;
    }

    Entity(size_t id)
    {
        m_id = id;
        m_type = entity_type::empty;
        m_alive = true;
    }

public:
    bool isAlive() { return m_alive; }
    void setAlive(bool val) { m_alive = val; }
    entity_type type() { return m_type; }
    size_t id()
    {
        return m_id;
    }
    template <typename T, typename... TArgs>
    T &add(TArgs &&...mArgs)
    {
        auto &component = get<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        return component;
    }
    template <typename T>
    T &get()
    {
        return std::get<T>(m_components);
    }

    template <typename T>
    const T &get() const
    {
        return std::get<T>(m_components);
    }

    template <typename T>
    bool has() const
    {
        return get<T>().exists;
    }

    template <typename T>
    void remove()
    {
        get<T>() = T();
    }
};