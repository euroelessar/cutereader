import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    SilicaListView {
        id: listView
        anchors.fill: parent

        header: PageHeader {
            id: pageHeader
            title: qsTr("Settings")
        }

        model: ListModel {
            id: listModel
        }

        delegate: ListItem {
            id: listItem
            contentHeight: Theme.itemSizeMedium
            x: Theme.paddingLarge
            width: page.width - 2 * Theme.paddingLarge

            Label {
                id: label
                text: model.title
                color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
            }
            Label {
                anchors.top: label.bottom
                text: model.subtitle
                font.pixelSize: Theme.fontSizeSmall
                color: listItem.highlighted ? Theme.highlightColor : Theme.secondaryColor
            }

            onClicked: pageStack.push(url)
        }

        Component.onCompleted: {
            function addItem(title, subtitle, url) {
                listModel.append({
                    title: title,
                    subtitle: subtitle,
                    url: url
                })
            }

            addItem(qsTr("Texts"), qsTr("Font size, borders, etc"), Qt.resolvedUrl("FontSettingsPage.qml"))
            addItem(qsTr("Colors"), qsTr("Background and foreground colors"), Qt.resolvedUrl("ColorsSettingsPage.qml"))
        }
    }
}
