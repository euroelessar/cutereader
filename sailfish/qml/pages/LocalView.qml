import QtQuick 2.0
import Sailfish.Silica 1.0
import org.qutim 0.3

Page {
    id: page

    LocalBookCollection {
        id: bookCollection
    }

    Component {
        id: booksView

        Page {
            property alias filter: bookModel.filter

            SilicaListView {
                anchors.fill: parent

                header: PageHeader {
                    title: qsTr("Books")
                }

                section {
                    property: 'section'
                    delegate: SectionHeader {
                        text: section
                    }
                }

                model: LocalBookModel {
                    id: bookModel
                    collection: bookCollection
                }

                delegate: BookListItem {
                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            application.openBook(source);
                        }
                    }
                }
            }
        }
    }

    Component {
        id: categorizedView

        Page {
            property alias type: bookModel.type

            SilicaListView {
                anchors.fill: parent

                header: PageHeader {
                    title: {
                        switch (bookModel.type) {
                        case CategoriesBookModel.Authors:
                            return qsTr("Authors")
                        case CategoriesBookModel.Genres:
                            return qsTr("Genres")
                        case CategoriesBookModel.Series:
                            return qsTr("Series")
                        default:
                            return qsTr("Unknown categories")
                        }
                    }
                }

                model: CategoriesBookModel {
                    id: bookModel
                    collection: bookCollection
                }

                delegate: CategoryListItem {
                    MouseArea {
                        anchors.fill: parent

                        onClicked: pageStack.push(booksView, { filter: filter })
                    }
                }
            }
        }
    }

    SilicaListView {
        anchors.fill: parent

        header: PageHeader {
            title: qsTr("Library")
        }

        section {
            property: 'section'
            delegate: SectionHeader {
                text: section
            }
        }

        model: ListModel {
            Component.onCompleted: {
                append({
                    title: qsTr("Authors"),
                    subtitle: qsTr("Books categorized by authors"),
                    component: categorizedView,
                    properties: {
                        type: CategoriesBookModel.Authors
                    },
                    section: qsTr("Local")
                });
                append({
                    title: qsTr("Genres"),
                    subtitle: qsTr("Books categorized by genres"),
                    component: categorizedView,
                    properties: {
                        type: CategoriesBookModel.Genres
                    },
                    section: qsTr("Local")
                });
                append({
                    title: qsTr("Series"),
                    subtitle: qsTr("Books categorized by series"),
                    component: categorizedView,
                    properties: {
                        type: CategoriesBookModel.Series
                    },
                    section: qsTr("Local")
                });
                append({
                    title: qsTr("Titles"),
                    subtitle: qsTr("Books categorized by titles"),
                    component: booksView,
                    properties: {
                    },
                    section: qsTr("Local")
                });
                append({
                    title: qsTr("Recent"),
                    subtitle: qsTr("Recently opened books"),
                    component: categorizedView,
                    properties: {
                    },
                     section: qsTr("Local")
                });
            }
        }

        delegate: CategoryListItem {
            id: listItem

            onClicked: pageStack.push(component, properties)
        }
    }
}
