import QtQuick 2.1
import org.qutim 0.3

FocusScope {
    id: root
    property Book book
//    property variant positionValue
    property alias positionValue: currentPageHelper.positionValue

    signal linkClicked(variant linkPosition)
    focus: true

    Config {
        id: config
        path: "books." + Qt.md5(root.book.source.toString())

        property string positionValue
    }

    onPositionValueChanged: config.positionValue = JSON.stringify(positionValue)

    readonly property real themePaddingLarge: 15
    readonly property real pageWidth: pagesView.width - 2 * themePaddingLarge
    readonly property real pageHeight: pagesView.height - 2 * themePaddingLarge

    BookPage {
        id: previousPageHelper

        width: root.pageWidth
        height: root.pageHeight
        book: root.book

        positionValue: currentPageHelper.previousPage

        visible: false
        enabled: false
    }

    BookPage {
        id: currentPageHelper

        width: root.pageWidth
        height: root.pageHeight
        book: root.book

        visible: false
        enabled: false
    }

    BookPage {
        id: nextPageHelper

        width: root.pageWidth
        height: root.pageHeight
        book: root.book

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

    ListView {
        id: pagesView
        anchors.fill: parent
        orientation: Qt.Horizontal
        snapMode: PathView.SnapOneItem
        clip: true
        model: root.positions

        delegate: Rectangle {
            width: pagesView.width
            height: pagesView.height
            color: 'white'

            property alias positionValue: listPage.positionValue

            BookPage {
                id: listPage
                anchors.margins: root.themePaddingLarge
                anchors.fill: parent
                book: root.book
                positionValue: modelData.positionValue
            }
        }

        function recalcCurrentPage() {
            currentPage = indexAt(contentX, contentY);
        }

        onMovementEnded: recalcCurrentPage()

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
    }
}
