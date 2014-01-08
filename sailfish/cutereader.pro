# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = cutereader

CONFIG += sailfishapp

include(../lib/lib.pri)

SOURCES += main.cpp

OTHER_FILES += qml/cutereader.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    cutereader.spec \
    cutereader.yaml \
    cutereader.desktop \
    qml/pages/BookPageItem.qml \
    qml/pages/BookPage.qml \
    qml/pages/LocalView.qml \
    qml/pages/BookListItem.qml \
    qml/pages/CategoryListItem.qml \
    qml/pages/LibraryListView.qml \
    qml/pages/LibraryBookView.qml \
    qml/pages/LibraryPageBase.qml \
    qml/pages/SettingsListPage.qml \
    qml/pages/FontSettingsPage.qml \
    qml/pages/ColorsSettingsPage.qml \
    qml/pages/ColorChooserDialog.qml \
    qml/pages/ColorChooser.qml \
    qml/pages/ValuesComboBox.qml \
    qml/pages/FormatTypesPage.qml

INCLUDEPATH += ../

books.path = /usr/share/cutereader/books
books.files = ../books/*
hyphen.path = /usr/share/cutereader/hyphen
hyphen.files = ../hyphen/*

INSTALLS += books hyphen

#include(cutereader.pri)

RESOURCES +=

