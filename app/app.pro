TEMPLATE = app
TARGET = cutereader

include(../lib/lib.pri)

SOURCES += $$PWD/main.cpp

android {
    QT += xmlpatterns #hack for deployment
    UI_TYPE = android
} else {
    UI_TYPE = desktop
}
#UI_TYPE = android

# Add more folders to ship with the application, here
folder_01.source = ../app/qml/$$UI_TYPE/qml
folder_01.target = qml/cutereader
folder_02.source = ../books
folder_02.target = ./
folder_03.source = ../hyphen
folder_03.target = ./
DEPLOYMENTFOLDERS = folder_01 folder_02 folder_03

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(../qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
