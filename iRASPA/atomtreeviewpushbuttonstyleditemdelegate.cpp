#/*************************************************************************************************************
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

 #include "atomtreeviewpushbuttonstyleditemdelegate.h"
 #include <QWidget>
 #include <QStyle>
 #include <QPainter>
 #include <QStylePainter>
 #include <QApplication>
 #include <QComboBox>
 #include <QCheckBox>
 #include <QDial>
 #include <QPushButton>
 #include <QDebug>
 #include <QMouseEvent>
 #include "skasymmetricbond.h"
 #include <QSpinBox>
 #include <QToolButton>
 #include <QTreeView>


 AtomTreeViewPushButtonStyledItemDelegate::AtomTreeViewPushButtonStyledItemDelegate(QWidget* parent) : QStyledItemDelegate(parent)
 {
   btn = new QPushButton();
   btn->setCheckable(true);

   btn->setStyleSheet(tr(":!checked {margin: 1px; border-radius: 5px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                         "stop: 0 #eaebee, stop: 1 #f8f9fc); border: 1px inset #808080 ; }"
                         ":checked {margin: 1px; border-radius: 5px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                         "stop: 0 #77a3f4, stop: 1 #2e6af6); border: 1px inset #404040; }"));
 }

 AtomTreeViewPushButtonStyledItemDelegate::~AtomTreeViewPushButtonStyledItemDelegate()
 {
   delete btn;
 }

 void AtomTreeViewPushButtonStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
 {
   // call super to draw selection
   QStyledItemDelegate::paint(painter, option, index);

   if(SKAtomTreeNode *atom = static_cast<SKAtomTreeNode*>(index.internalPointer()))
   {
     painter->save();
     QStyleOptionButton buttonOption;
     buttonOption.text = QString::number(atom->representedObject()->tag());
     buttonOption.state = atom->representedObject()->isFixed().x ? QStyle::State_On : QStyle::State_Off;
     buttonOption.rect = option.rect;
     //buttonOption.rect.setWidth(option.rect.width());
     btn->style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter, btn);
     painter->restore();
   }
 }


 bool AtomTreeViewPushButtonStyledItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
 {
   if (index.column() == 1)
   {
     if (event->type() == QEvent::MouseButtonRelease)
     {
       QMouseEvent* pME = static_cast<QMouseEvent*>(event);
       if(pME->button() == Qt::LeftButton)
       {
         if(SKAtomTreeNode *atom = static_cast<SKAtomTreeNode*>(index.internalPointer()))
         {
           if(atom->representedObject()->isFixed().x == true)
           {
             atom->representedObject()->setIsFixed(bool3(false,false,false));
           }
           else
           {
             atom->representedObject()->setIsFixed(bool3(true,true,true));
           }

           // update column 2
           if(QTreeView *item = qobject_cast<QTreeView*>(this->parent()))
           {
             int numberOfRows = item->model()->rowCount();
             QModelIndex firstIndex = item->model()->index(0,1, QModelIndex());
             QModelIndex lastIndex = item->model()->index(numberOfRows,1, QModelIndex());
             item->dataChanged(firstIndex, lastIndex);
           }
         }
       }
     }
   }
   return QStyledItemDelegate::editorEvent(event, model, option, index);
 }
