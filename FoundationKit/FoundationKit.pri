INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

macx{
  INCLUDEPATH += /usr/local/include
}

SOURCES += \
    $$PWD/indexpath.cpp \
    $$PWD/scanner.cpp \
    $$PWD/characterset.cpp \
    $$PWD/zip.cpp

HEADERS += \
    $$PWD/foundationkit.h \
    $$PWD/indexpath.h \
    $$PWD/scanner.h \
    $$PWD/characterset.h \
    $$PWD/flatten.h \
    $$PWD/aspectratiosingleitemlayout.h \
    $$PWD/zipreader.h \
    $$PWD/zipwriter.h \
    $$PWD/signalblocker.h \
    $$PWD/logreporting.h \
    $$PWD/serialization.h
