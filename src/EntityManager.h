#pragma once
#include <vector>
#include <memory>
#include <map>
#include "Entity.hpp"
#include <spdlog/spdlog.h>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<entity_type, EntityVec> EntityMap;

class EntityManager
{
    EntityVec m_entities;
    EntityVec m_toAdd;
    EntityMap m_entityMap;
    size_t m_totalEntities = 0;

public:
    EntityManager();
    ~EntityManager();
    void update();
    std::shared_ptr<Entity> addEntity(entity_type type);
    EntityVec &getEntities();
    EntityVec &getEntities(entity_type type);
};
