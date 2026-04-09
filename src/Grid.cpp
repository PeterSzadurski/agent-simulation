#include "Grid.h"
#include <assert.h>

Grid::Grid(int width, int height) : m_width(width), m_height(height), m_entities(width * height, nullptr)
{
    m_dense.reserve(width * height);
    m_sparse.resize(width * height, -1);

    for (int n = 0; n < width * height; n++)
    {
        m_sparse[n] = m_dense.size();
        m_dense.push_back(n);
    }
}

Grid::~Grid()
{
}

void Grid::removeEmpty(int idx)
{
    int pos = m_sparse[idx];

    int last = m_dense.back();
    m_dense[pos] = last;
    m_sparse[last] = pos;

    m_dense.pop_back();
    m_sparse[idx] = -1;
}

void Grid::addEmpty(int idx)
{
    m_sparse[idx] = m_dense.size();
    m_dense.push_back(idx);
}

void Grid::place(std::shared_ptr<Entity> entity)
{
    auto &pos = entity->get<CPosition>();
    int idx = getIdx(pos.cords.x, pos.cords.y);
    m_entities[idx] = entity;

    removeEmpty(idx);
}

bool Grid::placeRandom(std::shared_ptr<Entity> entity, std::mt19937 &rng)
{
    if (m_dense.empty())
    {
        return false;
    }

    std::uniform_int_distribution<int> dist(0, m_dense.size() - 1);
    int randIdx = dist(rng);
    entity->get<CPosition>().cords = toCords(randIdx);
    m_entities[randIdx] = entity;

    removeEmpty(randIdx);
    return true;
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
    auto &hunter = entity->get<CDecay>();
    int nX = pos.cords.x + x;
    int nY = pos.cords.y + y;

    if (!inBounds(nX, nY) || m_entities[getIdx(nX, nY)] != nullptr)
        return false;
    at(pos.cords.x, pos.cords.y) = nullptr;
    // if (m_emptyCells.)

    addEmpty(getIdx(pos.cords.x, pos.cords.y));
    pos.cords.x = nX;
    pos.cords.y = nY;
    removeEmpty(getIdx(pos.cords.x, pos.cords.y));
    m_entities[getIdx(pos.cords.x, pos.cords.y)] = entity;
    return true;
}

void Grid::remove(std::shared_ptr<Entity> entity)
{
    auto &pos = entity->get<CPosition>();
    if (inBounds(pos.cords.x, pos.cords.y))
    {
        at(pos.cords.x, pos.cords.y) = nullptr;
        addEmpty(getIdx(pos.cords.x, pos.cords.y));
        // m_sparse.push_back(getIdx(pos.cords.x, pos.cords.y));
    }
}

Cords Grid::toCords(int idx)
{
    assert(idx >= 0 && idx < m_width * m_height);
    return Cords(idx % m_width, idx / m_height);
}
