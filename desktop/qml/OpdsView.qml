import QtQuick 2.0
import QtQuick.XmlListModel 2.0
import org.qutim 0.3

ListView {
    id: root
    property alias source: opdsModel.source

    signal linkRequested(url source)
    signal bookRequested(variant opdsEntry)

    model: OpdsBookModel {
        id: opdsModel
    }
    delegate: Rectangle {
        id: item
        width: parent.width
        border.width: 1
        height: 40
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 5
            text: title
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (isBook)
                    root.bookRequested(opdsEntry)
                else
                    root.linkRequested(source)
            }
        }
    }
}
