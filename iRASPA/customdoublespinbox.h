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

#include <QObject>
#include <QSpinBox>
#include <QWheelEvent>

class CustomDoubleSpinBox: public QDoubleSpinBox
{
  Q_OBJECT

public:
  CustomDoubleSpinBox(QWidget* parent);

  enum SpinBoxStateFlag { Double = 0x00, Text = 0x01};

  void setText(QString text);
  void setValue(double value);

  void focusInEvent(QFocusEvent *event) override final;
  void wheelEvent(QWheelEvent *event) override final;
  bool focusNextPrevChild(bool next) override final;

  QString textFromValue(double value) const override final;
  double valueFromText(const QString &text) const override final;
  QValidator::State validate(QString& input, int& pos) const override final;

  void stepBy(int steps) override final;
protected:
    // reimplement keyPressEvent
    //void keyPressEvent(QKeyEvent *event) override;

private:
  double _doubleValue;
  mutable QString _textValue;
  SpinBoxStateFlag _state = Double;
  void privateEditingFinished();
};
