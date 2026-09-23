import QtQuick
import QtQuick.Window
import QtQuick.Controls
import Shell

Item {
    id: root

    property WindowInfo activeWin: bar.activeWindow

    onActiveWinChanged: {}

    Component.onCompleted: {}

    Rectangle {
        anchors {
            fill: parent
        }

        color: ThemeManager.background

        Row {
            anchors {
                verticalCenter: parent.verticalCenter
            }

            Repeater {
                model: bar.workspaces

                WorkspaceButton {
                    required property Workspace modelData

                    anchors {
                        verticalCenter: parent.verticalCenter
                    }

                    workspaceData: modelData

                    onEntered: drag => {}
                    onRequestWorkspaceActivate: id => {
                        bar.RequestActivateWorkspace(id);
                    }
                }
            }
        }

        Label {
            anchors {
                centerIn: parent
            }

            color: ThemeManager.textCol
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
            color: ThemeManager.textCol

            text: bar.clockString
        }
    }
}
