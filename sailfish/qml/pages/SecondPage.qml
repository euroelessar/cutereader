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


Page {
    id: page
    Drawer {
        id: drawer

        open: true

        background: Rectangle {
            anchors.fill: parent
            color: "#ff0000"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: drawer.open = !drawer.open
        }

        Rectangle {
            color: "#00ff00"

            width: page.width
            height: page.height
        }
    }
//    SilicaListView {
//        id: listView
//        model: 20
//        anchors.fill: parent
//        header: PageHeader {
//            title: "Nested Page"
//        }
//        delegate: BackgroundItem {
//            id: delegate

//            Label {
//                x: Theme.paddingLarge
//                text: "Item " + index
//                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
//            }
//            onClicked: console.log("Clicked " + index)
//        }
//    }
}





