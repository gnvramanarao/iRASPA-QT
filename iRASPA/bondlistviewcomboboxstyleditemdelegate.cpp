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

#include "bondlistviewcomboboxstyleditemdelegate.h"

#include <QWidget>
#include <QStyle>
#include <QPainter>
#include <QStylePainter>
#include <QApplication>
#include <QPushButton>
#include <QDial>
#include <QDebug>
#include <skasymmetricbond.h>
#include <QProxyStyle>
#include <type_traits>

BondListViewComboBoxStyledItemDelegate::BondListViewComboBoxStyledItemDelegate(QWidget *parent) : QStyledItemDelegate(parent), singleBondIcon(new QIcon(":/iRASPA/SingleBond.png"))
{
  isOneCellInEditMode = false;

  if(QTreeView *view = qobject_cast<QTreeView *>(parent))
  {
     treeView = view;
     connect(treeView, SIGNAL(entered(QModelIndex)), this, SLOT(cellEntered(QModelIndex)));
  }

  singleBondIcon = new QIcon(":/iRASPA/SingleBond.png");
  doubleBondIcon = new QIcon(":/iRASPA/DoubleBond.png");
  partialDoubleBondIcon = new QIcon(":/iRASPA/PartialDoubleBond.png");
  tripleBondIcon = new QIcon(":/iRASPA/TripleBond.png");

  comboBox = new QComboBox(treeView);
  comboBox->hide();
  comboBox->setAutoFillBackground(true);
  comboBox->addItem(*singleBondIcon,QString(""));
  comboBox->addItem(*doubleBondIcon, QString(""));
  comboBox->addItem(*partialDoubleBondIcon, QString(""));
  comboBox->addItem(*tripleBondIcon, QString(""));
  comboBox->setCurrentText("");
  comboBox->setCurrentIndex(0);
}

void BondListViewComboBoxStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
  // call super to draw selection
  QStyledItemDelegate::paint(painter, option, index);

  if (isOneCellInEditMode && (currentEditedCellIndex == index)) return;

  if(SKAsymmetricBond *asymmetricBond = static_cast<SKAsymmetricBond*>(index.internalPointer()))
  {
    painter->save();

    comboBox->resize(option.rect.size());

    QStyleOptionComboBox comboBoxOption;
    comboBoxOption.initFrom(comboBox);
    comboBoxOption.rect = option.rect;

    int value = static_cast<typename std::underlying_type<SKAsymmetricBond::SKBondType>::type>(asymmetricBond->getBondType());
    comboBoxOption.currentIcon = comboBox->itemIcon(value);
    comboBoxOption.iconSize = comboBox->iconSize();

    comboBox->style()->drawComplexControl(QStyle::CC_ComboBox, &comboBoxOption, painter, nullptr);
    comboBox->style()->drawControl(QStyle::CE_ComboBoxLabel, &comboBoxOption, painter, nullptr);

    painter->restore();
  }
}

void BondListViewComboBoxStyledItemDelegate::cellEntered(const QModelIndex &index)
{
  if(index.column() == 5)
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

void BondListViewComboBoxStyledItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

QWidget * BondListViewComboBoxStyledItemDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
  if(SKAsymmetricBond *asymmetricBond = static_cast<SKAsymmetricBond*>(index.internalPointer()))
  {
    int value = static_cast<typename std::underlying_type<SKAsymmetricBond::SKBondType>::type>(asymmetricBond->getBondType());

    QComboBox *realCombobox = new QComboBox(parent);
    realCombobox->setAutoFillBackground(false);
    QSize size = option.rect.size();
    realCombobox->resize(size);

    realCombobox->setAutoFillBackground(true);
    realCombobox->addItem(*singleBondIcon, QString(""));
    realCombobox->addItem(*doubleBondIcon, QString(""));
    realCombobox->addItem(*partialDoubleBondIcon, QString(""));
    realCombobox->addItem(*tripleBondIcon, QString(""));
    realCombobox->setCurrentIndex(value);

    connect(realCombobox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), realCombobox,
            [this, realCombobox, index](int value){ setModelData(realCombobox, treeView->model(), index);});

    return realCombobox;

    //connect(realSlider, static_cast<void (QDoubleSlider::*)()>(&QDoubleSlider::sliderReleased), realSlider,
    //        [this, realSlider, index](){ setModelData(realSlider, treeView->model(), index);});

    //connect(realSlider, static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved), realSlider,
    //        [this, realSlider, index](double value){ setModelData(realSlider, treeView->model(), index);});


  }
  return nullptr;
}

void BondListViewComboBoxStyledItemDelegate::setEditorData([[maybe_unused]] QWidget *editor, const QModelIndex &index) const
{
  //qDebug() << "setEditorData " << index.row();
    ////setup the editor - your data are in index.data(Qt::DataRoles) - stored in a QVariant;
    //QString value = index.model()->data(index,Qt::EditRole).toString();
    //SubscriberForm *subscriberForm =  static_cast<SubscriberForm*>(editor);
}

void BondListViewComboBoxStyledItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
  if(QComboBox *comboBox = qobject_cast<QComboBox *>(editor))
  {
    qDebug() << comboBox->currentIndex() << ", " << index.column();
    model->setData(index, QVariant(comboBox->currentIndex()), Qt::EditRole);
  }
}

