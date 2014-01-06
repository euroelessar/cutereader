/****************************************************************************
**
** CuteReader
**
** Copyright © 2013-2014 Ruslan Nigmatullin <euroelessar@yandex.ru>
**
*****************************************************************************
**
** $CUTEREADER_BEGIN_LICENSE$
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see http://www.gnu.org/licenses/.
** $CUTEREADER_END_LICENSE$
**
****************************************************************************/

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

        text: title === undefined ? "" : title
        color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
        elide: Text.ElideRight
    }
    Label {
        id: subLable
        x: image.rightOffset
        y: label.height
        width: parent.width - image.rightOffset

        text: subtitle === undefined ? "" : subtitle
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
