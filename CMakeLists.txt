cmake_minimum_required(VERSION 3.28)
project(pj_status2 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    URL file://${CMAKE_SOURCE_DIR}/3rd/SFML-2.6.2.zip
)
FetchContent_MakeAvailable(SFML)

add_executable(status2 
    src/main.cpp 
    src/Core/Game.h 
    src/Core/Game.cpp
    src/Core/ResourceManager.h
    src/Core/ResourceManager.cpp
    src/Core/GameState.h
    src/Core/StateManager.h
    src/Core/StateManager.cpp
    src/States/MenuState.h
    src/States/MenuState.cpp
)

target_compile_features(status2 PRIVATE cxx_std_17)
target_include_directories(status2 PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(status2 PRIVATE sfml-graphics sfml-window sfml-system sfml-audio sfml-network)
