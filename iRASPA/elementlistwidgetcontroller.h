/*************************************************************************************************************
 Copyright (c) 2006-2019 David Dubbeldam, Sofia Calero, Thijs J.H. Vlugt.

    D.Dubbeldam@uva.nl            http://www.acmm.nl/molsim/users/dubbeldam/index.html
    scaldia@upo.es                http://www.upo.es/raspa/sofiacalero.php
    t.j.h.vlugt@tudelft.nl        http://homepage.tudelft.nl/v9k6y

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 *************************************************************************************************************/

#pragma once

#include <QWidget>
#include <QListWidget>
#include <QObject>
#include <QListView>
#include <optional>
#include "elementsform.h"
#include "iraspamainwindowconsumerprotocol.h"
#include "ui_elementsform.h"

class ElementListWidgetController: public QListWidget, public MainWindowConsumer, public ProjectConsumer
{
  Q_OBJECT
public:
  ElementListWidgetController(QWidget* parent);
  void setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode) override final;
  void setStructures(std::vector<std::shared_ptr<iRASPAStructure> > structures);
  void setMainWindow(MainWindow *mainWindow) override final;
  void resetData();
  void reloadData();
  void reloadColorData();
private:
  ElementsForm* _elementsForm;
  MainWindow *_mainWindow;
  std::shared_ptr<ProjectStructure> _projectStructure;
  std::vector<std::shared_ptr<iRASPAStructure>> _structures{};
  int _selectedColorSetIndex = 0;
  int _selectedForceFieldSetIndex = 0;

  void setColorComboBoxIndex(size_t type);
  void setForceFieldComboBoxIndex(int type);

  std::optional<size_t> rowForItem(QObject* item);
  void addNewForceFieldAtomType();
  void removeForceFieldAtomType();
  void selectColorButton();
  void setEpsilonParameter(double parameter);
  void setSigmaParameter(double parameter);
  void setMass(double mass);
signals:
  void rendererReloadData();
  void invalidateCachedAmbientOcclusionTexture(std::vector<std::vector<std::shared_ptr<iRASPAStructure>>> structures);
  void invalidateIsosurface(std::vector<std::shared_ptr<RKRenderStructure>> structures);
};
