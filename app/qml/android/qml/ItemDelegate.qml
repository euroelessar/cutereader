import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.1

Item {
    id: root

    property alias title: titleLabel.text
    property alias description: descriptionLabel.text
    property alias imageSource: image.source
    property alias clickable: area.enabled

    signal clicked

    width: parent.width
    height: content.implicitHeight + 2*content.anchors.margins

    MouseArea {
        id: area

        onClicked: root.clicked()

        anchors.fill: parent
    }

    Rectangle {
        anchors.fill: parent
        color: "#11ffffff"
        visible: area.pressed
    }

    RowLayout {
        id: content

        anchors.fill: parent
        anchors.margins: 15

        Image {
            id: image
            Layout.alignment: Qt.AlignVCenter
            Layout.maximumWidth: 80
            Layout.maximumHeight: 80

            asynchronous: true
            cache: true
            fillMode: Image.PreserveAspectFit
            clip: true
        }
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Label {
                id: titleLabel
                Layout.fillWidth: true

                font.pointSize: 22
                color: "white"
                maximumLineCount: 1
                elide: "ElideRight"
            }
            Label {
                id: descriptionLabel
                Layout.fillWidth: true

                font.pointSize: titleLabel.font.pointSize * 0.8
                color: "white"
                maximumLineCount: 3
            }
        }
        Image {
            id: arrow
            Layout.alignment: Qt.AlignVCenter

            source: "style/images/navigation_next_item.png"
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        height: 1
        color: "#424246"
    }
}
