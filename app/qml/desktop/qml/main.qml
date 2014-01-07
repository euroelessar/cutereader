import QtQuick 2.1
import QtQuick.Controls 1.0
import org.qutim 0.3

ApplicationWindow {
    id: rootItem
    width: 360
    height: 360
    visible: true

    Config {
        id: genericConfig
        path: "generic"

        property url book
    }

    Config {
        id: textConfig
        path: "text"

        function ensure(name, value) {
            if (!hasValue(name))
                setValue(name, value);
        }

        Component.onCompleted: {
            // Default text settings
            ensure("base.fontPixelSize", 12);
            ensure("title.fontPixelSize", 14);
            ensure("title.fontWeight", Font.Bold);
            ensure("strong.fontWeight", Font.Bold);
            ensure("emphasis.fontItalic", true);
            ensure("strikeThrough.fontStrikeOut", true);
            ensure("sub.verticalAlignment", TextSettings.AlignSubScript);
            ensure("sup.verticalAlignment", TextSettings.AlignSuperScript);
            ensure("internalAnchor.underlineStyle", Qt.SolidLine);
            ensure("externalAnchor.underlineStyle", Qt.SolidLine);
            ensure("noteAnchor.verticalAlignment", TextSettings.AlignSuperScript);
        }
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
        style: BookStyle {
            id: bookStyle

            background: 'white'
            base: 'black'
            title: 'red'
        }

//        style.base.fontPointSize: styleConfig.fontPointSize
    }

//    DownloadBookList {
//        id: downloadBookList
//    }

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
                    property bool completed: false

                    minimumValue: 0
                    maximumValue: 255
                    onValueChanged: {
                        if (!status === Component.Ready)
                            return;
                        console.log(Qt.hsla(value / 255, 0.5, 0.5))
                        bookStyle.base = Qt.hsla(value / 255, 0.5, 0.5);
                    }

//                    value: styleConfig.fontPointSize
//                    onValueChanged: {
//                        console.log(value, rootBook.style.base.fontPointSize)
//                        if (status === Component.Ready) styleConfig.fontPointSize = value
//                    }
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
