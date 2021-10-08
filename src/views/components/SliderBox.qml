import QtQuick
import QtQuick.Controls

Slider {
    id: control

    background: Rectangle {
        x: control.leftPadding
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: 200
        implicitHeight: 8
        width: control.availableWidth
        height: implicitHeight
        radius: 4
        color: acrossConfig.deepColor

        Rectangle {
            width: control.visualPosition * parent.width
            height: parent.height
            color: acrossConfig.highlightColor
            radius: 4
        }
    }

    handle: CardBox {
        x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
        y: control.topPadding + control.availableHeight / 2 - height / 2
        width: 26
        height: width
        radius: width / 2
        color: control.pressed ? Qt.lighter(
                                     acrossConfig.highlightTextColor,
                                     0.9) : acrossConfig.highlightTextColor
    }
}
