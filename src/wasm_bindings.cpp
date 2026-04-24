#include <emscripten/bind.h>
#include "Engine.h"

EMSCRIPTEN_BINDINGS(simulation)
{
    emscripten::value_object<Statistics>("Statistics")
        .field("totalDeersSlain", &Statistics::totalDeersSlain)
        .field("totalMealsCooked", &Statistics::totalMealsCooked)
        .field("totalMealsEaten", &Statistics::totalMealsEaten)
        .field("totalTreesChopped", &Statistics::totalTreesChopped);

    emscripten::class_<Engine>("Engine")
        .constructor<uint32_t, int, int, int>()
        .function("simulate", &Engine::simulate)
        .function("getTick", &Engine::getTick)
        .function("getStatistics", &Engine::getStatistics)
        .function("getGridSnapshot", &Engine::getGridSnapshot);

    emscripten::register_vector<int>("VectorInt");
}