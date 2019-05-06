INCLUDEPATH += $$PWD

include(opengl.pri)

SOURCES += \
    $$PWD/rkcamera.cpp \
    $$PWD/rklight.cpp \
    $$PWD/rkfontatlas.cpp \
    $$PWD/rkrenderuniforms.cpp \
    $$PWD/trackball.cpp \
    $$PWD/spheregeometry.cpp \
    $$PWD/quadgeometry.cpp \
    $$PWD/cylindergeometry.cpp \
    $$PWD/cubegeometry.cpp \
    $$PWD/cappedcylindergeometry.cpp

HEADERS += \
    $$PWD/renderkit.h \
    $$PWD/rkcamera.h \
    $$PWD/rklight.h \
    $$PWD/rkfontatlas.h \
    $$PWD/rkrenderuniforms.h \
    $$PWD/rkrenderkitprotocols.h \
    $$PWD/trackball.h \
    $$PWD/spheregeometry.h \
    $$PWD/quadgeometry.h \
    $$PWD/cylindergeometry.h \
    $$PWD/cubegeometry.h \
    $$PWD/cappedcylindergeometry.h \
    $$PWD/optional.h

