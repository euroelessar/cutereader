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

ComboBox {
    id: comboBox

    width: parent === null ? 0 : parent.width

    property variant values
    property Config config
    property string configName

    menu: ContextMenu {
        Repeater {
            id: fontFamilyRepeater
            model: comboBox.values

            MenuItem {
                text: modelData.title
            }
        }
    }

    property bool inited: false

    Component.onCompleted: {
        var allValues = fontFamilyRepeater.model;

        var value = config.value(configName);
        if (value === undefined) {
            currentIndex = 0;
        } else {
            for (var i = 0; i < allValues.length; ++i) {
                if (allValues[i].value === value) {
                    currentIndex = i;
                    break;
                }
            }
        }

        inited = true;
    }

    onCurrentIndexChanged: {
        if (inited)
            config.setValue(configName, values[currentIndex].value);
    }
}
