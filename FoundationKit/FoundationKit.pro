#-------------------------------------------------
#
# Project created by QtCreator 2018-04-14T13:06:45
#
#-------------------------------------------------

QT       += core gui

TARGET = FoundationKit
TEMPLATE = lib

CONFIG += c++1z static

QMAKE_CXX = /usr/bin/clang++

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    indexpath.cpp \
    scanner.cpp \
    characterset.cpp \
    zip.cpp

HEADERS += \
        foundationkit.h \
    indexpath.h \
    scanner.h \
    characterset.h \
    flatten.h \
    aspectratiosingleitemlayout.h \
    zipreader.h \
    zipwriter.h \
    signalblocker.h \
    logreporting.h \
    serialization.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32{
INCLUDEPATH +=  C:\Users\David\Desktop\vcpkg\installed\x64-windows-static\include
}

macx{
INCLUDEPATH += /usr/local/include
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-MathKit-Desktop-Debug/release/ -lMathKit.1.0.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-MathKit-Desktop-Debug/debug/ -lMathKit.1.0.0
else:macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-MathKit-Desktop_Qt_5_11_1_clang_64bit-Release/ -lMathKit.1.0.0
else:macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-MathKit-Desktop_Qt_5_11_1_clang_64bit-Debug/ -lMathKit.1.0.0
else:unix:!macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-MathKit-Desktop-Release/ -lMathKit
else:unix:!macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-MathKit-Desktop-Debug/ -lMathKit

INCLUDEPATH += $$PWD/../MathKit
DEPENDPATH += $$PWD/../MathKit
