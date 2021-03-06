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
  #QMAKE_CXXFLAGS += -Wl,--stack,4194304
  INCLUDEPATH += /System/Library/Frameworks/Python.framework/Versions/Current/include/python2.7 /usr/local/include/freetype2/
  QMAKE_LFLAGS += -framework OpenCL -framework Python
  LIBS += -L/usr/local/lib /usr/local/lib/libfreetype.a -lx264 -lswscale -lavutil -lavformat -lavcodec -llzma -lz
}

win32{
  QMAKE_CFLAGS_RELEASE += /Zi
  QMAKE_LFLAGS_RELEASE += /INCREMENTAL:NO /DEBUG

  QMAKE_CXXFLAGS += /F 4194304
  QMAKE_LFLAGS   += /STACK:4194304

  INCLUDEPATH += "C:/msys64/home/ddubb/tmp/installed/include"
  INCLUDEPATH += "C:/vcpkg/installed/x64-windows-static/include"
  INCLUDEPATH += "C:/vcpkg/installed/x64-windows-static/include/python2.7"

  LIBS += "C:/msys64/home/ddubb/tmp/installed/lib/libavutil.a"
  LIBS += "C:/msys64/home/ddubb/tmp/installed/lib/libavfilter.a"
  LIBS += "C:/msys64/home/ddubb/tmp/installed/lib/libavformat.a"
  LIBS += "C:/msys64/home/ddubb/tmp/installed/lib/libavresample.a"
  LIBS += "C:/msys64/home/ddubb/tmp/installed/lib/libavdevice.a"
  LIBS += "C:/msys64/home/ddubb/tmp/installed/lib/libavcodec.a"
  LIBS += "C:/msys64/home/ddubb/tmp/installed/lib/libswresample.a"
  LIBS += "C:/msys64/home/ddubb/tmp/installed/lib/libswscale.a"
  LIBS += "C:/msys64/home/ddubb/tmp/installed/lib/libx264.lib"
  LIBS += "C:/msys64/home/ddubb/tmp/installed/lib/vpx.lib"
  LIBS += "C:/vcpkg/installed/x64-windows-static/lib/python27.lib"
  LIBS += "C:/vcpkg/installed/x64-windows-static/lib/lzma.lib"
  LIBS += "C:/vcpkg/installed/x64-windows-static/lib/zlib.lib"
  LIBS += "C:/vcpkg/installed/x64-windows-static/lib/OpenCL.lib"

  LIBS += secur32.lib ws2_32.lib bcrypt.lib kernel32.lib cfgmgr32.lib user32.lib ole32.lib advapi32.lib
}

win32:CONFIG(release, debug|release): LIBS += "C:/msys64/home/ddubb/tmp/installed/lib/x265-static.lib"
else:win32:CONFIG(debug, debug|release): LIBS += "C:/msys64/home/ddubb/tmp/installed/lib/x265-static-debug.lib"

unix:!macx{
  QMAKE_CXXFLAGS += -Wl,--stack,4194304

  LIBS += -lOpenCL
  INCLUDEPATH += /usr/include/python2.7
  LIBS += -lpython2.7 -llzma -lz -ldl -lutil
}

RESOURCES += \
    iRASPA/iraspa-resources.qrc
