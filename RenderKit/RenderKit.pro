#-------------------------------------------------
#
# Project created by QtCreator 2018-04-16T11:50:59
#
#-------------------------------------------------

QT       += core gui opengl

include(opengl.pri)

TARGET = RenderKit
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

INCLUDEPATH += $$PWD/../OpenCL

SOURCES += \
    rkcamera.cpp \
    rklight.cpp \
    rkfontatlas.cpp \
    rkrenderuniforms.cpp \
    trackball.cpp \
    spheregeometry.cpp \
    quadgeometry.cpp \
    cylindergeometry.cpp \
    cubegeometry.cpp \
    cappedcylindergeometry.cpp

HEADERS += \
        renderkit.h \
    rkcamera.h \
    rklight.h \
    rkfontatlas.h \
    rkrenderuniforms.h \
    rkrenderkitprotocols.h \
    trackball.h \
    spheregeometry.h \
    quadgeometry.h \
    cylindergeometry.h \
    cubegeometry.h \
    cappedcylindergeometry.h \
    optional.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}


QMAKE_LFLAGS += -framework OpenCL





win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-SymmetryKit-Desktop-Debug/release/ -lSymmetryKit.1.0.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-SymmetryKit-Desktop-Debug/debug/ -lSymmetryKit.1.0.0
else:macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-SymmetryKit-Desktop_Qt_5_11_1_clang_64bit-Release/ -lSymmetryKit.1.0.0
else:macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-SymmetryKit-Desktop_Qt_5_11_1_clang_64bit-Debug/ -lSymmetryKit.1.0.0
else:unix:!macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-SymmetryKit-Desktop-Release/ -lSymmetryKit
else:unix:!macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-SymmetryKit-Desktop-Debug/ -lSymmetryKit

INCLUDEPATH += $$PWD/../SymmetryKit
DEPENDPATH += $$PWD/../SymmetryKit

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-SimulationKit-Desktop-Debug/release/ -lSimulationKit.1.0.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-SimulationKit-Desktop-Debug/debug/ -lSimulationKit.1.0.0
else:macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-SimulationKit-Desktop_Qt_5_11_1_clang_64bit-Release/ -lSimulationKit.1.0.0
else:macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-SimulationKit-Desktop_Qt_5_11_1_clang_64bit-Debug/ -lSimulationKit.1.0.0
else:unix:!macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-SimulationKit-Desktop-Release/ -lSimulationKit
else:unix:!macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-SimulationKit-Desktop-Debug/ -lSimulationKit

INCLUDEPATH += $$PWD/../SimulationKit
DEPENDPATH += $$PWD/../SimulationKit

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

#LIBS += -lOpenCL
