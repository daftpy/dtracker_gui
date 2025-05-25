import QtCore
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Dialogs

Item {
    id: root
    signal fileSelected(path: string)

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: {
          root.fileSelected(fileDialog.selectedFile);
        }
    }

    Component.onCompleted: {
        // fileDialog.open();
    }
}
