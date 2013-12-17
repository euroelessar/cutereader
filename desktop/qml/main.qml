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

    Book {
        id: rootBook
        source: genericConfig.book
        style.base.fontPointSize: bookStackTab.item.sliderValue
    }

    TabView {
        anchors.fill: parent
        anchors.margins: 5

        Tab {
            id: bookStackTab
            title: rootBook.info.title

            Item {
                property StackView stackView: bookStack
                property alias sliderValue: fontSlider.value

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
                Slider {
                    id: fontSlider
                    anchors {
                        left: backButton.right
                        right: parent.right
                        verticalCenter: backButton.verticalCenter
                    }

                    minimumValue: 1
                    maximumValue: 48
                    value: 12
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
