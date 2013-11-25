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
            filePath: "sample.fb2"
        }
        
        Tab {
            title: "Book"
            
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
    }
}
