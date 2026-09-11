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
                    id: button

                    required property Workspace modelData
                    property real size: button.modelData.empty ? 20 : 28

                    anchors {
                        verticalCenter: parent.verticalCenter
                    }

                    height: size
                    width: size
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: 4
                        radius: 12
                        color: {
                            if (button.modelData.focused) {
                                return ThemeManager.selected;
                            } else if (button.modelData.active) {
                                return ThemeManager.textCol;
                            } else if (button.modelData.urgent) {
                                return "Yellow";
                            } else if (button.hovered) {
                                return ThemeManager.hover;
                            } else if (button.modelData.empty) {
                                return ThemeManager.empty;
                            } else {
                                return ThemeManager.fill;
                            }
                        }
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
