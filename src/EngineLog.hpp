#pragma once
#include "utility.hpp"
#include <spdlog/spdlog.h>

namespace EngineLog
{
    // Decay

    inline void decayTick(int tick, size_t id, int decayValue,
                          const std::string &resource, const Cords &pos)
    {
        spdlog::info("[Tick: {:08d}] ID:{:08d} has ({:02d}) {} at pos {}",
                     tick, id, decayValue, resource, pos.toStringPadded());
    }

    inline void entityStarved(int tick, size_t id)
    {
        spdlog::info("[Tick: {:08d}] ID:{:08d} starved to death.", tick, id);
    }

    // Gathering

    inline void setDestination(int tick, size_t id, const std::string &target)
    {
        spdlog::info("[Tick: {:08d}] ID:{:08d} set destination to {}.", tick, id, target);
    }

    inline void pickedUp(int tick, size_t id, const std::string &item)
    {
        spdlog::info("[Tick: {:08d}] ID:{:08d} picked up {}.", tick, id, item);
    }

    inline void treeChopped(int tick, size_t id)
    {
        spdlog::info("──[Tick: {:08d}] ID:{:08d} chopped a tree.", tick, id);
    }

    // Campfire actions

    inline void cookedFood(int tick, size_t id)
    {
        spdlog::info("[Tick: {:08d}] ID:{:08d} cooked food.", tick, id);
    }

    inline void pickedUpMeal(int tick, size_t id)
    {
        spdlog::info("[Tick: {:08d}] ID:{:08d} picked up meal from campfire.", tick, id);
    }

    inline void refueled(int tick, size_t id)
    {
        spdlog::info("[Tick: {:08d}] ID:{:08d} refueled the campfire.", tick, id);
    }

    // Eating

    inline void ateFood(int tick, size_t id)
    {
        spdlog::info("[Tick: {:08d}] ID:{:08d} ate food.", tick, id);
    }

    inline void ateGrass(int tick, size_t id, entity_type type)
    {
        spdlog::info("[Tick: {:08d}] ID:{:08d} {} ate grass.",
                     tick, id, entityTypeToString(type));
    }

    // Combat

    inline void dealtDamage(int tick,
                            entity_type attackerType, size_t attackerId,
                            int damage,
                            entity_type defenderType, size_t defenderId)
    {
        spdlog::info("[Tick: {:08d}] {}, ID:{:08d} dealt {} damage to {}, ID:{:08d}",
                     tick,
                     entityTypeToString(attackerType), attackerId,
                     damage,
                     entityTypeToString(defenderType), defenderId);
    }

    inline void blockedAttack(int tick,
                              entity_type defenderType, size_t defenderId,
                              entity_type attackerType, size_t attackerId)
    {
        spdlog::info("[Tick: {:08d}] {}, ID:{:08d} blocks an attack from {}, ID:{:08d}",
                     tick,
                     entityTypeToString(defenderType), defenderId,
                     entityTypeToString(attackerType), attackerId);
    }

    inline void entitySlain(int tick,
                            entity_type killerType, size_t killerId,
                            entity_type victimType, size_t victimId)
    {
        spdlog::info("[Tick: {:08d}] {}, ID:{:08d} slain {}, ID:{:08d}",
                     tick,
                     entityTypeToString(killerType), killerId,
                     entityTypeToString(victimType), victimId);
    }

    // Spawning/world

    inline void entityRemoved(int tick, size_t id)
    {
        spdlog::info("[Tick: {:08d}] ID:{:08d} removed.", tick, id);
    }

    inline void placedAt(int tick, entity_type type, const Cords &pos)
    {
        spdlog::info("[Tick: {:08d}] Placed {} at {}", tick, entityTypeToString(type), pos.toStringPadded());
    }

    // End-of-simulation summary

    inline void printFeats(size_t hunterId, int deerCount,
                           size_t cookId, int cookCount,
                           size_t eaterId, int eatCount,
                           size_t lumberId, int lumberCount,
                           const Statistics &stats)
    {
        spdlog::info("ID:{:08d} hunted {} deer.", hunterId, deerCount);
        spdlog::info("ID:{:08d} cooked {} meals.", cookId, cookCount);
        spdlog::info("ID:{:08d} ate {} meals.", eaterId, eatCount);
        spdlog::info("ID:{:08d} chopped {} trees.", lumberId, lumberCount);
        spdlog::info("{} total deer hunted.", stats.totalDeersSlain);
        spdlog::info("{} total meals cooked.", stats.totalMealsCooked);
        spdlog::info("{} total meals consumed.", stats.totalMealsEaten);
        spdlog::info("{} total trees chopped.", stats.totalTreesChopped);
    }
}