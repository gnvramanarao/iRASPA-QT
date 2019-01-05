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

#include "informationpanelview.h"
#include <QPainter>

InformationPanelView::InformationPanelView(QWidget * parent):QLabel(parent)
{
  setFixedWidth(340);
  setFixedHeight(30);
    //frame->setFrameStyle(QFrame::Box | QFrame::Raised);
  setFrameStyle(QFrame::Box);
  setLineWidth(2);
}

void InformationPanelView::paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  QLinearGradient grad1(0,0,0,30);
  grad1.setColorAt(0.0,QColor(237,241,225));
  grad1.setColorAt(0.5,QColor(230,235,213));
  grad1.setColorAt(0.5,QColor(222,228,199));
  grad1.setColorAt(1.0,QColor(242,245,224));

  QRectF rectangle(0,0,340,30);
  QPainterPath path;
  path.addRoundedRect(rectangle, 3.5, 3.5);
  QPen pen(Qt::darkGray, 2);
  painter.setPen(pen);
  painter.fillPath(path, grad1);
  painter.drawRoundedRect(rectangle, 3.5, 3.5);
}
