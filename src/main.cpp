#include "Engine.h"

int main()
{
    Engine engine(12345, 25, 25);
    printf("BEGIN\n");
    fflush(stdout);
    while (engine.getTick() < 1000000)
    {
        engine.simulate();
    }
    printf("END\n");
    fflush(stdout);
    return 0;
}