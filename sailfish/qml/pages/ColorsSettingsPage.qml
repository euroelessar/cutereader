import QtQuick 2.0
import Sailfish.Silica 1.0
import org.qutim 0.3

Page {
    id: page

    Config {
        id: styleConfig
        path: "style.default"
    }

    SilicaListView {
        id: listView
        anchors.fill: parent

        header: PageHeader {
            id: pageHeader
            title: qsTr("Color settings")
        }

        model: ListModel {
            id: listModel
        }

        delegate: ListItem {
            id: listItem
            contentHeight: Theme.itemSizeSmall
            x: Theme.paddingLarge
            width: page.width - 2 * Theme.paddingLarge

            Rectangle {
                width: label.height
                height: label.height
                color: model.color
            }

            Label {
                id: label
                x: label.height + Theme.paddingSmall

                text: model.title
                color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

            onClicked: pageStack.push(colorPicker, {
                                          name: model.name,
                                          color: model.color,
                                          index: model.index
                                      })
        }

        Component.onCompleted: {
            function addItem(name, title, url) {
                var color = styleConfig.value(name);
                listModel.append({
                    name: name,
                    title: title,
                    color: color.toString(),
                    index: listModel.count
                })
            }

            addItem("base.foreground", qsTr("Base text"))
            addItem("title.foreground", qsTr("Title"))
            addItem("internalAnchor.foreground", qsTr("Internal anchor"))
            addItem("noteAnchor.foreground", qsTr("Note anchor"))
            addItem("externalAnchor.foreground", qsTr("External anchor"))
        }
    }

    Component {
        id: colorPicker

        ColorChooserDialog {
            id: dialog

            property int index: -1

            onAccepted: {
                styleConfig.setValue(name, color);

                var data = listModel.get(index);
                data.color = color.toString();
                listModel.set(index, data);
            }
        }
    }
}
