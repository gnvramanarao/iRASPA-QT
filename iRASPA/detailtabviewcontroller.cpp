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

#include "detailtabviewcontroller.h"
#include "atomtreeview.h"

DetailTabViewController::DetailTabViewController(QWidget* parent ): QTabWidget(parent )
{
  QObject::connect(this, &QTabWidget::currentChanged, this, &DetailTabViewController::reloadTab);
}

QSize DetailTabViewController::sizeHint() const
{
  return QSize(500, 800);
}

void DetailTabViewController::rendererWidgetResized()
{
  foreach(QObject *child, currentWidget()->children())
  {
    if(CameraTreeWidgetController* widget = dynamic_cast<CameraTreeWidgetController*>(child))
    {
      widget->reloadPictureProperties();
    }
  }
}

void DetailTabViewController::reloadTab(int tab)
{
  setCurrentIndex(tab);

  foreach(QObject *child, widget(tab)->children())
  {
    if(Reloadable* reloadable = dynamic_cast<Reloadable*>(child))
    {
      reloadable->reloadData();
      reloadable->reloadSelection();
    }
  }
}

