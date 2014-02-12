import QtQuick 2.1
import org.qutim 0.3

FocusScope {
    id: root
    property Book book
    property alias positionValue: currentPageHelper.positionValue

    signal linkClicked(variant linkPosition)
    focus: true

    PseudoBookPage {
        id: currentPageHelper
        anchors.fill: parent
        anchors.margins: 15
        book: root.book

        onPositionCalculationReady: {
            if (root.previousPositionId === calculationId) {
                if (position.block === undefined) {
                    root.previousPositionId = -2;
                    return;
                }

                var horizontalVelocity = pagesView.horizontalVelocity;

                pagesView.contentX += pagesView.width;
                pagesModel.insert(0, { positionValue: position });

                root.previousPositionId = -1;

                pagesView.flick(horizontalVelocity, 0);
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
    Component.onCompleted: rebuildModel(4)

    Connections {
        target: root.book
        onStyleChanged: root.rebuildModel(3)
    }

    property int previousPositionId: -1
    property int nextPositionId: -1

    function rebuildModel(type) {
        currentPageHelper.dismiss(nextPositionId);
        currentPageHelper.dismiss(previousPositionId);

        nextPositionId = -3;
        previousPositionId = -3;

        pagesModel.clear();
        pagesModel.append({ positionValue: currentPageHelper.positionValue });

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

    ListModel {
        id: pagesModel
    }

    ListView {
        id: pagesView
        anchors.fill: parent
        orientation: Qt.Horizontal
        snapMode: PathView.SnapOneItem
        clip: true
        model: pagesModel
        pixelAligned: true

        onContentXChanged: {
            function index() {
                return Math.floor((contentX - originX) / width);
            }

            while (pagesModel.count > 0 && index() >= pagesModel.count - 2 && root.appendItem());
            while (pagesModel.count > 0 && index() === 0 && root.prependItem());
        }

        delegate: Rectangle {
            width: pagesView.width
            height: pagesView.height
            color: 'white'

            property alias positionValue: listPage.positionValue

            BookPage {
                id: listPage
                anchors.margins: 15
                anchors.fill: parent
                book: root.book
                positionValue: model.positionValue
            }
        }
    }
}
