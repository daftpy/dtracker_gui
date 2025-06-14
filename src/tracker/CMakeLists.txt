qt_add_library(tracker STATIC)
qt_add_qml_module(tracker
    URI dtracker_gui.tracker
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qt/qml/dtracker_gui/tracker"
    RESOURCES
        "icons/ear-slash-atlas.svg"
        "icons/speaker-atlas.svg"
        "icons/trash-atlas.svg"
        "icons/plus-square-fill-atlas.svg"
        "icons/stop-fill-atlas.svg"
        "icons/play-fill-atlas.svg"
        "icons/pause-fill-atlas.svg"
    QML_FILES
        qml/TrackView.qml
        qml/PlaybackTimer.qml
        qml/sample/SampleRegistryView.qml
        qml/sample/SampleRegistryDelegate.qml
        qml/pattern/PatternColumn.qml
    SOURCES
        track/track_list_model.h track/track_list_model.cpp
        track/track.h track/track.cpp
        track/track_manager.h track/track_manager.cpp
        sample/sample_registry_model.h sample/sample_registry_model.cpp
        sample/sample_entry.h
        types/track_config.h types/track_config.cpp
        pattern/pattern_step_model.h pattern/pattern_step_model.cpp
        sample/sample_facade.h sample/sample_facade.cpp
        sample/sample_manager_worker.h sample/sample_manager_worker.cpp
        playback/playback_facade.h playback/playback_facade.cpp
)

target_include_directories(tracker PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${dtracker_engine_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/sample
    ${CMAKE_CURRENT_SOURCE_DIR}/track
    ${CMAKE_CURRENT_SOURCE_DIR}/pattern
    ${CMAKE_CURRENT_SOURCE_DIR}/playback
)

target_link_libraries(tracker
    PUBLIC dtracker_engine
    PUBLIC audio
    PRIVATE Qt6::Multimedia
    PRIVATE Qt6::Svg
)
