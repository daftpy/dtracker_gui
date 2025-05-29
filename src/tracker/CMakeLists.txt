qt_add_library(tracker STATIC)
qt_add_qml_module(tracker
    URI dtracker_gui.tracker
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qt/qml/dtracker_gui/tracker"
    RESOURCES
        "icons/ear-slash-atlas.svg"
        "icons/speaker-atlas.svg"
        "icons/trash-atlas.svg"
    QML_FILES
        qml/TrackView.qml
        qml/sample/SampleRegistryView.qml
        qml/sample/SampleRegistryDelegate.qml
    SOURCES
        tracklistmodel.h tracklistmodel.cpp
        track.h track.cpp
        sample/sample_registry_model.h sample/sample_registry_model.cpp
        sample/sample_entry.h sample/sample_entry.cpp
        sample/sample_registry.h sample/sample_registry.cpp
)

target_include_directories(tracker PUBLIC
    ${dtracker_engine_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/sample
)

target_link_libraries(tracker
    PUBLIC dtracker_engine
    PUBLIC audio
    PRIVATE Qt6::Multimedia
    PRIVATE Qt6::Svg
)
