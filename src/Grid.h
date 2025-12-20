#pragma once
#include "Entity.hpp"
#include "memory"
#include <random>
#include "vector"
class Grid
{
private:
    int m_width, m_height = 0;
    std::vector<std::shared_ptr<Entity>> m_entities;
    std::vector<int> m_dense;
    std::vector<int> m_sparse;
    void removeEmpty(int idx);
    void addEmpty(int idx);
    void clear(int idx);

public:
    Grid(int width, int height);
    Grid() {};
    ~Grid();
    int const getIdx(int x, int y) const;
    std::shared_ptr<Entity> &at(int x, int y);
    const std::shared_ptr<Entity> &at(int x, int y) const;
    bool inBounds(int x, int y) const;
    bool move(std::shared_ptr<Entity> entity, int x, int y);
    void remove(std::shared_ptr<Entity> entity);
    bool updateLineOfSight(std::shared_ptr<Entity> entity);
    void place(std::shared_ptr<Entity>);
    bool placeRandom(std::shared_ptr<Entity>, std::mt19937 &rng);
    Cords toCords(int idx);
};
