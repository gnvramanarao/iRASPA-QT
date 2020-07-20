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

#include "mastertoolbar.h"
#include <QLayout>
#include "mainwindow.h"

MasterToolBar::MasterToolBar(QWidget * parent):QToolBar(parent)
{
  // the first toolbutton is the "project"-tab
  buttonToProjectTab = new QToolButton(this);
  buttonToProjectTab->setGeometry(0,0,24,24);
  buttonToProjectTab->setMinimumSize(24,24);
  buttonToProjectTab->setMaximumSize(24,24);
  buttonToProjectTab->setIcon(QIcon(":/iRASPA/ProjectListController.png"));
  buttonToProjectTab->setContentsMargins(0,0,0,0);
  buttonToProjectTab->setCheckable(true);
  buttonToProjectTab->setChecked(true);
  buttonToProjectTab->setDown(true);
  addWidget(buttonToProjectTab);

  // the second toolbutton is the "detail"-tab (scene for crystals, vasp-input files for VASP,...)
  buttonToMovieViewTab = new QToolButton(this);
  buttonToMovieViewTab->setGeometry(0,0,24,24);
  buttonToMovieViewTab->setMinimumSize(24,24);
  buttonToMovieViewTab->setMaximumSize(24,24);
  buttonToMovieViewTab->setIcon(QIcon(":/iRASPA/MovieListController.png"));
  buttonToMovieViewTab->setContentsMargins(0,0,0,0);
  buttonToMovieViewTab->setCheckable(true);
  buttonToMovieViewTab->setChecked(false);
  addWidget(buttonToMovieViewTab);

  // the second toolbutton is the "detail"-tab (scene for crystals, vasp-input files for VASP,...)
  buttonFrameViewTab = new QToolButton(this);
  buttonFrameViewTab->setGeometry(0,0,24,24);
  buttonFrameViewTab->setMinimumSize(24,24);
  buttonFrameViewTab->setMaximumSize(24,24);
  buttonFrameViewTab->setIcon(QIcon(":/iRASPA/FrameListController.png"));
  buttonFrameViewTab->setContentsMargins(0,0,0,0);
  buttonFrameViewTab->setCheckable(true);
  buttonFrameViewTab->setChecked(false);
  addWidget(buttonFrameViewTab);

  this->layout()->setSpacing(4);

  _mapper = new QSignalMapper(this);
  _mapper->setMapping(buttonToProjectTab, 0);
  _mapper->setMapping(buttonToMovieViewTab, 1);
  _mapper->setMapping(buttonFrameViewTab, 2);

  QObject::connect(buttonToProjectTab, &QToolButton::clicked,this,&MasterToolBar::reactToProjectToggled);
  QObject::connect(buttonToMovieViewTab, &QToolButton::clicked,this,&MasterToolBar::reactToMovieToggled);
  QObject::connect(buttonFrameViewTab, &QToolButton::clicked,this,&MasterToolBar::reactToFrameToggled);

  QObject::connect(buttonToProjectTab, &QToolButton::clicked, _mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
  QObject::connect(buttonToMovieViewTab, &QToolButton::clicked, _mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
  QObject::connect(buttonFrameViewTab, &QToolButton::clicked, _mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
}

void MasterToolBar::reactToProjectToggled(bool checked)
{

    buttonToProjectTab->setDown(true);
    buttonToMovieViewTab->setDown(false);
    buttonFrameViewTab->setDown(false);
    buttonToProjectTab->setChecked(true);
    buttonToMovieViewTab->setChecked(false);
    buttonFrameViewTab->setChecked(false);

}

void MasterToolBar::reactToMovieToggled(bool checked)
{

    buttonToProjectTab->setDown(false);
    buttonToMovieViewTab->setDown(true);
    buttonFrameViewTab->setDown(false);
    buttonToProjectTab->setChecked(false);
    buttonToMovieViewTab->setChecked(true);
    buttonFrameViewTab->setChecked(false);

}

void MasterToolBar::reactToFrameToggled(bool checked)
{

    buttonToProjectTab->setDown(false);
    buttonToMovieViewTab->setDown(false);
    buttonFrameViewTab->setDown(true);
    buttonToProjectTab->setChecked(false);
    buttonToMovieViewTab->setChecked(false);
    buttonFrameViewTab->setChecked(true);

}

QSize MasterToolBar::sizeHint() const
{
  return QSize(200, 24);
}
