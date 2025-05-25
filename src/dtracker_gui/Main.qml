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

    StackView {
        id: mainView
        anchors.fill: parent

        initialItem: FileTreeView {

            onPreviewSample: (path) => {
                fileLoader.load(path);
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
