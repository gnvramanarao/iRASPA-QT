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
#include <QPushButton>
#include <QDial>
#include <QDebug>
#include <skasymmetricbond.h>
#include "bondlistview.h"

BondListViewSliderStyledItemDelegate::BondListViewSliderStyledItemDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{
  isOneCellInEditMode = false;

  if(BondListView *view = qobject_cast<BondListView *>(parent))
  {
     treeView = view;
     connect(treeView, SIGNAL(entered(QModelIndex)), this, SLOT(cellEntered(QModelIndex)));
  }

  slider = new QDoubleSlider(Qt::Horizontal, treeView);
  slider->hide();
  slider->setDoubleMinimum(0.0);
  slider->setDoubleMaximum(3.0);
  slider->setAutoFillBackground(true);
}

void BondListViewSliderStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
  // call super to draw selection
  QStyledItemDelegate::paint(painter, option, index);

  if (isOneCellInEditMode && (currentEditedCellIndex == index)) return;

  if(SKAsymmetricBond *asymmetricBond = static_cast<SKAsymmetricBond*>(index.internalPointer()))
  {
    const QModelIndex bondLengthIndex = index.sibling(index.row(), index.column()-1);
    double bondLength = treeView->model()->data(bondLengthIndex).toDouble();

    painter->save();
    QStyleOptionSlider opt;
    opt.init(slider);
    opt.orientation = Qt::Horizontal;
    opt.minimum = slider->minimum();
    opt.maximum = slider->maximum();
    opt.rect = option.rect;
    slider->setDoubleValue(bondLength);
    opt.sliderPosition = slider->sliderPosition();
    opt.sliderValue = slider->value();
    opt.subControls = QStyle::SC_SliderHandle | QStyle::SC_SliderGroove;

    #ifdef Q_OS_MAC
      painter->translate(option.rect.left(),0);
    #endif

    slider->style()->drawComplexControl(QStyle::CC_Slider, &opt, painter, slider);

    painter->restore();
  }
}

void BondListViewSliderStyledItemDelegate::cellEntered(const QModelIndex &index)
{
  if(index.column() == 7)
  {
    if(isOneCellInEditMode)
    {
      treeView->closePersistentEditor(currentEditedCellIndex);
    }
    treeView->openPersistentEditor(index);
    isOneCellInEditMode = true;
    currentEditedCellIndex = index;
  }
  else
  {
    if(isOneCellInEditMode)
    {
      isOneCellInEditMode = false;
      treeView->closePersistentEditor(currentEditedCellIndex);
    }
  }
}

void BondListViewSliderStyledItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

QWidget * BondListViewSliderStyledItemDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
  if(SKAsymmetricBond *asymmetricBond = static_cast<SKAsymmetricBond*>(index.internalPointer()))
  {
    const QModelIndex bondLengthIndex = index.sibling(index.row(), index.column()-1);
    double bondLength = treeView->model()->data(bondLengthIndex).toDouble();

    QDoubleSlider *realSlider = new QDoubleSlider(Qt::Horizontal, parent);
    realSlider->setDoubleMinimum(0);
    realSlider->setDoubleMaximum(3.0);
    realSlider->setDoubleValue(bondLength);
    realSlider->setAutoFillBackground(false);
    QSize size = option.rect.size();
    realSlider->resize(size);

    //connect(realSlider, static_cast<void (QDoubleSlider::*)()>(&QDoubleSlider::sliderReleased), realSlider,
    //        [this, realSlider, index](){ setModelData(realSlider, treeView->model(), index);});

    connect(realSlider, static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved), realSlider,
            [this, realSlider, index](double value){ setModelData(realSlider, treeView->model(), index);});

    return realSlider;
  }
  return nullptr;
}

void BondListViewSliderStyledItemDelegate::setSliderValue(double level)
{
}


void BondListViewSliderStyledItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  //qDebug() << "setEditorData " << index.row();
    ////setup the editor - your data are in index.data(Qt::DataRoles) - stored in a QVariant;
    //QString value = index.model()->data(index,Qt::EditRole).toString();
    //SubscriberForm *subscriberForm =  static_cast<SubscriberForm*>(editor);
}

void BondListViewSliderStyledItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
  if(QDoubleSlider *slider = qobject_cast<QDoubleSlider *>(editor))
  {
    model->setData(index, QVariant(slider->doubleValue()), Qt::EditRole);
  }
}

