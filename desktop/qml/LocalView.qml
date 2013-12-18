import QtQuick 2.0
import org.qutim 0.3
import QtQuick.Controls 1.0

Item {
    id: root

    property alias source: bookCollection.baseDir
    property Book book

    signal bookRequested(url source)

    LocalBookCollection {
        id: bookCollection
    }

    Component {
        id: booksView

        ScrollView {
            property alias filter: bookModel.filter

            ListView {
                model: LocalBookModel {
                    id: bookModel
                    collection: bookCollection
                }

                delegate: BookDelegate {
                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            root.bookRequested(source);
                        }
                    }
                }
            }
        }
    }

    Component {
        id: categorizedView

        ScrollView {
            property alias type: bookModel.type

            ListView {
                model: CategoriesBookModel {
                    id: bookModel
                    collection: bookCollection
                }

                delegate: CategoryDelegate {
                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            stackView.push({
                                item: booksView,
                                properties: {
                                    filter: filter
                                }
                            })
                        }
                    }
                }
            }
        }
    }

    Component {
        id: opdsBookView

        OpdsBookView {
            onLinkRequested: {
                stackView.push({
                    item: opdsView,
                    properties: {
                        source: source
                    }
                })
            }
            onBookRequested: {
                stackView.push({
                    item: opdsBookView,
                    properties: {
                        opdsEntry: opdsEntry
                    }
                })
            }
        }
    }

    Component {
        id: opdsView

        OpdsView {
            onLinkRequested: {
                stackView.push({
                    item: opdsView,
                    properties: {
                        source: source
                    }
                })
            }
            onBookRequested: {
                stackView.push({
                    item: opdsBookView,
                    properties: {
                        opdsEntry: opdsEntry
                    }
                })
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

        initialItem: Component {
            ScrollView {
                ListView {
                    model: ListModel {
                        Component.onCompleted: {
                            append({
                                title: qsTr("Authors"),
                                subtitle: qsTr("Books categorized by authors"),
                                component: categorizedView,
                                properties: {
                                    type: CategoriesBookModel.Authors
                                }
                            });
                            append({
                                title: qsTr("Genres"),
                                subtitle: qsTr("Books categorized by genres"),
                                component: categorizedView,
                                properties: {
                                    type: CategoriesBookModel.Genres
                                }
                            });
                            append({
                                title: qsTr("Series"),
                                subtitle: qsTr("Books categorized by series"),
                                component: categorizedView,
                                properties: {
                                    type: CategoriesBookModel.Series
                                }
                            });
                            append({
                                title: qsTr("Titles"),
                                subtitle: qsTr("Books categorized by titles"),
                                component: booksView,
                                properties: {
                                }
                            });
                            append({
                                title: qsTr("Recent"),
                                subtitle: qsTr("Recently opened books"),
                                component: categorizedView,
                                properties: {
                                }
                            });
                            append({
                                title: qsTr("Opds"),
                                subtitle: qsTr("network library"),
                                component: opdsView,
                                properties: {
                                    source: "http://flibusta.net/opds/author/19011/alphabet/1"
                                }
                            });
                        }
                    }

                    delegate: CategoryDelegate {
                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                stackView.push({
                                    item: component,
                                    properties: properties
                                })
                            }
                        }
                    }
                }
            }
        }
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
