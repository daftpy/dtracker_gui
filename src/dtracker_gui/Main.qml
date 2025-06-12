import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Shapes
import dtracker_gui.audio
import dtracker_gui.file
import dtracker_gui.tracker

Window {
    id: mainWindow
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: qsTr("DTracker")

    // Background grid
    Rectangle {
        id: grid
        anchors.fill: parent

        // Subtle gradient
        gradient: LinearGradient {
            x1: 0
            x2: 0
            y1: 0
            y2: 300
            stops: [
                GradientStop { position: 0.0; color: "#dadee3" },
                GradientStop { position: 1.0; color: "#cbd1d8" }
            ]
        }


    SampleFacade {
        id: sampleFacade

        audioManager: audioManager
    }


    // Core component responsible for many critical application components
    AudioManager {
        id: audioManager

        /*
            Exposes the following!

            POINTERS
            sampleManager   -- provides access to the backend SampleManager
            trackManager    -- provides access to the backend TrackManager

            FUNCTIONS
            playSampleByID(sampleID)    -- plays a sampleById through the backend SampleManager
        */
    }

    // Registers samples in the backend and an internal list for exposure to QML
    SampleRegistry {
        id: sampleRegistry
        sampleManager: audioManager.sampleManager();

        onSampleAdded: (id) => {
            console.log("Sample added:", id);
        }
    }

    // Manages Tracks in the backend and exposes necessary parts through QML
    TrackManager {
        id: trackManager
        trackManager: audioManager.trackManager
    }

    // Main Layout
    ColumnLayout {
        anchors.fill: parent

        // Playback Bar Background
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 48
            Layout.margins: 8
            radius: 6

            color: "#272a2e"

            // Playback Navigation
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10

                Row {
                    id: playbackComponents
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Image {
                        id: playTrackIcon
                        anchors.verticalCenter: parent.verticalCenter

                        // atlast positions for different icon state
                        property var inactive: Qt.rect(0, 0, 40, 40)
                        property var hovered: Qt.rect(72, 0, 40, 40)

                        source: "tracker/icons/play-fill-atlas.svg"

                        // Scale the svg down
                        sourceSize.width: 96
                        sourceSize.height: 24
                        // Use a clip to only show a section of the atlas
                        sourceClipRect: playbackTimer.running ? hovered : playTrackIconHoverHandler.hovered ? hovered : inactive
                        antialiasing: true

                        HoverHandler {
                            id: playTrackIconHoverHandler

                            cursorShape: Qt.PointingHandCursor
                        }

                        TapHandler {
                            // onSingleTapped: {
                            //     audioManager.playTrack();
                            // }
                            onSingleTapped: {
                                let track = trackManager.model.get(0);
                                if (track && track.patternModel) {
                                    playbackTimer.patternModel = track.patternModel;
                                    playbackTimer.start();
                                }
                            }
                        }
                    }

                    Image {
                        id: stopTrackIcon
                        anchors.verticalCenter: parent.verticalCenter

                        // atlast positions for different icon state
                        property var inactive: Qt.rect(0, 0, 40, 40)
                        property var hovered: Qt.rect(108, 0, 40, 40)

                        source: "tracker/icons/stop-fill-atlas.svg"

                        // Scale the svg down
                        sourceSize.width: 96
                        sourceSize.height: 24
                        // Use a clip to only show a section of the atlas
                        sourceClipRect: stopTrackIconHoverHandler.hovered ? hovered : inactive
                        antialiasing: true

                        HoverHandler {
                            id: stopTrackIconHoverHandler

                            cursorShape: Qt.PointingHandCursor
                        }

                        TapHandler {
                            onSingleTapped: {
                                playbackTimer.stop();
                            }
                        }
                    }
                }

                // Add Track Button
                Rectangle {
                    width: addTrackIcon.implicitWidth
                    height: addTrackIcon.implicitHeight
                    color: "transparent"

                    Image {
                        id: addTrackIcon
                        anchors.centerIn: parent

                        // atlast positions for different icon state
                        property var inactive: Qt.rect(0, 0, 40, 40)
                        property var hovered: Qt.rect(36, 0, 40, 40)

                        source: "tracker/icons/plus-square-fill-atlas.svg"

                        // Scale the svg down
                        sourceSize.width: 96
                        sourceSize.height: 24
                        // Use a clip to only show a section of the atlas
                        sourceClipRect: addTrackIconHoverHandler.hovered ? hovered : inactive
                        antialiasing: true

                        HoverHandler {
                            id: addTrackIconHoverHandler

                            cursorShape: Qt.PointingHandCursor
                        }

                        TapHandler {
                            onSingleTapped: {
                                trackManager.createTrack();
                            }
                        }
                    }

                    Component.onCompleted: {
                        console.log("addTrackIconContainer:", width, "x", height);
                    }
                }
            }
        }

        // Main View
        // Splits the file nav and sample registry view on the left
        // Track View is on the right
        SplitView {
            id: mainView

            // Layout rules
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 8
            Layout.rightMargin: 8

            // Grabbable handle rules
            handle: Rectangle {
                id: mainViewHandle
                implicitWidth: 4
                radius: 6
                color: mainViewHoverHandler.hovered ? "#bcc5d1" : "#cad0d9"

                containmentMask: Item {
                    x: - 2
                    height: mainView.height
                    width: 8
                }
                HoverHandler {
                    id: mainViewHoverHandler
                }
            }


            // Sidebar sample preview and SampleRegistryView
            Rectangle {
                SplitView.preferredWidth: 150
                SplitView.fillHeight: true
                color: "transparent"
                SplitView {
                    id: sidebarView
                    anchors.fill: parent
                    anchors.rightMargin: 4
                    orientation: Qt.Vertical

                    handle: Rectangle {
                        id: sidebarViewHandle
                        implicitHeight: 4
                        color: sidebarViewHoverHandler.hovered ? "#bcc5d1" : "#cad0d9"
                        radius: 6

                        containmentMask: Item {
                            y: - 2
                            width: sidebarView.width
                            height: 8
                        }
                        HoverHandler {
                            id: sidebarViewHoverHandler
                        }
                    }

                    // Sample Preview
                    FileTreeView {
                        SplitView.fillWidth: true
                        SplitView.preferredHeight: 200
                        onPreviewSample: (path) => {
                            sampleFacade.checkCache(path);
                        }
                    }

                    // SampleRegistry
                    SampleRegistryView {
                        id: sampleRegistryView
                        SplitView.preferredHeight: 200
                        SplitView.fillWidth: true
                        model: sampleRegistry.model

                        onAddSampleToTrack: (sampleId) => {
                            console.log("Adding sample to track 0");
                            // audioManager.addSampleToTrack(sampleId, 0);
                        }

                        onRemoveSample: (id) => {
                            sampleRegistry.removeSample(id);
                        }
                    }
                }
            }

            // Track View
            Rectangle {
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                color: "transparent"

                ScrollView {
                    id: scrollView
                    anchors.fill: parent
                    ScrollBar.horizontal: ScrollBar {
                        id: scrollbar

                        policy: ScrollBar.AsNeeded
                        parent: mainView.parent
                        implicitWidth: parent.width
                        Layout.alignment: Qt.AlignBottom
                    }

                    ListView  {
                        id: trackListView
                        anchors.fill: parent
                        anchors.leftMargin: 4
                        // model: sampleRegistry.model
                        model: trackManager.model
                        orientation: ListView.Horizontal
                        spacing: 8
                        clip: true

                        delegate: TrackView {
                            onPlaySample: (id) => {
                                console.log("Playing by ID");
                                audioManager.playSampleById(id);
                            }
                        }
                    }
                }
            }
        }
    }

    PlaybackTimer {
        id: playbackTimer

        audioManager: audioManager
    }

    // Hidden file picker dialog
    FilePickerDialog {
        anchors.fill: parent

        onFileSelected: (path) => {
            console.log(`Calling fileLoader.load(${path})`)
            // fileLoader.load(path);
        }
    }
    }
}
