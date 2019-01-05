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

#include "customintspinbox.h"
#include <climits>
#include <QApplication>
#include <iostream>
#include <QLineEdit>
#include <math.h>
#include <iostream>

CustomIntSpinBox::CustomIntSpinBox(QWidget* parent) : QSpinBox(parent)
{
  QObject::connect(this, &CustomIntSpinBox::editingFinished, this, &CustomIntSpinBox::privateEditingFinished);
}

void CustomIntSpinBox::privateEditingFinished()
{
  std::cout << "Read text: " <<  _textValue.toStdString() << std::endl;
  bool success = false;
  int value = _textValue.toInt(&success);
  _state=Text;
  setPrefix(_textValue);

  if(success)
  {
    _state = Int;
    _intValue = value;
    setPrefix("");
  }

  clearFocus();
}

void CustomIntSpinBox::setText(QString text)
{
  std::cout << "set text: " << text.toStdString() << std::endl;
  _textValue = text;
  _state = Text;
  setPrefix(text);
}

void CustomIntSpinBox::setValue(int value)
{
  setPrefix("");
  _state = Int;
  _intValue = value;
  QSpinBox::setValue(value);
}




void CustomIntSpinBox::focusInEvent(QFocusEvent *event)
{
  std::cout << "focusInEvent" << std::endl;
  if(_state == Text)
  {
    setPrefix("");
    _state = Int;
    _textValue = "";
    lineEdit()->setText("");
  }
  QSpinBox::focusInEvent(event);
}

QValidator::State CustomIntSpinBox::validate(QString& input, int& pos) const
{
  if(_state == Text)
  {
    return QValidator::Acceptable;
  }
  else
  {
    return QSpinBox::validate(input,pos);
  }
}


QString CustomIntSpinBox::textFromValue(int value) const
{
   switch(_state)
   {
   case Int:
       return QSpinBox::textFromValue(value);
   case Text:
       return "";
   }
}

int CustomIntSpinBox::valueFromText(const QString &text) const
{
  _textValue = text;
  return QSpinBox::valueFromText(text);
}

void CustomIntSpinBox::stepBy(int steps)
{
  QSpinBox::stepBy(steps);
  _textValue = text();
}




void CustomIntSpinBox::timerEvent(QTimerEvent *event)
{
  //QWidget::timerEvent(event);


  // Process all events, which may include a mouse release event
  // Only allow the timer to trigger additional value changes if the user
  //   has in fact held the mouse button, rather than the timer expiry
  //   simply appearing before the mouse release in the event queue

  qApp->processEvents();
  if (QApplication::mouseButtons() & Qt::LeftButton)
  {
    QSpinBox::timerEvent (event);
  }
}
