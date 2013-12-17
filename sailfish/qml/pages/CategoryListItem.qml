import QtQuick 2.0
import Sailfish.Silica 1.0

ListItem {
    id: listItem
    contentHeight: Theme.itemSizeMedium // two line delegate
    x: Theme.paddingLarge
    width: page.width - 2 * Theme.paddingLarge

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
}
