import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    SilicaFlickable {
        anchors.fill: parent

        Column {
            width: parent.width

            ComboBox {
                width: parent.width

                label: qsTr("Font family")

                menu: ContextMenu {
                    MenuItem { text: "Comic Sans"; font.family: text }
                    MenuItem { text: "Arial"; font.family: text }
                    MenuItem { text: "Times New Roman"; font.family: text }
                }
            }
        }
    }
}
