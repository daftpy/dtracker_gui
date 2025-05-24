import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: audioSettingsPage
    property audioDeviceInfo info
    signal stopTone()
    signal startTone()

    ColumnLayout {
        anchors.fill: parent
        Rectangle {
            width: nameContainer.implicitWidth
            height: nameContainer.implicitHeight
            color: "red"

            Layout.alignment: Qt.AlignHCenter
            ColumnLayout {
                id: nameContainer
                Text {
                    text: "Current Device"
                    font.bold: true;
                    Layout.alignment: Qt.AlignHCenter
                }

                Text {
                    id: deviceName
                    text: info.name
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }

        Rectangle {
            width: outputContainer.implicitWidth
            height: outputContainer.implicitHeight
            color: "blue"
            Layout.alignment: Qt.AlignHCenter
            ColumnLayout {
                id: outputContainer
                Text {
                    text: "Current Device"
                    font.bold: true;
                    Layout.alignment: Qt.AlignHCenter
                }

                Text {
                    id: deviceOutputChannels
                    text: info.outputChannels
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Button {
                id: startButton
                text: "Start"
                onPressed: {
                    audioSettingsPage.startEngine();
                }
            }

            Button {
                id: exitButton
                text: "Exit"

                onPressed: {
                    audioSettingsPage.closePage();
                }
            }
        }
    }
}
