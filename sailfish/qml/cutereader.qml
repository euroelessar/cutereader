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

    property Component previousPageComponent: ColorPage {
        number: -1
    }
    property Component currentPageComponent: ColorPage {
        number: 0
    }
    property Component nextPageComponent: ColorPage {
        number: 1
    }

    Book {
        id: rootBook
        source: {
            console.log(Qt.resolvedUrl("../books/the_three_musketeers.fb2"))
            return Qt.resolvedUrl("../books/the_three_musketeers.fb2")
        }
    }

    property ColorPage previousPage
    property ColorPage currentPage
    property ColorPage nextPage

    initialPage: previousPageComponent

    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    Component.onCompleted: {
        previousPage = pageStack.currentPage
        currentPage = pageStack.pushAttached(currentPageComponent);
        pageStack.navigateForward(PageStackAction.Immediate);
        nextPage = pageStack.pushAttached(nextPageComponent);
    }

    Connections {
        target: pageStack
        onBusyChanged: {
            if (application.previousPage === undefined || application.nextPage === undefined || pageStack.busy)
                return;

            var number = application.currentPage.number;

            if (pageStack.currentPage === application.previousPage) {
                number--;
                application.currentPage.number = number;
                application.nextPage.number = number + 1;

                pageStack.navigateForward(PageStackAction.Immediate);

                application.previousPage.number = number - 1;
            } else if (pageStack.currentPage === application.nextPage) {
                number++;
                application.currentPage.number = number;
                application.previousPage.number = number - 1;

                pageStack.navigateBack(PageStackAction.Immediate);

                application.nextPage.number = number + 1;
            }
        }

        onCurrentPageChanged: {
        }
    }
}


