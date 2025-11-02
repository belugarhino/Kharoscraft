cmake_minimum_required(VERSION 3.20)
project(Kharoscraft LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

set(KHAROSCRAFT_SDL_INCLUDES "")
set(KHAROSCRAFT_SDL_LIBS "")

find_package(SDL2 CONFIG)
if (SDL2_FOUND)
    set(KHAROSCRAFT_SDL_LIBS SDL2::SDL2)
    if (TARGET SDL2::SDL2main)
        list(APPEND KHAROSCRAFT_SDL_LIBS SDL2::SDL2main)
    endif()
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    set(KHAROSCRAFT_SDL_INCLUDES ${SDL2_INCLUDE_DIRS})
    set(KHAROSCRAFT_SDL_LIBS ${SDL2_LIBRARIES})
endif()

file(GLOB_RECURSE KHAROSCRAFT_HEADERS CONFIGURE_DEPENDS include/*.hpp)
file(GLOB_RECURSE KHAROSCRAFT_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(kharoscraft
    ${KHAROSCRAFT_HEADERS}
    ${KHAROSCRAFT_SOURCES}
)

target_include_directories(kharoscraft
    PRIVATE
        include
        ${KHAROSCRAFT_SDL_INCLUDES}
)

target_link_libraries(kharoscraft
    PRIVATE
        ${KHAROSCRAFT_SDL_LIBS}
        OpenGL::GL
)

if (MSVC)
    target_compile_options(kharoscraft PRIVATE /W4 /permissive-)
else()
    target_compile_options(kharoscraft PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS kharoscraft RUNTIME DESTINATION bin)
install(DIRECTORY assets DESTINATION share/kharoscraft)
