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

#include "iraspatoolbar.h"
#include <QHBoxLayout>
#include <QComboBox>
#include <QToolButton>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include "informationpanelview.h"

iRASPAToolBar::iRASPAToolBar(QWidget * parent):QToolBar(parent)
{
  setMovable(false);
  setFloatable(false);

  InformationPanelView *frame = new InformationPanelView(this);
  //frame->setStyleSheet("background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #3783a5, stop: 1 #1e698c);");




  QHBoxLayout *layout = new QHBoxLayout;
  QWidget *separator = new QWidget();
  separator->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  addWidget(separator);

  addWidget(frame);
  layout->setAlignment(frame, Qt::AlignHCenter);

  QWidget *separator2 = new QWidget();
  separator2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  addWidget(separator2);


  left=new QToolButton(this);
  left->setIcon(QIcon(":/iRASPA/LeftPanelCollapse.png"));
  left->setIconSize(QSize(left->size().height(),left->size().height()));
  left->setCheckable(true);
  left->setChecked(true);
  left->setDown(true);
  //left->setFixedWidth(24);
  //left->setFixedHeight(24);
  left->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
  addWidget(left);
  QObject::connect(left, SIGNAL(toggled(bool)),this,SLOT(reactToToggleHideLeft(bool)));

  down=new QToolButton(this);
  down->setIcon(QIcon(":/iRASPA/BottomPanelCollapse.png"));
  down->setIconSize(QSize(down->size().height(),down->size().height()));
  down->setCheckable(true);
  down->setChecked(true);
  down->setDown(true);
  //down->setFixedWidth(24);
  //down->setFixedHeight(24);
  down->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
  addWidget(down);
  QObject::connect(down, SIGNAL(toggled(bool)),this,SLOT(reactToToggleHideDown(bool)));

  right=new QToolButton(this);
  right->setIcon(QIcon(":/iRASPA/RightPanelCollapse.png"));
  right->setIconSize(QSize(right->size().height(),right->size().height()));
  right->setCheckable(true);
  right->setChecked(true);
  right->setDown(true);
  //right->setFixedWidth(24);
  //right->setFixedHeight(24);
  right->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
  addWidget(right);
  QObject::connect(right, SIGNAL(toggled(bool)),this,SLOT(reactToToggleHideRight(bool)));

  QWidget *separator3 = new QWidget();
  separator3->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
  addWidget(separator3);
}

void iRASPAToolBar::reactToToggleHideLeft(bool checked)
{
  if (checked)
  {
    left->setDown(true);
  }
  else
  {
    left->setDown(false);
  }
}

void iRASPAToolBar::reactToToggleHideDown(bool checked)
{
  if (checked)
  {
    down->setDown(true);
  }
  else
  {
    down->setDown(false);
  }
}

void iRASPAToolBar::reactToToggleHideRight(bool checked)
{
  if (checked)
  {
    right->setDown(true);
  }
  else
  {
    right->setDown(false);
  }
}

QSize iRASPAToolBar::sizeHint() const
{
    return QSize(1200, 40);
}
