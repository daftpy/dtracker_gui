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

        onPlaybackSample: (unit) => {
            console.log("Sending too playbacck facade");
            playbackFacade.playbackSample(unit);
        }

        onPlaybackSampleDescriptor: (descriptor) => {
            playbackFacade.handlePlaybackSampleDescriptor(descriptor);
        }
    }

    PlaybackFacade {
        engine: audioManager.engine
        sampleManager: sampleFacade.manager
        trackManager: trackManager.trackManager
        id: playbackFacade
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
            Layout.preferredHeight: playbackNavigation.implicitHeight + 12

            Layout.leftMargin: 4
            Layout.rightMargin: 4
            Layout.topMargin: 6
            Layout.bottomMargin: 2
            radius: 3
            border.width: 1
            border.color: "#56585c"
            color: "#272a2e"

            // Playback Navigation
            RowLayout {
                id: playbackNavigation
                anchors.fill: parent
                anchors.leftMargin: 6
                anchors.rightMargin: 6

                Row {
                    id: playbackComponents
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 4

                    Rectangle {
                        anchors.verticalCenter: parent.verticalCenter
                        width: playTrackIcon.width + 10
                        height: playTrackIcon.height + 10
                        radius: 2
                        // color: "#3f4145"
                        border.width: 1
                        border.color: "#56585c"
                        gradient: LinearGradient {
                            x1: 0
                            x2: 0
                            y1: 0
                            y2: 300
                            stops: [
                                GradientStop { position: 0.0; color: playTrackTapHandler.pressed ? "#242629" : "#46474a" },
                                GradientStop { position: 1.0; color: playTrackIconHoverHandler.hovered ? "#242629" : "#141517" }
                            ]
                        }

                        Image {
                            id: playTrackIcon
                            anchors.centerIn: parent

                            // atlast positions for different icon state
                            property var inactive: Qt.rect(2, 0, 54, 54)
                            property var hovered: Qt.rect(110, 0, 54, 54)

                            source: "tracker/icons/play-fill-atlas.svg"

                            width: 18
                            height: 18
                            // Scale the svg down
                            sourceSize.width: width * 8
                            sourceSize.height: height * 2
                            // Use a clip to only show a section of the atlas
                            sourceClipRect: playbackTimer.running ? hovered : playTrackIconHoverHandler.hovered ? hovered : inactive
                            antialiasing: true
                        }
                        HoverHandler {
                            id: playTrackIconHoverHandler

                            cursorShape: Qt.PointingHandCursor
                        }

                        TapHandler {
                            id: playTrackTapHandler
                            // onSingleTapped: {
                            //     audioManager.playTrack();
                            // }
                            onSingleTapped: {
                                // let track = trackManager.model.get(0);
                                // if (track && track.patternModel) {
                                //     playbackTimer.patternModel = track.patternModel;
                                //     playbackTimer.start();
                                // }
                                playbackFacade.playTrack(0);
                            }
                        }
                    }

                    Rectangle {
                        anchors.verticalCenter: parent.verticalCenter
                        width: stopTrackIcon.width + 10
                        height: stopTrackIcon.height + 10
                        radius: 2
                        //color: "#3f4145"
                        border.width: 1
                        border.color: "#56585c"

                        gradient: LinearGradient {
                            x1: 0
                            x2: 0
                            y1: 0
                            y2: 300
                            stops: [
                                GradientStop { position: 0.0; color: stopTrackTapHandler.pressed ? "#242629" : "#46474a" },
                                GradientStop { position: 1.0; color: stopTrackIconHoverHandler.hovered ? "#242629" : "#141517" }
                            ]
                        }
                        Image {
                            id: stopTrackIcon
                            anchors.centerIn: parent

                            // atlast positions for different icon state
                            property var inactive: Qt.rect(0, 0, 54, 54)
                            property var hovered: Qt.rect(162, 0, 54, 54)

                            source: "tracker/icons/stop-fill-atlas.svg"

                            width: 18
                            height: 18
                            // Scale the svg down
                            sourceSize.width: width * 8
                            sourceSize.height: height * 2
                            // Use a clip to only show a section of the atlas
                            sourceClipRect: stopTrackIconHoverHandler.hovered ? hovered : inactive
                            antialiasing: true
                        }

                        HoverHandler {
                            id: stopTrackIconHoverHandler

                            cursorShape: Qt.PointingHandCursor
                        }

                        TapHandler {
                            id: stopTrackTapHandler
                            onSingleTapped: {
                                playbackTimer.stop();
                            }
                        }
                    }

                    Rectangle {
                        width: stopTrackIcon.width + 10
                        height: stopTrackIcon.height + 10
                        radius: 2
                        border.width: 1
                        border.color: "#56585c"
                        anchors.verticalCenter: parent.verticalCenter

                        gradient: LinearGradient {
                            x1: 0
                            x2: 0
                            y1: 0
                            y2: 300
                            stops: [
                                GradientStop { position: 0.0; color: repeatPlaybackTapHandler.pressed ? "#242629" : "#46474a" },
                                GradientStop { position: 1.0; color: repeatPlaybackHoverHandler.hovered ? "#242629" : "#141517" }
                            ]
                        }

                        Image {
                            id: repeatPlayback
                            anchors.centerIn: parent

                            // atlast positions for different icon state
                            property var inactive: Qt.rect(0, 0, 54, 54)
                            property var hovered: Qt.rect(54, 0, 54, 54)

                            source: "tracker/icons/repeat-atlas.svg"

                            width: 18
                            height: 18
                            // Scale the svg down
                            sourceSize.width: width * 8
                            sourceSize.height: height * 2
                            // Use a clip to only show a section of the atlas
                            sourceClipRect: repeatPlaybackHoverHandler.hovered ? hovered : inactive
                            antialiasing: true
                        }
                        HoverHandler {
                            id: repeatPlaybackHoverHandler

                            cursorShape: Qt.PointingHandCursor
                        }

                        TapHandler {
                            id: repeatPlaybackTapHandler
                            onSingleTapped: {
                                trackManager.createTrack();
                            }
                        }
                    }
                }

                // Add Track Button
                Rectangle {
                    width: stopTrackIcon.width + 10
                    height: stopTrackIcon.height + 10
                    radius: 2
                    border.width: 1
                    border.color: "#56585c"
                    Layout.alignment: Qt.AlignVCenter

                    gradient: LinearGradient {
                        x1: 0
                        x2: 0
                        y1: 0
                        y2: 300
                        stops: [
                            GradientStop { position: 0.0; color: addTrackTapHandler.pressed ? "#242629" : "#46474a" },
                            GradientStop { position: 1.0; color: addTrackHoverHandler.hovered ? "#242629" : "#141517" }
                        ]
                    }

                    Image {
                        id: addTrackIcon
                        anchors.centerIn: parent

                        // atlast positions for different icon state
                        property var inactive: Qt.rect(0, 0, 54, 54)
                        property var hovered: Qt.rect(54, 0, 54, 54)

                        source: "tracker/icons/plus-square-fill-atlas.svg"

                        width: 18
                        height: 18
                        // Scale the svg down
                        sourceSize.width: width * 8
                        sourceSize.height: height * 2
                        // Use a clip to only show a section of the atlas
                        sourceClipRect: addTrackHoverHandler.hovered ? hovered : inactive
                        antialiasing: true
                    }
                    HoverHandler {
                        id: addTrackHoverHandler

                        cursorShape: Qt.PointingHandCursor
                    }

                    TapHandler {
                        id: addTrackTapHandler
                        onSingleTapped: {
                            trackManager.createTrack();
                        }
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
            Layout.leftMargin: 4
            Layout.rightMargin: 4

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
                            sampleFacade.previewSample(path);
                        }

                        onAddSample: (path) => {
                            sampleFacade.registerSample(path);
                        }
                    }

                    // SampleRegistry
                    SampleRegistryView {
                        id: sampleRegistryView
                        SplitView.preferredHeight: 200
                        SplitView.fillWidth: true
                        //model: sampleRegistry.model
                        model: sampleFacade.model

                        onPreviewSample: (sampleId) => {
                            console.log("Previewing sample");

                            sampleFacade.previewSample(sampleId);
                        }

                        onRemoveSample: (id) => {
                            // TODO: sampleFacade.remove(id)
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
                                // audioManager.playSampleById(id);
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
