import QtQuick
import Shell

DropArea {
    id: root

    required property Workspace workspaceData
    required property int index

    readonly property real sizeFull: 28
    readonly property real sizeEmpty: 20
    readonly property real size: workspaceData.empty ? sizeEmpty : sizeFull

    property bool dragging: false

    signal requestWorkspaceActivate(id: string)
    signal requestSetIndex(index: int)

    width: size
    height: size

    Drag.source: mouseArea
    Drag.active: mouseArea.drag.active
    Drag.hotSpot.x: size / 2
    Drag.hotSpot.y: size / 2

    onDropped: drag => {}

    Rectangle {
        anchors {
            fill: parent
            margins: 4
        }

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

        onPressAndHold: root.dragging = true
        // onReleased: root.dragging = false
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
}
