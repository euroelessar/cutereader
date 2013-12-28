import QtQuick 2.1
import Sailfish.Silica 1.0

ListItem {
    id: listItem
    contentHeight: Theme.itemSizeMedium // two line delegate
    menu: actions !== undefined && actions.length > 0 ? contextMenu : undefined
    x: Theme.paddingLarge
    width: page.width - 2 * Theme.paddingLarge

    signal actionClicked(variant data)

    Label {
        id: label
        width: parent.width
        text: title
        color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
        elide: Text.ElideRight
    }
    Label {
        anchors.top: label.bottom
        width: parent.width
        text: subtitle
        font.pixelSize: Theme.fontSizeSmall
        color: listItem.highlighted ? Theme.highlightColor : Theme.secondaryColor
        elide: Text.ElideRight
    }

    Component {
        id: contextMenu
        ContextMenu {
            Repeater {
                model: actions
                MenuItem {
                    text: modelData.title

                    onClicked: listItem.actionClicked(modelData)
                }
            }
        }
    }
}
