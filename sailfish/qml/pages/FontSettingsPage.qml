import QtQuick 2.0
import Sailfish.Silica 1.0
import org.qutim 0.3

Page {
    id: page


    Config {
        id: styleConfig
        path: "style.default.base"
    }

    SilicaFlickable {
        anchors.fill: parent

        Column {
            width: parent.width

            PageHeader {
                title: qsTr("Font settings")
            }

            ComboBox {
                x: Theme.paddingLarge
                width: parent.width - 2 * Theme.paddingLarge

                label: qsTr("Font family")
                currentIndex: {
                    var family = styleConfig.value("fontFamily")
                    if (family === undefined)
                        return -1;
                    return fontModel.families().indexOf(family)
                }

                menu: ContextMenu {
                    Repeater {
                        model: FontListModel {
                            id: fontModel
                        }

                        MenuItem {
                            text: model.display
                        }
                    }
                }
            }

            TextField {
                x: Theme.paddingLarge
                width: parent.width - 2 * Theme.paddingLarge

                placeholderText: "qweqweqwe"
            }

            ComboBox {
                x: Theme.paddingLarge
                width: parent.width - 2 * Theme.paddingLarge

                label: qsTr("Font size")
                currentIndex: {
                    var size = styleConfig.value("fontPixelSize");
                    if (size === undefined)
                        return -1;
                    return fontModel.standardSizes().indexOf(size);
                }

                menu: ContextMenu {
                    Repeater {
                        model: fontModel.standardSizes()

                        MenuItem {
                            text: modelData
                        }
                    }
                }
            }
        }
    }
}
