cmake_minimum_required(VERSION 3.16)

project(dtracker_gui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

include(FetchContent)

FetchContent_Declare(
    dtracker_engine
    GIT_REPOSITORY https://github.com/daftpy/dtracker_engine.git
    GIT_TAG main  # or a release tag like v0.1 if you make one
)

FetchContent_MakeAvailable(dtracker_engine)

qt_add_executable(appdtracker_gui
    main.cpp
)

target_include_directories(appdtracker_gui PRIVATE
    ${dtracker_engine_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/audio
)

qt_add_qml_module(appdtracker_gui
    URI dtracker_gui
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES
        audio/audiomanager.h audio/audiomanager.cpp
        audio/types/audiodeviceinfo.h audio/types/audiodeviceinfo.cpp
        RESOURCES README.md
        RESOURCES .gitignore
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appdtracker_gui PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appdtracker_gui
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appdtracker_gui
    PRIVATE Qt6::Quick
    PRIVATE dtracker_engine
)

include(GNUInstallDirs)
install(TARGETS appdtracker_gui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
