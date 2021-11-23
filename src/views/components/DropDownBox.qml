import QtQuick
import QtQuick.Controls

import ACross

ComboBox {
    id: comboBox
    implicitWidth: 128
    implicitHeight: 32

    property int controlWidth: 10
    property int controlHeight: 8

    contentItem: Text {
        anchors.left: parent.left
        anchors.leftMargin: acrossConfig.itemSpacing * 3

        text: comboBox.displayText
        color: acrossConfig.deepTextColor
        verticalAlignment: Text.AlignVCenter
    }

    delegate: ItemDelegate {
        implicitWidth: comboBox.width

        background: Rectangle {
            color: hovered ? acrossConfig.highlightColor : acrossConfig.backgroundColor
        }

        contentItem: Label {
            id: itemText
            text: modelData
            color: hovered ? acrossConfig.highlightTextColor : acrossConfig.textColor
        }
    }

    indicator: Canvas {
        id: canvas
        x: comboBox.width - width - comboBox.rightPadding
        y: Math.round((comboBox.height - height) / 2)

        implicitWidth: controlWidth
        implicitHeight: controlHeight

        Connections {
            target: comboBox
            function onPressedChanged() {
                canvas.requestPaint()
            }
        }

        onPaint: {
            var ctx = getContext("2d")

            ctx.reset()
            ctx.moveTo(0, 0)
            ctx.lineTo(width, 0)
            ctx.lineTo(Math.round(width / 2), height)
            ctx.closePath()
            ctx.fillStyle
                    = comboBox.pressed ? acrossConfig.backgroundColor : acrossConfig.highlightColor
            ctx.fill()
        }
    }

    popup: Popup {
        y: comboBox.height

        implicitWidth: comboBox.width

        contentItem: ListView {
            implicitHeight: contentHeight
            implicitWidth: comboBox.width

            clip: true
            model: comboBox.delegateModel
            currentIndex: comboBox.highlightedIndex
            ScrollIndicator.vertical: ScrollIndicator {}
        }

        background: CardBox {
            color: acrossConfig.backgroundColor
            borderWidth: acrossConfig.borderWidth
            borderColor: acrossConfig.deepColor
        }
    }

    background: Rectangle {
        color: acrossConfig.deepColor
        radius: acrossConfig.borderRadius
    }
}
