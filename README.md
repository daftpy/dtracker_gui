# dtracker_gui

A simple Qt Quick graphical frontend for the [dtracker_engine](https://github.com/daftpy/dtracker_engine) audio engine.

## Overview

This GUI project connects to the `dtracker_engine` library and displays device information such as the name, output channels, and sample rate in a QML-friendly interface.

## Features

- Auto-starts and initializes audio engine
- Displays the active audio output device info in QML
- Uses `AudioManager` and `AudioDeviceInfo` types exposed to QML

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

## Run

```bash
./build/appdtracker_gui
```

## Notes

The audio engine library is automatically fetched via `FetchContent` and does not require a manual clone step.

