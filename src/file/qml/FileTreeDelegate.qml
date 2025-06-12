import QtQuick
import QtQuick.Controls

// Custom delegate for each item in the TreeView
TreeViewDelegate {
    id: delegate
    indentation: 8
    // Ensure the delegate width matches the TreeView width, or fall back to 150
    implicitWidth: fileTreeView.width > 0 ? fileTreeView.width : 150

    // Properties exposed from the model
    required property int index
    required property url filePath
    required property string fileName

    signal fileClicked(string path)
    signal fileControlClicked(string path)


    // Indicator shown to the left of each item (only visible if the item has children)
    indicator: Image {
        id: directoryIcon

        // Indent based on tree depth
        x: delegate.leftMargin + (delegate.depth * delegate.indentation) + 2
        anchors.verticalCenter: parent.verticalCenter

        // Show folder icons for directories, or a music note for files
        source: delegate.hasChildren ? (delegate.expanded
                    ? "../icons/folder-open.svg" : "../icons/folder.svg")
                : "../icons/musicnotes.svg"

        sourceSize.width: 16
        sourceSize.height: 16
        fillMode: Image.PreserveAspectFit

        smooth: true
        antialiasing: true
        asynchronous: true
    }

    // Main row content: icon + file name
    contentItem: Row {
        width: parent.implicitWidth
        spacing: 4
        padding: 2

        // Secondary icon for files, hidden for folders (since folders already use the indicator)
        Image {
            id: delegateIcon
            source: delegate.hasChildren ? (delegate.expanded
                        ? "../icons/folder.svg" : "../icons/folder.svg")
                    : "../icons/musicnotes.svg"
            visible: delegate.hasChildren ? false : true
            sourceSize.width: 14
            sourceSize.height: 14
            anchors.verticalCenter: parent.verticalCenter
        }

        // File or folder name
        Text {
            width: parent.width - delegateIcon.width
            text: delegate.fileName
            color: "#eeeeee"
            elide: Text.ElideRight
            font.weight: delegate.hasChildren ? 600 : 400 // Bold for folders
        }
    }

    // Background changes color on hover or selection
    background: Rectangle {
        color: (fileTreeView.hoveredIndex === delegate.index) ? "#1c6bd4"
            : ((delegate.index === fileTreeView.lastIndex) ? "#333940" : "transparent")
    }

    // Handle hover state to update hoveredIndex for consistent visuals
    HoverHandler {
        id: hoverHandler
        onHoveredChanged: {
            if (hovered) {
                // Set hovered index on hover in
                fileTreeView.hoveredIndex = index;
            }
        }
    }

    // Handle mouse clicks to support selection and folder expansion
    MouseArea {
        id: delegateMouseArea
        anchors.fill: delegate
        cursorShape: Qt.PointingHandCursor

        onClicked: (mouse) => {
            const path = model.filePath;
            console.log("Clicked:", path);

            // Toggle directory expanded state
            fileTreeView.toggleExpanded(delegate.row);

            // Clear selection if collapsing a folder
            if (delegate.hasChildren && !delegate.expanded) {
                fileTreeView.lastIndex = -1;
            }

            // Update selected index
            fileTreeView.lastIndex = delegate.index;

            if (!delegate.hasChildren) {
                if (mouse.modifiers & Qt.ControlModifier) {
                    console.log("FileTreeView: Ctrl+Clicked, emit fileControlClicked");
                    delegate.fileControlClicked(path);
                } else {
                    console.log("FileTreeView: Regular click, emit fileClicked");
                    delegate.fileClicked(path);
                }
            }

            console.log(fileTreeView.lastIndex, delegate.index);
        }

    }
}
