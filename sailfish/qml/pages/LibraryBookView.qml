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

        VerticalScrollDecorator {}

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
}
