import QtQuick
import dtracker_gui.tracker

// Delegate for displaying a single registered sample in a ListView.
// It shows the sample's index, name, and a delete icon.
Item {
    id: root

    // Required propeerties from the model data
    required property int index
    required property string name
    required property int id
    signal previewSample(int id)
    signal removeSample(int id)


    // Takes the full width of the ListView with height determined by content
    width: ListView.view.width
    height: delegateBackground.height

    // Main background for the delegate content
    Rectangle {
        id: delegateBackground

        // Full width and height based on content
        width: parent.width
        height: contentRow.implicitHeight

        // Highlighting for selection or deletion
        color: delegateHoverHandler.hovered ? (deleteIconHoverHandler.hovered ? "#5c1717" : "#4a5263" ) : "transparent"

        // Content container
        Row {
            id: contentRow

            // Size and spacing rules
            width: parent.width
            spacing: 5
            padding: 6

            // Background for the sampleId
            Rectangle {
                id: sampleIdRect

                // Small and centered
                width: 16
                height: 16
                anchors.verticalCenter: sampleName.verticalCenter

                // Style rules
                color: "#a2a6ab"
                border.color: "#909499"
                border.width: 1
                radius: 2

                Text {
                    id: sampleId
                    anchors.centerIn: parent

                    // Style rules
                    textFormat: Text.RichText // Allows HTML
                    text: "<strong><tt>" + root.id + "</tt></strong>" // <tt> for monospace
                    color: "#1a1c1b"
                }
            }

            // Sample name
            Text {
                id: sampleName
                // Subtract the other row elements, padding, and spacing
                width: parent.width - parent.padding * 2 - parent.spacing * 2 - sampleIdRect.width - deleteIcon.width

                // Font rules
                font.pointSize: 9
                color: "#eeeeee"
                font.weight: 600
                text: name
                elide: Text.ElideRight // Ellipsis
            }

            // Delete icon
            Image {
                id: deleteIcon

                // atlast positions for different icon state
                property var inactive: Qt.rect(0, 0, 24, 24) // gray
                property var hovered: Qt.rect(72, 0, 24, 24) // red

                source: "../../icons/trash-atlas.svg"

                // Scale the svg down
                sourceSize.width: 64
                sourceSize.height: 16
                // Use a clip to only show a section of the atlas
                sourceClipRect: deleteIconHoverHandler.hovered ? hovered : inactive
                anchors.verticalCenter: sampleName.verticalCenter

                HoverHandler {
                    id: deleteIconHoverHandler
                }

                TapHandler {
                    onSingleTapped: {
                        root.removeSample(root.id);
                    }
                }
            }
        }

        // Border rect
        Rectangle {
            id: delegateBorder

            // Full length along the bottom of the delegate
            width: parent.width
            anchors.bottom: parent.bottom

            // style rules
            height: 1
            color: "#45484f"

            opacity: root.index + 1 !== root.ListView.view.model.rowCount() ? 1.0 : 0.0
        }

        HoverHandler {
            id: delegateHoverHandler
            cursorShape: Qt.PointingHandCursor
        }
        TapHandler {
            id: listViewTapHandler

            onDoubleTapped: {
                root.previewSample(root.id);
            }
        }
    }
    Connections {
        target: root.ListView.view.model

        function onRowsInserted(parent, first, last) {
            delegateBorder.opacity = root.index + 1 !== root.ListView.view.model.rowCount() ? 1.0 : 0.0
        }

        function onRowsRemoved(parent, first, last) {
            delegateBorder.opacity = root.index + 1 !== root.ListView.view.model.rowCount() ? 1.0 : 0.0

        }

        function onRowsMoved(parent, first, last) {
            delegateBorder.opacity = root.index + 1 !== root.ListView.view.model.rowCount() ? 1.0 : 0.0
        }
    }
}
