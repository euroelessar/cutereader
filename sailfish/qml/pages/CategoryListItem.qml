import QtQuick 2.1
import Sailfish.Silica 1.0

ListItem {
    id: listItem
    contentHeight: Theme.itemSizeMedium // two line delegate
    menu: actions !== undefined && actions.length > 0 ? contextMenu : undefined
    x: Theme.paddingLarge
    width: page.width - 2 * Theme.paddingLarge

    signal actionClicked(variant data)

    Image {
        id: image

        property bool validSource: coverSource !== undefined
        property real rightOffset: validSource ? width + Theme.paddingSmall : 0

        y: (label.height + subLable.height - height) / 2
        width: validSource ? Theme.iconSizeMedium : 0
        height: validSource ? Theme.iconSizeMedium : 0
        fillMode: Image.PreserveAspectCrop
        smooth: true
        clip: true

        source: coverSource === undefined ? "" : coverSource

        BusyIndicator {
            anchors.centerIn: parent
            running: parent.validSource && parent.status === Image.Loading
        }
    }

    Label {
        id: label
        x: image.rightOffset
        width: parent.width - image.rightOffset

        text: title
        color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
        elide: Text.ElideRight
    }
    Label {
        id: subLable
        x: image.rightOffset
        y: label.height
        width: parent.width - image.rightOffset

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
