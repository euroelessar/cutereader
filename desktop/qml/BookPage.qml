import QtQuick 2.0
import org.qutim 0.3

BaseBookPage {
    id: page

    signal linkClicked(variant linkPosition)

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
                    var page = parent.parent;
                    page.linkClicked(page.positionForId(parent.href.substring(1)));
                } else {
                    Qt.openUrlExternally(parent.href);
                }
            }
        }
    }
}
