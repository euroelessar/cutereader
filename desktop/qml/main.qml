import QtQuick 2.0
import QtQuick.Controls 1.0
import org.qutim 0.3

Item {
    id: rootItem
    width: 360
    height: 360
    visible: true

    Config {
        id: genericConfig
        path: "generic"

        property url book
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

    TabView {
        anchors.fill: parent
        anchors.margins: 5

        Book {
            id: rootBook
            source: genericConfig.book
        }

        Tab {
            id: bookStackTab
            title: rootBook.info.title

            Item {
                property StackView stackView: bookStack

                StackView {
                    id: bookStack

                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        bottom: backButton.top
                    }

                    initialItem: bookSurfaceComponent
                }
                Button {
                    id: backButton

                    anchors {
                        left: parent.left
                        bottom: parent.bottom
                    }

                    text: "Previous"
                    enabled: bookStack.depth > 1
                    onClicked: bookStack.pop()
                }
            }
        }
//        Tab {
//            title: "OPDS"
//            clip: true
//            OpdsView {
//                source: "http://flibusta.net/opds/"
//            }
//        }
        Tab {
            title: "Library"
            clip: true
            LocalView {
                book: rootBook
                source: "file:///home/elessar/.books/"
                onBookRequested: {
                    var book = bookStackTab.item;
                    book.stackView.clear();
                    genericConfig.book = source;
                    book.stackView.push(bookSurfaceComponent);
                }
            }
        }
    }
}
