cmake_minimum_required(VERSION 3.28)
project(status12 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_compile_options(-finput-charset=UTF-8)  

include(FetchContent)
FetchContent_Declare(SFML
    URL file://${CMAKE_SOURCE_DIR}/3rd/SFML-2.6.2.zip
)
FetchContent_MakeAvailable(SFML)

# 主程序入口
set(MAIN_SOURCES
    src/main.cpp
)

# 核心系统源文件
set(CORE_SOURCES
    src/Core/Game.cpp
    src/Core/StateManager.cpp
    src/Core/ResourceManager.cpp
)

set(CORE_HEADERS
    src/Core/Game.h
    src/Core/GameState.h
    src/Core/StateManager.h
    src/Core/ResourceManager.h
)

# 游戏状态源文件
set(STATES_SOURCES
    src/States/MenuState.cpp
    src/States/GamePlayState.cpp
    src/States/PauseState.cpp
    src/States/GameOverState.cpp
    src/States/VictoryState.cpp
)

set(STATES_HEADERS
    src/States/MenuState.h
    src/States/GamePlayState.h
    src/States/PauseState.h
    src/States/GameOverState.h
    src/States/VictoryState.h
)

# 实体基类源文件
set(ENTITIES_SOURCES
    src/Entities/Entity.cpp
    src/Entities/Plant.cpp
    src/Entities/Zombie.cpp
    src/Entities/Projectile.cpp
    src/Entities/Sun.cpp
)

set(ENTITIES_HEADERS
    src/Entities/Entity.h
    src/Entities/Plant.h
    src/Entities/Zombie.h
    src/Entities/Projectile.h
    src/Entities/Sun.h
)

# 植物类源文件
set(PLANTS_SOURCES
    src/Plants/Peashooter.cpp
    src/Plants/Sunflower.cpp
    src/Plants/WallNut.cpp
    src/Plants/IcePeashooter.cpp
)

set(PLANTS_HEADERS
    src/Plants/Peashooter.h
    src/Plants/Sunflower.h
    src/Plants/WallNut.h
    src/Plants/IcePeashooter.h
)

# 僵尸类源文件
set(ZOMBIES_SOURCES
    src/Zombies/BasicZombie.cpp
    src/Zombies/BigZombie.cpp
    src/Zombies/BossZombie.cpp
    src/Zombies/QuickZombie.cpp
   
)

set(ZOMBIES_HEADERS
    src/Zombies/BasicZombie.h
    src/Zombies/BigZombie.h
    src/Zombies/BossZombie.h
    src/Zombies/QuickZombie.h
)

# 游戏系统源文件
set(SYSTEMS_SOURCES
    src/Systems/Grid.cpp
    src/Systems/CollisionSystem.cpp
    src/Systems/WaveManager.cpp
    src/Systems/PlantManager.cpp
    src/Systems/ZombieManager.cpp
    src/Systems/SunManager.cpp
    src/Systems/ProjectileManager.cpp
)

set(SYSTEMS_HEADERS
    src/Systems/Grid.h
    src/Systems/CollisionSystem.h
    src/Systems/WaveManager.h
    src/Systems/PlantManager.h
    src/Systems/ZombieManager.h
    src/Systems/SunManager.h
    src/Systems/ProjectileManager.h
)

# UI组件源文件
set(UI_SOURCES
    src/UI/Button.cpp
    src/UI/SeedManager.cpp
    src/UI/SeedPacket.cpp
    src/UI/ProgressBar.cpp
    src/UI/HUD.cpp
)

set(UI_HEADERS
    src/UI/Button.h
    src/UI/SeedManager.h
    src/UI/SeedPacket.h
    src/UI/ProgressBar.h
    src/UI/HUD.h
)

# 工具类源文件
set(UTILS_SOURCES
    src/Utils/SoundManager.cpp
)

set(UTILS_HEADERS
    src/Utils/Constants.h

    src/Utils/SoundManager.h
)

# 子弹类源文件
set(PROJECTILES_SOURCES
    src/Projectiles/Pea.cpp
    src/Projectiles/IcePea.cpp
)

set(PROJECTILES_HEADERS
    src/Projectiles/Pea.h
    src/Projectiles/IcePea.h
)

# 合并所有源文件
set(ALL_SOURCES
    ${MAIN_SOURCES}
    ${CORE_SOURCES}
    ${STATES_SOURCES}
    ${ENTITIES_SOURCES}
    ${PLANTS_SOURCES}
    ${ZOMBIES_SOURCES}
    ${SYSTEMS_SOURCES}
    ${UI_SOURCES}
    ${UTILS_SOURCES}
    ${PROJECTILES_SOURCES}
)

# 合并所有头文件
set(ALL_HEADERS
    ${CORE_HEADERS}
    ${STATES_HEADERS}
    ${ENTITIES_HEADERS}
    ${PLANTS_HEADERS}
    ${ZOMBIES_HEADERS}
    ${SYSTEMS_HEADERS}
    ${UI_HEADERS}
    ${UTILS_HEADERS}
    ${PROJECTILES_HEADERS}
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${ALL_SOURCES} ${ALL_HEADERS})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# 链接SFML库
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio
    sfml-network
)
