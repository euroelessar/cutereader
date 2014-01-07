import QtQuick 2.1
import org.qutim 0.3

FocusScope {
    id: root
    property Book book
    property alias positionValue: currentPageHelper.positionValue
    readonly property string title: book.info.title

    signal linkClicked(variant linkPosition)
    focus: true

    PseudoBookPage {
        id: currentPageHelper
        anchors.fill: parent
        anchors.margins: 15
        book: root.book
    }

    onWidthChanged: rebuildModel(1)
    onHeightChanged: rebuildModel(2)
    Component.onCompleted: rebuildModel(4)

    Connections {
        target: root.book
        onStyleChanged: root.rebuildModel(3)
    }

    function rebuildModel(type) {
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
        var position = pagesModel.get(pagesModel.count - 1).positionValue;
        var newPosition = currentPageHelper.nextPageForPosition(position);
        if (newPosition.block === undefined)
            return false;

        pagesModel.append({ positionValue: newPosition })
        return true;
    }

    function prependItem() {
        var position = pagesModel.get(0).positionValue;
        var newPosition = currentPageHelper.previousPageForPosition(position);
        if (newPosition.block === undefined)
            return false;

        console.log('prependItem', pagesView.contentX, pagesView.originX, Math.floor((pagesView.contentX - pagesView.originX) / width))
        pagesView.contentX += pagesView.width;
        pagesModel.insert(0, { positionValue: newPosition });
        return true;
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
