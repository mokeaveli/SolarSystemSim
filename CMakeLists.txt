cmake_minimum_required(VERSION 3.10)
project(SolarSystemSim)

set(CMAKE_CXX_STANDARD 11)

# Add source files
set(SOURCES
    src/main.cpp
    src/Body.cpp
    src/Vector2D.cpp
)

# Add include directories
include_directories(
    include
    /opt/homebrew/include
)

# Find SFML package
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add executable
add_executable(SolarSystemSim ${SOURCES})

# Link SFML libraries
target_link_libraries(SolarSystemSim sfml-graphics sfml-window sfml-system)
