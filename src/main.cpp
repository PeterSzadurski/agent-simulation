#include "Engine.h"

int main(int argc, char *argv[])
{
    uint32_t seed = 12345;
    if (argc > 1)
    {
        seed = static_cast<uint32_t>(std::stoul(argv[1]));
    }

    Engine engine(seed, 500, 25, 25);
    printf("BEGIN\n");
    fflush(stdout);
    while (engine.getTick() < 1000000)
    {
        engine.simulate();
    }
    engine.printFeats();
    fflush(stdout);
    return 0;
}