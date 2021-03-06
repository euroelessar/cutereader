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

LibraryPageBase {
    id: page

    property alias data: frontModel.data

    SilicaListView {
        anchors.fill: parent

        header: Column {
            width: parent ? parent.width : Screen.width

            PageHeader {
                id: pageHeader
                title: page.title
            }
            SearchField {
                id: searchField
                width: parent.width
                placeholderText: qsTr("Search")
                onTextChanged: {
                    console.log('Filter changed:', text);
                    frontModel.filter = text;
                }
            }
        }

        VerticalScrollDecorator {}

        PushUpMenu {
            busy: frontModel.busy
            visible: frontModel.hasNextPage

            MenuItem {
                text: "Load more"
                onClicked: frontModel.loadNext()
            }
        }

        section {
            property: 'section'
            delegate: SectionHeader {
                text: section
            }
        }

        model: FrontModel {
            id: frontModel
            collection: page.collection
        }

        delegate: CategoryListItem {
            id: listItem

            onClicked: {
                if (isBook)
                    page.openBookView(entryData)
                else
                    page.openView(title, entryData)
            }
            onActionClicked: page.processAction(data)
        }
    }
}
