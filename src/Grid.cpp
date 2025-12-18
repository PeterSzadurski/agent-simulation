#include "Grid.h"
#include <assert.h>

Grid::Grid(int width, int height) : m_width(width), m_height(height), m_entities(width * height, nullptr)
{
}

Grid::~Grid()
{
}

int const Grid::getIdx(int x, int y) const
{
    assert(inBounds(x, y));
    return y * m_width + x;
}

const std::shared_ptr<Entity> &Grid::at(int x, int y) const
{
    return m_entities[getIdx(x, y)];
}
std::shared_ptr<Entity> &Grid::at(int x, int y)
{
    return m_entities[getIdx(x, y)];
}

bool Grid::inBounds(int x, int y) const
{
    return (x >= 0 && y >= 0) && (x < m_width && y < m_height);
}

bool Grid::move(std::shared_ptr<Entity> entity, int x, int y)
{
    auto &pos = entity->get<CPosition>();
    int nX = pos.cords.x + x;
    int nY = pos.cords.y + y;

    if (!inBounds(nX, nY))
        return false;
    at(pos.cords.x, pos.cords.y) = nullptr;
    pos.cords.x = nX;
    pos.cords.y = nY;
    m_entities[getIdx(pos.cords.x, pos.cords.y)] = entity;
    return true;
}

void Grid::remove(std::shared_ptr<Entity> entity)
{
    auto &pos = entity->get<CPosition>();
    if (inBounds(pos.cords.x, pos.cords.y))
    {
        at(pos.cords.x, pos.cords.y) = nullptr;
    }
}
