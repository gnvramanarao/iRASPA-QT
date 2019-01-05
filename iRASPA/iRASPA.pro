#-------------------------------------------------
#
# Project created by QtCreator 2018-03-18T19:44:19
#
#-------------------------------------------------

QT       += core gui widgets opengl

greaterThan (QT_MINOR_VERSION, 5): QT += webenginewidgets

lessThan (QT_MINOR_VERSION, 6): QT += webkitwidgets


CONFIG += c++1z

QMAKE_CXX = /usr/bin/clang++

ICON = iRASPA.icns

TARGET = iRASPA
TEMPLATE = app

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
        main.cpp \
        mainwindow.cpp \
    projecttreeview.cpp \
    renderstackedwidget.cpp \
    aboutdialog.cpp \
    iraspatoolbar.cpp \
    mastertoolbar.cpp \
    cameratreewidgetcontroller.cpp \
    scenetreeview.cpp \
    framelistview.cpp \
    detailtabviewcontroller.cpp \
    cameracameraform.cpp \
    atomtreeview.cpp \
    atomtreeviewmodel.cpp \
    framelistviewmodel.cpp \
    scenetreeviewmodel.cpp \
    informationpanelview.cpp \
    textfield.cpp \
    masterstackedwidget.cpp \
    cellcellform.cpp \
    celltreewidgetcontroller.cpp \
    customintspinbox.cpp \
    customdoublespinbox.cpp \
    appearanceatomsform.cpp \
    appearancetreewidgetcontroller.cpp \
    appearancebondsform.cpp \
    appearanceunitcellform.cpp \
    appearanceadsorptionsurfaceform.cpp \
    appearanceannotationform.cpp \
    cameraselectionform.cpp \
    cameralightsform.cpp \
    camerapicturesform.cpp \
    camerabackgroundform.cpp \
    elementsform.cpp \
    infocreatorform.cpp \
    infocreationform.cpp \
    infochemicalform.cpp \
    infocitationform.cpp \
    cellstructuralform.cpp \
    cellsymmetryform.cpp \
    infotreewidgetcontroller.cpp \
    qdoubleslider.cpp \
    selectcolorbutton.cpp \
    fixedaspectratiolayoutitem.cpp \
    bondlistviewmodel.cpp \
    bondlistview.cpp \
    bondlistviewsliderstyleditemdelegate.cpp \
    qtsegmentcontrol.cpp \
    elementlistwidgetcontroller.cpp \
    linenumberplaintextedit.cpp \
    logviewcontroller.cpp \
    pythonviewcontroller.cpp \
    helpbrowser.cpp \
    helpwidget.cpp \
    projecttreeviewmodel.cpp

HEADERS += \
        mainwindow.h \
    projecttreeview.h \
    renderstackedwidget.h \
    aboutdialog.h \
    iraspatoolbar.h \
    mastertoolbar.h \
    cameratreewidgetcontroller.h \
    scenetreeview.h \
    framelistview.h \
    detailtabviewcontroller.h \
    cameracameraform.h \
    iraspamainwindowconsumerprotocol.h \
    atomtreeview.h \
    atomtreeviewmodel.h \
    framelistviewmodel.h \
    scenetreeviewmodel.h \
    informationpanelview.h \
    textfield.h \
    masterstackedwidget.h \
    cellcellform.h \
    celltreewidgetcontroller.h \
    customintspinbox.h \
    customdoublespinbox.h \
    appearanceatomsform.h \
    appearancetreewidgetcontroller.h \
    appearancebondsform.h \
    appearanceunitcellform.h \
    appearanceadsorptionsurfaceform.h \
    appearanceannotationform.h \
    cameraselectionform.h \
    cameralightsform.h \
    camerapicturesform.h \
    camerabackgroundform.h \
    elementsform.h \
    infocreatorform.h \
    infocreationform.h \
    infochemicalform.h \
    infocitationform.h \
    cellstructuralform.h \
    cellsymmetryform.h \
    infotreewidgetcontroller.h \
    qdoubleslider.h \
    selectcolorbutton.h \
    qcolorhash.h \
    fixedaspectratiolayoutitem.h \
    bondlistviewmodel.h \
    bondlistview.h \
    bondlistviewsliderstyleditemdelegate.h \
    qtsegmentcontrol.h \
    elementlistwidgetcontroller.h \
    linenumberplaintextedit.h \
    logviewcontroller.h \
    pythonviewcontroller.h \
    helpbrowser.h \
    helpwidget.h \
    projecttreeviewmodel.h \
    optional.h

FORMS += \
        mainwindow.ui \
    aboutdialog.ui \
    cameracameraform.ui \
    cellcellform.ui \
    appearanceatomsform.ui \
    appearancebondsform.ui \
    appearanceunitcellform.ui \
    appearanceadsorptionsurfaceform.ui \
    appearanceannotationform.ui \
    cameraselectionform.ui \
    cameralightsform.ui \
    camerapicturesform.ui \
    camerabackgroundform.ui \
    elementsform.ui \
    infocreatorform.ui \
    infocreationform.ui \
    infochemicalform.ui \
    infocitationform.ui \
    cellstructuralform.ui \
    cellsymmetryform.ui



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../iRASPAKit/release/  -liRASPAKit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../iRASPA/debug/ -liRASPAKit
else:macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-iRASPAKit-Desktop_Qt_5_11_2_clang_64bit-Release/ ../build-iRASPAKit-Desktop_Qt_5_10_1_clang_64bit-Release/libiRASPAKit.a
else:macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-iRASPAKit-Desktop_Qt_5_11_2_clang_64bit-Debug/ -liRASPAKit
else:unix:!macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-iRASPAKit-Desktop-Release/ -liRASPAKit
else:unix:!macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-iRASPAKit-Desktop-Debug/ -liRASPAKit


