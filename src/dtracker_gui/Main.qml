import QtQuick
import QtQuick.Controls
import dtracker_gui.audio
import dtracker_gui.file

Window {
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: qsTr("Hello World")

    AudioManager {
        id: audioManager
    }

    AudioDecoder {
        id: fileLoader

        onSampleReady: (pcmData, sampleRate) => {
                           console.log("Playing sample");
                           audioManager.playSample(pcmData, sampleRate)
                       }

        onErrorOccurred: (error) => {
                             console.log(error);
                         }
    }

    // StackView {
    //     id: mainView
    //     anchors.fill: parent

    //     initialItem: DeviceInfoPage {
    //         info: audioManager.deviceInfo;

    //         anchors.fill: parent

    //         onStopTone: {
    //             // mainView.replace(mainPage);
    //             audioManager.stopSin();
    //         }

    //         onStartTone: {
    //             // audioManager.startEngine();
    //             audioManager.startSin();
    //         }
    //     }
    // }

    StackView {
        id: mainView
        anchors.fill: parent

        initialItem: FileFinder {
            anchors.fill: parent


            onFileSelected: (path) => {
                console.log(`Calling fileLoader.load(${path})`)
                fileLoader.load(path);
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
