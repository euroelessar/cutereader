#!/bin/sh
cp fbreader.pri.base fbreader.pri

# Different os-specific implementations and trash
EXCLUDE_PATTERN="test|win32|unix|fbreader/tools|fbreader/trash|fbreader/fbreader/moto"
# Different platform-specific interface implementations, we want to depend only on Qt
EXCLUDE_PATTERN="$EXCLUDE_PATTERN|zlibrary/ui"
# Unsupported formats, they are broken
EXCLUDE_PATTERN="$EXCLUDE_PATTERN|formats/dummy|formats/docbook|formats/pdf"
# This implementations have symbols we want to fully reimplement
EXCLUDE_PATTERN="$EXCLUDE_PATTERN|main.cpp|library/ZLibrary.cpp"
# We don't have bzip2 on Sailfish :(
EXCLUDE_PATTERN="$EXCLUDE_PATTERN|filesystem/bzip2"

echo 'SOURCES += \' >> fbreader.pri
find ../3rdparty/fbreader -name \*.cpp | sed 's/^\(.*\)$/    \1 \\/' | grep -Pv "$EXCLUDE_PATTERN" >> fbreader.pri

echo '' >> fbreader.pri
echo 'HEADERS += \' >> fbreader.pri
find ../3rdparty/fbreader -name \*.h | sed 's/^\(.*\)$/    \1 \\/' | grep -Pv "$EXCLUDE_PATTERN" >> fbreader.pri
