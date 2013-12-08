import QtQuick 2.0
import QtQuick.Controls 1.0
import org.qutim 0.3

Item {
    width: 360
    height: 360
    visible: true
    TabView {
        anchors.fill: parent
        anchors.margins: 5
        id: root

        Book {
            id: rootBook
            source: {
                console.log("../../../books/the_three_musketeers.fb2.zip:the_three_musketeers.fb2")
                return Qt.resolvedUrl("../../../books/the_three_musketeers.fb2.zip:the_three_musketeers.fb2")
            }
        }

        Tab {
            title: rootBook.info.title

            BookSurface {
                book: rootBook
            }
        }
        Tab {
            title: "OPDS"
            clip: true
            OpdsView {
                source: "http://flibusta.net/opds/"
            }
        }
        LocalBookCollection {
            baseDir: "file:///home/elessar/.books/"
        }
    }
}
