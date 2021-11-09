import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

import ACross

Menu {
    id: nodeItemPopMenu
    topPadding: 8
    bottomPadding: 8

    property real menuWidth: 168

    onClosed: {
        nodeItemPopMenu.close()
        nodeItemPopMenu.destroy()
    }

    background: CardBox {
        id: popMenuBackground
        color: acrossConfig.backgroundColor

        implicitWidth: menuWidth

        borderWidth: 1
        borderColor: acrossConfig.deepColor
    }

    delegate: MenuItem {
        id: menuItem

        contentItem: Text {
            text: menuItem.text
            color: menuItem.highlighted ? acrossConfig.highlightTextColor : acrossConfig.textColor
        }

        background: Rectangle {
            // TODO: align center
            implicitWidth: menuWidth - 2 * acrossConfig.itemSpacing
            color: menuItem.highlighted ? acrossConfig.highlightColor : "transparent"
        }
    }

    Action {
        text: qsTr("Edit")
        onTriggered: {
            openEditForm(model)
        }
    }

    Action {
        text: qsTr("Set as default")

        onTriggered: {
            acrossNodes.setAsDefault(nodeID)
        }
    }

    MenuSeparator {
        background: Rectangle {
            height: 1
            color: acrossConfig.deepColor
        }
    }

    Action {
        id: shareConfigAction
        text: qsTr("Share Config")

        onTriggered: {
            openShareForm(model)
        }
    }

    Action {
        text: qsTr("TCP Ping")

        onTriggered: {
            acrossNodes.testLatency(nodeID)
        }
    }

    MenuSeparator {
        background: Rectangle {
            height: 1
            color: acrossConfig.deepColor
        }
    }

    Action {
        text: qsTr("Delete")

        onTriggered: {
            acrossNodes.removeNodeByID(nodeID)
        }
    }
}
