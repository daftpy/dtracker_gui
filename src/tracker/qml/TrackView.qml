import QtQuick
import QtQuick.Layouts

Item {
    id: root
    width: 200
    height: ListView.view.height
    required property string name
    required property int id
    signal playSample(int id)

    Rectangle {
        width: parent.width
        height: parent.height

        color: "#bdc2c9"
        border.color: "#b7bcc4"
        border.width: 1
        radius: 6

        Column {
            anchors.fill: parent
            anchors.margins: 8
            Rectangle {
                width: parent.width
                height: trackTitle.implicitHeight + 12
                color: "#191d21"
                radius: 3
                Row {
                    anchors.fill: parent
                    anchors.margins: 8
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 5

                    Image {
                        id: muteIcon
                        property var inactive: Qt.rect(0, 0, 24, 24)
                        property var muted: Qt.rect(72, 0, 24, 24)

                        source: "../icons/ear-slash-atlas.svg"
                        sourceSize.width: 64
                        sourceSize.height: 16
                        anchors.verticalCenter: parent.verticalCenter
                        sourceClipRect: muteHoverHandler.hovered ? muted : inactive

                        HoverHandler {
                            id: muteHoverHandler
                            cursorShape: Qt.PointingHandCursor
                        }

                        Component.onCompleted: {
                            console.log("mute", muteIcon.implicitWidth)
                        }
                    }
                    Image {
                        id: previewIcon

                        property var inactive: Qt.rect(0, 0, 24, 24)
                        property var hover: Qt.rect(24, 0, 24, 24)
                        property var active: Qt.rect(48, 0, 24, 24)
                        property var muted: Qt.rect(72, 0, 24, 24)

                        source: "../icons/speaker-atlas.svg"
                        sourceSize.width: 64
                        sourceSize.height: 16
                        sourceClipRect: previewHoverHandler.hovered ? hover : inactive

                        anchors.verticalCenter: parent.verticalCenter

                        HoverHandler {
                            id: previewHoverHandler
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onPressed: {
                                root.playSample(root.id);
                            }
                        }

                        Component.onCompleted: {
                            console.log("Image", previewIcon.implicitWidth)
                        }
                    }
                    Text {
                        id: trackTitle
                        leftPadding: 2
                        width: parent.width - muteIcon.width - previewIcon.width - 12
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        text: root.name
                        color: "#eeeeee"
                        font.weight: 600
                        font.pointSize: 10
                    }
                }
            }
        }
    }
}
