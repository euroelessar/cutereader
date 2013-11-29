# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += $$PWD/booksurfaceitem.cpp \
    $$PWD/booktextblock.cpp \
    $$PWD/booktextfragment.cpp \
    $$PWD/bookblockitem.cpp \
    $$PWD/bookpageitem.cpp \
    $$PWD/bookitem.cpp

QMAKE_CXXFLAGS += -std=c++0x
QT += concurrent

HEADERS += \
    $$PWD/booksurfaceitem.h \
    $$PWD/booktextblock.h \
    $$PWD/booktextfragment.h \
    $$PWD/bookblockitem.h \
    $$PWD/bookpageitem.h \
    $$PWD/bookitem.h
