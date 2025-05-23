import QtQuick
import QtQuick.Controls
import dtracker_gui.audio

Window {
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: qsTr("Hello World")

    AudioManager {
        id: audioManager
    }

    StackView {
        id: mainView
        anchors.fill: parent

        initialItem: AudioSettingsPage {
            info: audioManager.deviceInfo;

            anchors.fill: parent

            onClosePage: {
                console.log("pop");
                mainView.replace(mainPage);
            }

            onStartEngine: {
                audioManager.startEngine();
            }
        }
    }

    Component {
        id: mainPage

        Page {
            Text {
                text: "Main Page"
            }
        }
    }
}
