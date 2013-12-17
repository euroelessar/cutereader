import QtQuick 2.0
import QtQuick.XmlListModel 2.0

ListView {
    id: root
    anchors.fill: parent
    property alias source: opdsModel.source
    
    model: XmlListModel {
        id: opdsModel
        query: "/feed/entry"
        namespaceDeclarations: "declare default element namespace 'http://www.w3.org/2005/Atom';"
        XmlRole {
            name: "title"
            query: "title/string()"
        }
        XmlRole {
            name: "link"
            query: "link[@type=\"application/atom+xml;profile=opds-catalog\"][1]/@href/string()"
        }
        onStatusChanged: {
            console.log(status, count, errorString())
        }
    }
    delegate: Rectangle {
        id: item
        width: parent.width
        border.width: 1
        height: 40
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 5
            text: title //"title: " + title + "\nlink: " + "http://flibusta.net/" + link
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                opdsModel.source = "http://flibusta.net/" + link
            }
        }
    }
}
