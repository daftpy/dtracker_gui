# Define the main application target as an executable
qt_add_executable(appdtracker_gui
    main.cpp
)

# Declare this target as a QML module so QML files are processed and type-registered
qt_add_qml_module(appdtracker_gui
    URI dtracker_gui            # QML import URI
    VERSION 1.0                 # QML module version
    QML_FILES
        Main.qml               # Main entry point of the QML UI
)

# Include directories needed for this target
target_include_directories(appdtracker_gui PRIVATE
    ${dtracker_engine_SOURCE_DIR}/include  # Include path for the dtracker_engine headers
)

# Link required libraries
target_link_libraries(appdtracker_gui
    PRIVATE Qt6::Quick         # Qt Quick runtime
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Multimedia
    PRIVATE dtracker_engine    # Your audio engine library
    PRIVATE audioplugin        # AudioManager QML plugin (from src/audio)
    PRIVATE fileplugin
)

# Set platform properties
set_target_properties(appdtracker_gui PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Define installation paths for different platforms
install(TARGETS appdtracker_gui
    BUNDLE DESTINATION .                      # For macOS
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}  # For shared libraries (Linux)
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}  # For Windows and executable targets
)
