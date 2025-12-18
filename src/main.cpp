#include "Engine.h"

int main()
{
    Engine engine(12345, 25, 25);
    while (engine.getTick() < 1000000)
    {
        engine.simulate();
    }
    return 0;
}