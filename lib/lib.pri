# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

include(../karchive.pri)

QT += concurrent
CONFIG += c++11

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += ../lib/blocks/booktextblock.cpp \
    ../lib/bookblockitem.cpp \
    ../lib/bookpageitem.cpp \
    ../lib/bookitem.cpp \
    ../lib/bookblock.cpp \
    ../lib/blocks/bookimageblock.cpp \
    ../lib/formats/fb2/fb2reader.cpp \
    ../lib/providers/fb2imageprovider.cpp \
    ../lib/archivereader.cpp \
    ../lib/bookinfo.cpp \
    ../lib/bookinfoitem.cpp \
    ../lib/models/localbookcollection.cpp \
    ../lib/bookblockfactory.cpp \
    ../lib/blocks/booktextblockfactory.cpp \
    ../lib/blocks/bookimageblockfactory.cpp \
    ../lib/models/opdsbookcollection.cpp \
    ../lib/models/opdsbookmodel.cpp \
    ../lib/models/localbookmodel.cpp \
    ../lib/models/sortedlocalbookmodel.cpp \
    ../lib/models/bookcategoriesmodel.cpp \
    ../lib/config.cpp \
    ../lib/bookstyle.cpp \
    ../lib/booktextstyleitem.cpp \
    ../lib/models/opdsparser.cpp \
    ../lib/models/opdsentryitem.cpp \
    ../lib/models/downloadbooklist.cpp \
    ../lib/models/downloadbookjob.cpp \
    ../lib/models/opdsdownloadjob.cpp \
    ../lib/models/frontmodel.cpp \
    ../lib/models/bookrootmodel.cpp \
    ../lib/bookplugin.cpp \
    ../lib/pseudobookpageitem.cpp \
    ../lib/bookpageiterator.cpp \
    ../lib/saferunnable.cpp \
    ../lib/fontlistmodel.cpp \
    ../lib/blocks/texthyphenator.cpp

HEADERS += ../lib/blocks/booktextblock.h \
    ../lib/bookblockitem.h \
    ../lib/bookpageitem.h \
    ../lib/bookitem.h \
    ../lib/bookblock.h \
    ../lib/blocks/bookimageblock.h \
    ../lib/formats/fb2/fb2reader.h \
    ../lib/providers/fb2imageprovider.h \
    ../lib/archivereader.h \
    ../lib/bookinfo.h \
    ../lib/bookinfoitem.h \
    ../lib/models/localbookcollection.h \
    ../lib/bookblockfactory.h \
    ../lib/blocks/booktextblockfactory.h \
    ../lib/blocks/bookimageblockfactory.h \
    ../lib/models/opdsbookcollection.h \
    ../lib/models/opdsbookmodel.h \
    ../lib/models/localbookmodel.h \
    ../lib/models/sortedlocalbookmodel.h \
    ../lib/models/bookcategoriesmodel.h \
    ../lib/config.h \
    ../lib/bookstyle.h \
    ../lib/booktextstyleitem.h \
    ../lib/models/opdsparser.h \
    ../lib/models/opdsentryitem.h \
    ../lib/models/downloadbooklist.h \
    ../lib/models/downloadbookjob.h \
    ../lib/models/opdsdownloadjob.h \
    ../lib/models/frontmodel.h \
    ../lib/models/bookrootmodel.h \
    ../lib/bookplugin.h \
    ../lib/pseudobookpageitem.h \
    ../lib/bookpageiterator.h \
    ../lib/saferunnable.h \
    ../lib/fontlistmodel.h \
    ../lib/blocks/texthyphenator.h
