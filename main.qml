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

            move: Transition {
                NumberAnimation {
                    properties: "x,y"
                    easing.type: Easing.OutQuad
                }
            }
            Repeater {
                model: DelegateModel {
                    id: visualModel

                    model: bar.workspaces

                    delegate: DropArea {
                        required property Workspace modelData
                        required property int index

                        width: button.width
                        height: button.height

                        onEntered: drag => {
                            visualModel.items.move((drag.source as WorkspaceButton).index, index);
                        }
                        onDropped: drag => {
                            let targetID = (drag.source as WorkspaceButton).workspaceData.id;
                            bar.RequestSetWorkspaceIndex(targetID, index);
                        }

                        WorkspaceButton {
                            id: button

                            anchors {
                                verticalCenter: parent.verticalCenter
                            }

                            workspaceData: parent.modelData
                            index: parent.index

                            onRequestWorkspaceActivate: id => {
                                bar.RequestActivateWorkspace(id);
                            }
                        }
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
