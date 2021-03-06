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
import org.qutim 0.3

Page {
    id: page

    property variant contents
    property string title: contents.title

    SilicaListView {
        id: listView
        anchors.fill: parent

        header: PageHeader {
            id: pageHeader
            title: page.title
        }

        model: contents.children

        delegate: ListItem {
            id: listItem
            contentHeight: Theme.itemSizeSmall
            x: Theme.paddingLarge
            width: page.width - 2 * Theme.paddingLarge

            Label {
                id: label
                text: modelData.title
                color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

            onClicked: {
                if (modelData.children.length === 0) {
                    application.clickLink(modelData.position);
                } else {
                    pageStack.push(Qt.resolvedUrl("ContentsListPage.qml"), {
                                       contents: modelData
                                   })
                }
            }
        }
    }
}
