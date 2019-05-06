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

#TARGET.files += $$OUT_PWD/$(DESTDIR)/$(TARGET)
#TARGET.path = /usr
#INSTALLS += TARGET

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
  INCLUDEPATH += $$PWD/../OpenCL
  LIBS += opengl32.lib User32.lib
  LIBS += -L"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v10.0/lib/x64" -lOpenCL

  INCLUDEPATH += C:/Users/David/Desktop/vcpkg/installed/x64-windows-static/include/python2.7
  LIBS += "C:/Users/David/Desktop/vcpkg/installed/x64-windows-static/lib/python27.lib" -lAdvapi32
  LIBS += C:\Users\David\Desktop\vcpkg\installed\x64-windows-static\lib\lzma.lib C:\Users\David\Desktop\vcpkg\installed\x64-windows-static\lib\zlib.lib
}

unix:!macx{
  LIBS += -lOpenCL
  INCLUDEPATH += /usr/include/python2.7
  LIBS += -lpython2.7 -llzma -lz -ldl -lutil
}

RESOURCES += \
    iRASPA/iraspa-resources.qrc

DISTFILES += \
    ./LibraryOfStructures.irspdoc \
    ./bearer/qgenericbearer.dll \
    ./iconengines/qsvgicon.dll \
    ./imageformats/qtiff.dll \
    ./imageformats/qwbmp.dll \
    ./imageformats/qwebp.dll \
    ./imageformats/qgif.dll \
    ./imageformats/qicns.dll \
    ./imageformats/qico.dll \
    ./imageformats/qjpeg.dll \
    ./imageformats/qsvg.dll \
    ./imageformats/qtga.dll \
    ./platforms/qwindows.dll \
    ./position/qtposition_serialnmea.dll \
    ./position/qtposition_geoclue.dll \
    ./qtposition_positionpoll.dll \
    ./printsupport/windowsprintersupport.dll \
    ./qtwebengine_resources.pak \
    ./resources/qtwebengine_resources_100p.pak \
    ./resources/qtwebengine_resources_200p.pak \
    ./resources/icudtl.dat \
    ./resources/qtwebengine_devtools_resources.pak \
    ./styles/qwindowsvistastyle.dll \
    ./D3Dcompiler_47.dll \
    ./libEGL.dll \
    ./libGLESV2.dll \
    ./moc_predefs.h \
    ./moc_predefs.h.cbt \
    ./opengl32sw.dll \
    ./Qt5Core.dll \
    ./Qt5Gui.dll \
    ./Qt5Network.dll \
    ./Qt5OpenGL.dll \
    ./Qt5Positioning.dll \
    ./Qt5PrintSupport.dll \
    ./Qt5Qml.dll \
    ./Qt5Quick.dll \
    ./Qt5QuickWidgets.dll \
    ./Qt5SerialPort.dll \
    ./Qt5Svg.dll \
    ./Qt5WebChannel.dll \
    ./Qt5WebEngineCore.dll \
    ./Qt5WebEngineWidgets.dll \
    ./Qt5Widgets.dll \
    ./QtWebEngineProcess.exe

