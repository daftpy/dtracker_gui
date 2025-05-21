import QtQuick
import dtracker_gui

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    AudioManager {
        id: audioManager
    }

    Column {
        anchors.fill: parent;
        Text {
            id: deviceInfoTitle;
            text: "Current Device";
            font.bold: true;
        }

        Text {
            id: deviceName
            text: audioManager.hasDeviceInfo && audioManager.deviceInfo.name;
        }
        Text {
            id: deviceOutputChannels
            text: audioManager.hasDeviceInfo && audioManager.deviceInfo.outputChannels;
        }
    }
}
