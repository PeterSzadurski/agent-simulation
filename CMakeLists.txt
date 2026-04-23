cmake_minimum_required(VERSION 3.28)
project(agent-simulation)
set(CMAKE_CXX_STANDARD 23)

if(EMSCRIPTEN)
  add_executable(${PROJECT_NAME}
   src/DecaySystem.cpp
   src/Grid.cpp
   src/Engine.cpp
   src/wasm_bindings.cpp
   src/KnowledgeSystem.cpp
   src/MovementSystem.cpp
   src/DecisionSystem.cpp
   src/ActionSystem.cpp
   src/Component.cpp
   src/EntityManager.cpp
   src/wasm_bindings.cpp
  )

  target_compile_definitions(${PROJECT_NAME} PRIVATE __EMSCRIPTEN__)
  target_link_options(${PROJECT_NAME} PRIVATE
      -sWASM=1
      -sALLOW_MEMORY_GROWTH=1
      --bind
  )
  set_target_properties(${PROJECT_NAME} PROPERTIES
      SUFFIX ".js"
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/web"
  )
else()
add_executable(${PROJECT_NAME}
   src/DecaySystem.cpp
   src/Grid.cpp
   src/Engine.cpp
   src/wasm_bindings.cpp
   src/KnowledgeSystem.cpp
   src/MovementSystem.cpp
   src/DecisionSystem.cpp
   src/ActionSystem.cpp
   src/Component.cpp
   src/EntityManager.cpp
   src/main.cpp
)

target_link_libraries(${PROJECT_NAME} fmt spdlog)
endif()