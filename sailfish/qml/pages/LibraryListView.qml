import QtQuick 2.0
import Sailfish.Silica 1.0
import org.qutim 0.3

LibraryPageBase {
    id: page

    property alias data: frontModel.data

    SilicaListView {
        anchors.fill: parent

        header: PageHeader {
            id: pageHeader
            title: page.title
        }

        section {
            property: 'section'
            delegate: SectionHeader {
                text: section
            }
        }

        model: FrontModel {
            id: frontModel
            collection: page.collection
        }

        delegate: CategoryListItem {
            id: listItem

            onClicked: {
                if (isBook)
                    page.openBookView(entryData)
                else
                    page.openView(title, entryData)
            }
            onActionClicked: page.processAction(data)
        }
    }
}
