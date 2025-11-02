cmake_minimum_required(VERSION 3.16)
project(Kharoscraft CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(kharoscraft
    src/main.cpp
    src/Game.cpp
    src/World.cpp
    src/Player.cpp
    src/Renderer.cpp
)

find_package(SDL2 REQUIRED)

if(TARGET SDL2::SDL2)
    target_link_libraries(kharoscraft PRIVATE SDL2::SDL2)
else()
    target_include_directories(kharoscraft PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(kharoscraft PRIVATE ${SDL2_LIBRARIES})
endif()

if (MSVC)
    target_compile_options(kharoscraft PRIVATE /W4 /permissive-)
else()
    target_compile_options(kharoscraft PRIVATE -Wall -Wextra -Wpedantic)
endif()
