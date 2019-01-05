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

#include "customdoublespinbox.h"
#include <iostream>
#include <QLineEdit>
//#include <QKeyEvent>
#include <math.h>
#include <iostream>

CustomDoubleSpinBox::CustomDoubleSpinBox(QWidget* parent) : QDoubleSpinBox(parent)
{
  QObject::connect(this, &CustomDoubleSpinBox::editingFinished, this, &CustomDoubleSpinBox::privateEditingFinished);
}


void CustomDoubleSpinBox::privateEditingFinished()
{
  if(!isReadOnly())
  {
    std::cout << "Read text: " <<  _textValue.toStdString() << std::endl;
    bool success = false;
    double value = _textValue.toDouble(&success);
    _state=Text;
    setPrefix(_textValue);

    if(success)
    {
      _state = Double;
      _doubleValue = value;
      setPrefix("");
    }
    //clearFocus();
  }
}

void CustomDoubleSpinBox::setValue(double value)
{
  setPrefix("");
  _state = Double;
  _doubleValue = value;
  QDoubleSpinBox::setValue(value);
}



void CustomDoubleSpinBox::setText(QString text)
{
  std::cout << "set text: " << text.toStdString() << std::endl;
  _textValue = text;
  _state = Text;
  setPrefix(text);
}

void CustomDoubleSpinBox::focusInEvent(QFocusEvent *event)
{
  std::cout << "focusInEvent" << std::endl;
  if(_state == Text && !isReadOnly())
  {
    setPrefix("");
    _state = Double;
    _textValue = "";
    lineEdit()->setText("");
  }
  QDoubleSpinBox::focusInEvent(event);
}

QValidator::State CustomDoubleSpinBox::validate(QString& input, int& pos) const
{
  if(_state == Text)
  {
    return QValidator::Acceptable;
  }
  else
  {
    return QDoubleSpinBox::validate(input,pos);
  }
}


QString CustomDoubleSpinBox::textFromValue(double value) const
{
   switch(_state)
   {
   case Double:
       return QDoubleSpinBox::textFromValue(value);
   case Text:
       return "";
   }
}

double CustomDoubleSpinBox::valueFromText(const QString &text) const
{
  _textValue = text;
  return QDoubleSpinBox::valueFromText(text);
}

void CustomDoubleSpinBox::stepBy(int steps)
{
  QDoubleSpinBox::stepBy(steps);
  _textValue = text();
}

/*
void CustomDoubleSpinBox::keyPressEvent(QKeyEvent *event)
{
  event->ignore();
}
*/
