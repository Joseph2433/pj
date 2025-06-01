cmake_minimum_required(VERSION 3.28)
project(pj_status3 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    URL file://${CMAKE_SOURCE_DIR}/3rd/SFML-2.6.2.zip
)
FetchContent_MakeAvailable(SFML)

add_executable(${PROJECT_NAME} 
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
    src/Systems/Grid.h
    src/Systems/Grid.cpp
    src/Utils/Constants.h
    src/Entities/Entity.h
    src/Entities/Entity.cpp
)

# # 收集所有源文件
# file(GLOB_RECURSE SOURCES 
#     "src/*.cpp"
#     "src/*.h"
# )

# # 创建可执行文件  
# add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system sfml-audio sfml-network)