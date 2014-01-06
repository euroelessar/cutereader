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
        color: listItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
        elide: Text.ElideRight
    }
}
