cmake_minimum_required(VERSION 3.17)
project(X1337)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra") # -Werror # TODO
include(cmake/CMakeRC.cmake)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)

# Clang # TODO
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(pybind11 INTERFACE -fsized-deallocation)
endif()




find_package(SFML 2 COMPONENTS audio graphics window system REQUIRED)
find_package(pybind11 REQUIRED)
find_package(PugiXML REQUIRED)

message("${SFML_LIBRARIES}")


if (SFML_FOUND)
    set(SFML_STATIC_LIBRARIES FALSE CACHE BOOL "Use static SFML librares")
    # General compiler options
    if (SFML_STATIC_LIBRARIES)
        add_definitions(-DSFML_STATIC)
    endif()

    set(XLEET_LIBRARIES "${SFML_LIBRARIES}")
    set(XLEET_INCLUDES "${SFML_INCLUDE_DIR}")
else()
    message( FATAL_ERROR "You can not do this at all, CMake will exit. ${SFML_FOUND}" )
endif()

if (PUGIXML_FOUND)
    set(XLEET_LIBRARIES ${XLEET_LIBRARIES} ${PUGIXML_LIBRARIES})
    set(XLEET_INCLUDES ${XLEET_INCLUDES} ${PUGIXML_INCLUDE_DIR})
else()
    message(FATAL_ERROR "You can not do this at all, CMake will exit. ${PUGIXML_FOUND}" )
endif()



SET(XLEET_INCLUDES
        include/Background.h
        include/Bullet.h
        include/BulletFactory.h
        include/Enemy.h
        include/GameEngine.h
        include/GameShape.h
        include/Log.h
        include/Menu.h
        include/Object.h
        include/Player.h
        include/Powerup.h
        include/ResourceManager.h
        include/Scene.h
        include/Script.h
        include/Shooter.h
        include/VectorN.h
        include/World.h
        include/Constants.h
        include/HighScoreItem.h
        )
set(XLEET_SOURCES
        src/Background.cpp
        src/Bullet.cpp
        src/BulletFactory.cpp
        src/Enemy.cpp
        src/GameEngine.cpp
        src/GameShape.cpp
        src/Menu.cpp
        src/Object.cpp
        src/Player.cpp
        src/Powerup.cpp
        src/ResourceManager.cpp
        src/Scene.cpp
        src/Script.cpp
        src/Shooter.cpp
        src/World.cpp
        )


set(XLEET_RESOURCES
assets/fonts/comicate.ttf
assets/fonts/sansation.ttf
assets/scripts/game_menu.xml
assets/scripts/stage1.xml
assets/scripts/stage2.xml
assets/scripts/test1.xml
assets/scripts/test2.xml
assets/sound/countdown.ogg
assets/sound/death_star_theme.ogg
assets/sound/emote_death_star_beware_cowards.ogg
assets/sound/emote_death_star_enough.ogg
assets/sound/emote_death_star_greet.ogg
assets/sound/emote_death_star_periodic_1.ogg
assets/sound/emote_death_star_periodic_2.ogg
assets/sound/emote_death_star_periodic_3.ogg
assets/sound/emote_death_star_periodic_4.ogg
assets/sound/emote_death_star_rage.ogg
assets/sound/ExplosionSound.ogg
assets/sound/FireOneSound.ogg
assets/sound/FireTwoSound.ogg
assets/sound/fx_bounce.ogg
assets/sound/fx_error.wav
assets/sound/game_menu.ogg
assets/sound/health_pickup.wav
assets/sound/in-game.ogg
assets/sound/menu_click.ogg
assets/sound/menu_return.ogg
assets/sound/story_death_star_intro.ogg
assets/sound/story_stage1_intro.ogg
assets/sound/story_stage2_intro.ogg
assets/sprites/stage_portraits/council.png
assets/sprites/stage_portraits/deathstar.png
assets/sprites/stage_portraits/twins.png
assets/sprites/audio-off.png
assets/sprites/audio-on.png
assets/sprites/bg2.png
assets/sprites/bg3.png
assets/sprites/boss.png
assets/sprites/chubby.png
assets/sprites/deathstar.png
assets/sprites/enemy_ship.png
assets/sprites/health-kit.png
assets/sprites/heart.png
assets/sprites/heavy_shot.png
assets/sprites/monitor.png
assets/sprites/noid.png
assets/sprites/player-bar.png
assets/sprites/player_ship.png
assets/sprites/pulse-gun.png
assets/sprites/regular_bullet_1.png
assets/state.xml
assets/credits.xml
)

cmrc_add_resource_library(
        x1337_resources
        NAMESPACE xleetrc
        ${XLEET_RESOURCES}
)

add_library(x1337_lib STATIC ${XLEET_SOURCES})
target_link_libraries(x1337_lib PUBLIC x1337_resources)
target_link_libraries(x1337_lib PUBLIC ${XLEET_LIBRARIES})
target_include_directories(x1337_lib PUBLIC include)

add_executable(X1337 src/main.cpp)
target_link_libraries(X1337 PRIVATE x1337_lib)


file(GLOB BIND_FILES ${PROJECT_SOURCE_DIR}/src/pybind/*.cpp)
pybind11_add_module(xleet ${SOURCE_FILES} ${BIND_FILES})
target_link_libraries(xleet PRIVATE x1337_lib)


