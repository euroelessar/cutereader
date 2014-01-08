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
    id: root
    property Book book: application.book
    property int positionIndex: -1
    property alias positionValue: currentPageHelper.positionValue

    property bool __book_page: true

    signal linkClicked(variant linkPosition)
    focus: true

    Keys.onVolumeUpPressed: pagesView.flick(pagesView.width, 0)
    Keys.onVolumeDownPressed: pagesView.flick(-pagesView.width, 0)

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
        onTextSettingsChanged: root.rebuildModel(4)
        onSourceChanged: root.rebuildModel(5)
        onStateChanged: root.rebuildModel(6)
    }

    property int previousPositionId: -1
    property int nextPositionId: -1

    function rebuildModel(type) {
        nextPositionId = -3;
        previousPositionId = -3;

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

        nextPositionId = -1;
        previousPositionId = -1;
    }

    function appendItem() {
        if (nextPositionId !== -1)
            return;

        var position = pagesModel.get(pagesModel.count - 1).positionValue;
        nextPositionId = currentPageHelper.calculateNextPage(position);
    }

    function prependItem() {
        if (previousPositionId !== -1)
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
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsListPage.qml"))
            }
            MenuItem {
                text: qsTr("Library")
                onClicked: pageStack.push(Qt.resolvedUrl("LocalView.qml"))
            }
            MenuItem {
                visible: root.book.contents.title.length !== 0
                text: qsTr("Contents")
                onClicked: pageStack.push(Qt.resolvedUrl("ContentsListPage.qml"), {
                                              contents: root.book.contents
                                          })
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
            pixelAligned: true
            model: ListModel {
                id: pagesModel
            }

            property int contentXBy10: Math.floor(contentX * 2 / width)
            property int index: Math.floor((contentX - originX) / width)

            onContentXBy10Changed: {
                if (pagesModel.count > 0 && index >= pagesModel.count - 2)
                    root.appendItem();
                if (pagesModel.count > 0 && index <= 2)
                    root.prependItem();
            }

            onMovementEnded: root.positionValue = pagesModel.get(index).positionValue

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


