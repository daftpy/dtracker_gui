import QtQuick
import QtQuick.Controls
import dtracker_gui.file

Item {
    id: root
    signal previewSample(path : string)

    FileSystemModel {
        id: fileModel
    }

    TreeView {
        id: fileTreeView

        // Set a hard width for now
        width: 150
        height: parent.height

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

        delegate: TreeViewDelegate {
            id: delegate
            indentation: 8
            implicitWidth: fileTreeView.width > 0 ? fileTreeView.width : 150

            // Bind to the models role names, making them available in the delegate
            required property int index
            required property url filePath
            required property string fileName

            indicator: Text {
                text: ">"
                x: delegate.leftMargin + (delegate.depth * delegate.indentation)
                color: delegate.index === fileTreeView.lastIndex ? "#8186a6" : "#eeeeee";
            }

            contentItem: Text {
                width: parent.implicitWidth
                text: delegate.fileName
                color: "#eeeeee"
                elide: Text.ElideRight
            }

            background: Rectangle {
                color: (delegate.index === fileTreeView.lastIndex) ? "#464963"
                    : ((fileTreeView.hoveredIndex == delegate.index) ? "#292b3b" : "#3d3e4a")
            }

            HoverHandler {
                id: hoverHandler

                onHoveredChanged: {
                    if (hovered) {
                        // Track hover at TreeView level to persist clean state
                        fileTreeView.hoveredIndex = index;
                    }
                }
            }

            MouseArea {
                id: mouseArea
                anchors.fill: delegate
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    const path = model.filePath;
                    console.log("Clicked:", path);

                    // Expand/collapse directory
                    fileTreeView.toggleExpanded(delegate.row)

                    // Clear selection if directory is closed
                    if (delegate.hasChildren && !delegate.expanded) {
                        fileTreeView.lastIndex = -1;
                    }

                    // Mark current row as selected
                    fileTreeView.lastIndex = delegate.index;

                    // Emit signal to preview file if it's not a directory
                    if (!delegate.hasChildren) {
                        root.previewSample(path);
                    }

                    console.log(fileTreeView.lastIndex, delegate.index);
                }
            }
        }
        // Key handler for tracker-style navigation
        Keys.onPressed: event => {
            let nextIndex = fileTreeView.lastIndex;

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
                root.previewSample(info.path);
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
