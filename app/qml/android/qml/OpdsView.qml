import QtQuick 2.0
import QtQuick.XmlListModel 2.0
import org.qutim 0.3

ListView {
    id: root
    property alias source: opdsModel.source

    signal linkRequested(url source)
    signal bookRequested(variant opdsEntry)

    model: OpdsBookModel {
        id: opdsModel
    }
    delegate: ItemDelegate {
        id: item
        width: parent.width
        title: model.title
        description: model.subtitle
        imageSource: model.cover

        onClicked: {
            if (isBook)
                root.bookRequested(opdsEntry)
            else
                root.linkRequested(source)
        }
    }
}
