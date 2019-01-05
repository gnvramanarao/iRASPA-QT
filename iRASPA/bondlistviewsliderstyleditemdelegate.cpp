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

#include "bondlistviewsliderstyleditemdelegate.h"
#include <QWidget>
#include <QStyle>
#include <QPainter>
#include <QStylePainter>
#include <QApplication>
#include <QComboBox>
#include <QCheckBox>
#include <QDial>

BondListViewSliderStyledItemDelegate::BondListViewSliderStyledItemDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{

}


void BondListViewSliderStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
   //painter->fillRect(option.rect, option.palette.highlight());
   //painter->fillRect(option.rect, QColor(255,0,255,255));

   //QStyle style = QApplication::style();
    QStyleOptionSlider sliderOption;
    sliderOption.rect = option.rect;
    sliderOption.minimum = 0.0;
    sliderOption.maximum = 3.0;

    //QSlider slider = QSlider(Qt::Horizontal);
    //slider.setMinimum(0.0);
    //slider.setMaximum(3.0);
    //slider.setAutoFillBackground(true);


    // This line crashes a QMessageBox. CE_ProgressBarContents and CE_ProgressBar lead to a crash.
    // All other values of QStyle::CE_*** work fine.
    // drawComplexControl has no problem either.
    //QApplication::style()->drawControl(QStyle::CE_ProgressBarContents, &progressBarOption, painter);
    //QApplication::style()->drawComplexControl(QStyle::CC_Slider, &sliderOption, painter);

    QStyleOptionSlider box;

     box.rect = option.rect;


  //   QApplication::style()->drawComplexControl(QStyle::CC_Slider, &box, painter, &slider);
     //QApplication::style()->drawControl(QStyle::CE_, &box, painter, nullptr);

    QStyledItemDelegate::paint(painter, option, index);
}
