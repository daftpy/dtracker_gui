import QtQuick
import dtracker_gui.tracker

Column {
    id: root

    property PatternStepModel model: null
    property int trackId: -1

    ListView {
        id: stepView
        model: root.model
        height: contentHeight
        width: contentWidth

        delegate: Rectangle {
            width: 24
            height: 24
            color: model.sampleId === -1 ? "#333" : "#1c6bd4"
            border.color: "#9ba1a8"

            TextInput {
                id: stepInput
                width: parent.width
                height: parent.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: ""
                font.bold: true
                color: "#eeeeee"
                maximumLength: 1
                inputMethodHints: Qt.ImhPreferUppercase | Qt.ImhNoPredictiveText

                onTextChanged: {
                    if (text.length > 1) {
                        text = text.slice(-1)  // Keep only the last character entered
                    }
                    let number = parseInt(stepInput.text);
                    if (!isNaN(number)) {
                        root.model.setStep(index, number);
                        console.log("Setting step", index, "id", number);
                    } else {
                        root.model.setStep(index, -1);
                        text = "";
                    }
                }
            }

            // MouseArea {
            //     anchors.fill: parent
            //     onClicked: {
            //         const newId = 1;
            //         root.model.setStep(index, newId)
            //     }
            // }

            // MouseArea {
            //     anchors.fill: parent
            //     onClicked: {
            //         const newId = 1;
            //         // stepInput.selectAll();
            //         stepInput.focus = true;
            //     }
            // }
        }
    }

}
