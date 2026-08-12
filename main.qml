import QtQuick
import QtQuick.Window
import Shell

Item {
    id: root

    property WindowInfo activeWin: bar.activeWindow

    onActiveWinChanged: {}

    Component.onCompleted: {}

    Rectangle {
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        visible: true
        width: 400
        height: parent.height
        color: "red"

        MouseArea {
            anchors {
                fill: parent
            }
            onClicked: mouse => {}
        }
        Text {
            id: title

            anchors {
                centerIn: parent
            }

            text: bar.activeWindow.title
            // text: "test"
            color: "white"
        }
    }
}
