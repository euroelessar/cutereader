import QtQuick 2.0

Rectangle {
    id: item
    width: parent.width
    border.width: 1
    height: 48
    Image {
        id: image

        anchors.verticalCenter: parent.verticalCenter
        width: 48
        height: 48
        fillMode: Image.PreserveAspectFit
        smooth: true

        source: coverSource
    }

    Text {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: image.right
        anchors.margins: 5
        text: title
    }
}
