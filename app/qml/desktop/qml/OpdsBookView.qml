import QtQuick 2.1
import org.qutim 0.3
import QtQuick.Controls 1.0

Item {
    id: root
    property alias opdsEntry: entry.entry

    signal actionTriggered(variant type, variant entry)

    property OpdsEntry entryItem: OpdsEntry {
        id: entry
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent

        Column {
            id: column
            width: scrollView.width

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

                            onTriggered: root.actionTriggered(modelData.type, modelData.entry)
                        }
                        onObjectAdded: contextMenu.insertItem(index, object)
                        onObjectRemoved: contextMenu.removeItem(object)
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        console.log(JSON.stringify(entry.actions))
    }
}
