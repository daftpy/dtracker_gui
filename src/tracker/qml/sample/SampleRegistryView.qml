import QtQuick
import dtracker_gui.tracker

Item {
    id: root
    required property SampleRegistryModel model
    signal removeSample(int id)
    signal addSampleToTrack(int id)

    Rectangle {
        anchors.fill: parent
        color: "#3b3e45"
        anchors.topMargin: 4
        radius: 3

        ListView {
            anchors.fill: parent
            anchors.topMargin: 4
            anchors.bottomMargin: 4
            height: 100
            clip: true

            model: root.model

            delegate: SampleRegistryDelegate {
                id: delegate

                onRemoveSample: (id) => {
                    root.removeSample(id);
                }

                onAddSampleToTrack: (id) => {
                    root.addSampleToTrack(id);
                }
            }
        }
    }
}
