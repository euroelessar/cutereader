/****************************************************************************
**
** CuteReader
**
** Copyright © 2013-2014 Ruslan Nigmatullin <euroelessar@yandex.ru>
**
*****************************************************************************
**
** $CUTEREADER_BEGIN_LICENSE$
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see http://www.gnu.org/licenses/.
** $CUTEREADER_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import Sailfish.Silica 1.0
import org.qutim 0.3

Page {
    id: page


    Config {
        id: styleConfig
        path: "text.base"
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
