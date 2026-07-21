import QtQuick
import QtQuick.Window
import Shell

Item {
    id: root

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
            onClicked: mouse => {
                // ShellBackend.TestPrint();
            }
        }
        Text {
            anchors {
                centerIn: parent
            }

            text: "Bar"
            color: "white"
        }
    }
}
