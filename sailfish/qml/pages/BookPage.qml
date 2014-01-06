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
import org.qutim 0.3
import Sailfish.Silica 1.0

BaseBookPage {
    id: page
    linkDelegate: Rectangle {
        property alias href: mouseArea.href
        border.color: '#ff0000'
        color: '#00000000'

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            property string href

            onClicked: {
                if (parent.href.charAt(0) === '#') {
                    var page = parent.parent;
                    application.clickLink(page.positionForId(parent.href.substring(1)));
                } else {
                    Qt.openUrlExternally(parent.href);
                }
            }
        }
    }
    imageDelegate: Image {
        id: image

        BusyIndicator {
            anchors.centerIn: parent
            running: parent.status === Image.Loading
        }
    }
}
