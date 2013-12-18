import QtQuick 2.1
import org.qutim 0.3
import QtQuick.Controls 1.0

Item {
    id: root
    property alias opdsEntry: entry.entry
    signal linkRequested(url source)
    signal bookRequested(variant opdsEntry)
    
    property OpdsEntry entryItem: OpdsEntry {
        id: entry
    }
    
    Column {
        id: column
        anchors.fill: parent
        Text {
            text: entry.title
            width: column.width
        }
        Image {
            source: entry.cover
            width: implicitWidth / 2
            height: implicitHeight / 2
        }
        Text {
            text: entry.content
            width: column.width
            wrapMode: Text.WordWrap
        }
        Button {
            id: button
            menu: contextMenu
            
            Menu {
                id: contextMenu
                Instantiator {
                    model: entry.actions
                    MenuItem {
                        text: modelData.title
                        
                        onTriggered: {
                            if (modelData.type === "text/html")
                                Qt.openUrlExternally(modelData.source)
                            else if (modelData.type === "application/atom+xml")
                                root.linkRequested(modelData.source)
                            else if (modelData.relation === "http://opds-spec.org/acquisition/open-access")
                                console.log("Request to load:", modelData.source)
                        }
                    }
                    onObjectAdded: contextMenu.insertItem(index, object)
                    onObjectRemoved: contextMenu.removeItem(object)
                }
            }
        }
    }
    
    Component.onCompleted: {
        console.log(JSON.stringify(entry.actions))
    }
}
