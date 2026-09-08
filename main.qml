import QtQuick
import QtQuick.Window
import QtQuick.Controls
import Shell

Item {
    id: root

    property WindowInfo activeWin: bar.activeWindow

    onActiveWinChanged: {}

    Component.onCompleted: {}

    Pane {
        anchors {
            fill: parent
        }
        visible: true

        Row {
            anchors {
                verticalCenter: parent.verticalCenter
            }

            Repeater {
                model: bar.workspaces

                WorkspaceButton {
                    id: button

                    required property Workspace modelData

                    height: button.hovered ? 25 : 20
                    width: 20
                    Rectangle {
                        anchors.fill: parent
                        color: button.modelData.active ? "Red" : "Grey"
                    }
                    onClicked: button => {
                        if (button === Qt.LeftButton) {
                            bar.RequestActivateWorkspace(modelData.id);
                        } else if (button === Qt.RightButton) {
                            console.log("Right Click");
                        } else {
                            console.log("Other");
                        }
                    }
                    onMousePressed: button => {
                        if (button === Qt.LeftButton) {} else if (button === Qt.RightButton) {} else {}
                    }
                }
            }
        }

        Label {
            anchors {
                centerIn: parent
            }

            text: root.activeWin !== null ? root.activeWin.title : ""
        }

        Label {
            id: clock

            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
            }

            font {
                family: "Jetbrains Mono"
                pixelSize: 16
            }
            text: bar.clockString
        }
    }
}
