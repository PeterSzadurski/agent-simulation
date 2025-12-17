#include "Engine.h"

int main()
{
    Engine engine(12345);
    while (engine.getTick() < 10)
    {
        engine.simulate();
    }
    return 0;
}