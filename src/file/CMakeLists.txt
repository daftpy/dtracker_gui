qt_add_library(file STATIC)
qt_add_qml_module(file
    URI dtracker_gui.file
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qt/qml/dtracker_gui/file"
    QML_FILES
        qml/FilePickerDialog.qml
    RESOURCES
        "icons/folder.svg"
        "icons/musicnotes.svg"
        "icons/folder-open.svg"
    SOURCES
        filetreemodel.h filetreemodel.cpp
    QML_FILES
        qml/FileTreeView.qml
        qml/FileTreeDelegate.qml
)

target_include_directories(file PUBLIC
    ${dtracker_engine_SOURCE_DIR}/include
)

target_link_libraries(file
    PUBLIC dtracker_engine
    PRIVATE Qt6::Multimedia
    PRIVATE Qt6::Svg
)
