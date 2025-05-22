qt_add_executable(appdtracker_gui
    main.cpp
)

qt_add_qml_module(appdtracker_gui
    URI dtracker_gui
    VERSION 1.0
    QML_FILES
        Main.qml
)

target_include_directories(appdtracker_gui PRIVATE
    ${dtracker_engine_SOURCE_DIR}/include
)

target_link_libraries(appdtracker_gui
    PRIVATE Qt6::Quick
    PRIVATE dtracker_engine
    PRIVATE audioplugin
)

set_target_properties(appdtracker_gui PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS appdtracker_gui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