INCLUDEPATH += $$PWD/../iRASPAKit
DEPENDPATH += $$PWD/../iRASPAKit

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../RenderKit/release/  -lRenderKit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../RenderKit/debug/ -lRenderKit
else:macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-RenderKit-Desktop_Qt_5_11_2_clang_64bit-Release/ ../build-RenderKit-Desktop_Qt_5_10_1_clang_64bit-Release/libRenderKit.a
else:macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-RenderKit-Desktop_Qt_5_11_2_clang_64bit-Debug/ -lRenderKit
else:unix:!macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-RenderKit-Desktop-Release/ -lRenderKit
else:unix:!macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-RenderKit-Desktop-Debug/ -lRenderKit


INCLUDEPATH += $$PWD/../RenderKit
DEPENDPATH += $$PWD/../RenderKit

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../SymmetryKit/release/  -lSymmetryKit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../SymmetryKit/debug/ -lSymmetryKit
else:macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-SymmetryKit-Desktop_Qt_5_11_2_clang_64bit-Release/ ../build-SymmetryKit-Desktop_Qt_5_10_1_clang_64bit-Release/libSymmetryKit.a
else:macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-SymmetryKit-Desktop_Qt_5_11_2_clang_64bit-Debug/ -lSymmetryKit
else:unix:!macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-SymmetryKit-Desktop-Release/ -lSymmetryKit
else:unix:!macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-SymmetryKit-Desktop-Debug/ -lSymmetryKit


INCLUDEPATH += $$PWD/../SymmetryKit
DEPENDPATH += $$PWD/../SymmetryKit

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../SimulationKit/release/  -lSimulationKit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../SimulationKit/debug/ -lSimulationKit
else:macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-SimulationKit-Desktop_Qt_5_11_2_clang_64bit-Release/ ../build-SimulationKit-Desktop_Qt_5_10_1_clang_64bit-Release/libSimulationKit.a
else:macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-SimulationKit-Desktop_Qt_5_11_2_clang_64bit-Debug/ -lSimulationKit
else:unix:!macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-SimulationKit-Desktop-Release/ -lSimulationKit
else:unix:!macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-SimulationKit-Desktop-Debug/ -lSimulationKit


INCLUDEPATH += $$PWD/../SimulationKit
DEPENDPATH += $$PWD/../SimulationKit

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../FoundationKit/release/ -lFoundationKit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/FoundationKit/debug/ -lFoundationKit
else:macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-FoundationKit-Desktop_Qt_5_11_2_clang_64bit-Release/ ../build-FoundationKit-Desktop_Qt_5_10_1_clang_64bit-Release/libFoundationKit.a
else:macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-FoundationKit-Desktop_Qt_5_11_2_clang_64bit-Debug/ -lFoundationKit
else:unix:!macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-FoundationKit-Desktop-Release/ -lFoundationKit
else:unix:!macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-FoundationKit-Desktop-Debug/ -lFoundationKit

INCLUDEPATH += $$PWD/../FoundationKit
DEPENDPATH += $$PWD/../FoundationKit

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../MathKit/release/ -lMathKit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../MathKit/debug/ -lMathKit
else:macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-MathKit-Desktop_Qt_5_11_2_clang_64bit-Release/ ../build-MathKit-Desktop_Qt_5_10_1_clang_64bit-Release/libMathKit.a
else:macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-MathKit-Desktop_Qt_5_11_2_clang_64bit-Debug/ -lMathKit
else:unix:!macx:CONFIG(release, debug|release) LIBS += -L$$PWD/../build-MathKit-Desktop-Release/ -lMathKit
else:unix:!macx:CONFIG(debug, debug|release) LIBS += -L$$PWD/../build-MathKit-Desktop-Debug/ -lMathKit

INCLUDEPATH += $$PWD/../MathKit
DEPENDPATH += $$PWD/../MathKit


win32{
LIBS += opengl32.lib User32.lib
}

macx{
  INCLUDEPATH += /System/Library/Frameworks/Python.framework/Versions/Current/include/python2.7
  QMAKE_LFLAGS += -framework OpenCL -framework Python
}

win32{
INCLUDEPATH += $$PWD/../OpenCL
LIBS += -L"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v10.0/lib/x64" -lOpenCL

INCLUDEPATH += C:/Users/David/Desktop/vcpkg/installed/x64-windows-static/include/python2.7
LIBS += "C:/Users/David/Desktop/vcpkg/installed/x64-windows-static/lib/python27.lib" -lAdvapi32
}


unix:!macx{
LIBS += -lOpenCL
INCLUDEPATH += /usr/include/python2.7
LIBS += -lpython2.7 -llzma -lz -ldl -lutil
}

win32{
LIBS += C:\Users\David\Desktop\vcpkg\installed\x64-windows-static\lib\lzma.lib C:\Users\David\Desktop\vcpkg\installed\x64-windows-static\lib\zlib.lib
}


macx{
LIBS += -llzma -lz
}

RESOURCES += \
    iraspa-resources.qrc

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

