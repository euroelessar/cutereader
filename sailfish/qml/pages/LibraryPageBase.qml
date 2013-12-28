import QtQuick 2.0
import Sailfish.Silica 1.0
import org.qutim 0.3

Page {
    id: page

    property string title
    property LocalBookCollection collection

    function openView(title, entry) {
        pageStack.push(Qt.resolvedUrl("LibraryListView.qml"),
                       {
                           title: title,
                           data: entry,
                           collection: page.collection
                       })
    }

    function openBookView(entry) {
        pageStack.push(Qt.resolvedUrl("LibraryBookView.qml"),
                       {
                           title: title,
                           data: entry,
                           collection: page.collection
                       })
    }

    function openBook(entry) {
        application.openBook(entry);
    }

    function downloadBook(entry) {
        console.log("Download request:", JSON.stringify(entry), entry.toString())
    }

    function processAction(data) {
        switch (data.type) {
        case OpdsEntry.OpenExternally:
            Qt.openUrlExternally(data.entry)
            break
        case OpdsEntry.OpenView:
            page.openView(data.title, data.entry)
            break
        case OpdsEntry.OpenBookView:
            page.openBookView(data.entry)
            break
        case OpdsEntry.OpenBook:
            page.openBook(data.entry)
            break
        case OpdsEntry.Download:
            page.downloadBook(data.entry)
            break
        }
    }
}
