import QtQuick 2.0
import org.qutim 0.3

Rectangle {
    id: root
    width: 360
    height: 360
    
    Book {
        id: book
        filePath: "/home/elessar/.books/Oldi_Oykumena_1_Kukolnik.131765.fb2"
    }
    
    Row {
        id: pagesRow
        anchors.fill: parent
        spacing: 5
        property real bookPageWidth: Math.floor((width - spacing) / 3 - 2)
        
        BookPage {
            id: previousBookPage
            book: book
            height: parent.height
            width: pagesRow.bookPageWidth
        }
        
        BookPage {
            id: page
            book: book
            height: parent.height
            width: pagesRow.bookPageWidth
            
            onPositionValueChanged: {
                console.log("onPositionChanged")
                var positionValue = page.previousPage();
                previousBookPage.block = positionValue.block
                previousBookPage.blockPosition = positionValue.blockPosition
                positionValue = page.nextPage();
                nextBookPage.block = positionValue.block
                nextBookPage.blockPosition = positionValue.blockPosition
            }
        }
        
        BookPage {
            id: nextBookPage
            book: book
            height: parent.height
            width: pagesRow.bookPageWidth
        }
    }
    
    MouseArea {
        anchors.fill: parent
        onClicked: {
            var result;
            if (mouseX > width / 2) {
                result = page.nextPage();
                console.log("next", JSON.stringify(result));
                if (result.block !== undefined) {
                    page.positionValue = result
                }
            } else {
                result = page.previousPage();
                console.log("previous", JSON.stringify(result));
                if (result.block !== undefined) {
                    page.positionValue = result
                }
            }
        }
    }
}
