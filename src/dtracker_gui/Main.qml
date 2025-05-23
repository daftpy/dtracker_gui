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
                // mainView.replace(mainPage);
                audioManager.stopSin();
            }

            onStartEngine: {
                // audioManager.startEngine();
                audioManager.startSin();
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
