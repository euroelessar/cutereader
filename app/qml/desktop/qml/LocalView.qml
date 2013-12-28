import QtQuick 2.1
import org.qutim 0.3
import QtQuick.Controls 1.0

Item {
    id: root

    property alias source: bookCollection.baseDir
    property Book book

    signal bookRequested(url source)

    function openView(entry) {
        stackView.push({
            item: libraryListView,
            properties: {
                data: entry
            }
        })
    }

    function openBookView(entry) {
        stackView.push({
            item: opdsBookView,
            properties: { opdsEntry: entry }
        })
    }

    function openBook(entry) {
        root.bookRequested(entry)
    }

    function downloadBook(entry) {
        console.log("Download request:", JSON.stringify(entry), entry.toString())
    }

    function processAction(type, entry) {
        switch (type) {
        case OpdsEntry.OpenExternally:
            Qt.openUrlExternally(entry)
            break
        case OpdsEntry.OpenView:
            root.openView(entry)
            break
        case OpdsEntry.OpenBookView:
            root.openBookView(entry)
            break
        case OpdsEntry.OpenBook:
            root.openBook(entry)
            break
        case OpdsEntry.Download:
            root.downloadBook(entry)
            break
        }
    }

    LocalBookCollection {
        id: bookCollection
    }

    Component {
        id: opdsBookView

        OpdsBookView {
            onActionTriggered: root.processAction(type, entry)
        }
    }

    Component {
        id: libraryListView
        ScrollView {
            property alias data: frontModel.data

            ListView {
                model: FrontModel {
                    id: frontModel
                    collection: bookCollection
                }

                delegate: CategoryDelegate {
                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton

                        onClicked: {
                            if (mouse.button == Qt.RightButton) {
                                if (contextMenu.items.length > 0)
                                    contextMenu.popup()
                            } else if (isBook) {
                                root.openBookView(entryData)
                            } else {
                                root.openView(entryData)
                            }
                        }
                    }

                    Menu {
                        id: contextMenu
                        Instantiator {
                            model: actions
                            MenuItem {
                                text: modelData.title

                                onTriggered: root.processAction(modelData.type, modelData.entry)
                            }
                            onObjectAdded: contextMenu.insertItem(index, object)
                            onObjectRemoved: contextMenu.removeItem(object)
                        }
                    }
                }
            }
        }
    }

    StackView {
        id: stackView

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: backButton.top
        }

        initialItem: libraryListView
    }
    Button {
        id: backButton

        anchors {
            left: parent.left
            bottom: parent.bottom
        }

        text: "Back"
        enabled: stackView.depth > 1
        onClicked: stackView.pop()
    }
}
