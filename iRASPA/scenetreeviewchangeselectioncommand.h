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

#include <QUndoCommand>
#include <set>
#include <vector>
#include <map>
#include "iraspakit.h"
#include "symmetrykit.h"
#include "mathkit.h"
#include "mainwindow.h"

class SceneChangeSelectionCommand : public QUndoCommand
{
public:
  SceneChangeSelectionCommand(MainWindow *main_window,
                         std::shared_ptr<Movie> newSelectedMovie, std::map<std::shared_ptr<Scene>, std::set<std::shared_ptr<Movie>>> newSelection,
                         std::shared_ptr<Movie> oldSelectedMovie, std::map<std::shared_ptr<Scene>, std::set<std::shared_ptr<Movie>>> oldSelection,
                         QUndoCommand *parent = nullptr);
  void undo() override final;
  void redo() override final;
private:
  MainWindow *_main_window;
  std::shared_ptr<Movie> _newSelectedMovie;
  std::map<std::shared_ptr<Scene>, std::set<std::shared_ptr<Movie>>>  _newSelection;
  std::shared_ptr<Movie> _oldSelectedMovie;
  std::map<std::shared_ptr<Scene>, std::set<std::shared_ptr<Movie>>>  _oldSelection;
};
