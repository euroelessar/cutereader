import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.1
import org.qutim 0.3
import "style" as Android

ApplicationWindow {
    id: application
    property alias stackView: stackView

    width: 768
    height: 1280
    visible: true
    title: stackView.title

    menuBar: MenuBar {
        Menu {
            title: qsTr("Menu")
            MenuItem {
                text: qsTr("Settings")
            }
            MenuItem {
                onTriggered: stackView.push(collectionView)
                text: qsTr("Collection")
            }
            MenuItem {
                text: qsTr("Bookmarks")
            }
        }
    }

    Rectangle {
        color: "#212126"
        anchors.fill: parent
        focus: true
        // Implements back key navigation
        Keys.onReleased: {
            if (event.key === Qt.Key_Back) {
                if (stackView.depth > 1) {
                    stackView.pop();
                    event.accepted = true;
                }
            }
        }
    }
    Config {
        id: genericConfig
        path: "generic"

        property url book
    }

    Config {
        id: styleConfig
        path: "style.default.base"

        property real fontPointSize: 28
    }

    Component {
        id: bookSurfaceComponent

        BookSurface {
            book: rootBook
            onLinkClicked: {
                bookStackTab.item.stackView.push({
                    item: bookSurfaceComponent,
                    properties: { positionValue: linkPosition }
                });
            }
        }
    }

    Book {
        id: rootBook
        source: genericConfig.book
    }

    StackView {
        id: stackView

        readonly property string title: (currentItem && currentItem.title) ? currentItem.title : qsTr("CuteReader")

        initialItem: bookSurfaceComponent
    }

    CollectionView {
        id: collectionView
        property string title: qsTr("Collection")

        onBookRequested: {
            genericConfig.book = source;
            stackView.replace(bookSurfaceComponent);
        }
    }
}
