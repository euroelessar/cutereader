import QtQuick 2.0
import org.qutim 0.3
import QtQuick.Controls 1.0

Item {
    property alias source: bookCollection.baseDir

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
//                            var filter = {};
//                            switch (bookModel.type) {
//                            case CategoriesBookModel.Authors:
//                                filter['author'] = title;
//                                break;
//                            default:
//                                break;
//                            }

                            console.log('filter:', JSON.stringify(filter))

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
