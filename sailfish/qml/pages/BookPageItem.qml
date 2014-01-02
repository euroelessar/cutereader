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

Page {
    id: root
    property Book book: application.book
    property alias positionValue: currentPageHelper.positionValue

    signal linkClicked(variant linkPosition)
    focus: true

    PseudoBookPage {
        id: currentPageHelper
        anchors.fill: parent
        anchors.margins: Theme.paddingLarge
        book: root.book

        onPositionCalculationReady: {
            if (root.previousPositionId === calculationId) {
                if (position.block === undefined) {
                    root.previousPositionId = -2;
                    return;
                }

                pagesView.contentX += pagesView.width;
                pagesModel.insert(0, { positionValue: position });

                root.previousPositionId = -1;
            }
            if (root.nextPositionId === calculationId) {
                if (position.block === undefined) {
                    root.nextPositionId = -2;
                    return;
                }

                console.log('calced next page', JSON.stringify(position))
                pagesModel.append({ positionValue: position });

                root.nextPositionId = -1;
            }
        }
    }

    onWidthChanged: rebuildModel(1)
    onHeightChanged: rebuildModel(2)
    Component.onCompleted: rebuildModel(3)

    Connections {
        target: root.book
        onStyleChanged: root.rebuildModel(4)
        onSourceChanged: root.rebuildModel(5)
        onStateChanged: root.rebuildModel(6)
    }

    property int previousPositionId: -1
    property int nextPositionId: -1

    function rebuildModel(type) {
        nextPositionId = -1;
        previousPositionId = -1;

        console.log('rebuildModel', type)
        pagesModel.clear()

        var positions = new Array(5);
        var i;
        var positionIndex = 2;

        positions[2] = currentPageHelper.positionValue;
        for (i = 1; i >= 0; --i) {
            positions[i] = currentPageHelper.previousPageForPosition(positions[i + 1]);
            if (positions[i].block === undefined)
                --positionIndex;
        }
        for (i = 3; i < 5; ++i)
            positions[i] = currentPageHelper.nextPageForPosition(positions[i - 1]);

        positions = positions.filter(function (a) { return a.block !== undefined });

        for (i = 0; i < positions.length; ++i)
            pagesModel.append({ positionValue: positions[i] })

        pagesView.contentX = positionIndex * pagesView.width;
        console.log(JSON.stringify(positions))
    }

    function appendItem() {
        if (nextPositionId !== -1)
            return;

        var position = pagesModel.get(pagesModel.count - 1).positionValue;
        nextPositionId = currentPageHelper.calculateNextPage(position);
    }

    function prependItem() {
        if (nextPositionId !== -1)
            return;

        var position = pagesModel.get(0).positionValue;
        previousPositionId = currentPageHelper.calculatePreviousPage(position);
    }

    SilicaFlickable {
        id: flickable
        anchors.fill: parent
        contentWidth: width

        PullDownMenu {
            MenuItem {
                text: qsTr("Library")
                onClicked: pageStack.push(Qt.resolvedUrl("LocalView.qml"))
            }
            MenuItem {
                text: qsTr("About the book")
                onClicked: pageStack.push(Qt.resolvedUrl("LibraryBookView.qml"),
                                          {
                                              title: root.book.title,
                                              data: root.book.bookData
                                          })
            }
        }

        SilicaListView {
            id: pagesView
            width: parent.width
            height: parent.height
            orientation: Qt.Horizontal
            snapMode: PathView.SnapOneItem
            model: ListModel {
                id: pagesModel
            }

            property int contentXBy10: Math.floor(contentX * 2 / width)
            property int index: Math.floor((contentX - originX) / width)

            onContentXBy10Changed: {
                if (pagesModel.count > 0 && index >= pagesModel.count - 2)
                    root.appendItem();
                if (pagesModel.count > 0 && index === 0)
                    root.prependItem();
            }

            delegate: Item {
                width: pagesView.width
                height: pagesView.height

                BookPage {
                    anchors.margins: Theme.paddingLarge
                    anchors.fill: parent
                    book: root.book
                    positionValue: model.positionValue
                }
            }
        }
    }
}


