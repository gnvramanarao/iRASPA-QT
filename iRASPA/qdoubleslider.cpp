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

#include "qdoubleslider.h"
#include <cmath>

QDoubleSlider::QDoubleSlider(QWidget *parent) :
  QSlider(parent)
{
  m_DoubleMin = 0.0;
  m_DoubleMax = 1.0;
  m_DoubleStep = 0.01;
  updateRange();

  connect(this, &QSlider::sliderMoved, this, &QDoubleSlider::convertIntToDouble);
  connect(this, &QSlider::valueChanged, this, &QDoubleSlider::convertIntToDouble);
}

QDoubleSlider::QDoubleSlider(Qt::Orientation orientation, QWidget *parent) : QSlider(orientation, parent)
{
  m_DoubleMin = 0.0;
  m_DoubleMax = 1.0;
  m_DoubleStep = 0.01;
  updateRange();

  connect(this, &QSlider::sliderMoved, this, &QDoubleSlider::convertIntToDouble);
  connect(this, &QSlider::valueChanged, this, &QDoubleSlider::convertIntToDouble);
}

void QDoubleSlider::convertIntToDouble(int value)
{
  double t = value * 1.0 / this->maximum();
  double dValue = m_DoubleMin + t * (m_DoubleMax - m_DoubleMin);
  emit sliderMoved(dValue);
}
void QDoubleSlider::updateRange()
{
  int mymax = ceil((m_DoubleMax - m_DoubleMin) / m_DoubleStep);
  this->setMinimum(0);
  this->setMaximum(mymax);
  this->setSingleStep(1);

  this->setDoubleValue(m_DoubleValue);
}

void QDoubleSlider::setDoubleValue(double x)
{
  m_DoubleValue = x;
  double t = (m_DoubleValue - m_DoubleMin) / (m_DoubleMax - m_DoubleMin);
  t = std::max(0.0, std::min(1.0, t));
  int p = (int)(0.5 + this->maximum() * t);
  if(this->value() != p)
    this->setValue(p);
  m_CorrespondingIntValue = p;
}

double QDoubleSlider::doubleValue()
{
  if(this->value() != m_CorrespondingIntValue)
  {
    double t = this->value() * 1.0 / this->maximum();
    m_DoubleValue = m_DoubleMin + t * (m_DoubleMax - m_DoubleMin);
    m_CorrespondingIntValue = this->value();
  }

  return m_DoubleValue;
}
