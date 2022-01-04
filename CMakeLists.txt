cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(terrainrender)

add_compile_definitions(DEBUG_BUILD)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

file(GLOB SOURCES ./*.cpp)

add_executable(terrainrender ${SOURCES})
target_include_directories(terrainrender PUBLIC ./)
target_include_directories(terrainrender PRIVATE ${STB_INCLUDE_DIRS})
target_link_libraries(terrainrender PRIVATE glm::glm SDL2::SDL2 SDL2::SDL2main GLEW::GLEW)

