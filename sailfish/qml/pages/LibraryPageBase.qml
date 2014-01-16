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

    property string title
    property LocalBookCollection collection

    function openView(title, entry) {
        pageStack.push(Qt.resolvedUrl("LibraryListView.qml"),
                       {
                           title: title,
                           data: entry,
                           collection: page.collection
                       })
    }

    function openBookView(entry) {
        pageStack.push(Qt.resolvedUrl("LibraryBookView.qml"),
                       {
                           title: title,
                           data: entry,
                           collection: page.collection
                       })
    }

    function openBook(entry) {
        application.openBook(entry);
    }

    function downloadBook(entry) {
        console.log("Download request:", JSON.stringify(entry), entry.toString())

        application.downloads.download(entry);
    }

    function processAction(data) {
        switch (data.type) {
        case OpdsEntry.OpenExternally:
            Qt.openUrlExternally(data.entry)
            break
        case OpdsEntry.OpenView:
            page.openView(data.title, data.entry)
            break
        case OpdsEntry.OpenBookView:
            page.openBookView(data.entry)
            break
        case OpdsEntry.OpenBook:
            page.openBook(data.entry)
            break
        case OpdsEntry.Download:
            page.downloadBook(data.entry)
            break
        }
    }
}
