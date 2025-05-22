cmake_minimum_required(VERSION 3.16)

# Project definition
project(dtracker_gui VERSION 0.1 LANGUAGES CXX)

# Enforce C++ standard requirement
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Quick)

# Standard Qt project setup (enables features like QML type generation)
qt_standard_project_setup(REQUIRES 6.8)

# Ensure QML language server and Qt Creator can locate compiled QML modules
set(QML_IMPORT_PATH
    ${CMAKE_BINARY_DIR}/qt/qml
    CACHE STRING "" FORCE
)

# Enable automatic MOC and QML type information generation
set(CMAKE_AUTOMOC ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

# Determine if local engine source exists
if(EXISTS "${CMAKE_SOURCE_DIR}/../projects/dtracker_engine/CMakeLists.txt")
    message(STATUS "Using local dtracker_engine at ../dtracker_engine")
    add_subdirectory(${CMAKE_SOURCE_DIR}/../projects/dtracker_engine dtracker_engine_build)
else()
    message(STATUS "Fetching dtracker_engine from GitHub")
    include(FetchContent)
    FetchContent_Declare(
        dtracker_engine
        GIT_REPOSITORY https://github.com/daftpy/dtracker_engine.git
        GIT_TAG main # Prefer pinned tag in production
    )
    FetchContent_MakeAvailable(dtracker_engine)
endif()


# Add subdirectories for components/modules
add_subdirectory(src/audio)
add_subdirectory(src/dtracker_gui)
