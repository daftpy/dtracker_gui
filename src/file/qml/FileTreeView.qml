import QtQuick
import QtQuick.Controls
import dtracker_gui.file

// This component displays a navigable tree view of the filesystem using a TreeView
// and a custom QFileSystemModel-derived backend. It supports mouse and keyboard
// interaction, allowing the user to explore and preview audio files (.wav, .aac).
Item {
    id: root
    signal previewSample(path : string)
    signal addSample(path : string)

    FileTreeModel {
        id: fileModel
    }

    Rectangle {
        anchors.fill: parent
        anchors.bottomMargin: 4
        color: "#272a2e"
        radius: 3

        TreeView {
            id: fileTreeView

            // Set a hard width for now
            anchors.fill: parent
            anchors.topMargin: 4
            anchors.bottomMargin: 4
            clip: true
            focus: true // Allows the component to grab focus

            // Scroll behavior
            boundsBehavior: Flickable.StopAtBounds
            boundsMovement: Flickable.StopAtBounds

            // Stores the index of the currently hovered delegate.
            // This avoids stale hover visuals caused by delegate reuse during expansion.
            property int hoveredIndex: -1

            // Stores the index of the selected (clicked) delegate.
            property int lastIndex: -1

            model: fileModel
            rootIndex: fileModel.rootIndex

            delegate: FileTreeDelegate {
                id: delegate

                onFileClicked: (path) => {
                    console.log("Delegate emitted fileClicked for path:", path)
                    root.previewSample(path)
                }

                onFileControlClicked: (path) => root.addSample(path);
            }

            // If the mouse leaves the tree, reset hoverstate
            HoverHandler {
                id: fileTreeViewHover

                onHoveredChanged: {
                    if (!hovered) {
                        fileTreeView.hoveredIndex = -1;
                    }
                }
            }

            // Key handler for tracker-style navigation
            Keys.onPressed: event => {
                let nextIndex = fileTreeView.lastIndex;

                let expandDir = false;

                // Move the next index based on the key press and check bounds
                switch (event.key) {
                    case Qt.Key_W:
                    case Qt.Key_Up:
                        nextIndex = Math.max(0, fileTreeView.lastIndex - 1);
                        break;
                    case Qt.Key_S:
                    case Qt.Key_Down:
                        nextIndex = fileTreeView.lastIndex + 1;
                        break;
                    case Qt.Key_Space:
                        // Mark dir for expansion/collapse
                        expandDir = true;
                        break;
                    default:
                        return;
                }

                // Update selection
                fileTreeView.lastIndex = nextIndex;

                // Get the highlighted delegate
                const row = fileTreeView.lastIndex;
                const modelIndex = fileTreeView.index(row, 0);  // Get the QModelIndex from the visual row

                const info = fileModel.fileInfoFromIndex(modelIndex);

                // Preview file if it's not a directory
                if (!info.isDir) {
                    console.log("FileTreeView: Audio file clicked. emit previewSample")
                    root.previewSample(info.path);
                    return;
                }

                // If expandDir true, collapse/expand the selected directory
                if (info.isDir && expandDir) {
                    if (fileTreeView.itemAtIndex(modelIndex).expanded) {
                        fileTreeView.collapse(row);
                    } else {
                        fileTreeView.expand(row);
                    }
                }

                // Stop event prop
                event.accepted = true;
            }
            Component.onCompleted: fileTreeView.forceActiveFocus()

            // Styling for the scrollbar
            ScrollIndicator.vertical: ScrollIndicator {
                active: true
                implicitWidth: 8

                contentItem: Rectangle {
                    color: "#8fa1b8"
                    opacity: fileTreeView.movingVertically ? 0.5 : 0.0

                    Behavior on opacity {
                        OpacityAnimator {
                            duration: 300
                        }
                    }
                }
            }
        }
    }
}
