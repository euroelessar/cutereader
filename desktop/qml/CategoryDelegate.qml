import QtQuick 2.0

Rectangle {
    id: item
    width: parent.width
    border.width: 1
    height: 48

    Column {
        anchors.margins: 5
        anchors.fill: parent
        spacing: 5
        Text {
            id: titleItem
            elide: Text.ElideRight
            width: parent.width
            text: title
        }
        Text {
            text: subtitle
            elide: Text.ElideRight
            width: parent.width
            font.pointSize: 0.9 * titleItem.font.pointSize
        }
    }
}
