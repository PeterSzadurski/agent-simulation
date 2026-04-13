cmake_minimum_required(VERSION 4.0)
project(agent-simulation)
set(CMAKE_CXX_STANDARD 23)


add_executable(${PROJECT_NAME}
   src/Grid.cpp
   src/Engine.cpp
   src/KnowledgeSystem.cpp
   src/MovementSystem.cpp
   src/DecisionSystem.cpp
   src/Component.cpp
   src/EntityManager.cpp
   src/main.cpp
)

target_link_libraries(${PROJECT_NAME} fmt spdlog)
