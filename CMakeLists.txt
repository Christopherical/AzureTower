cmake_minimum_required(VERSION 3.28.0)
project(azureTower CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(sfml URL https://github.com/SFML/SFML/archive/refs/tags/3.0.2.tar.gz)
FetchContent_MakeAvailable(sfml)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS Source/*.cpp)
add_executable(azureTower ${SOURCES})

target_include_directories(azureTower PRIVATE Include/)

target_link_libraries(azureTower SFML::Graphics SFML::Audio)

