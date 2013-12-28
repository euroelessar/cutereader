import QtQuick 2.1
import org.qutim 0.3
import Sailfish.Silica 1.0

LibraryPageBase {
    id: root

    property alias data: entry.entry

    property OpdsEntry entryItem: OpdsEntry {
        id: entry
    }

    SilicaFlickable {
        id: scrollView
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            id: contextMenu

            Repeater {
                model: entry.actions
                MenuItem {
                    text: modelData.title

                    onClicked: root.processAction(modelData)
                }
            }
        }

        Column {
            id: column
            width: scrollView.width

            PageHeader {
                id: pageHeader
                title: entry.title
            }

            Image {
                source: entry.cover
                anchors.horizontalCenter: parent.horizontalCenter

                property real factor: Math.min(implicitWidth, parent.width) / implicitWidth

                width: implicitWidth * factor
                height: implicitHeight * factor
            }

            Label {
                text: entry.content
                width: column.width
                wrapMode: Text.WordWrap
            }
        }
    }

    Component.onCompleted: {
        console.log(JSON.stringify(entry.actions))
    }
}
