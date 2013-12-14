import QtQuick 2.0
import org.qutim 0.3
import Sailfish.Silica 1.0

BaseBookPage {
    id: page
    linkDelegate: Rectangle {
        property alias href: mouseArea.href
        border.color: '#ff0000'
        color: '#00000000'

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            property string href

            onClicked: {
                if (parent.href.charAt(0) === '#') {
                    parent.parent.positionValue = parent.parent.positionForId(parent.href.substring(1));
                } else {
                    Qt.openUrlExternally(parent.href);
                }
            }
        }
    }
    imageDelegate: Image {
        id: image

        BusyIndicator {
            anchors.centerIn: parent
            running: parent.status === Image.Loading
        }
    }
}
