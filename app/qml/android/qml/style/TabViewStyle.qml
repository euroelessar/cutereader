import QtQuick 2.1

import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1 as Style

Style.TabViewStyle {
    tabsAlignment: Qt.AlignVCenter
    tabOverlap: 0
    frame: Item { }
    tab: Item {
        implicitWidth: control.width/control.count
        implicitHeight: 50
        BorderImage {
            anchors.fill: parent
            border.bottom: 8
            border.top: 8
            source: styleData.selected ? "images/tab_selected.png" : "images/tabs_standard.png"
            Label {
                anchors.centerIn: parent
                color: "white"
                text: styleData.title.toUpperCase()
                font.pointSize: 16
            }
            Rectangle {
                visible: index > 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 10
                width:1
                color: "#3a3a3a"
            }
        }
    }
}
