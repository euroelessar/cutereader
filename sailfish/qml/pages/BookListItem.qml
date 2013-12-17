import QtQuick 2.0
import Sailfish.Silica 1.0

ListItem {
    id: listItem
    contentHeight: Theme.itemSizeMedium // two line delegate
    x: Theme.paddingLarge
    width: page.width - 2 * Theme.paddingLarge

    Image {
        id: image

        y: (label.height + subLable.height - height) / 2
        width: Theme.iconSizeMedium
        height: Theme.iconSizeMedium
        fillMode: Image.PreserveAspectFit
        smooth: true

        source: coverSource

        BusyIndicator {
            anchors.centerIn: parent
            running: parent.status === Image.Loading
        }
    }

    Label {
        id: label
        anchors.left: image.right
        anchors.right: parent.right
        text: title
        color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
        elide: Text.ElideRight
    }
    Label {
        id: subLable
        anchors.left: image.right
        anchors.top: label.bottom
        anchors.right: parent.right
        text: subtitle
        font.pixelSize: Theme.fontSizeSmall
        color: listItem.highlighted ? Theme.highlightColor : Theme.secondaryColor
        elide: Text.ElideRight
    }
}
