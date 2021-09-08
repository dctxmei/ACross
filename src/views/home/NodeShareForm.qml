import QtQuick 2.12
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import ACross 1.0

import "../components/"

Window {
    id: nodeShareFormWindow
    width: 680
    height: 420
    minimumWidth: 680
    minimumHeight: 420
    title: qsTr("Share Configuration")

    flags: Qt.WindowStaysOnTopHint
    modality: Qt.ApplicationModal


    Rectangle {
        id: background
        anchors.fill: parent

        color: acrossConfig.deepColor

        CardBox {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing

            Row {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing

                Rectangle {
                    implicitWidth: parent.width / 2
                    implicitHeight: parent.height

                    color: "transparent"

                    SVGBox {
                        anchors.centerIn: parent

                        source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/repo_qr_code.svg"
                        sourceWidth: parent.implicitWidth / 1.2
                        sourceHeight: sourceWidth
                    }
                }

                ColumnLayout {
                    implicitWidth: parent.width / 2
                    implicitHeight: parent.height

                    spacing: acrossConfig.itemSpacing

                    Label {
                        text: qsTr("Node Info")
                        color: acrossConfig.textColor
                        font.pixelSize: 18
                    }

                    GridLayout {
                        columns:2

                        Label {
                            text: qsTr("Name")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: name
                            readOnly: true
                        }

                        Label {
                            text: qsTr("Address")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: address
                            readOnly: true
                        }

                        Label {
                            text: qsTr("Port")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: port
                            readOnly: true
                        }
                        
                        Label {
                            text: qsTr("Password")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: password
                            readOnly: true
                        }
                    }

                    Label {
                        text: qsTr("Copy URL")
                        color: acrossConfig.textColor
                        font.pixelSize: 18
                    }

                    TextAreaBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Item {
                            Layout.fillWidth: true
                        }

                        ButtonBox {
                            text: qsTr("Save")
                        }

                        ButtonBox {
                            text: qsTr("Copy")
                        }
                    }
                }
            }
        }
    }
}