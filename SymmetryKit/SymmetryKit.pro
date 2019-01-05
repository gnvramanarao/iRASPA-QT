#-------------------------------------------------
#
# Project created by QtCreator 2018-04-16T10:32:26
#
#-------------------------------------------------

QT       += core gui

TARGET = SymmetryKit
TEMPLATE = lib

CONFIG += c++1z static

//QMAKE_CXX = /usr/bin/clang++

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
    skcell.cpp \
    skcolorsets.cpp \
    skcolorset.cpp \
    skspacegroup.cpp \
    skboundingbox.cpp \
    skelement.cpp \
    skparser.cpp \
    skcifparser.cpp \
    skasymmetricatom.cpp \
    skbond.cpp \
    skatomtreecontroller.cpp \
    skatomtreenode.cpp \
    skbondsetcontroller.cpp \
    skpdbparser.cpp \
    skspacegroupsetting.cpp \
    skseitzmatrix.cpp \
    skrotationmatrix.cpp \
    sksymmetryoperationset.cpp \
    skpointgroup.cpp \
    skonethirdseitzmatrix.cpp \
    skrotationaloccurancetable.cpp

HEADERS += \
        symmetrykit.h \
    skcell.h \
    skcolorsets.h \
    skcolorset.h \
    skspacegroup.h \
    skboundingbox.h \
    skelement.h \
    symmetrykitprotocols.h \
    skparser.h \
    skcifparser.h \
    skasymmetricatom.h \
    skbond.h \
    skatomtreecontroller.h \
    skatomtreenode.h \
    skbondsetcontroller.h \
    skpdbparser.h \
    skspacegroupsetting.h \
    skseitzmatrix.h \
    skrotationmatrix.h \
    sksymmetryoperationset.h \
    skpointgroup.h \
    skonethirdseitzmatrix.h \
    skdefinitions.h \
    skrotationaloccurancetable.h \
    optional.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-FoundationKit-Desktop-Debug/release/ -lFoundationKit.1.0.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-FoundationKit-Desktop-Debug/debug/ -lFoundationKit.1.0.0
else:macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-FoundationKit-Desktop_Qt_5_11_1_clang_64bit-Release/ -lFoundationKit.1.0.0
else:macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-FoundationKit-Desktop_Qt_5_11_1_clang_64bit-Debug/ -lFoundationKit.1.0.0
else:unix:!macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-FoundationKit-Desktop-Release/ -lFoundationKit
else:unix:!macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-FoundationKit-Desktop-Debug/ -lFoundationKit

INCLUDEPATH += $$PWD/../FoundationKit
DEPENDPATH += $$PWD/../FoundationKit

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-MathKit-Desktop-Debug/release/ -lMathKit.1.0.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-MathKit-Desktop-Debug/debug/ -lMathKit.1.0.0
else:macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-MathKit-Desktop_Qt_5_11_1_clang_64bit-Release/ -lMathKit.1.0.0
else:macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-MathKit-Desktop_Qt_5_11_1_clang_64bit-Debug/ -lMathKit.1.0.0
else:unix:!macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-MathKit-Desktop-Release/ -lMathKit
else:unix:!macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-MathKit-Desktop-Debug/ -lMathKit

INCLUDEPATH += $$PWD/../MathKit
DEPENDPATH += $$PWD/../MathKit
