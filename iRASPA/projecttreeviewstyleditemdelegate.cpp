#include "projecttreeviewstyleditemdelegate.h"
#include <qtreeview.h>
#include "iraspastructure.h"
#include "projecttreeview.h"
#include "projecttreeviewmodel.h"

ProjectTreeViewStyledItemDelegate::ProjectTreeViewStyledItemDelegate(QWidget* parent) : QStyledItemDelegate(parent)
{

}


void ProjectTreeViewStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  if(!index.isValid())
    return;

  QStyledItemDelegate::paint(painter, option, index);


  painter->save();
  painter->setRenderHint(QPainter::Antialiasing);

  QStyleOptionViewItem opt = option;
  QStyledItemDelegate::initStyleOption(&opt, index);
  QRect rect = opt.rect;
  rect.setX(0);

  // handle selection
  if(option.state & QStyle::State_Selected)
  {
    painter->save();

    painter->setPen(Qt::white);
    painter->drawRoundedRect(rect.adjusted(1,1,-1,-1), 5, 5);

    painter->restore();
  }

  painter->restore();
}
