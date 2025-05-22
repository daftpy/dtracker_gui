qt_add_library(audio STATIC)
qt_add_qml_module(audio
    URI dtracker_gui.audio
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qt/qml/dtracker_gui/audio"
    QML_FILES
        qml/pages/AudioSettingsPage.qml
    SOURCES
        audiomanager.h audiomanager.cpp
        types/audiodeviceinfo.h types/audiodeviceinfo.cpp
)

target_include_directories(audio PUBLIC
    ${dtracker_engine_SOURCE_DIR}/include
)

target_link_libraries(audio
    PUBLIC dtracker_engine
)
