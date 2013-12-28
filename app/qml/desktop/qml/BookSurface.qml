import QtQuick 2.1
import org.qutim 0.3

FocusScope {
    id: root
    property Book book
    property alias positionValue: firstPage.positionValue

    signal linkClicked(variant linkPosition)
    focus: true

    Config {
        id: config
        path: "books." + Qt.md5(root.book.source.toString())

        property string positionValue
    }

    onPositionValueChanged: config.positionValue = JSON.stringify(positionValue)

    Rectangle {
        id: firstPageItem
        height: parent.height
        width: parent.width
        border.width: 1
        z: 1

        focus: true
        activeFocusOnTab: true
        Keys.onRightPressed: firstPage.positionValue = firstPage.nextPage
        Keys.onLeftPressed: firstPage.positionValue = firstPage.previousPage

        BookPage {
            id: firstPage
            book: root.book
            anchors.fill: parent
            anchors.margins: 5
            onLinkClicked: root.linkClicked(linkPosition)
            positionValue: config.positionValue.length == 0 ? {} : JSON.parse(config.positionValue)
        }

        Behavior on x {
            id: firstPageXBehavior
            PropertyAnimation {
                id: firstPageXAnimation
                duration: 150
                easing.type: Easing.Linear
            }
        }
    }

    Rectangle {
        id: secondPageItem
        height: parent.height
        width: parent.width
        border.width: 1

        BookPage {
            id: secondPage
            book: root.book
            anchors.fill: parent
            anchors.margins: 5
            positionValue: firstPageItem.x > 0 ? firstPage.previousPage : firstPage.nextPage
        }
    }

    MouseArea {
        id: dragMouseArea
        acceptedButtons: Qt.LeftButton
        preventStealing: true
        anchors.fill: parent
        property real startX: 0
        property real previousX: 0
        property real previousTime: 0
        property real newPositionX: 0
        property real minX: 0
        property real maxX: 0
        readonly property real deltaX: Math.max(minX, Math.min(startX - mouseX, maxX))
        signal pageChanged

        onPressed: {
            newPositionX = 0
            startX = mouseX
            previousX = mouseX
            previousTime = new Date().getTime()

            var previousResult = firstPage.previousPage;
            if (previousResult.block !== undefined)
                minX = -parent.width
            else
                minX = 0

            var nextResult = firstPage.nextPage;
            if (nextResult.block !== undefined)
                maxX = parent.width
            else
                maxX = 0

//                        console.log(JSON.stringify([previousResult, nextResult]))
        }
        onReleased: {
            var distance = newPositionX - firstPageItem.x
            var animationTime = firstPageXAnimation.duration - Math.min(new Date().getTime() - previousTime, firstPageXAnimation.duration)

            if (Math.abs(deltaX) > parent.width / 2
                    || (distance != 0 && Math.abs(distance * 1000 / animationTime / parent.width) > 1)) {
                var resultX = -deltaX * parent.width / Math.abs(deltaX)
                firstPageItem.x = resultX

                if (firstPageItem.x == resultX) {
                    dragMouseArea.pageChanged()
                }
            } else {
                firstPageItem.x = 0
            }
        }
        onDeltaXChanged: {
            firstPageItem.x = -deltaX
            newPositionX = -deltaX

            var time = new Date().getTime();

            previousX = mouseX
            previousTime = time
        }
        onPageChanged: {
            firstPage.positionValue = secondPage.positionValue
            firstPageXBehavior.enabled = false
            firstPageItem.x = 0
            firstPageXBehavior.enabled = true
        }
        Connections {
            target: firstPageItem
            onXChanged: {
                if (!dragMouseArea.pressed && Math.abs(firstPageItem.x) == root.width) {
                    dragMouseArea.pageChanged()
                }
            }
        }
    }
}
