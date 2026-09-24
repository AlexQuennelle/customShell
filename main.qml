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
                    properties: "x"
                    easing.type: Easing.OutQuad
                }
            }
            Repeater {
                model: DelegateModel {
                    id: visualModel

                    model: bar.workspaces

                    delegate: DropArea {
                        id: drop
                        required property Workspace modelData
                        required property int index

                        width: button.width
                        height: button.height

                        onEntered: drag => {
                            let target = (drag.source as WorkspaceButton).index;
                            let vIdx = index;
                            visualModel.items.move(target, vIdx);
                            index = (drag.source as WorkspaceButton).index;
                            (drag.source as WorkspaceButton).parent.index = vIdx;
                        }
                        onDropped: drag => {
                            drag.source.parent = drop;
                            let targetID = modelData.id;
                            let targetIndex = (drag.source as WorkspaceButton).index;
                            bar.RequestSetWorkspaceIndex(targetID, targetIndex);
                        }

                        WorkspaceButton {
                            id: button

                            anchors {
                                verticalCenter: parent.verticalCenter
                                horizontalCenter: parent.horizontalCenter
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
