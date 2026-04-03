#include <math.h>
enum STATE
{
    idle,
    wander,
    walking_to
};

enum entity_type
{
    empty,
    npc,
    meal,
    grass,
    animal,
    campfire,
    raw_meat
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
};

inline float Distance2(const Cords &a, const Cords &b)
{
    int dX = a.x - b.x;
    int dY = a.y - b.y;
    return dX * dX + dY * dY;
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
