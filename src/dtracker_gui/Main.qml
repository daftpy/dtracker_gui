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
    title: qsTr("Hello World")

    Rectangle {
        id: grid
        anchors.fill: parent

        gradient: LinearGradient {
            x1: 0
            x2: 0
            y1: 0
            y2: 300
            stops: [
                GradientStop { position: 0.0; color: "#dadee3" },
                GradientStop { position: 1.0; color: "#ccd2d9" }
            ]
        }


    AudioManager {
        id: audioManager
    }

    AudioDecoder {
        id: fileLoader

        onSampleReady: (pcmData, sampleRate, sampleName) => {
            console.log("Adding sample", sampleName);
            // audioManager.playSample(pcmData, sampleRate)
            sampleRegistry.addSample(sampleName, pcmData, sampleRate);
        }

        onErrorOccurred: (error) => {
            console.log(error);
        }
    }

    SampleRegistry {
        id: sampleRegistry
        sampleManager: audioManager.sampleManager();

        onSampleAdded: (id) => {
            console.log("Sample added:", id);
        }
    }

    SplitView {
        id: mainView
        width: parent.width
        height: parent.height
        anchors.fill: parent
        anchors.margins: 8
        anchors.bottomMargin: scrollView.contentWidth > scrollView.width ? 12 : 8

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

                FileTreeView {
                    SplitView.fillWidth: true
                    SplitView.preferredHeight: 200
                    onPreviewSample: (path) => {
                        fileLoader.load(path);
                    }
                }

                SampleRegistryView {
                    SplitView.preferredHeight: 200
                    SplitView.fillWidth: true
                    model: sampleRegistry.model
                }
            }
        }


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
                    anchors.bottom: parent.bottom
                }

                ListView  {
                    id: trackListView
                    anchors.fill: parent
                    anchors.leftMargin: 4
                    model: sampleRegistry.model
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

    FilePickerDialog {
        anchors.fill: parent

        onFileSelected: (path) => {
            console.log(`Calling fileLoader.load(${path})`)
            fileLoader.load(path);
        }
    }
    }
}
