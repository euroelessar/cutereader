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
include(../karchive.pri)

SOURCES += main.cpp

OTHER_FILES += qml/cutereader.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    qml/pages/ColorPage.qml \
    cutereader.spec \
    cutereader.yaml \
    cutereader.desktop

INCLUDEPATH += ../

books.path = /usr/share/cutereader/books
books.files = ../books/*

INSTALLS += books

#include(cutereader.pri)

RESOURCES +=

