# Define project
cmake_minimum_required(VERSION 2.8.11)
project(FrameworkQt)

cmake_policy(SET CMP0020 NEW)

if(${CMAKE_VERSION} VERSION_EQUAL "3.0" OR ${CMAKE_VERSION} VERSION_GREATER "3.0")
	cmake_policy(SET CMP0043 NEW)
endif()

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set target name
set(target FrameworkQt)

# Widgets finds its own dependencies.
find_package(Qt5Widgets REQUIRED)

# Define source files
set(sources
    src/main.cpp
	src/stable.h
	src/stable.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/exercise123.cpp
    src/exercise123.h
    src/exercise4a.cpp
    src/exercise4a.h
    src/exercise4b.cpp
    src/exercise4b.h
	src/util/imageview.h
	src/util/imageview.cpp
)

# Create executable
add_executable(${target} ${sources})

# Use the Widgets module from Qt 5
target_link_libraries(${target} Qt5::Widgets)
