# Agent Simulation

A deterministic survival simulation built with an ECS and a priority based AI.
Runs 1,000,000 ticks in 4-5 seconds on a Ryzen 5800X3D.

## The Agents
**NPC:** Hunts deer and cuts down trees in order to cook meals at a central campfire.
Must eat cooked food or starve. Can store extra meals at the campfire.

**Deer:** Eats grass to survive. Flees if an NPC is within line of sight. 
Can kill NPCs in combat.

## How It Works

### Hunger and Survival
All agents have a hunger decay timer. NPCs must cook and eat meals, deer must 
find grass. An agent that cannot eat will die. The campfire must stay fueled 
with wood or cooking is impossible, creating a chain of dependencies that drives 
all NPC behaviour.

### Knowledge System
Every agent has its own Knowledge component. Agents memorize the positions of 
trees, grass, deer, and other NPCs within their line of sight. When an agent 
needs a resource it checks stored knowledge for the closest known location rather 
than wandering blindly.

NPCs read/write knowledge at the campfire. This means NPCs can act on information gathered by 
other NPCs without any direct communication. Agents that have never visited the 
campfire have no shared knowledge.

The knowledge system makes the agents behave differently from the traditional blackboard AI approach.

### Priority Based AI
Each tick, every NPC scores all possible actions and takes the highest scoring 
one. Actions are opportunistic. if an NPC arrives at the campfire carrying both 
wood and raw meat, it will refuel first, then cook, rather than making two 
separate trips.

### Combat
Combat is resolved with dice rolls based on the strength stat. Strength is the 
maximum potential damage an agent can deal. Whoever rolls highest deals the 
difference as damage. A deer can kill an NPC if it rolls higher.

### Spawn System
Every 500 ticks a new entity spawns. Grass is most common, new NPCs are most 
rare. This keeps the world populated over the full simulation.


## Simulation Loop
Each tick runs in this order:
1. Line of sight: agents update their knowledge
2. Action: agents decide and act
3. Movement: agents move toward destinations or wander
4. Decay: hunger and fuel decay
5. Cleanup: dead entities removed, pending drops placed
6. Spawn: periodic entity spawning

## Known Limitations
- In rare cases a raw meat drop is lost if two NPCs kill deer on the same tile 
  in the same tick. This simulation has been tested up to 100,000,000 ticks.

## Compatibility
Developed and tested on Arch Linux (6.18.22-1-lts) with GCC and C++23. Docker is the 
recommended way to run this on other platforms.

## Building Locally
**Arch Linux:** `sudo pacman -S gcc cmake spdlog fmt`

**Debian/Ubuntu:** `sudo apt-get install g++ cmake libspdlog-dev libfmt-dev`

A `.vscode` folder is included with IntelliSense, debug, and build task 
configuration. Requires the CMake Tools and C/C++ extensions. Also requires 
`clang` for IntelliSense (`sudo pacman -S clang` on Arch).

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/agent-simulation 123455
```

## Running with Docker

Build the image:
```bash
docker build -t agent-simulation .
```

Run with a seed:
```bash
docker run agent-simulation 123455
```

Save output to a file:
```bash
docker run agent-simulation 123455 > sim.log
```

## Seed

The seed controls all random behaviour. Spawning, agent stats, wandering, and combat encounters.
The same seed will always produce the same simulation. Defaults to `123455`.

Determinism is guaranteed on the same platform and compiler. Output may differ 
across architectures or compilers.

## Verifying Correctness
Run with the default seed and check the final summary matches:
```bash
[2026-04-20 01:29:19.805] [info] ID:00000003 hunted 3 deer.
[2026-04-20 01:29:19.805] [info] ID:00000003 cooked 2 meals.
[2026-04-20 01:29:19.805] [info] ID:00000003 ate 3 meals.
[2026-04-20 01:29:19.805] [info] ID:00000692 chopped 10 trees.
[2026-04-20 01:29:19.805] [info] 46 total deer hunted.
[2026-04-20 01:29:19.805] [info] 26 total meals cooked.
[2026-04-20 01:29:19.805] [info] 23 total meals consumed.
[2026-04-20 01:29:19.805] [info] 509 total trees chopped.
```

## Output

Logs each significant simulation event to stdout.
Prints a summary at the end:
- Best hunter, cook, lumberjack
- Total deer hunted, meals cooked, meals consumed, trees chopped

The summary makes it easy to check if the simulation is repeatable without reading the entire log.

## Attribution

The ECS is partly derived from COMP 4300 - Intro to C++ Game Programming (2024-09) by Dave Churchill. I was not a student at his university. I would highly recommend this course if you would like to learn C++ with game development or need a C++ refresher.
The components are mostly original. The majority of the systems are original. 

If you happen to be one of his students, please do the assignments yourself.

