import QtQuick 2.1
import Sailfish.Silica 1.0
import org.qutim 0.3

Dialog {
    id: root

    property string name
    property color color

    SilicaFlickable {
        anchors.fill: parent

        Column {
            id: column
            width: parent.width

            DialogHeader {
                id: header
                acceptText: qsTr("Choose color")
            }

            Item {
                x: Theme.paddingLarge
                width: parent.width - 2 * Theme.paddingLarge
                height: Theme.itemSizeMedium

                Label {
                    id: label
                    y: (colorName.height / 2 - label.height) / 2

                    text: qsTr("#")
                }

                TextField {
                    id: colorName
                    x: label.width
                    height: parent.height
                    width: parent.width - x

                    validator: RegExpValidator {
                        regExp: /^[0-9a-f]{6}$/
                    }
                    color: errorHighlight ? "red" : Theme.primaryColor
                    inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhPreferLatin

                    onTextChanged: {
                        if (acceptableInput && chooser.color.toString().substr(1) !== text)
                            chooser.color = '#' + text
                    }
                }
            }

            Row {
                width: parent.width
                x: Theme.paddingLarge

            }

            ColorChooser {
                id: chooser

                x: Theme.paddingLarge
                width: parent.width - 2 * Theme.paddingLarge
                height: width

                onColorChanged: {
                    var name = color.toString().substr(1);
                    if (name !== colorName.text)
                        colorName.text = name;
                }
            }
        }
    }

    Component.onCompleted: {
        var hsv = chooser.color2hsv(root.color);
        chooser.hue = hsv.h;
        chooser.saturation = hsv.s;
        chooser.value = hsv.v;
    }
    onDone: root.color = chooser.color
}
