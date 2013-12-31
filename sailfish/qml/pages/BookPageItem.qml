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

    property variant positionValue
//    property alias positionValue: bookPage.positionValue

    Config {
        id: config
        path: "books." + Qt.md5(application.book.source.toString())

        property string positionValue
    }

    onPositionValueChanged: config.positionValue = JSON.stringify(positionValue)

    readonly property real themePaddingLarge: Theme.paddingLarge
    readonly property real pageWidth: pagesView.width - 2 * themePaddingLarge
    readonly property real pageHeight: pagesView.height - 2 * themePaddingLarge

    BookPage {
        id: previousPageHelper

        width: root.pageWidth
        height: root.pageHeight
        book: application.book

        positionValue: currentPageHelper.previousPage

        visible: false
        enabled: false
    }

    BookPage {
        id: currentPageHelper

        width: root.pageWidth
        height: root.pageHeight
        book: application.book

        visible: false
        enabled: false
    }

    BookPage {
        id: nextPageHelper

        width: root.pageWidth
        height: root.pageHeight
        book: application.book

        positionValue: currentPageHelper.nextPage

        visible: false
        enabled: false
    }

    readonly property variant positions: {
        var result = [
            currentPageHelper.previousPage.block === undefined ? undefined : previousPageHelper.previousPage,
            currentPageHelper.previousPage,
            currentPageHelper.positionValue,
            currentPageHelper.nextPage,
            currentPageHelper.nextPage.block === undefined ? undefined : nextPageHelper.nextPage
        ];

        return result
            .filter(function (arg) { return arg !== undefined && arg.block !== undefined; })
            .map(function (arg) { return { positionValue: arg }; });
    }
    readonly property int currentPosition: {
        var result = [
            currentPageHelper.previousPage.block === undefined ? {} : previousPageHelper.previousPage,
            currentPageHelper.previousPage
        ];
        return 2 - (result[0].block === undefined ? 1 : 0) - (result[1].block === undefined ? 1 : 0);
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
                                              title: application.book.title,
                                              data: application.book.bookData
                                          })
            }
        }

        SilicaListView {
            id: pagesView
//            anchors.fill: parent
            width: parent.width
            height: parent.height
            orientation: Qt.Horizontal
            snapMode: PathView.SnapOneItem
            model: root.positions

            onMovementEnded: currentPage = indexAt(contentX, contentY)

            property int currentPage: -2
            property bool insideThePageChange: false

            onCurrentPageChanged: {
                if (insideThePageChange)
                    return;
                insideThePageChange = true;

                currentPageHelper.positionValue = root.positions[currentPage].positionValue;
                contentX = root.currentPosition * width;
                currentPage = root.currentPosition;

                insideThePageChange = false;
            }

            delegate: Item {
                width: pagesView.width
                height: pagesView.height

                BookPage {
                    anchors.margins: Theme.paddingLarge
                    anchors.fill: parent
                    book: application.book
                    positionValue: modelData.positionValue
                }
            }

    //        Rectangle {
    //            width: page.width
    //            height: page.height
    //            color: "#ffffff"

    //            MouseArea {
    //                anchors.fill: parent

    //                onClicked: {
    //                    if (mouseX < width / 2) {
    //                        if (bookPage.previousPage.block !== undefined)
    //                            bookPage.positionValue = bookPage.previousPage;
    //                    } else {
    //                        if (bookPage.nextPage.block !== undefined)
    //                            bookPage.positionValue = bookPage.nextPage;
    //                    }
    //                }
    //            }

    //            BookPage {
    //                id: bookPage
    //                anchors.fill: parent
    //                anchors.margins: 5
    //                book: application.book
    //                positionValue: config.positionValue.length == 0 ? {} : JSON.parse(config.positionValue)
    //            }
    //        }
        }
    }
}


