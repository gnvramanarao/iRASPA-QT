#-------------------------------------------------
#
# Project created by QtCreator 2018-04-13T19:22:32
#
#-------------------------------------------------

QT       += gui
QT       += core

TARGET = MathKit
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
    float3x3.cpp \
    double3x3.cpp \
    float4x4.cpp \
    double4x4.cpp \
    float3.cpp \
    double3.cpp \
    float4.cpp \
    double4.cpp \
    float2.cpp \
    double2.cpp \
    int3.cpp \
    bool3.cpp \
    simd_quatd.cpp \
    nsrect.cpp \
    nssize.cpp \
    int3x3.cpp \
    drand48.cpp

HEADERS += \
        mathkit.h \
    float3x3.h \
    double3x3.h \
    float4x4.h \
    double4x4.h \
    float3.h \
    double3.h \
    float4.h \
    double4.h \
    float.h \
    float2.h \
    double2.h \
    int3.h \
    bool3.h \
    simd_quatd.h \
    nsrect.h \
    nssize.h \
    int3x3.h \
    drand48.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
