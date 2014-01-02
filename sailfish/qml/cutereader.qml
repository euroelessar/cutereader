/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import org.qutim 0.3
import "pages"

ApplicationWindow
{
    id: application
    property Book book: rootBook

    function openBook(source) {
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


