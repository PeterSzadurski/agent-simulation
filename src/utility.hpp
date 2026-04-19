#pragma once
#include <math.h>
#include <string>
#include <fmt/format.h>

enum STATE
{
    idle,
    wander,
    walking_to,
    feeling_from
};

enum entity_type
{
    empty,
    npc,
    meal,
    grass,
    deer,
    campfire,
    raw_meat,
    tree,
    wood
};

inline const char *entityTypeToString(entity_type type)
{
    static const char *names[]{
        "empty",
        "NPC",
        "meal",
        "grass",
        "deer",
        "campfire",
        "raw meat",
        "tree",
        "wood"};
    return names[static_cast<int>(type)];
};

struct Cords
{
    int x = 0, y = 0;
    Cords() { x = -1, y = -1; }
    Cords(int x, int y) : x(x), y(y) {};

    bool operator<(const Cords &other) const
    {
        if (x != other.x)
            return x < other.x;
        return y < other.y;
    }
    static Cords normal(Cords pos, Cords dest)
    {
        int dX = pos.x - dest.x;
        int dY = pos.y - dest.y;

        if (dX != 0)
        {
            dX = dX / abs(dX);
        }
        if (dY != 0)
        {
            dY = dY / abs(dY);
        }

        return Cords(dX, dY);
    }

    bool operator==(const Cords &other) const
    {
        return x == other.x && y == other.y;
    }

    std::string toStringPadded() const
    {
        return fmt::format("({:02d},{:02d})", x, y);
    }
};

inline float Distance2(const Cords &a, const Cords &b)
{
    int dX = a.x - b.x;
    int dY = a.y - b.y;
    return dX * dX + dY * dY;
}

inline bool isNextToCord(Cords &a, Cords &b)
{
    int x = a.x - b.x;
    int y = a.y - b.y;
    return abs(x) <= 1 && abs(y) <= 1;
}

struct Seen
{
    entity_type type;
    int tick;
    Seen(entity_type type, int tick) : type(type), tick(tick)
    {
    }
    Seen() {}
};
