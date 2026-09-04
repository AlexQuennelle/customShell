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
                    onMousePressed: button => {
                        if (button === Qt.LeftButton) {
                            console.log(modelData.id)
                            console.log("Left");
                        } else if (button === Qt.RightButton) {
                            console.log("Right");
                        } else {
                            console.log(button);
                        }
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
    }
}
