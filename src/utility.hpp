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
    meal
};

struct Cords
{
    int x = 0, y = 0;
    Cords() { x = 0, y = 0; }
    Cords(int x, int y) : x(x), y(y) {};

    bool operator<(const Cords &other) const
    {
        if (x != other.x)
            return x < other.x;
        return y < other.y;
    }
};

struct Seen
{
    entity_type type;
    int tick;
    Seen(entity_type type, int tick) : type(type), tick(tick)
    {
    }
};
