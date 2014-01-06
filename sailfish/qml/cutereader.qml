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
import "pages"

ApplicationWindow
{
    id: application
    property Book book: rootBook

    function openBook(source) {
        if (book.source === source)
            return;

        book.source = source;
        genericConfig.book = source;

        var pages = [];

        var positions = book.positions;
        if (positions.length === 0) {
            positions = [{
                body: 0,
                block: 0,
                position: 0
            }]
        }

        for (var i = 0; i < positions.length; ++i) {
            pages.push({
                page: bookPageComponent,
                properties: { positionValue: positions[i], positionIndex: i }
            })
        }

        pageStack.replaceAbove(null, pages);
    }

    function clickLink(positionValue) {
        pageStack.push(bookPageComponent, { positionValue: positionValue, positionIndex: pageStack.depth })
        updatePositions();
    }

    function getPositions() {
        var positions = [];

        pageStack.find(function (page) {
            if (page.positionIndex !== undefined && page.positionIndex >= 0)
                positions.push(page.positionValue)
        })

        positions.reverse();
        return positions;
    }

    function updatePositions() {
        rootBook.positions = getPositions();
    }

    Config {
        id: genericConfig
        path: "generic"

        property url book: Qt.resolvedUrl("../books/the_three_musketeers.fb2")
    }

    Book {
        id: rootBook

        style.base.foreground: Theme.primaryColor
        style.base.fontPixelSize: Theme.fontSizeMedium
        style.base.fontFamily: Theme.fontFamily
        style.title.fontPixelSize: Theme.fontSizeLarge
        style.title.fontFamily: Theme.fontFamilyHeading
        style.title.foreground: Theme.highlightColor
        style.internalAnchor.foreground: Theme.highlightColor
        style.noteAnchor.foreground: Theme.highlightColor
        style.externalAnchor.foreground: Theme.highlightColor
    }

    Component {
        id: bookPageComponent

        BookPageItem {
            id: page

            onPositionValueChanged: updatePositions()
        }
    }

    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    Component.onCompleted: openBook(genericConfig.book)
}


