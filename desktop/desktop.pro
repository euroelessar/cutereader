TEMPLATE = app
TARGET = cutereader

include(../lib/lib.pri)

SOURCES += $$PWD/main.cpp

# Add more folders to ship with the application, here
folder_01.source = ../desktop/qml
folder_01.target = qml/cutereader
folder_02.source = ../books
folder_02.target = ./
DEPLOYMENTFOLDERS = folder_01 folder_02

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(../qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
