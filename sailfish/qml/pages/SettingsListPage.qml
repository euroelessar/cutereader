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

    SilicaListView {
        id: listView
        anchors.fill: parent

        header: PageHeader {
            id: pageHeader
            title: qsTr("Settings")
        }

        model: ListModel {
            id: listModel
        }

        delegate: ListItem {
            id: listItem
            contentHeight: Theme.itemSizeMedium
            x: Theme.paddingLarge
            width: page.width - 2 * Theme.paddingLarge

            Label {
                id: label
                text: model.title
                color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
            }
            Label {
                anchors.top: label.bottom
                text: model.subtitle
                font.pixelSize: Theme.fontSizeSmall
                color: listItem.highlighted ? Theme.highlightColor : Theme.secondaryColor
            }

            onClicked: pageStack.push(url)
        }

        Component.onCompleted: {
            function addItem(title, subtitle, url) {
                listModel.append({
                    title: title,
                    subtitle: subtitle,
                    url: url
                })
            }

            addItem(qsTr("Texts"), qsTr("Font size, borders, etc"), Qt.resolvedUrl("FontSettingsPage.qml"))
            addItem(qsTr("Colors"), qsTr("Background and foreground colors"), Qt.resolvedUrl("ColorsSettingsPage.qml"))
        }
    }
}
