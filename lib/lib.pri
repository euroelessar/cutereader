# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

include(../karchive.pri)

QT += concurrent
CONFIG += c++11

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += $$PWD/booktextblock.cpp \
    $$PWD/booktextfragment.cpp \
    $$PWD/bookblockitem.cpp \
    $$PWD/bookpageitem.cpp \
    $$PWD/bookitem.cpp \
    $$PWD/bookblock.cpp \
    $$PWD/bookimageblock.cpp \
    $$PWD/fb2imageprovider.cpp

HEADERS += $$PWD/booktextblock.h \
    $$PWD/booktextfragment.h \
    $$PWD/bookblockitem.h \
    $$PWD/bookpageitem.h \
    $$PWD/bookitem.h \
    $$PWD/bookblock.h \
    $$PWD/bookimageblock.h \
    $$PWD/fb2imageprovider.h
