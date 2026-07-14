import QtQuick
import QtQuick.Window

Item {
    id: root

    Rectangle {
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        visible: true
        width: 400
        height: parent.height
        color: "red"

        Text {
            anchors {
                centerIn: parent
            }

            text: "Bar"
            color: "white"
        }
    }
}
