INCLUDEPATH += $$PWD/karchive/src

HEADERS += $$PWD/karchive/src/config-compression.h \
    $$PWD/karchive/src/karchive_export.h \
    $$PWD/karchive/src/kfilterbase.h \
    $$PWD/karchive/src/k7zip.h \
    $$PWD/karchive/src/kgzipfilter.h \
    $$PWD/karchive/src/kxzfilter.h \
    $$PWD/karchive/src/kcompressiondevice.h \
    $$PWD/karchive/src/kzip.h \
    $$PWD/karchive/src/ktar.h \
    $$PWD/karchive/src/kbzip2filter.h \
    $$PWD/karchive/src/karchive_p.h \
    $$PWD/karchive/src/kar.h \
    $$PWD/karchive/src/knonefilter.h \
    $$PWD/karchive/src/klimitediodevice_p.h \
    $$PWD/karchive/src/kfilterdev.h \
    $$PWD/karchive/src/karchive.h

SOURCES += $$PWD/karchive/src/kfilterbase.cpp \
    $$PWD/karchive/src/klimitediodevice.cpp \
    $$PWD/karchive/src/kgzipfilter.cpp \
    $$PWD/karchive/src/kzip.cpp \
    $$PWD/karchive/src/kxzfilter.cpp \
    $$PWD/karchive/src/kfilterdev.cpp \
    $$PWD/karchive/src/kbzip2filter.cpp \
    $$PWD/karchive/src/knonefilter.cpp \
    $$PWD/karchive/src/k7zip.cpp \
    $$PWD/karchive/src/kar.cpp \
    $$PWD/karchive/src/kcompressiondevice.cpp \
    $$PWD/karchive/src/karchive.cpp \
    $$PWD/karchive/src/ktar.cpp

LIBS += -lz # -lbz2 -llzma
