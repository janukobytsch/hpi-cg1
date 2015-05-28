# Define project
cmake_minimum_required(VERSION 2.8.11)
project(FrameworkQt)

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

# Include QT5 libraries
set(CMAKE_AUTOMOC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)

if(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=gnu++11)
elseif(APPLE)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
elseif(UNIX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=gnu++11)
endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/glm/")

# Define source files
set(sources

    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    
    src/exercise12.h
    src/exercise12.cpp
    src/exercise13.h    
    src/exercise13.cpp
    src/exercise14.cpp
    src/exercise14.h
    
    src/util/objio.cpp
    src/util/objio.h
    src/util/polygonaldrawable.cpp
    src/util/polygonaldrawable.h
    src/util/abstractexercise.cpp
    src/util/abstractexercise.h
    src/util/abstractcoordinateprovider.h
    src/util/abstractpainter.h
    src/util/abstractpainter.cpp
    src/util/axisalignedboundingbox.h
    src/util/axisalignedboundingbox.cpp
    src/util/cachedvalue.h
    src/util/cachedvalue.hpp
    src/util/camera.h
    src/util/camera.cpp
    src/util/canvas.h
    src/util/canvas.cpp
    src/util/fileassociatedasset.h
    src/util/fileassociatedasset.cpp
    src/util/fileassociatedshader.h
    src/util/fileassociatedshader.cpp
    src/util/glviewer.h
    src/util/glviewer.cpp
    src/util/icosahedron.cpp
    src/util/icosahedron.h
    src/util/navigation.h
    src/util/navigation.cpp
    src/util/navigationmath.h
    src/util/navigationmath.cpp
    src/util/openglfunctions.h
    src/util/paintermode.h
    src/util/triangle.h
    src/util/unitcube.h
    src/util/unitcube.cpp
)

# Create executable
add_executable(${target} ${sources})

# Create executable
include_directories(
    ${CMAKE_SOURCE_DIR}
)

# Use the Widgets and OpenGL module from Qt 5
target_link_libraries(${target} Qt5::Widgets Qt5::OpenGL)
