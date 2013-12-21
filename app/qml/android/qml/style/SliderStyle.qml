import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1


SliderStyle {
    handle: Rectangle {
        width: 30
        height: 30
        radius: height
        antialiasing: true
        color: Qt.lighter("#468bb7", 1.2)
    }

    groove: Item {
        implicitHeight: 50
        implicitWidth: 400
        Rectangle {
            height: 8
            width: parent.width
            anchors.verticalCenter: parent.verticalCenter
            color: "#444"
            opacity: 0.8
            Rectangle {
                antialiasing: true
                radius: 1
                color: "#468bb7"
                height: parent.height
                width: parent.width * control.value / control.maximumValue
            }
        }
    }
}
