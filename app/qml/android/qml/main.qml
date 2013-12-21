import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.1
import org.qutim 0.3
import "style" as Android

ApplicationWindow {
    id: rootItem
    width: 768
    height: 1280
    visible: true

    // Implements back key navigation
    Keys.onReleased: {
        if (event.key === Qt.Key_Back) {
            //TODO
        }
    }

    toolBar: ToolBar {
        style: Android.ToolBarStyle {}

        RowLayout {
            anchors.fill: parent
            anchors.margins: 15

            Image {
                id: backButton
                visible: false
                Layout.alignment: Qt.AlignVCenter
                source: "style/images/navigation_previous_item.png"
            }

            Label {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true

                font.pointSize: 24
                color: "white"
                text: rootBook.info.title
            }
        }
    }

    Rectangle {
        color: "#212126"
        anchors.fill: parent
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
//        style.base.fontPointSize: styleConfig.fontPointSize
    }

    TabView {
        anchors.fill: parent
        anchors.margins: 5
        style: Android.TabViewStyle {}

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
                        bottom: fontSlider.top
                    }

                    initialItem: bookSurfaceComponent
                }

                Slider {
                    id: fontSlider
                    style: Android.SliderStyle {}
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }
                    property bool completed: false

                    minimumValue: 1
                    maximumValue: 48
                    value: styleConfig.fontPointSize
                    onValueChanged: {
                        rootBook.style.base.fontPointSize = value;
                        console.log(value, rootBook.style.base.fontPointSize)
                        if (status === Component.Ready) styleConfig.fontPointSize = value
                    }
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
