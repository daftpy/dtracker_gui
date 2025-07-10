# dtracker_gui

A simple Qt Quick graphical frontend for the [dtracker_engine](https://github.com/daftpy/dtracker_engine) audio engine.

## Overview
This project serves as the user-facing application that leverages the `dtracker_engine` library to provide a functional audio tracker. It demonstrates a clean separation between the C++ backend logic and a declarative QML frontend, using a series of "Facade" classes as the primary bridge.

The application allows users to browse their file system for audio samples, manage a registry of loaded sounds, sequence those sounds in a multi-track pattern editor, and control playback with features like looping and dynamic tempo adjustment.

## Demo

https://github.com/user-attachments/assets/725101a5-9d44-42af-a322-fc42357808da



## Core Features & Architecture
The GUI is built using modern C++ and Qt 6 principles, emphasizing a decoupled and responsive design.

-   **Engine Integration:** The application dynamically links to the `dtracker_engine` library (fetched automatically via CMake's `FetchContent`) to handle all audio processing.

-   **Asynchronous Workflow:** All potentially blocking operations, such as decoding audio files, are offloaded to a background `QThread`. This ensures the user interface remains smooth and responsive at all times, even when loading large samples.

-   **Facade Design Pattern:** A series of `QObject`-based "Facade" classes (`PlaybackFacade`, `SampleFacade`, `TrackManager`) act as a clean API boundary between QML and the C++ backend. This prevents complex engine logic from leaking into the UI code and makes the QML declarative and easy to read.

-   **Model/View/Delegate:** The application uses Qt's Model/View pattern to efficiently display data. The `SampleRegistryModel` and `TrackListModel` provide data from the C++ backend directly to QML `ListView`s, which use custom delegates (`SampleRegistryView`, `TrackView`) for rendering.

-   **Live Audio Visualization:** The GUI features a live, real-time oscilloscope display of the master audio output. This is achieved by:
    1.  "Tapping" the final mixed audio on the real-time audio thread in the C++ engine.
    2.  Safely passing this data to the GUI thread using a high-performance, lock-free SPSC queue.
    3.  Polling the queue with a `QTimer` and drawing the master audio waveform, ensuring a smooth, responsive visualization without impacting audio performance.
## Requirements

- Qt 6.8.1
- CMake 3.16+
- Git
- Compiler: **MSVC2022 64-bit** (confirmed working)

## Setup Instructions

```bash
git clone https://github.com/yourusername/dtracker_gui.git
cd dtracker_gui
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
```

## Notes

The audio engine library is automatically fetched via `FetchContent` and does not require a manual clone step.


