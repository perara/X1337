
cmake_minimum_required(VERSION 2.8.7)
project(X1337)
set(CMAKE_CXX_STANDARD 14)


add_executable(X1337
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
        include/ResourceHandler.h
        include/Scene.h
        include/Script.h
        include/Shooter.h
        include/VectorN.h
        include/World.h
        src/Background.cpp
        src/Bullet.cpp
        src/BulletFactory.cpp
        src/Enemy.cpp
        src/GameEngine.cpp
        src/GameShape.cpp
        src/main.cpp
        src/Menu.cpp
        src/Object.cpp
        src/Player.cpp
        src/Powerup.cpp
        src/ResourceHandler.cpp
        src/Scene.cpp
        src/Script.cpp
        src/Shooter.cpp
        src/World.cpp
        )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)
find_package(SFML 2 REQUIRED system window graphics network audio)
set(SFML_STATIC_LIBRARIES TRUE)
message("lel ${SFML_STATIC_LIBRARIES}")
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(X1337 ${SFML_LIBRARIES})
else()
    message( FATAL_ERROR "You can not do this at all, CMake will exit. ${SFML_FOUND}" )
endif()


find_package(PugiXML REQUIRED)
if (PUGIXML_FOUND)
    include_directories(${PUGIXML_INCLUDE_DIR})
    target_link_libraries(X1337 ${PUGIXML_LIBRARIES})
else()
    message(FATAL_ERROR "You can not do this at all, CMake will exit. ${PUGIXML_FOUND}" )
endif()


add_custom_command(
        TARGET X1337 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/
        ${CMAKE_CURRENT_BINARY_DIR}/assets/)

