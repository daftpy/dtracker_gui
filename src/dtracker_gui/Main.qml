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

    SplitView {
        id: mainView
        anchors.fill: parent

        FileTreeView {
            SplitView.preferredWidth: 150
            SplitView.fillHeight: true
            onPreviewSample: (path) => {
                fileLoader.load(path);
            }
        }

        Rectangle {
            SplitView.fillWidth: true
            SplitView.fillHeight: true
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
