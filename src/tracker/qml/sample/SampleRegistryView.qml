import QtQuick
import dtracker_gui.tracker

Item {
    id: root
    required property SampleRegistryModel model
    signal removeSample(int id)
    signal previewSample(int id)

    Rectangle {
        anchors.fill: parent
        color: "#3b3e45"
        anchors.topMargin: 2
        radius: 2

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

                onPreviewSample: (id) => {
                    root.previewSample(id);
                }
            }
        }
    }
}
