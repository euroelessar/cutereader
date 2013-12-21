import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

ButtonStyle {
    panel: Item {
        implicitHeight: 50
        implicitWidth: 320
        BorderImage {
            anchors.fill: parent
            antialiasing: true
            border.bottom: 8
            border.top: 8
            border.left: 8
            border.right: 8
            anchors.margins: control.pressed ? -4 : 0
            source: control.pressed ? "images/button_pressed.png" : "images/button_default.png"
            Label {
                text: control.text
                anchors.centerIn: parent
                color: "white"
                font.pointSize: 18
            }
        }
    }
}
