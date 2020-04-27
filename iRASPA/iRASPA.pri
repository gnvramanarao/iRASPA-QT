INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

include(undocommands.pri)

SOURCES += \
    $$PWD/atomtreeviewpushbuttonstyleditemdelegate.cpp \
    $$PWD/bondlistpushbuttonstyleditemdelegate.cpp \
    $$PWD/bondlistviewcomboboxstyleditemdelegate.cpp \
    $$PWD/main.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/projecttreeview.cpp \
    $$PWD/renderstackedwidget.cpp \
    $$PWD/aboutdialog.cpp \
    $$PWD/iraspatoolbar.cpp \
    $$PWD/mastertoolbar.cpp \
    $$PWD/cameratreewidgetcontroller.cpp \
    $$PWD/scenetreeview.cpp \
    $$PWD/framelistview.cpp \
    $$PWD/detailtabviewcontroller.cpp \
    $$PWD/cameracameraform.cpp \
    $$PWD/atomtreeview.cpp \
    $$PWD/atomtreeviewmodel.cpp \
    $$PWD/framelistviewmodel.cpp \
    $$PWD/scenetreeviewmodel.cpp \
    $$PWD/informationpanelview.cpp \
    $$PWD/textfield.cpp \
    $$PWD/masterstackedwidget.cpp \
    $$PWD/cellcellform.cpp \
    $$PWD/celltreewidgetcontroller.cpp \
    $$PWD/customintspinbox.cpp \
    $$PWD/customdoublespinbox.cpp \
    $$PWD/appearanceatomsform.cpp \
    $$PWD/appearancetreewidgetcontroller.cpp \
    $$PWD/appearancebondsform.cpp \
    $$PWD/appearanceunitcellform.cpp \
    $$PWD/appearanceadsorptionsurfaceform.cpp \
    $$PWD/appearanceannotationform.cpp \
    $$PWD/cameraselectionform.cpp \
    $$PWD/cameralightsform.cpp \
    $$PWD/camerapicturesform.cpp \
    $$PWD/camerabackgroundform.cpp \
    $$PWD/elementsform.cpp \
    $$PWD/infocreatorform.cpp \
    $$PWD/infocreationform.cpp \
    $$PWD/infochemicalform.cpp \
    $$PWD/infocitationform.cpp \
    $$PWD/cellstructuralform.cpp \
    $$PWD/cellsymmetryform.cpp \
    $$PWD/infotreewidgetcontroller.cpp \
    $$PWD/qdoubleslider.cpp \
    $$PWD/selectcolorbutton.cpp \
    $$PWD/fixedaspectratiolayoutitem.cpp \
    $$PWD/bondlistviewmodel.cpp \
    $$PWD/bondlistview.cpp \
    $$PWD/bondlistviewsliderstyleditemdelegate.cpp \
    $$PWD/qtsegmentcontrol.cpp \
    $$PWD/elementlistwidgetcontroller.cpp \
    $$PWD/linenumberplaintextedit.cpp \
    $$PWD/logviewcontroller.cpp \
    $$PWD/pythonviewcontroller.cpp \
    $$PWD/helpbrowser.cpp \
    $$PWD/helpwidget.cpp \
    $$PWD/projecttreeviewmodel.cpp

HEADERS += \
    $$PWD/atomtreeviewpushbuttonstyleditemdelegate.h \
    $$PWD/bondlistpushbuttonstyleditemdelegate.h \
    $$PWD/bondlistviewcomboboxstyleditemdelegate.h \
    $$PWD/mainwindow.h \
    $$PWD/projecttreeview.h \
    $$PWD/renderstackedwidget.h \
    $$PWD/aboutdialog.h \
    $$PWD/iraspatoolbar.h \
    $$PWD/mastertoolbar.h \
    $$PWD/cameratreewidgetcontroller.h \
    $$PWD/scenetreeview.h \
    $$PWD/framelistview.h \
    $$PWD/detailtabviewcontroller.h \
    $$PWD/cameracameraform.h \
    $$PWD/iraspamainwindowconsumerprotocol.h \
    $$PWD/atomtreeview.h \
    $$PWD/atomtreeviewmodel.h \
    $$PWD/framelistviewmodel.h \
    $$PWD/scenetreeviewmodel.h \
    $$PWD/informationpanelview.h \
    $$PWD/textfield.h \
    $$PWD/masterstackedwidget.h \
    $$PWD/cellcellform.h \
    $$PWD/celltreewidgetcontroller.h \
    $$PWD/customintspinbox.h \
    $$PWD/customdoublespinbox.h \
    $$PWD/appearanceatomsform.h \
    $$PWD/appearancetreewidgetcontroller.h \
    $$PWD/appearancebondsform.h \
    $$PWD/appearanceunitcellform.h \
    $$PWD/appearanceadsorptionsurfaceform.h \
    $$PWD/appearanceannotationform.h \
    $$PWD/cameraselectionform.h \
    $$PWD/cameralightsform.h \
    $$PWD/camerapicturesform.h \
    $$PWD/camerabackgroundform.h \
    $$PWD/elementsform.h \
    $$PWD/infocreatorform.h \
    $$PWD/infocreationform.h \
    $$PWD/infochemicalform.h \
    $$PWD/infocitationform.h \
    $$PWD/cellstructuralform.h \
    $$PWD/cellsymmetryform.h \
    $$PWD/infotreewidgetcontroller.h \
    $$PWD/qdoubleslider.h \
    $$PWD/selectcolorbutton.h \
    $$PWD/qcolorhash.h \
    $$PWD/fixedaspectratiolayoutitem.h \
    $$PWD/bondlistviewmodel.h \
    $$PWD/bondlistview.h \
    $$PWD/bondlistviewsliderstyleditemdelegate.h \
    $$PWD/qtsegmentcontrol.h \
    $$PWD/elementlistwidgetcontroller.h \
    $$PWD/linenumberplaintextedit.h \
    $$PWD/logviewcontroller.h \
    $$PWD/pythonviewcontroller.h \
    $$PWD/helpbrowser.h \
    $$PWD/helpwidget.h \
    $$PWD/projecttreeviewmodel.h

FORMS += \
    $$PWD/mainwindow.ui \
    $$PWD/aboutdialog.ui \
    $$PWD/cameracameraform.ui \
    $$PWD/cellcellform.ui \
    $$PWD/appearanceatomsform.ui \
    $$PWD/appearancebondsform.ui \
    $$PWD/appearanceunitcellform.ui \
    $$PWD/appearanceadsorptionsurfaceform.ui \
    $$PWD/appearanceannotationform.ui \
    $$PWD/cameraselectionform.ui \
    $$PWD/cameralightsform.ui \
    $$PWD/camerapicturesform.ui \
    $$PWD/camerabackgroundform.ui \
    $$PWD/elementsform.ui \
    $$PWD/infocreatorform.ui \
    $$PWD/infocreationform.ui \
    $$PWD/infochemicalform.ui \
    $$PWD/infocitationform.ui \
    $$PWD/cellstructuralform.ui \
    $$PWD/cellsymmetryform.ui
