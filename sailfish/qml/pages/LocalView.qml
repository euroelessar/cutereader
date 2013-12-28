import QtQuick 2.0
import Sailfish.Silica 1.0
import org.qutim 0.3

LibraryListView {
    id: page

    title: qsTr("Library")
    collection: LocalBookCollection {
        id: bookCollection
    }
}
