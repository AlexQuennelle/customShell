import QtQuick
import Shell

Item {
    id: root

    required property Workspace workspaceData
    required property int index

    readonly property real sizeFull: 28
    readonly property real sizeEmpty: 20
    readonly property real size: workspaceData.empty ? sizeEmpty : sizeFull

    signal requestWorkspaceActivate(id: string)

    width: size
    height: sizeFull

    Drag.source: root
    Drag.active: mouseArea.drag.active
    Drag.hotSpot.x: root.width / 2
    Drag.hotSpot.y: root.height / 2

    Rectangle {
        anchors {
            margins: 4
            centerIn: parent
        }

        width: parent.width - 8
        height: parent.width - 8

        radius: 12
        color: {
            if (root.workspaceData.focused) {
                return ThemeManager.selected;
            } else if (root.workspaceData.active) {
                return ThemeManager.textCol;
            } else if (root.workspaceData.urgent) {
                return "Yellow";
            } else if (mouseArea.containsMouse) {
                return ThemeManager.hover;
            } else if (root.workspaceData.empty) {
                return ThemeManager.empty;
            } else {
                return ThemeManager.fill;
            }
        }
    }
    MouseArea {
        id: mouseArea

        anchors {
            fill: parent
        }

        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        drag.target: root

        onReleased: root.Drag.drop()
        onClicked: event => {
            if (event.button === Qt.LeftButton) {
                root.requestWorkspaceActivate(root.workspaceData.id);
            } else if (event.button === Qt.RightButton) {
                console.log("Right Click");
            } else {
                console.log("Other");
            }
        }
    }
    states: State {
        when: mouseArea.drag.active
        AnchorChanges {
            target: root
            anchors {
                horizontalCenter: undefined
            }
        }
    }
}
