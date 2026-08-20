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
            // horizontalCenter: parent.horizontalCenter
            fill: parent
        }
        visible: true
        // width: parent.width
        // height: parent.height
        // color: "red"

        Row {
            Repeater {
                model: bar.workspaces
                Rectangle {
                    color: modelData.active ? "Red" : "Grey"
                    height: 10
                    width: 10
                }
            }
        }

        MouseArea {
            anchors {
                fill: parent
            }
            onClicked: mouse => {}
        }
        Label {
            anchors {
                centerIn: parent
            }

            text: root.activeWin !== null ? root.activeWin.title : ""
        }
        // Text {
        //     id: title
        //
        //     anchors {
        //         centerIn: parent
        //     }
        //
        //     text: root.activeWin !== null ? root.activeWin.title : ""
        //     color: "white"
        // }
    }
}
