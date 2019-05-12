#-------------------------------------------------
#
# Project created by QtCreator 2018-03-18T19:44:19
#
#-------------------------------------------------

QT       += core gui widgets opengl

greaterThan (QT_MINOR_VERSION, 5): QT += webenginewidgets

lessThan (QT_MINOR_VERSION, 6): QT += webkitwidgets

CONFIG += c++1z


include(MathKit/MathKit.pri)
include(FoundationKit/FoundationKit.pri)
include(SymmetryKit/SymmetryKit.pri)
include(SimulationKit/SimulationKit.pri)
include(RenderKit/RenderKit.pri)
include(iRASPAKit/iRASPAKit.pri)
include(iRASPA/iRASPA.pri)

ICON = iRASPA/iRASPA.icns

TARGET = iraspa
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

macx{
  INCLUDEPATH += /System/Library/Frameworks/Python.framework/Versions/Current/include/python2.7
  QMAKE_LFLAGS += -framework OpenCL -framework Python
  LIBS += -llzma -lz
}

win32{
  QMAKE_CFLAGS_RELEASE += /Zi
  QMAKE_LFLAGS_RELEASE += /INCREMENTAL:NO /DEBUG
  
  LIBS += "C:/Users/David/Desktop/vcpkg/installed/x64-windows-static/lib/opengl32.lib" User32.lib

  INCLUDEPATH += "C:/Users/David/Desktop/vcpkg/packages/opencl_x64-windows-static/include"
  LIBS += "C:/Users/David/Desktop/vcpkg/packages/opencl_x64-windows-static/lib/OpenCL.lib"

  #INCLUDEPATH += $(CUDA_PATH)/include -L$(AMDAPPSDKROOT)/include -L$(INTELOCLSDKROOT)/include
  #LIBS += -L$(CUDA_PATH)/lib/x64 -L$(AMDAPPSDKROOT)/lib/x86_64 -L$(INTELOCLSDKROOT)/lib/x64 -lOpenCL

  INCLUDEPATH += "C:/Users/David/Desktop/vcpkg/installed/x64-windows-static/include/python2.7"
  LIBS += "C:/Users/David/Desktop/vcpkg/installed/x64-windows-static/lib/python27.lib" -lAdvapi32

  INCLUDEPATH += "C:/Users/David/Desktop/vcpkg/packages/liblzma_x64-windows-static/include"
  LIBS += "C:/Users/David/Desktop/vcpkg/installed/x64-windows-static/lib/lzma.lib"

  INCLUDEPATH += "C:/Users/David/Desktop/vcpkg/packages/zlib_x64-windows-static/include"
  LIBS += "C:/Users/David/Desktop/vcpkg/installed/x64-windows-static/lib/zlib.lib"
}

unix:!macx{
  LIBS += -lOpenCL
  INCLUDEPATH += /usr/include/python2.7
  LIBS += -lpython2.7 -llzma -lz -ldl -lutil
}

RESOURCES += \
    iRASPA/iraspa-resources.qrc
