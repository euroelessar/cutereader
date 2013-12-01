# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

include(../karchive.pri)

QT += concurrent
CONFIG += c++11

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += ../lib/booktextblock.cpp \
    ../lib/booktextfragment.cpp \
    ../lib/bookblockitem.cpp \
    ../lib/bookpageitem.cpp \
    ../lib/bookitem.cpp \
    ../lib/bookblock.cpp \
    ../lib/bookimageblock.cpp \
    ../lib/fb2imageprovider.cpp

HEADERS += ../lib/booktextblock.h \
    ../lib/booktextfragment.h \
    ../lib/bookblockitem.h \
    ../lib/bookpageitem.h \
    ../lib/bookitem.h \
    ../lib/bookblock.h \
    ../lib/bookimageblock.h \
    ../lib/fb2imageprovider.h
