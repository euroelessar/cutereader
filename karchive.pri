INCLUDEPATH += ../karchive/src

HEADERS += ../karchive/src/config-compression.h \
    ../karchive/src/karchive_export.h \
    ../karchive/src/kfilterbase.h \
    ../karchive/src/k7zip.h \
    ../karchive/src/kgzipfilter.h \
    ../karchive/src/kxzfilter.h \
    ../karchive/src/kcompressiondevice.h \
    ../karchive/src/kzip.h \
    ../karchive/src/ktar.h \
    ../karchive/src/kbzip2filter.h \
    ../karchive/src/karchive_p.h \
    ../karchive/src/kar.h \
    ../karchive/src/knonefilter.h \
    ../karchive/src/klimitediodevice_p.h \
    ../karchive/src/kfilterdev.h \
    ../karchive/src/karchive.h

SOURCES += ../karchive/src/kfilterbase.cpp \
    ../karchive/src/klimitediodevice.cpp \
    ../karchive/src/kgzipfilter.cpp \
    ../karchive/src/kzip.cpp \
    ../karchive/src/kxzfilter.cpp \
    ../karchive/src/kfilterdev.cpp \
    ../karchive/src/kbzip2filter.cpp \
    ../karchive/src/knonefilter.cpp \
    ../karchive/src/k7zip.cpp \
    ../karchive/src/kar.cpp \
    ../karchive/src/kcompressiondevice.cpp \
    ../karchive/src/karchive.cpp \
    ../karchive/src/ktar.cpp

LIBS += -lz # -lbz2 -llzma
