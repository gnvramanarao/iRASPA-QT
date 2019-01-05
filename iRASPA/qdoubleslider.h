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

#include <QSlider>

class QDoubleSlider : public QSlider
{
  Q_OBJECT
public:
  explicit QDoubleSlider(QWidget *parent = nullptr);

  double doubleMinimum()
  {
    return m_DoubleMin;
  }

  double doubleMaximum()
  {
    return m_DoubleMax;
  }

  double doubleSingleStep()
  {
    return m_DoubleStep;
  }

  void setDoubleMinimum(double value)
  {
    m_DoubleMin = value;
    updateRange();
  }

  void setDoubleMaximum(double value)
  {
    m_DoubleMax = value;
    updateRange();
  }

  void setDoubleSingleStep(double value)
  {
    m_DoubleStep = value;
    updateRange();
  }

  double doubleValue();

  void setDoubleValue(double x);



signals:
  void sliderMoved(double value);
  void valueChanged(double value);
public slots:

private:
  double m_DoubleMin;
  double m_DoubleMax;
  double m_DoubleStep;
  double m_DoubleValue;

  int m_CorrespondingIntValue;

  void updateRange();

  void convertIntToDouble(int value);

  void updateStep()
  {
    QSlider::setSingleStep((int)(1000 * m_DoubleStep / (m_DoubleMax - m_DoubleMin)));
  }
};
