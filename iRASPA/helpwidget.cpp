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

#include "helpwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>

HelpWidget::HelpWidget(QWidget *parent): QMainWindow(parent)
{
  setAttribute(Qt::WA_DeleteOnClose);
  setAttribute(Qt::WA_GroupLeader);
  setWindowFlags(Qt::WindowStaysOnTopHint);
  setWindowFlags(Qt::Window);

  QWidget* mainWidget = new QWidget(this);

  textBrowser = new HelpBrowser();

  homeButton = new QPushButton(tr("&Home"));
  forwardButton = new QPushButton(tr("&Forward"));
  backButton = new QPushButton(tr("&Back"));
  closeButton = new QPushButton(tr("Close"));
  closeButton->setShortcut(tr("Esc"));

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(homeButton);
  buttonLayout->addWidget(forwardButton);
  buttonLayout->addWidget(backButton);
  buttonLayout->addStretch();
  buttonLayout->addWidget(closeButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  //mainLayout->addLayout(buttonLayout);
  mainLayout->addWidget(textBrowser);
  mainLayout->setContentsMargins(0,0,0,0);

  this->setCentralWidget(mainWidget);
  mainWidget->setLayout(mainLayout);

  //QObject::connect(homeButton, &QPushButton::clicked, textBrowser, &HelpBrowser::home);
  //QObject::connect(forwardButton, &QPushButton::clicked, textBrowser, &HelpBrowser::forward);
  //QObject::connect(backButton, &QPushButton::clicked, textBrowser, &HelpBrowser::back);
  //QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

void HelpWidget::updateWindowTitle()
{
  //setWindowTitle(tr("Help: %1").arg(textBrowser->documentTitle()));
}
